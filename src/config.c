/*******************************
 * Config
 *******************************/

#include "main.h"
#include "appinfo.h"
#include "config.h"

typedef struct {
  char weather_conditions[50];
  int weather_temp;
  time_t weather_last_updated;
  bool weather_enabled;
} ConfigV1;

static Config s_config = {
  .weather_enabled = true,
  .is_loaded = false
};

void config_init(void) {
  if (persist_exists(PERSIST_CONFIG))
    persist_read_data(PERSIST_CONFIG, &s_config, sizeof(s_config));
  s_config.is_loaded = true;
}

void config_deinit(void) {
  persist_write_data(PERSIST_CONFIG, &s_config, sizeof(s_config));
}

Config* config() {
  return &s_config;
}
