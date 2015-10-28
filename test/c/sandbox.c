#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "sandbox.h"
#include "seatest.h"

static void test_i_can_create_a_custom_time() {
  time_t custom_time = mktime(&(struct tm) {
    .tm_year = 2015 - 1900,
    .tm_mon = 10 - 1,
    .tm_mday = 15,
    .tm_hour = 10 - 1,
    .tm_min = 0
  });

  struct tm *temp_time = localtime(&custom_time);
  char str_time[50];
  strftime(str_time, sizeof(str_time), "%a %e %b %Y %H:%M", temp_time);
  
  assert_string_equal("Thu 15 Oct 2015 10:00", str_time);
}

static void test_size_of_time_t_same_as_long() {
  assert_int_equal(sizeof(long), sizeof(time_t));
}

static void test_should_print_number_given_a_short() {
  int8_t number = 1;
  char actual[2];

  sprintf(actual, "%d", number);
  assert_string_equal("1", actual);
}

static void test_should_print_number_given_an_int() {
  int number = 1;
  char actual[2];

  sprintf(actual, "%d", number);
  assert_string_equal("1", actual);
}

void sandbox_test_fixture(void) {
  test_fixture_start();      
  run_test(test_should_print_number_given_an_int);   
  run_test(test_should_print_number_given_a_short);   
  run_test(test_size_of_time_t_same_as_long);
  run_test(test_i_can_create_a_custom_time);
  test_fixture_end();       
}
