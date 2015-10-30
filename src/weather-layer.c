/*******************************
 * Weather Layer
 *******************************/

#include "main.h"
#include "appinfo.h"
#include "config.h"
#include "weather-layer.h"

static TextLayer *s_weather_layer = NULL;
static bool s_phone_loaded = false;

static void update_weather_text(int temperature, char *conditions) {
  static char weather_info[32];

  if (s_weather_layer == NULL)
    return;

  // Assemble full string and display
  snprintf(
      weather_info,
      sizeof(weather_info),
      (strlen(conditions) == 0) ? "%dC" : "%dC, %s",
      temperature,
      conditions);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Update weather %s", weather_info);
  text_layer_set_text(s_weather_layer, weather_info);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // We'll be sent a message when the js first loads
  if (!s_phone_loaded)
    s_phone_loaded = true;

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case KEY_TEMPERATURE:
        config()->weather_temp = (int)t->value->int32;
        break;
      case KEY_CONDITIONS:
        snprintf(config()->weather_conditions, sizeof(config()->weather_conditions), "%s", t->value->cstring);
        break;
      case KEY_SHOW_WEATHER:
        config()->weather_enabled = t->value->int32 > 0;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "s_weather_enabled %s", config()->weather_enabled ? "true" : "false");
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
  if (difftime(now, config()->weather_last_updated) < WEATHER_EXPIRY_SECS) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last update is fresh enough, using it");
    update_weather_text(config()->weather_temp, config()->weather_conditions);
  }
}

void clear_weather_text() {
    text_layer_set_text(s_weather_layer, "");
}

void weather_update(struct tm *tick_time) {
  if (!s_phone_loaded)
    return;

  // Get weather update at a regular interval
  if(tick_time->tm_min % WEATHER_UPDATE_MINS == 0) {
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "weather_window_load");
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, g_small_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));

  // This gets called by weather_init and here because we don't know which will return first.
  // We check to see if s_weather_layer has been initialised before trying to set any value.
  if (config()->is_loaded)
    load_last_weather();
}

void weather_window_unload(Window *window) {
  text_layer_destroy(s_weather_layer);
}

void weather_init() {
  if (s_weather_layer != NULL)
    load_last_weather();

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
