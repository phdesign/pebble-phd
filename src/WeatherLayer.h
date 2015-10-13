#ifndef WEATHER_LAYER_H
#define WEATHER_LAYER_H

#include <pebble.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

void weather_update(struct tm *tick_time);
void weather_window_load(Window *window);
void weather_window_unload(Window *window);
void weather_init();

#endif
