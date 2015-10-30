/*******************************
 * Utils
 *******************************/

#include "utils.h"

// Note that this returns a static string, it will be changed the next time
// it is called so copy to another memory address
char* format_date_time(time_t datetime) {
  struct tm *temp_time = localtime(&datetime);
  static char str_time[18];
  strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M", temp_time);
  return str_time;
}
