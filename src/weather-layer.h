#ifndef WEATHER_LAYER_H
#define WEATHER_LAYER_H

#include <pebble.h>

#define WEATHER_EXPIRY_SECS 60 * 60
#define WEATHER_UPDATE_MINS 15

void weather_update(struct tm *tick_time);
void weather_window_load(Window *window);
void weather_window_unload(Window *window);
void weather_init();
void weather_deinit();

#endif
