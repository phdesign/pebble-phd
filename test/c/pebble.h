#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

// Dictionary
// ================

typedef enum {
  DICT_OK = 0,
  DICT_NOT_ENOUGH_STORAGE = 1 << 1,
  DICT_INVALID_ARGS = 1 << 2,
  DICT_INTERNAL_INCONSISTENCY = 1 << 3,
  DICT_MALLOC_FAILED = 1 << 4,
} DictionaryResult;

typedef enum {
  TUPLE_BYTE_ARRAY = 0,
  TUPLE_CSTRING = 1,
  TUPLE_UINT = 2,
  TUPLE_INT = 3,
} TupleType;

// Typedefs
typedef struct __attribute__((__packed__)) {
  uint32_t key;
  TupleType type:8;
  uint16_t length;
  union {
    uint8_t data[0];
    char cstring[0];
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    int8_t int8;
    int16_t int16;
    int32_t int32;
  } value[];
} Tuple;

struct Dictionary;
typedef struct Dictionary Dictionary;

typedef struct {
  Dictionary *dictionary;  
  const void *end;  
  Tuple *cursor;
} DictionaryIterator;

DictionaryResult dict_write_uint8(DictionaryIterator *iter, const uint32_t key, const uint8_t value);
Tuple * dict_read_first(DictionaryIterator *iter);
Tuple * dict_read_next(DictionaryIterator *iter);

// Mocks
void pebble_mock_dict_read_first(Tuple * (*mock)(DictionaryIterator *iter));
void pebble_mock_dict_read_next(Tuple * (*mock)(DictionaryIterator *iter));

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
void layer_set_hidden(Layer *layer, bool hidden);

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
void pebble_mock_text_layer_create(TextLayer* (*mock_text_layer_create)(struct GRect frame));

// Window
// ================

// Typedefs
typedef void* Window;

// Functions
Layer* window_get_root_layer(const Window * window);

// App Message
// ================

// Typedefs
typedef enum {
  APP_MSG_OK = 0,
  APP_MSG_SEND_TIMEOUT = 1 << 1,
  APP_MSG_SEND_REJECTED = 1 << 2,
  APP_MSG_NOT_CONNECTED = 1 << 3,
  APP_MSG_APP_NOT_RUNNING = 1 << 4,
  APP_MSG_INVALID_ARGS = 1 << 5,
  APP_MSG_BUSY = 1 << 6,
  APP_MSG_BUFFER_OVERFLOW = 1 << 7,
  APP_MSG_ALREADY_RELEASED = 1 << 9,
  APP_MSG_CALLBACK_ALREADY_REGISTERED = 1 << 10,
  APP_MSG_CALLBACK_NOT_REGISTERED = 1 << 11,
  APP_MSG_OUT_OF_MEMORY = 1 << 12,
  APP_MSG_CLOSED = 1 << 13,
  APP_MSG_INTERNAL_ERROR = 1 << 14,
} AppMessageResult;

typedef void (*AppMessageInboxReceived)(DictionaryIterator *iterator, void *context);
typedef void (*AppMessageInboxDropped)(AppMessageResult reason, void *context);
typedef void (*AppMessageOutboxSent)(DictionaryIterator *iterator, void *context);
typedef void (*AppMessageOutboxFailed)(DictionaryIterator *iterator, AppMessageResult reason, void *context);

AppMessageResult app_message_open(const uint32_t size_inbound, const uint32_t size_outbound);
void app_message_deregister_callbacks(void);
AppMessageInboxReceived app_message_register_inbox_received(AppMessageInboxReceived received_callback);
AppMessageInboxDropped app_message_register_inbox_dropped(AppMessageInboxDropped dropped_callback);
AppMessageOutboxSent app_message_register_outbox_sent(AppMessageOutboxSent sent_callback);
AppMessageOutboxFailed app_message_register_outbox_failed(AppMessageOutboxFailed failed_callback);
AppMessageResult app_message_outbox_begin(DictionaryIterator **iterator);
AppMessageResult app_message_outbox_send(void);

// Macros
#define APP_MESSAGE_INBOX_SIZE_MINIMUM 124
#define APP_MESSAGE_OUTBOX_SIZE_MINIMUM 636

// Mocks
void pebble_mock_app_message_register_inbox_received(AppMessageInboxReceived (*mock)(AppMessageInboxReceived received_callback));
void pebble_mock_app_message_outbox_send(AppMessageResult (*mock)(void));

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
void pebble_mock_enable_logs(bool enabled);
