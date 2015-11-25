/*******************************
 * Weather Layer
 *******************************/

#include "main.h"
#include "appinfo.h"
#include "config.h"
#include "utils.h"
#include "weather-layer.h"

static TextLayer *s_weather_layer = NULL;
static int s_update_retry_count = 0;
static time_t s_last_retry;

static void update_weather_text(int temperature, char *conditions) {
  static char weather_info[32];
  char formatted_conditions[11] = "";

  if (s_weather_layer == NULL)
    return;

  if (strlen(conditions) > 0)
    snprintf(formatted_conditions, sizeof(formatted_conditions), ", %s", conditions);
  snprintf(weather_info, sizeof(weather_info), "%dC%s", temperature, formatted_conditions);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Update weather %s", weather_info);
  text_layer_set_text(s_weather_layer, weather_info);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case KEY_TEMPERATURE:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received temp %d", (int)t->value->int32);
        config()->weather_temp = (int)t->value->int32;
        config()->weather_last_updated = time(NULL);
        s_update_retry_count = 0;
        break;
      case KEY_CONDITIONS:
        snprintf(config()->weather_conditions, sizeof(config()->weather_conditions), "%s", t->value->cstring);
        break;
      case KEY_SHOW_WEATHER:
        config()->weather_enabled = t->value->int32 > 0;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "s_weather_enabled %s", config()->weather_enabled ? "true" : "false");
        layer_set_hidden(text_layer_get_layer(s_weather_layer), !config()->weather_enabled);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  update_weather_text(config()->weather_temp, config()->weather_conditions);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void load_last_weather() {
  // Check if last weather update is less than expiry
  time_t now = time(NULL);
  int seconds_till_weather_expires = WEATHER_EXPIRY_MINS * 60;
  double seconds_since_last_update = difftime(now, config()->weather_last_updated);

  if (seconds_since_last_update < seconds_till_weather_expires) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last weather saved %s. It's still valid so we'll use it", format_date_time(config()->weather_last_updated));
    update_weather_text(config()->weather_temp, config()->weather_conditions);
  }
  else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last weather saved %s. It's stale so we'll ignore it", format_date_time(config()->weather_last_updated));
  }
}

static bool update_is_required(struct tm *tick_time) {
  if (!config()->weather_enabled)
    return false;

  time_t now = mktime(tick_time);
  double seconds_since_last_update = difftime(now, config()->weather_last_updated);
  int seconds_till_next_update = WEATHER_UPDATE_MINS * 60;

  APP_LOG(APP_LOG_LEVEL_DEBUG, 
      "Checking if update required. %f seconds since last update, %d seconds till next update", 
      seconds_since_last_update,
      seconds_till_next_update);

  if (seconds_since_last_update < seconds_till_next_update)
    return false;

  double seconds_since_last_retry = difftime(now, s_last_retry);
  int seconds_till_next_retry = 0;

  if (s_update_retry_count == 1)
    seconds_till_next_retry = WEATHER_RETRY_MIN_SECS;
  else if (s_update_retry_count == 2)
    seconds_till_next_retry = WEATHER_RETRY_MIN_SECS + ((WEATHER_RETRY_MAX_SECS - WEATHER_RETRY_MIN_SECS) / 2);
  else if (s_update_retry_count == 3)
    seconds_till_next_retry = WEATHER_RETRY_MAX_SECS;

  APP_LOG(APP_LOG_LEVEL_DEBUG, 
      "Update required, checking if in retry window. %d retries so far, %f seconds since last retry, %d seconds till next retry",
      s_update_retry_count,
      seconds_since_last_retry,
      seconds_till_next_retry);

  if (seconds_since_last_retry < seconds_till_next_retry)
    return false;

  s_update_retry_count++;
  s_last_retry = now;
  return true;
}

void weather_update(struct tm *tick_time) {
  if (update_is_required(tick_time)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating weather, last update was %s", format_date_time(config()->weather_last_updated));
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

void weather_window_load(Window *window) {
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, g_small_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_set_hidden(text_layer_get_layer(s_weather_layer), !config()->weather_enabled);

  // Load our stored weather if it's valid so we see something better than Loading...
  load_last_weather();
  if (config()->weather_enabled) {
    time_t now = time(NULL);
    weather_update(localtime(&now));
  }
}

void weather_window_unload(Window *window) {
  text_layer_destroy(s_weather_layer);
}

void weather_init() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM, APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
}

void weather_deinit() {
  app_message_deregister_callbacks();
}
