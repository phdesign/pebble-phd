#include <stdio.h>
#include <stdint.h>
#include <time.h>

static void print_date(time_t datetime) {
  struct tm *temp_time = localtime(&datetime);
  char str_time[40];

  strftime(str_time, 40, "The current time is %a %e %b %H:%M\n", temp_time);
  printf("%s", str_time);
}

int main(void) {
  int number = 1;
  int8_t shorter = 3;

  printf("Here's a number: %d. And another: %d\n", number, shorter);
  printf("size of long %lo\n", sizeof(long));
  printf("size of time_t: %lo\n", sizeof(time_t));

  time_t now = time(NULL);
  print_date(now);

  //time_t s_last_updated = mktime((struct tm) {
    //.tm_year = 2015,
    //.tm_mon = 10,
    //.tm_mday = 15,
    //.tm_hour = 10,
    //.tm_min = 0
  //});

  return 0;
}
