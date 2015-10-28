#include "seatest.h"
#include "sandbox.h"

void all_tests(void) {
  sandbox_test_fixture();
}

int main(void) {
  return run_tests(all_tests);
}
