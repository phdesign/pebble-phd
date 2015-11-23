#include <stdbool.h>
#include "seatest.h"
#include "sandbox-tests.h"
#include "time-layer-tests.h"
#include "weather-layer-tests.h"
#include "config-tests.h"

void all_tests(void) {
  sandbox_test_fixture();
  time_layer_test_fixture();
  weather_layer_test_fixture();
  config_test_fixture();
}

int main(void) {
  bool passed = run_tests(all_tests);
  return passed ? 0 : -1;
}
