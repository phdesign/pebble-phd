#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Graphics Types
// ================

// Macros
#define GSizeZero   struct GSize(0, 0)
#define GSize( w, h )   ((struct GSize){(w), (h)})
#define GRectZero   struct GRect(0, 0, 0, 0)
#define GRect( x, y, w, h )   ((struct GRect){{(x), (y)}, {(w), (h)}})
#define GPointZero   struct GPoint(0, 0)
#define GPoint( x, y )   ((struct GPoint){(x), (y)})

// Structs
struct GPoint {
  int16_t x;
  int16_t y;
};

struct GSize {
  int16_t h;
  int16_t w;
};

struct GRect {
  struct GPoint origin;
  struct GSize size;
};

// Enums
enum GColor {
  GColorClear,
  GColorBlack,
  GColorWhite
};

enum GTextAlignment {
  GTextAlignmentLeft,
  GTextAlignmentCenter,
  GTextAlignmentRight
};

// Fonts
// ================

// Typedefs
typedef void* GFont;

// Wall Time
// ================

// Functions
bool clock_is_24h_style(void);

// Mocks
void pebble_mock_clock_is_24h_style(bool (*mock)());

// Layers
// =================

typedef void* Layer;

void layer_add_child(Layer * parent, Layer * child );

// Text Layer
// ================

// Typedefs
typedef void* TextLayer;

// Functions
TextLayer* text_layer_create(struct GRect frame);
void text_layer_destroy(TextLayer * text_layer);
Layer* text_layer_get_layer(TextLayer * text_layer);
void text_layer_set_background_color(TextLayer * text_layer, enum GColor color);
void text_layer_set_font(TextLayer * text_layer, GFont font );
void text_layer_set_text(TextLayer * text_layer, const char * text );
void text_layer_set_text_alignment(TextLayer * text_layer, enum GTextAlignment text_alignment );
void text_layer_set_text_color(TextLayer * text_layer, enum GColor color );

// Mocks
void pebble_mock_text_layer_set_text(void (*mock)(TextLayer * text_layer, const char * text));

// Window
// ================

// Typedefs
typedef void* Window;

// Functions
Layer* window_get_root_layer(const Window * window);

// Storage
// =================

// Typedefs
typedef int32_t status_t;

// Enums
enum StatusCode {
  S_SUCCESS,
  E_ERROR,
  E_UNKNOWN,
  E_INTERNAL,
  E_INVALID_ARGUMENT,
  E_OUT_OF_MEMORY,
  E_OUT_OF_STORAGE,
  E_OUT_OF_RESOURCES,
  E_RANGE,
  E_DOES_NOT_EXIST,
  E_INVALID_OPERATION,
  E_BUSY,
  S_TRUE,
  S_FALSE,
  S_NO_MORE_ITEMS,
  S_NO_ACTION_REQUIRED
};

// Functions
status_t persist_delete(const uint32_t key);
bool persist_exists(const uint32_t key);
int persist_get_size(const uint32_t key);
bool persist_read_bool(const uint32_t key);
int persist_read_data(const uint32_t key, void * buffer, const size_t buffer_size);
int32_t persist_read_int(const uint32_t key);
int persist_read_string(const uint32_t key, char * buffer, const size_t buffer_size);
status_t persist_write_bool(const uint32_t key, const bool value);
int persist_write_data(const uint32_t key, const void * data, const size_t size);
status_t persist_write_int(const uint32_t key, const int32_t value);
int persist_write_string(const uint32_t key, const char * cstring);

// Mocks
void pebble_mock_persist_read_int(int32_t (*mock)(const uint32_t key));
void pebble_mock_persist_read_data(int (*mock)(const uint32_t key, void * buffer, const size_t buffer_size));
void pebble_mock_persist_exists(bool (*mock)(const uint32_t key));

// Logging
// =================

// Macros
#define APP_LOG(level, fmt, args...) app_log(level, __FILE__, __LINE__, fmt, ## args)

// Enums
enum AppLogLevel {
  APP_LOG_LEVEL_ERROR,
  APP_LOG_LEVEL_WARNING,
  APP_LOG_LEVEL_INFO,
  APP_LOG_LEVEL_DEBUG,
  APP_LOG_LEVEL_DEBUG_VERBOSE
};

// Functions
void app_log(uint8_t log_level, const char * src_filename, int src_line_number, const char * fmt, ...);
