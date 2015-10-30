#pragma once

#include <pebble.h>

void time_update(struct tm *tick_time);
void time_window_load(Window *window);
void time_window_unload(Window *window);
