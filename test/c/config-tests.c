#include <pebble.h>
#include "seatest.h"
#include "config.h"

static int mock_persist_read_data_returns_stored_data(const uint32_t key, void * buffer, const size_t buffer_size) {
  buffer = &(Config) {
    .weather_conditions = "Windy",
    .weather_temp = 14,
    //.weather_last_updated = 
    .weather_enabled = true
  };
  return 1;
}

static int32_t mock_persist_read_int_returns_3(const uint32_t key) {
  return 3;
}

static int32_t mock_persist_read_int_returns_1(const uint32_t key) {
  return 1;
}

static int32_t mock_persist_read_int_returns_0(const uint32_t key) {
  return 0;
}

static void test_should_return_stored_config_given_current_storage_version() {
  mock_persist_read_data(mock_persist_read_data_returns_stored_data);
  mock_persist_read_int(mock_persist_read_int_returns_1);
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
}

static void test_should_return_defaults_given_unrecognised_storage_version() {
  mock_persist_read_int(mock_persist_read_int_returns_3);
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
}

static void test_should_return_defaults_given_no_saved_data() {
  mock_persist_read_int(mock_persist_read_int_returns_0);
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
}

void config_test_fixture(void) {
  test_fixture_start();
  run_test(test_should_return_defaults_given_no_saved_data);
  run_test(test_should_return_defaults_given_unrecognised_storage_version);
  run_test(test_should_return_stored_config_given_current_storage_version);
  test_fixture_end();
}
