#ifndef DATE_LAYER_H
#define DATE_LAYER_H

#include <pebble.h>

void date_update(struct tm *tick_time);
void date_window_load(Window *window);
void date_window_unload(Window *window);

#endif
