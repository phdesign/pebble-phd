/*******************************
 * PHD Watchface
 *******************************/

#include "PHD.h"
#include "TimeLayer.h"
#include "DateLayer.h"
#include "WeatherLayer.h"

static Window *s_main_window;

GFont g_large_font;
GFont g_medium_font;
GFont g_small_font;

static void main_window_load(Window *window) {
  window_set_background_color(window, GColorBlack);

  // Load fonts
  g_large_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_36));
  g_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_20));
  g_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_16));

  time_window_load(window);
  date_window_load(window);
  weather_window_load(window);
}

static void main_window_unload(Window *window) {
  tick_timer_service_unsubscribe();

  time_window_unload(window);
  date_window_unload(window);
  weather_window_unload(window);
  
  fonts_unload_custom_font(g_large_font);
  fonts_unload_custom_font(g_medium_font);
  fonts_unload_custom_font(g_small_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_update(tick_time);
  date_update(tick_time);
  weather_update(tick_time);
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  time_update(tick_time);
  date_update(tick_time);

  weather_init();
}

static void deinit() {
  window_destroy(s_main_window);
  tick_timer_service_unsubscribe();
  weather_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
