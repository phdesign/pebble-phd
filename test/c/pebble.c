#include "pebble.h"

// Dictionary
// ===============

Tuple * (*mock_dict_read_first)(DictionaryIterator *iter) = NULL;
void pebble_mock_dict_read_first(Tuple * (*mock)(DictionaryIterator *iter)) {
  mock_dict_read_first = mock;
}

Tuple * (*mock_dict_read_next)(DictionaryIterator *iter) = NULL;
void pebble_mock_dict_read_next(Tuple * (*mock)(DictionaryIterator *iter)) {
  mock_dict_read_next = mock;
}

DictionaryResult dict_write_uint8(DictionaryIterator *iter, const uint32_t key, const uint8_t value) {
  return DICT_OK;
}

Tuple * dict_read_first(DictionaryIterator *iter) { 
  if (mock_dict_read_first != NULL)
    return mock_dict_read_first(iter);
  return NULL; 
}

Tuple * dict_read_next(DictionaryIterator *iter) {
  if (mock_dict_read_next != NULL)
    return mock_dict_read_next(iter);
  return NULL; 
}

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

void layer_add_child(Layer * parent, Layer * child ) { }
void layer_set_hidden(Layer *layer, bool hidden) { }

// Text Layer
// ================

void (*mock_text_layer_set_text)(TextLayer * text_layer, const char * text) = NULL;
void pebble_mock_text_layer_set_text(void (*mock)(TextLayer * text_layer, const char * text)) {
  mock_text_layer_set_text = mock;
}

TextLayer* (*mock_text_layer_create)(struct GRect frame) = NULL;
void pebble_mock_text_layer_create(TextLayer* (*mock)(struct GRect frame)) {
  mock_text_layer_create = mock;
}

TextLayer* text_layer_create(struct GRect frame) { 
  if (mock_text_layer_create != NULL)
    return mock_text_layer_create(frame);
  return NULL; 
}

void text_layer_destroy(TextLayer * text_layer) { }

Layer* text_layer_get_layer(TextLayer * text_layer) { return NULL; }

void text_layer_set_background_color(TextLayer * text_layer, enum GColor color) { }

void text_layer_set_font(TextLayer * text_layer, GFont font ) { }

void text_layer_set_text(TextLayer * text_layer, const char * text ) {
  if (mock_text_layer_set_text != NULL)
    mock_text_layer_set_text(text_layer, text);
}

void text_layer_set_text_alignment(TextLayer * text_layer, enum GTextAlignment text_alignment ) { }

void text_layer_set_text_color(TextLayer * text_layer, enum GColor color ) { }

// Window
// =================

Layer* window_get_root_layer(const Window * window) {
  return NULL;
}

// App Message
// =================

AppMessageInboxReceived (*mock_app_message_register_inbox_received)(AppMessageInboxReceived received_callback) = NULL;
void pebble_mock_app_message_register_inbox_received(AppMessageInboxReceived (*mock)(AppMessageInboxReceived received_callback)) {
  mock_app_message_register_inbox_received = mock;
}

AppMessageResult (*mock_app_message_outbox_send)(void) = NULL;
void pebble_mock_app_message_outbox_send(AppMessageResult (*mock)(void)) {
  mock_app_message_outbox_send = mock;
}

AppMessageResult app_message_open(const uint32_t size_inbound, const uint32_t size_outbound) {
  return APP_MSG_OK;
}

void app_message_deregister_callbacks(void) { }

AppMessageInboxReceived app_message_register_inbox_received(AppMessageInboxReceived received_callback) {
  if (mock_app_message_register_inbox_received != NULL)
    return mock_app_message_register_inbox_received(received_callback);
  return NULL;
}

AppMessageInboxDropped app_message_register_inbox_dropped(AppMessageInboxDropped dropped_callback) {
  return NULL;
}

AppMessageOutboxSent app_message_register_outbox_sent(AppMessageOutboxSent sent_callback) {
  return NULL;
}

AppMessageOutboxFailed app_message_register_outbox_failed(AppMessageOutboxFailed failed_callback) {
  return NULL;
}

AppMessageResult app_message_outbox_begin(DictionaryIterator **iterator) {
  return APP_MSG_OK;
}

AppMessageResult app_message_outbox_send(void) {
  if (mock_app_message_outbox_send != NULL)
    return mock_app_message_outbox_send();
  return APP_MSG_OK;
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

int persist_get_size(const uint32_t key) { return 1; }

bool persist_read_bool(const uint32_t key) { return true; }

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

static bool enable_logs = false;
void pebble_mock_enable_logs(bool enabled) {
  enable_logs = enabled;
}

void app_log(uint8_t log_level, const char * src_filename, int src_line_number, const char * fmt, ...) {
  if (!enable_logs) return;


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
