#ifndef CONFIG_H
#define CONFIG_H

#include <pebble.h>

typedef struct {
  char weather_conditions[50];
  int weather_temp;
  time_t weather_last_updated;
  bool weather_enabled;
  bool is_loaded;
} Config;

void config_init(void);
void config_deinit(void);
Config* config();

#endif
