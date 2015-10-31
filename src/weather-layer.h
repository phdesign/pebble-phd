#pragma once

#include <pebble.h>

// Weather is stale and shouldn't be displayed after it's an hour old
#define WEATHER_EXPIRY_MINS 60
// Update weather every 15 mins
#define WEATHER_UPDATE_MINS 15

void weather_update(struct tm *tick_time);
void weather_window_load(Window *window);
void weather_window_unload(Window *window);
void weather_init();
void weather_deinit();
