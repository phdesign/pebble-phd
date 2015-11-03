#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pebble.h"

// Wall Time
// ================

bool (*mock_clock_is_24h_style)() = NULL;
void pebble_mock_clock_is_24h_style(bool (*mock)()) {
  mock_clock_is_24h_style = mock;
}

bool clock_is_24h_style(void) {
  if (mock_clock_is_24h_style != NULL)
    return mock_clock_is_24h_style();
  return false;
}
// Layers
// ================

void layer_add_child(Layer * parent, Layer * child ) {
}

// Text Layer
// ================

void (*mock_text_layer_set_text)(TextLayer * text_layer, const char * text);
void pebble_mock_text_layer_set_text(void (*mock)(TextLayer * text_layer, const char * text)) {
  mock_text_layer_set_text = mock;
}

TextLayer* text_layer_create(struct GRect frame) {
  return NULL;
}

void text_layer_destroy(TextLayer * text_layer) {
}

Layer* text_layer_get_layer(TextLayer * text_layer) {
  return NULL;
}

void text_layer_set_background_color(TextLayer * text_layer, enum GColor color) {
}

void text_layer_set_font(TextLayer * text_layer, GFont font ) {
}

void text_layer_set_text(TextLayer * text_layer, const char * text ) {
  if (mock_text_layer_set_text != NULL)
    mock_text_layer_set_text(text_layer, text);
}

void text_layer_set_text_alignment(TextLayer * text_layer, enum GTextAlignment text_alignment ) {
}

void text_layer_set_text_color(TextLayer * text_layer, enum GColor color ) {
}

// Window
// =================

Layer* window_get_root_layer(const Window * window) {
  return NULL;
}

// Storage
// =================

int32_t (*mock_persist_read_int)(const uint32_t key) = NULL;
void pebble_mock_persist_read_int(int (*mock)(const uint32_t key)) {
  mock_persist_read_int = mock;
}

status_t (*mock_persist_delete)(const uint32_t key) = NULL;
void pebble_mock_persist_delete(status_t (*mock)(const uint32_t key)) {
  mock_persist_delete = mock;
}

int (*mock_persist_read_data)(const uint32_t key, void * buffer, const size_t buffer_size) = NULL;
void pebble_mock_persist_read_data(int (*mock)(const uint32_t key, void * buffer, const size_t buffer_size)) {
  mock_persist_read_data = mock;
}

bool (*mock_persist_exists)(const uint32_t key) = NULL;
void pebble_mock_persist_exists(bool (*mock)(const uint32_t key)) {
  mock_persist_exists = mock;
}

status_t persist_delete(const uint32_t key) {
  if (mock_persist_delete != NULL)
    return mock_persist_delete(key);
  return S_SUCCESS;
}

bool persist_exists(const uint32_t key) {
  if (mock_persist_exists != NULL)
    return mock_persist_exists(key);
  return true;
}

int persist_get_size(const uint32_t key) {
  return 1;
}

bool persist_read_bool(const uint32_t key) {
  return true;
}

int persist_read_data(const uint32_t key, void * buffer, const size_t buffer_size) {
  if (mock_persist_read_data != NULL)
    return mock_persist_read_data(key, buffer, buffer_size);
  return E_DOES_NOT_EXIST;
}

int32_t persist_read_int(const uint32_t key) {
  if (mock_persist_read_int != NULL)
    return mock_persist_read_int(key);
  return 1;
}

int persist_read_string(const uint32_t key, char * buffer, const size_t buffer_size) {
  return 1;
}

status_t persist_write_bool(const uint32_t key, const bool value) {
  return S_SUCCESS;
}

int persist_write_data(const uint32_t key, const void * data, const size_t size) {
  return 1;
}

status_t persist_write_int(const uint32_t key, const int32_t value) {
  return S_SUCCESS;
}

int persist_write_string(const uint32_t key, const char * cstring) {
  return 1;
}

// Logging
// =================

void app_log(uint8_t log_level, const char * src_filename, int src_line_number, const char * fmt, ...) {
  char msg[255] = "";
  char level[8] = "";

  va_list args;
  va_start(args, fmt);
  vsnprintf(msg, sizeof(msg), fmt, args);
  va_end(args);
  
  switch (log_level) {
    case APP_LOG_LEVEL_ERROR:
      strcpy(level, "error");
      break;
    case APP_LOG_LEVEL_WARNING:
      strcpy(level, "warning");
      break;
    case APP_LOG_LEVEL_INFO:
      strcpy(level, "info");
      break;
    case APP_LOG_LEVEL_DEBUG:
      strcpy(level, "debug");
      break;
    case APP_LOG_LEVEL_DEBUG_VERBOSE:
      strcpy(level, "verbose");
      break;
  }

  printf("%s:%d: %s: %s\n", src_filename, src_line_number, level, msg);
}
