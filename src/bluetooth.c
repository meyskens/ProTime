#include "bluetooth.h"
#include "settings.h"

static Window *s_connected_window;
static Window *s_disconnected_window;
static BitmapLayer *s_connected_layer;
static BitmapLayer *s_disconnected_layer;
static GBitmap *s_connected_bitmap;
static GBitmap *s_disconnected_bitmap;

static void connected_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  s_connected_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  s_connected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHCONNECTED);
  bitmap_layer_set_bitmap(s_connected_layer, s_connected_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_connected_layer));
}

static void connected_window_unload(Window *window) {
  gbitmap_destroy(s_connected_bitmap);
  bitmap_layer_destroy(s_connected_layer);
}

static void disconnected_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  s_disconnected_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  s_disconnected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHDISCONNECTED);
  bitmap_layer_set_bitmap(s_disconnected_layer, s_disconnected_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_disconnected_layer));
}

static void disconnected_window_unload(Window *window) {
  gbitmap_destroy(s_disconnected_bitmap);
  bitmap_layer_destroy(s_disconnected_layer);
}

static void hide_connected_window(void *data) {
  window_stack_remove(s_connected_window, true);
}

static void hide_disconnected_window(void *data) {
  window_stack_remove(s_disconnected_window, true);
}

static void handle_connected(void) {
  window_stack_remove(s_disconnected_window, true);
  window_stack_push(s_connected_window, true);
  vibes_long_pulse();
  app_timer_register(2000, hide_connected_window, NULL);
  light_enable_interaction();
}

static void handle_disconnected(void) {
  window_stack_remove(s_connected_window, true);
  window_stack_push(s_disconnected_window, true);
  
  uint32_t vibes_pulse_segments[] = { 200, 100, 200, 100, 200, 100, 200, 100, 200 };
  VibePattern vibes_pulse_pattern = {
    .durations = vibes_pulse_segments,
    .num_segments = ARRAY_LENGTH(vibes_pulse_segments),
  };
  
  vibes_enqueue_custom_pattern(vibes_pulse_pattern);
  light_enable_interaction();
  app_timer_register(5000, hide_disconnected_window, NULL);
}

void bluetooth_connection_callback(bool is_connected) {
  if (!settings_bluetooth_detection_enabled()) return;
  
  if (is_connected) {
    handle_connected();
  } else {
    handle_disconnected();
  }
}

void bluetooth_startup_check(void) {
  if (!settings_bluetooth_detection_enabled()) return;
  
  if (!bluetooth_connection_service_peek()) {
    handle_disconnected();
  }
}

void bluetooth_init(void) {
  s_connected_window = window_create();
  window_set_window_handlers(s_connected_window, (WindowHandlers) {
    .load = connected_window_load,
    .unload = connected_window_unload,
  });
  
  s_disconnected_window = window_create();
  window_set_window_handlers(s_disconnected_window, (WindowHandlers) {
    .load = disconnected_window_load,
    .unload = disconnected_window_unload,
  });
  
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
}

void bluetooth_deinit(void) {
  bluetooth_connection_service_unsubscribe();
  window_destroy(s_connected_window);
  window_destroy(s_disconnected_window);
}
