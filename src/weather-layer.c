/*******************************
 * Weather Layer
 *******************************/

#include "main.h"
#include "appinfo.h"
#include "weather-layer.h"

static TextLayer *s_weather_layer;

static bool s_window_loaded = false;
static bool s_phone_loaded = false;
static bool s_weather_enabled = true;
static char s_weather_info[32];
static time_t s_last_updated;

static void clear_weather_text() {
    text_layer_set_text(s_weather_layer, "");
}

static void update_weather_text(char *temperature, char *conditions) {

  if (!s_weather_enabled) {
    clear_weather_text();
    return;
  }

  // If we don't have a new temperature, use the last value
  if (strlen(temperature) > 0) {
    // Assemble full string and display
    snprintf(
        s_weather_info,
        sizeof(s_weather_info),
        (strlen(conditions) == 0) ? "%s" : "%s, %s",
        temperature,
        conditions);
    s_last_updated = time(NULL);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "(update_weather) s_weather_info %s", s_weather_info);
  text_layer_set_text(s_weather_layer, s_weather_info);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  char temperature_buffer[8];
  char conditions_buffer[32];

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
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
      case KEY_SHOW_WEATHER:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "s_weather_info %s", s_weather_info);
        s_weather_enabled = t->value->int32 > 0;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "s_weather_enabled %s", s_weather_enabled ? "true" : "false");
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  update_weather_text(temperature_buffer, conditions_buffer);
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

static void load_config() {
  const int HOUR = 60 * 60;
  char weather_info[32];

  if (persist_exists(PERSIST_WEATHER_INFO)) {
    persist_read_string(PERSIST_WEATHER_INFO, weather_info, 32);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded weather info: %s", weather_info);
  }
  if (persist_exists(PERSIST_WEATHER_UPDATED)) {
    persist_read_data(PERSIST_WEATHER_UPDATED, &s_last_updated, sizeof(s_last_updated));
    char str_time[50];
    struct tm *temp_time = localtime(&s_last_updated);
    strftime(str_time, 50, "Weather was last updated %Y-%m-%d %H:%M\n", temp_time);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", str_time);
  } 

  time_t now = time(NULL);
  if (difftime(now, s_last_updated) < 3 * HOUR) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last update is fresh enough, using it");
    strcpy(s_weather_info, weather_info);
    if (s_window_loaded)
      update_weather_text("", "");
  }
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
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text(s_weather_layer, strlen(s_weather_info) > 0 ? s_weather_info : "Loading...");
  text_layer_set_font(s_weather_layer, g_small_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));

  s_window_loaded = true;
}

void weather_window_unload(Window *window) {
  text_layer_destroy(s_weather_layer);
}

void weather_init() {
  load_config();

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
  persist_write_string(PERSIST_WEATHER_INFO, s_weather_info);
  persist_write_data(PERSIST_WEATHER_UPDATED, &s_last_updated, sizeof(s_last_updated));
}
