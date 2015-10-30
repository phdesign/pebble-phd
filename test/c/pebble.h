#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Macros
// ================

#define GSizeZero   struct GSize(0, 0)
#define GSize( w, h )   ((struct GSize){(w), (h)})
#define GRectZero   struct GRect(0, 0, 0, 0)
#define GRect( x, y, w, h )   ((struct GRect){{(x), (y)}, {(w), (h)}})
#define GPointZero   struct GPoint(0, 0)
#define GPoint( x, y )   ((struct GPoint){(x), (y)})

// Typedefs
// ================

typedef void* Window;
typedef void* Layer;
typedef void* TextLayer;
typedef void* GFont;

// Structs
// ================

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
// ================

enum GColor {
  GColorClear = 1,
  GColorBlack = 2,
  GColorWhite = 3
};

enum GTextAlignment {
  GTextAlignmentLeft = 1,
  GTextAlignmentCenter = 2,
  GTextAlignmentRight = 3
};

// Window
// ================

Layer* window_get_root_layer(const Window * window);

// Wall Time
// ================

bool clock_is_24h_style(void);

// Text Layer
// ================

TextLayer* text_layer_create(struct GRect frame);
void text_layer_destroy(TextLayer * text_layer);
Layer* text_layer_get_layer(TextLayer * text_layer);
void text_layer_set_background_color(TextLayer * text_layer, enum GColor color);
void text_layer_set_font(TextLayer * text_layer, GFont font );
void text_layer_set_text(TextLayer * text_layer, const char * text );
void text_layer_set_text_alignment(TextLayer * text_layer, enum GTextAlignment text_alignment );
void text_layer_set_text_color(TextLayer * text_layer, enum GColor color );

// Layers
// =================

void layer_add_child(Layer * parent, Layer * child );

// Mocks
// =================

void mock_clock_is_24h_style(bool (*mock_callback)());
