#include <pebble.h>
#include "appinfo.h"
#include "seatest.h"
#include "config.h"
#include "weather-layer.h"

#define MAX_TEXT_LAYER_LENGTH 255

static char *s_text_layer_text = NULL;
static void *s_weather_layer;
static Tuple *temperature = NULL;
static Tuple *conditions = NULL;
static int dict_next_count = 0;
static int received_msg_count = 0;
static AppMessageInboxReceived s_send_message = NULL;

// Mock functions
// ==============

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
  // This is an iterator function, we have to return NULL when done
  if (dict_next_count++ > 0) 
    return NULL;

  return conditions;
}

static AppMessageResult mock_app_message_outbox_send(void) {
  received_msg_count++;
  return APP_MSG_OK;
}

// Helpers
// =======

static Tuple * make_tuple(key, type, length) {
  Tuple *ret = calloc(sizeof(Tuple) + length, 1);
  if (ret) {
    memcpy(
      ret, 
      (&(Tuple const){
        .key = key,
        .length = length,
        .type = type
      }), 
      sizeof(Tuple)
    );
  }
  return ret;
}

// Tests 
// =====

static void test_should_delay_update_retries_given_no_response() {
  time_t now = time(NULL);
  time_t test_time;
  *(config()) = (Config) {
    .weather_last_updated = now - (60 * 60),
    .weather_enabled = true
  };
  pebble_mock_enable_logs(true);

  // Setup the app message callbacks
  weather_init();
  // Send the initial phone message so the app knows it's ready to send requests
  assert_false(s_send_message == NULL);
  s_send_message(NULL, NULL);

  // Request an update
  weather_update(localtime(&now));
  // Verify request sent
  assert_int_equal(1, received_msg_count);
  
  // Request another update
  weather_update(localtime(&now));
  // Verify no request sent 
  assert_int_equal(1, received_msg_count);

  // Request another update after 30 sec
  now += 30;
  weather_update(localtime(&now));
  // Verify request sent
  assert_int_equal(2, received_msg_count);
  
  // Request another update after 30 sec
  test_time = now + 30;
  weather_update(localtime(&test_time));
  // Verify no request sent 
  assert_int_equal(2, received_msg_count);

  // Request another update after 3 min
  now += (3 * 60);
  weather_update(localtime(&now));
  // Verify request sent
  assert_int_equal(3, received_msg_count);

  // Request another update after 3 min
  test_time = now + (3 * 60);
  weather_update(localtime(&test_time));
  // Verify no request sent 
  assert_int_equal(3, received_msg_count);

  // Request another update after 5 min
  now += (5 * 60);
  weather_update(localtime(&now));
  // Verify request sent
  assert_int_equal(4, received_msg_count);

  pebble_mock_enable_logs(false);
}

static void test_should_display_updated_weather_given_no_recent_update() {
  time_t now = time(NULL);
  *(config()) = (Config) {
    .weather_last_updated = now - (60 * 60),
    .weather_enabled = true
  };

  // Setup the app message callbacks
  weather_init();
  // Send the initial phone message so the app knows it's ready to send requests
  assert_false(s_send_message == NULL);
  s_send_message(NULL, NULL);

  // Request an update
  weather_update(localtime(&now));

  // Now fake the async response
  temperature = make_tuple(KEY_TEMPERATURE, TUPLE_INT, sizeof(int));
  temperature->value->int32 = 28;
  conditions = make_tuple(KEY_CONDITIONS, TUPLE_CSTRING, 7);
  strcpy(conditions->value->cstring, "Cloudy");

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

// Text fixture stuff
// ==================

static void test_setup() {
  pebble_mock_text_layer_set_text(mock_text_layer_set_text);
  pebble_mock_text_layer_create(mock_text_layer_create);
  pebble_mock_app_message_register_inbox_received(mock_app_message_register_inbox_received);
  pebble_mock_dict_read_first(mock_dict_read_first);
  pebble_mock_dict_read_next(mock_dict_read_next);
  pebble_mock_app_message_outbox_send(mock_app_message_outbox_send);

  s_text_layer_text = malloc(MAX_TEXT_LAYER_LENGTH);
  // Here I'm just randomly giving s_weather_layer something to point to so it's not null
  s_weather_layer = malloc(sizeof(int));
  *((int*)s_weather_layer) = 10;
  temperature = NULL;
  conditions = NULL;
  dict_next_count = 0;
  received_msg_count = 0;
}

static void test_teardown() {
  // Clean up all the things so tests are atomic
  pebble_mock_text_layer_set_text(NULL);
  pebble_mock_text_layer_create(NULL);
  pebble_mock_app_message_register_inbox_received(NULL);
  pebble_mock_dict_read_first(NULL);
  pebble_mock_dict_read_next(NULL);

  free(s_text_layer_text);
  free(s_weather_layer);
}

void weather_layer_test_fixture(void) {
  test_fixture_start();      
  fixture_setup(test_setup);
  fixture_teardown(test_teardown);

  run_test(test_should_display_saved_weather_given_recent_data);   
  run_test(test_should_display_updated_weather_given_no_recent_update);   
  run_test(test_should_delay_update_retries_given_no_response);   

  test_fixture_end();       
}
