/*******************************
 * Time Layer
 *******************************/

#include "PHD.h"
#include "appinfo.h"
#include "TimeLayer.h"

static TextLayer *s_time_layer;

void time_update(struct tm *tick_time) {
  // Create a long-lived buffer
  static char time_buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, time_buffer);
}

void time_window_load(Window *window) {
  // Create and add the time layer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, g_large_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

void time_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}

