/*******************************
 * Date Layer
 *******************************/

#include "PHD.h"
#include "DateLayer.h"

static TextLayer *s_date_layer;

void date_update(struct tm *tick_time) {
  static char date_buffer[] = "MON 00";
  strftime(date_buffer, sizeof("MON 00"), "%a %e", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
}

void date_window_load(Window *window) {
  // Create and add the date layer
  s_date_layer = text_layer_create(GRect(0, 110, 144, 20));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, g_medium_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

void date_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
}

