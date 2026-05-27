#pragma once
#include <pebble.h>
#include "settings.h"

// Hand path creation and management
void hands_init(GPoint center);
void hands_deinit(void);
void hands_draw(Layer *layer, GContext *ctx, struct tm *t);

// Get appropriate hand paths for current display
void hands_get_current_paths(GPath **hour_path, GPath **minute_path);

// Individual hand drawing functions
void hands_draw_hour_hand(GContext *ctx, GPath *hour_path, GPoint center, struct tm *t);
void hands_draw_minute_hand(GContext *ctx, GPath *minute_path, GPoint center, struct tm *t);
void hands_draw_second_hand(GContext *ctx, GPoint center, struct tm *t);
void hands_draw_date_box(GContext *ctx, GRect bounds);
void hands_draw_center_circle(GContext *ctx, GPoint center);

// Color helpers
void hands_set_color_for_dial(GContext *ctx);
void hands_set_stroke_color_for_dial(GContext *ctx);
GColor hands_get_date_color(void);
