#include <pebble.h>
#include <stdlib.h>
#include <string.h>
#include "seatest.h"
#include "time-layer.h"

static bool s_clock_is_24h_style = false;
static char *s_text_layer_text = NULL;

static bool mock_clock_is_24h_style() {
  return s_clock_is_24h_style;
}

static void mock_text_layer_set_text(TextLayer * text_layer, const char * text) {
  if (s_text_layer_text != NULL)
    free(s_text_layer_text);

  s_text_layer_text = malloc(strlen(text) + 1);
  strcpy(s_text_layer_text, text);
}

static void test_should_update_time_layer_when_time_update_called() {
  struct tm test_time = {
    .tm_year = 2015 - 1900,
    .tm_mon = 10 - 1,
    .tm_mday = 15,
    .tm_hour = 10 - 1,
    .tm_min = 0
  };
  s_clock_is_24h_style = false;

  time_update(&test_time);

  assert_string_equal("10:00", s_text_layer_text);
  free(s_text_layer_text);
}

//static void test_should_use_24_hour_time_when_24_hour_setting_enabled() {

//}

static void test_mock() {
  pebble_mock_clock_is_24h_style(mock_clock_is_24h_style);
  assert_false(clock_is_24h_style());
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
  //run_test(test_should_use_24_hour_time_when_24_hour_setting_enabled);   
  run_test(test_mock);   

  test_fixture_end();       
}
