#ifndef CONFIG_H
#define CONFIG_H

#include <pebble.h>

#define CONFIG_STORAGE_VERSION 1

typedef struct {
  char weather_conditions[50];
  int weather_temp;
  time_t weather_last_updated;
  bool weather_enabled;
} Config;

void config_init(void);
void config_deinit(void);
Config* config();
bool config_is_loaded(void);

#endif
