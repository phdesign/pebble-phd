#include <pebble.h>
#include "seatest.h"
#include "config.h"
#include "appinfo.h"

typedef struct {
  char weather_conditions[50];
  int weather_temp;
  time_t weather_last_updated;
  bool weather_enabled;
} Config_v1;

static int s_read_int_result = 0;

static bool mock_persist_exists_returns_false(const uint32_t key) {
  return false;
}

static int mock_persist_read_data(const uint32_t key, void * buffer, const size_t buffer_size) {
  if (key == PERSIST_KEY_CONFIG) {
    if (s_read_int_result == 1) {
      *(Config_v1*)(buffer) = (Config_v1) {
        .weather_conditions = "Windy",
        .weather_temp = 14,
        .weather_last_updated = mktime(&(struct tm) {
            .tm_year = 2015 - 1900,
            .tm_mon = 10 - 1,
            .tm_mday = 15,
            .tm_hour = 10 - 1,
            .tm_min = 0
            }),
        .weather_enabled = true
      };
    } 
    if (s_read_int_result == 2) {
      *(Config*)(buffer) = (Config) {
        .weather_conditions = "Windy",
        .weather_temp = 14,
        .weather_last_updated = mktime(&(struct tm) {
            .tm_year = 2015 - 1900,
            .tm_mon = 10 - 1,
            .tm_mday = 15,
            .tm_hour = 10 - 1,
            .tm_min = 0
            }),
        .weather_enabled = true,
        .weather_temp_unit = TEMP_UNIT_FAHRENHEIT
      };
    }
    return sizeof(Config);
  }
  
  return E_DOES_NOT_EXIST;
}

static int32_t mock_persist_read_int(const uint32_t key) {
  return s_read_int_result;
}

static void test_should_return_defaults_given_current_storage_version_but_no_saved_data() {
  s_read_int_result = 2;
  pebble_mock_persist_exists(mock_persist_exists_returns_false);
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
  assert_string_equal("", config()->weather_conditions);
  assert_ulong_equal(0, config()->weather_last_updated);
  assert_int_equal(TEMP_UNIT_CELSIUS, config()->weather_temp_unit);
  pebble_mock_persist_exists(NULL);
}

static void test_should_return_stored_config_given_current_storage_version() {
  s_read_int_result = 2;
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(14, config()->weather_temp);
  assert_string_equal("Windy", config()->weather_conditions);
  assert_ulong_equal(1444863600, config()->weather_last_updated);
  assert_int_equal(TEMP_UNIT_FAHRENHEIT, config()->weather_temp_unit);
}

static void test_should_return_defaults_given_unrecognised_storage_version() {
  s_read_int_result = 99;
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
  assert_string_equal("", config()->weather_conditions);
  assert_ulong_equal(0, config()->weather_last_updated);
  assert_int_equal(TEMP_UNIT_CELSIUS, config()->weather_temp_unit);
}

static void test_should_return_defaults_given_no_saved_data() {
  s_read_int_result = 0;
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(0, config()->weather_temp);
  assert_string_equal("", config()->weather_conditions);
  assert_ulong_equal(0, config()->weather_last_updated);
  assert_int_equal(TEMP_UNIT_CELSIUS, config()->weather_temp_unit);
}

static void test_should_migrate_v1_data_to_v2() {
  s_read_int_result = 1;
  config_init();
  assert_true(config()->weather_enabled);
  assert_int_equal(14, config()->weather_temp);
  assert_string_equal("Windy", config()->weather_conditions);
  assert_ulong_equal(1444863600, config()->weather_last_updated);
  assert_int_equal(TEMP_UNIT_CELSIUS, config()->weather_temp_unit);
}

static void test_setup() {
  pebble_mock_persist_read_data(mock_persist_read_data);
  pebble_mock_persist_read_int(mock_persist_read_int);

  // Reset config
  *(config()) = (Config){
    .weather_enabled = true,
    .weather_temp_unit = TEMP_UNIT_CELSIUS
  };
}

static void test_teardown() {
  pebble_mock_persist_read_data(NULL);
  pebble_mock_persist_read_int(NULL);
}

void config_test_fixture(void) {
  test_fixture_start();
  fixture_setup(test_setup);
  fixture_teardown(test_teardown);

  // Tests must be in correct order because each test is NOT atomic, if the test changes
  // data in the target then it changes it for all tests
  run_test(test_should_return_defaults_given_unrecognised_storage_version);
  run_test(test_should_return_defaults_given_current_storage_version_but_no_saved_data);
  run_test(test_should_return_stored_config_given_current_storage_version);
  run_test(test_should_return_defaults_given_no_saved_data);
  run_test(test_should_migrate_v1_data_to_v2);

  test_fixture_end();
}
