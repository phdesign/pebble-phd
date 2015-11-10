#include <pebble.h>
#include "seatest.h"
#include "time-layer.h"

static bool s_clock_is_24h_style = false;
static char *s_text_layer_text = NULL;

static bool mock_clock_is_24h_style() {
  return s_clock_is_24h_style;
}

static void mock_text_layer_set_text(TextLayer * text_layer, const char * text) {
  s_text_layer_text = malloc(strlen(text) + 1);
  strcpy(s_text_layer_text, text);
}

static void test_should_update_time_layer_when_time_update_called() {
  struct tm test_time = {
    .tm_hour = 22,
    .tm_min = 0
  };
  s_clock_is_24h_style = false;

  time_update(&test_time);

  assert_string_equal("10:00", s_text_layer_text);
  free(s_text_layer_text);
}

static void test_should_use_24_hour_time_when_24_hour_setting_enabled() {
  struct tm test_time = {
    .tm_hour = 22,
    .tm_min = 0
  };
  s_clock_is_24h_style = true;

  time_update(&test_time);

  assert_string_equal("22:00", s_text_layer_text);
  free(s_text_layer_text);
}

static void test_setup() {
  pebble_mock_clock_is_24h_style(mock_clock_is_24h_style);
  pebble_mock_text_layer_set_text(mock_text_layer_set_text);
}

static void test_teardown() {
  pebble_mock_clock_is_24h_style(NULL);
  pebble_mock_text_layer_set_text(NULL);
}

void time_layer_test_fixture(void) {
  test_fixture_start();      
  fixture_setup(test_setup);
  fixture_teardown(test_teardown);

  run_test(test_should_update_time_layer_when_time_update_called);   
  run_test(test_should_use_24_hour_time_when_24_hour_setting_enabled);   

  test_fixture_end();       
}
