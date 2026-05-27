#include <pebble.h>
#include "main.h"
#include "config.h"
#include "settings.h"
#include "hands.h"
#include "bluetooth.h"
#include "app_message.h"

// UI Elements
static Window *s_window;
static Layer *s_hands_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static Layer *s_bg_color_layer;
static TextLayer *s_textlayer_date;
static GFont s_res_gothic_18_bold;

// Background color layer update proc for round face on square Pebble
static void bg_color_update_proc(Layer *layer, GContext *ctx) {
  #if !defined(PBL_ROUND)
    if (settings_is_round_mode()) {
      uint8_t r, g, b;
      parse_hex_color(g_settings.roundbackgroundcolor, &r, &g, &b);
      
      #ifdef PBL_COLOR
        graphics_context_set_fill_color(ctx, GColorFromRGB(r, g, b));
      #else
        uint8_t brightness = (r + g + b) / 3;
        graphics_context_set_fill_color(ctx, brightness > 127 ? GColorWhite : GColorBlack);
      #endif
      graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    }
  #endif
}

// Get the appropriate background resource ID based on dial color
static uint32_t get_background_resource(void) {
  if (settings_is_round_logo_mode()) {
    return RESOURCE_ID_IMAGE_NS_FACE_ROUND_LOGO;
  }
  return RESOURCE_ID_IMAGE_NS_FACE_ROUND;
}

// Set window background color based on settings
static void update_window_background(void) {
  #if !defined(PBL_ROUND)
    if (settings_is_round_mode()) {
      uint8_t r, g, b;
      parse_hex_color(g_settings.roundbackgroundcolor, &r, &g, &b);
      #ifdef PBL_COLOR
        window_set_background_color(s_window, GColorFromRGB(r, g, b));
      #else
        uint8_t brightness = (r + g + b) / 3;
        window_set_background_color(s_window, brightness > 127 ? GColorWhite : GColorBlack);
      #endif
    } else {
      window_set_background_color(s_window, GColorBlack);
    }
  #else
    window_set_background_color(s_window, GColorBlack);
  #endif
}

// Update the display based on current settings
static void update_display(void) {
  // Reload background image
  gbitmap_destroy(s_background_bitmap);
  s_background_bitmap = gbitmap_create_with_resource(get_background_resource());
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  
  update_window_background();
  
  // Mark background color layer dirty to redraw
  if (s_bg_color_layer) {
    layer_mark_dirty(s_bg_color_layer);
  }
  
  // Update date visibility
  layer_set_hidden((Layer *)s_textlayer_date, !settings_show_date());
  
  // Update date text color
  text_layer_set_text_color(s_textlayer_date, hands_get_date_color());
  
  // Mark hands layer dirty to redraw
  layer_mark_dirty(s_hands_layer);
}

// Hands layer update procedure
static void hands_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  // Update date text
  if (settings_show_date()) {
    static char date[3];
    strftime(date, sizeof(date), "%d", t);
    text_layer_set_text(s_textlayer_date, date);
  }
  
  hands_draw(layer, ctx, t);
}

// Tick handler
void handle_tick(struct tm *t, TimeUnits units_changed) {
  if (settings_is_stop2go()) {
    for (int i = 0; i < STOP2GO_TICK_RESOLUTION; i++) {
      app_timer_register(1000 / STOP2GO_TICK_RESOLUTION * i, 
        (void*)layer_mark_dirty, window_get_root_layer(s_window));
    }
  } else {
    layer_mark_dirty(window_get_root_layer(s_window));
  }
  
  // Hourly vibration
  if (t->tm_min == 0 && t->tm_sec == 0) {
    if (strcmp(g_settings.hourlyvibration, "short") == 0) {
      vibes_short_pulse();
    } else if (strcmp(g_settings.hourlyvibration, "long") == 0) {
      vibes_long_pulse();
    } else if (strcmp(g_settings.hourlyvibration, "double") == 0) {
      vibes_double_pulse();
    }
  }
}

// Window load handler
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "window_load: dialcolor='%s' roundbackgroundcolor='%s'", 
          g_settings.dialcolor, g_settings.roundbackgroundcolor);
  
  update_window_background();
  
  // Create background color layer
  s_bg_color_layer = layer_create(bounds);
  layer_set_update_proc(s_bg_color_layer, bg_color_update_proc);
  layer_add_child(window_layer, s_bg_color_layer);
  
  // Load background image
  s_background_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  s_background_bitmap = gbitmap_create_with_resource(get_background_resource());
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  bitmap_layer_set_background_color(s_background_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Initialize date layer
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  static char date[3];
  strftime(date, sizeof(date), "%d", t);

  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

  // Position date layer
  if (settings_is_round_mode()) {
    s_textlayer_date = text_layer_create(GRect(bounds.size.w / 2 + 25, bounds.size.h / 2 - 10, 22, 20));
  } else {
    s_textlayer_date = text_layer_create(GRect(102, 72, 22, 20));
  }

  text_layer_set_text_color(s_textlayer_date, hands_get_date_color());
  text_layer_set_background_color(s_textlayer_date, GColorClear);
  text_layer_set_text(s_textlayer_date, date);
  text_layer_set_text_alignment(s_textlayer_date, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_date, s_res_gothic_18_bold);
  layer_add_child(window_layer, (Layer *)s_textlayer_date);
  
  layer_set_hidden((Layer *)s_textlayer_date, !settings_show_date());
  
  // Initialize hands layer
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
  
  // Initialize hands
  hands_init(center);
}

// Window unload handler
static void window_unload(Window *window) {
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  layer_destroy(s_bg_color_layer);
  layer_destroy(s_hands_layer);
  text_layer_destroy(s_textlayer_date);
  hands_deinit();
}

// Settings changed callback
static void on_settings_changed(void) {
  update_display();
  
  // Update tick timer subscription based on second hand setting
  tick_timer_service_unsubscribe();
  if (settings_second_hand_enabled()) {
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  }
}

// Initialize the app
static void init(void) {
  // Load settings first
  settings_init();
  
  // Set up AppMessage
  app_message_init();
  app_message_set_settings_changed_callback(on_settings_changed);
  
  // Create main window
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
  
  // Subscribe to tick timer
  if (settings_second_hand_enabled()) {
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  }
  
  // Initialize bluetooth handling
  bluetooth_init();
  bluetooth_startup_check();
}

// Deinitialize the app
static void deinit(void) {
  settings_save();
  
  bluetooth_deinit();
  tick_timer_service_unsubscribe();
  app_message_deinit();
  
  window_destroy(s_window);
}

// Main entry point
int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}
