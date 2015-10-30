#include "seatest.h"
#include "sandbox-tests.h"
#include "time-layer-tests.h"

void all_tests(void) {
  sandbox_test_fixture();
  time_layer_test_fixture();
}

int main(void) {
  return run_tests(all_tests);
}
