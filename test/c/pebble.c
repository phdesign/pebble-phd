#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pebble.h"

// Wall Time
// ================

bool (*mocked_clock_is_24h_style)() = NULL;

void mock_clock_is_24h_style(bool (*mock)()) {
  mocked_clock_is_24h_style = mock;
}

bool clock_is_24h_style(void) {
  if (mocked_clock_is_24h_style != NULL)
    return mocked_clock_is_24h_style();
  return false;
}

// Storage
// =================

int32_t (*mocked_persist_read_int)(const uint32_t key) = NULL;
status_t (*mocked_persist_delete)(const uint32_t key) = NULL;
int (*mocked_persist_read_data)(const uint32_t key, void * buffer, const size_t buffer_size) = NULL;

void mock_persist_read_int(int (*mock)(const uint32_t key)) {
  mocked_persist_read_int = mock;
}

void mock_persist_delete(status_t (*mock)(const uint32_t key)) {
  mocked_persist_delete = mock;
}

void mock_persist_read_data(int (*mock)(const uint32_t key, void * buffer, const size_t buffer_size)) {
  mocked_persist_read_data = mock;
}

status_t persist_delete(const uint32_t key) {
  if (mocked_persist_delete != NULL)
    return mocked_persist_delete(key);
  return S_SUCCESS;
}

bool persist_exists(const uint32_t key) {
  return true;
}

int persist_get_size(const uint32_t key) {
  return 1;
}

bool persist_read_bool(const uint32_t key) {
  return true;
}

int persist_read_data(const uint32_t key, void * buffer, const size_t buffer_size) {
  if (mocked_persist_read_data != NULL)
    return mocked_persist_read_data(key, buffer, buffer_size);
  return 1;
}

int32_t persist_read_int(const uint32_t key) {
  if (mocked_persist_read_int != NULL)
    return mocked_persist_read_int(key);
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
