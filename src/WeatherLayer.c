/*******************************
 * Weather Layer
 *******************************/

#include "PHD.h"
#include "appinfo.h"
#include "WeatherLayer.h"

static TextLayer *s_weather_layer;
static bool s_loaded = false;

static void update_weather_text(char *temperature, char *conditions) {
  static char weather_layer_buffer[32];

  // Assemble full string and display
  snprintf(
      weather_layer_buffer,
      sizeof(weather_layer_buffer),
      (strlen(conditions) == 0) ? "%s" : "%s, %s",
      temperature,
      conditions);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  char temperature_buffer[8];
  char conditions_buffer[32];

  // We'll be sent a message when the js first loads
  if (!s_loaded)
    s_loaded = true;

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

void weather_update(struct tm *tick_time) {
  if (!s_loaded)
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
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, g_small_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
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
