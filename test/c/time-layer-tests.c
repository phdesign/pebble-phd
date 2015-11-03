#include <pebble.h>
#include "seatest.h"
//#include "time-layer.h"

static bool mock_clock_is_24h_style_returns_false() {
  return false;
}

//static void test_should_update_time_layer_when_time_update_called() {
  //struct tm *testtime = {
    //.tm_year = 2015 - 1900,
    //.tm_mon = 10 - 1,
    //.tm_mday = 15,
    //.tm_hour = 10 - 1,
    //.tm_min = 0
  //};

  //pebble_mock(clock_is_24h_style, mock_clock_is_24h_style);
  //pebblemock_mock(clock_is_24h_style, false);

  //time_update(testtime);

  //pebblemock_expect(text_layer_set_text)->to_have_been_called_with(anything, "10:00");
//}

//static void test_should_use_24_hour_time_when_24_hour_setting_enabled() {

//}

static void test_mock() {
  mock_clock_is_24h_style(mock_clock_is_24h_style_returns_false);
  assert_false(clock_is_24h_style());
}

void time_layer_test_fixture(void) {
  test_fixture_start();      
  //run_test(test_should_update_time_layer_when_time_update_called);   
  //run_test(test_should_use_24_hour_time_when_24_hour_setting_enabled);   
  run_test(test_mock);   
  test_fixture_end();       
}
