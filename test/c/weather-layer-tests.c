#include <pebble.h>
#include "appinfo.h"
#include "seatest.h"
#include "config.h"
#include "weather-layer.h"

#define MAX_TEXT_LAYER_LENGTH 255

static char *s_text_layer_text = NULL;
static void *s_weather_layer;
static Tuple *temperature;
static Tuple *conditions;
static AppMessageInboxReceived s_send_message = NULL;

static TextLayer* mock_text_layer_create(struct GRect frame) {
  // Return a mock text layer, if it's NULL then our code won't try writing to it
  return s_weather_layer;
}

static void mock_text_layer_set_text(TextLayer * text_layer, const char * text) {
  strncpy(s_text_layer_text, text, MAX_TEXT_LAYER_LENGTH);
}

static AppMessageInboxReceived mock_app_message_register_inbox_received(AppMessageInboxReceived received_callback) {
  s_send_message = received_callback;
  return received_callback;
}

static Tuple * mock_dict_read_first(DictionaryIterator *iter) {
  return temperature;
}

static Tuple * mock_dict_read_next(DictionaryIterator *iter) {
  static int count = 0;
  // This is an iterator function, we have to return NULL when done
  if (count++ > 0) 
    return NULL;

  return conditions;
}

static void test_should_display_updated_weather_given_no_recent_update() {
  time_t now = time(NULL);
  *(config()) = (Config) {
    .weather_last_updated = now - (60 * 60),
    .weather_enabled = true
  };

  temperature = calloc(sizeof(Tuple) + sizeof(int), 1);
  if (temperature) {
    memcpy(
      temperature, 
      (&(Tuple const){
        .key = KEY_TEMPERATURE,
        .length = sizeof(int),
        .type = TUPLE_INT
      }), 
      sizeof(Tuple)
    );
  }
  temperature->value->int32 = 28;

  const char *conditions_val = "Cloudy";
  conditions = calloc(sizeof(Tuple) + sizeof(conditions_val), 1);
  if (conditions) {
    memcpy(
      conditions, 
      (&(Tuple const){
        .key = KEY_CONDITIONS,
        .length = sizeof(conditions_val),
        .type = TUPLE_CSTRING
      }), 
      sizeof(Tuple)
    );
  }
  strcpy(conditions->value->cstring, conditions_val);

  // Setup the app message callbacks
  weather_init();
  // Send the initial phone message so the app knows it's ready to send requests
  assert_false(s_send_message == NULL);
  s_send_message(NULL, NULL);

  // Request an update
  weather_update(localtime(&now));

  // Now fake the async response
  pebble_mock_dict_read_first(mock_dict_read_first);
  pebble_mock_dict_read_next(mock_dict_read_next);
  s_send_message(NULL, NULL);
  pebble_mock_dict_read_first(NULL);

  assert_string_equal("28C, Cloudy", s_text_layer_text);

  free(temperature);
  free(conditions);
}

static void test_should_display_saved_weather_given_recent_data() {
  *(config()) = (Config) {
    .weather_conditions = "Windy",
    .weather_temp = 14,
    .weather_last_updated = time(NULL),
    .weather_enabled = true
  };
  
  weather_window_load(NULL);

  assert_string_equal("14C, Windy", s_text_layer_text);
}

static void test_setup() {
  pebble_mock_text_layer_set_text(mock_text_layer_set_text);
  pebble_mock_text_layer_create(mock_text_layer_create);
  pebble_mock_app_message_register_inbox_received(mock_app_message_register_inbox_received);

  s_text_layer_text = malloc(MAX_TEXT_LAYER_LENGTH);
  // Here I'm just randomly giving s_weather_layer something to point to so it's not null
  s_weather_layer = malloc(sizeof(int));
  *((int*)s_weather_layer) = 10;
}

static void test_teardown() {
  // Clean up all the things so tests are atomic
  pebble_mock_text_layer_set_text(NULL);
  pebble_mock_text_layer_create(NULL);
  pebble_mock_app_message_register_inbox_received(NULL);

  free(s_text_layer_text);
  free(s_weather_layer);
}

void weather_layer_test_fixture(void) {
  test_fixture_start();      
  fixture_setup(test_setup);
  fixture_teardown(test_teardown);

  run_test(test_should_display_saved_weather_given_recent_data);   
  run_test(test_should_display_updated_weather_given_no_recent_update);   

  test_fixture_end();       
}
