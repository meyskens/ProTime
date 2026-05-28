#include "hands.h"
#include "config.h"

// Platform-specific hand parameters defined at compile time
#if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
  // Gabbro - hour hand width 14, minute hand width 12
  #define HOUR_HAND_LENGTH 78
  #define MINUTE_HAND_LENGTH 115
  #define HOUR_TIP_RADIUS 6
  #define MINUTE_TIP_RADIUS 5
#elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
  // Emery - hour hand width 12, minute hand width 10
  #define HOUR_HAND_LENGTH 60
  #define MINUTE_HAND_LENGTH 84
  #define HOUR_TIP_RADIUS 5
  #define MINUTE_TIP_RADIUS 4
#elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
  // Chalk - hour hand width 10, minute hand width 10
  #define HOUR_HAND_LENGTH 51
  #define MINUTE_HAND_LENGTH 75
  #define HOUR_TIP_RADIUS 4
  #define MINUTE_TIP_RADIUS 4
#elif defined(PBL_ROUND)
  // Round watches (Chalk, Gabbro) - hour hand width 8, minute hand width 8
  #define HOUR_HAND_LENGTH 34
  #define MINUTE_HAND_LENGTH 50
  #define HOUR_TIP_RADIUS 3
  #define MINUTE_TIP_RADIUS 3
#else
  // Standard rectangular (aplite, basalt) - hour hand width 10, minute hand width 8
  #define HOUR_HAND_LENGTH 37
  #define MINUTE_HAND_LENGTH 55
  #define HOUR_TIP_RADIUS 3
  #define MINUTE_TIP_RADIUS 3
#endif

static GPath *s_minute_arrow;
static GPath *s_hour_arrow;
static GPath *s_minute_arrow_round;
static GPath *s_hour_arrow_round;
static GPath *s_minute_arrow_chalk;
static GPath *s_hour_arrow_chalk;
static GPath *s_minute_arrow_emery;
static GPath *s_hour_arrow_emery;
static GPath *s_minute_arrow_gabbro;
static GPath *s_hour_arrow_gabbro;

// Hand path definitions
const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -4, 16 },
    { 4, 16 },
    { 4, -56 },
    { -4, -56 }
  }
};

const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 16 },
    { 5, 16 },
    { 5, -38 },
    { -5, -38 }
  }
};

const GPathInfo MINUTE_HAND_POINTS_ROUND = {
  4,
  (GPoint []) {
    { -4, 16 },
    { 4, 16 },
    { 4, -53 },
    { -4, -53 }
  }
};

const GPathInfo HOUR_HAND_POINTS_ROUND = {
  4,
  (GPoint []) {
    { -4, 16 },
    { 4, 16 },
    { 4, -37 },
    { -4, -37 }
  }
};

const GPathInfo MINUTE_HAND_POINTS_CHALK = {
  4,
  (GPoint []) {
    { -5, 20 },
    { 5, 20 },
    { 5, -79 },
    { -5, -79 }
  }
};

const GPathInfo HOUR_HAND_POINTS_CHALK = {
  4,
  (GPoint []) {
    { -5, 20 },
    { 5, 20 },
    { 5, -55 },
    { -5, -55 }
  }
};

const GPathInfo MINUTE_HAND_POINTS_EMERY = {
  4,
  (GPoint []) {
    { -5, 20 },
    { 5, 20 },
    { 5, -88 },
    { -5, -88 }
  }
};

const GPathInfo HOUR_HAND_POINTS_EMERY = {
  4,
  (GPoint []) {
    { -6, 20 },
    { 6, 20 },
    { 6, -60 },
    { -6, -60 }
  }
};

const GPathInfo MINUTE_HAND_POINTS_GABBRO = {
  4,
  (GPoint []) {
    { -6, 24 },
    { 6, 24 },
    { 6, -115 },
    { -6, -115 }
  }
};

const GPathInfo HOUR_HAND_POINTS_GABBRO = {
  4,
  (GPoint []) {
    { -7, 24 },
    { 7, 24 },
    { 7, -78 },
    { -7, -78 }
  }
};

void hands_init(GPoint center) {
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  s_minute_arrow_round = gpath_create(&MINUTE_HAND_POINTS_ROUND);
  s_hour_arrow_round = gpath_create(&HOUR_HAND_POINTS_ROUND);
  s_minute_arrow_chalk = gpath_create(&MINUTE_HAND_POINTS_CHALK);
  s_hour_arrow_chalk = gpath_create(&HOUR_HAND_POINTS_CHALK);
  s_minute_arrow_emery = gpath_create(&MINUTE_HAND_POINTS_EMERY);
  s_hour_arrow_emery = gpath_create(&HOUR_HAND_POINTS_EMERY);
  s_minute_arrow_gabbro = gpath_create(&MINUTE_HAND_POINTS_GABBRO);
  s_hour_arrow_gabbro = gpath_create(&HOUR_HAND_POINTS_GABBRO);

  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);
  gpath_move_to(s_minute_arrow_round, center);
  gpath_move_to(s_hour_arrow_round, center);
  gpath_move_to(s_minute_arrow_chalk, center);
  gpath_move_to(s_hour_arrow_chalk, center);
  gpath_move_to(s_minute_arrow_emery, center);
  gpath_move_to(s_hour_arrow_emery, center);
  gpath_move_to(s_minute_arrow_gabbro, center);
  gpath_move_to(s_hour_arrow_gabbro, center);
}

void hands_deinit(void) {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
  gpath_destroy(s_minute_arrow_round);
  gpath_destroy(s_hour_arrow_round);
  gpath_destroy(s_minute_arrow_chalk);
  gpath_destroy(s_hour_arrow_chalk);
  gpath_destroy(s_minute_arrow_emery);
  gpath_destroy(s_hour_arrow_emery);
  gpath_destroy(s_minute_arrow_gabbro);
  gpath_destroy(s_hour_arrow_gabbro);
}

void hands_get_current_paths(GPath **hour_path, GPath **minute_path) {
  #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
    *hour_path = s_hour_arrow_gabbro;
    *minute_path = s_minute_arrow_gabbro;
  #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
    *hour_path = s_hour_arrow_emery;
    *minute_path = s_minute_arrow_emery;
  #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
    *hour_path = s_hour_arrow_chalk;
    *minute_path = s_minute_arrow_chalk;
  #else
    if (settings_is_round_mode()) {
      *hour_path = s_hour_arrow_round;
      *minute_path = s_minute_arrow_round;
    } else {
      *hour_path = s_hour_arrow;
      *minute_path = s_minute_arrow;
    }
  #endif
}

void hands_set_color_for_dial(GContext *ctx) {
  #ifdef PBL_COLOR
    if (settings_is_white_dial() || settings_is_round_mode()) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_context_set_stroke_color(ctx, GColorBlack);
    } else if (settings_is_black_dial()) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_context_set_stroke_color(ctx, GColorWhite);
    }
  #else
    if (settings_is_white_dial() || settings_is_round_mode()) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_context_set_stroke_color(ctx, GColorBlack);
    } else if (settings_is_black_dial()) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_context_set_stroke_color(ctx, GColorWhite);
    }
  #endif
}

void hands_set_stroke_color_for_dial(GContext *ctx) {
  #ifdef PBL_COLOR
    graphics_context_set_stroke_width(ctx, 2);
    if (settings_is_white_dial()) {
      graphics_context_set_fill_color(ctx, GColorDarkGray);
      graphics_context_set_stroke_color(ctx, GColorDarkGray);
    } else if (settings_is_black_dial()) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_context_set_stroke_color(ctx, GColorWhite);
    } else if (settings_is_round_mode()) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_context_set_stroke_color(ctx, GColorBlack);
    }
  #else
    if (settings_is_white_dial() || settings_is_round_mode()) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_context_set_stroke_color(ctx, GColorBlack);
    } else if (settings_is_black_dial()) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_context_set_stroke_color(ctx, GColorWhite);
    }
  #endif
}

GColor hands_get_date_color(void) {
  #ifdef PBL_COLOR
    if (settings_is_white_dial()) {
      return GColorDarkGray;
    } else if (settings_is_round_mode()) {
      return GColorBlack;
    } else {
      return GColorWhite;
    }
  #else
    if (settings_is_white_dial() || settings_is_round_mode()) {
      return GColorBlack;
    } else {
      return GColorWhite;
    }
  #endif
}

void hands_draw_date_box(GContext *ctx, GRect bounds) {
  if (!settings_show_date()) return;
  
  hands_set_stroke_color_for_dial(ctx);
  
  if (settings_is_round_mode()) {
    graphics_draw_rect(ctx, GRect(bounds.size.w / 2 + 25, bounds.size.h / 2 - 8, 22, 20));
  } else {
    graphics_draw_rect(ctx, GRect(102, 74, 22, 20));
  }

  #ifdef PBL_COLOR
    graphics_context_set_stroke_width(ctx, 1);
  #endif
}

void hands_draw_hour_hand(GContext *ctx, GPath *hour_path, GPoint center, struct tm *t) {
  graphics_context_set_fill_color(ctx, GColorFromHEX(0x000055));
  graphics_context_set_stroke_color(ctx, GColorFromHEX(0x000055));

  int32_t angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  gpath_rotate_to(hour_path, angle);
  gpath_draw_filled(ctx, hour_path);
  gpath_draw_outline(ctx, hour_path);

  // Draw rounded tip using compile-time defined constants
  GPoint tip = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)HOUR_HAND_LENGTH / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)HOUR_HAND_LENGTH / TRIG_MAX_RATIO) + center.y,
  };
  graphics_fill_circle(ctx, tip, HOUR_TIP_RADIUS);
}

void hands_draw_minute_hand(GContext *ctx, GPath *minute_path, GPoint center, struct tm *t) {
  graphics_context_set_fill_color(ctx, GColorFromHEX(0x000055));
  graphics_context_set_stroke_color(ctx, GColorFromHEX(0x000055));

  int32_t angle = TRIG_MAX_ANGLE * t->tm_min / 60;
  gpath_rotate_to(minute_path, angle);
  gpath_draw_filled(ctx, minute_path);
  gpath_draw_outline(ctx, minute_path);

  // Draw rounded tip using compile-time defined constants
  GPoint tip = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)MINUTE_HAND_LENGTH / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)MINUTE_HAND_LENGTH / TRIG_MAX_RATIO) + center.y,
  };
  graphics_fill_circle(ctx, tip, MINUTE_TIP_RADIUS);
}

void hands_draw_second_hand(GContext *ctx, GPoint center, struct tm *t) {
  if (!settings_second_hand_enabled()) return;
  
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_context_set_stroke_color(ctx, GColorRed);
  #else
    hands_set_color_for_dial(ctx);
  #endif

  #ifdef PBL_COLOR
    #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
      graphics_context_set_stroke_width(ctx, 5);
    #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
      graphics_context_set_stroke_width(ctx, 4);
    #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
      graphics_context_set_stroke_width(ctx, 4);
    #else
      graphics_context_set_stroke_width(ctx, 3);
    #endif
  #endif

  int16_t second_hand_length;
  int16_t second_hand_opp_length;
  int16_t second_circle_offset;
  
  #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
    second_hand_length = 115;
    second_hand_opp_length = 28;
    second_circle_offset = 45;
  #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
    second_hand_length = 92;
    second_hand_opp_length = 22;
    second_circle_offset = 38;
  #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
    second_hand_length = 82;
    second_hand_opp_length = 20;
    second_circle_offset = 35;
  #else
    if (settings_is_round_mode()) {
      second_hand_length = 60;
      second_hand_opp_length = 16;
      second_circle_offset = 30;
    } else {
      GRect bounds = layer_get_bounds(window_get_root_layer(window_stack_get_top_window()));
      second_hand_length = (bounds.size.w / 2) + 2;
      second_hand_opp_length = 23;
      second_circle_offset = 30;
    }
  #endif
   
  double second_angle = 0;
  
  if (settings_is_quartz()) {
    second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  } else if (settings_is_stop2go()) {
    time_t sec;
    uint16_t ms;
    time_ms(&sec, &ms);
    struct tm *now = localtime(&sec);
    
    if (now->tm_sec < 58) {
      double fractional_sec = now->tm_sec + ms / 1000.0;
      second_angle = TRIG_MAX_ANGLE * fractional_sec / 58.0;
    } else {
      second_angle = 0;
    }
  }
  
  int32_t angle = (int32_t)second_angle;
  
  // Calculate circle position - offset from tip toward center
  int16_t circle_distance = second_hand_length - second_circle_offset;
  GPoint second_circle = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)circle_distance / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)circle_distance / TRIG_MAX_RATIO) + center.y,
  };

  // Determine circle radius based on display
  int16_t circle_radius;
  #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
    circle_radius = 12;
  #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
    circle_radius = 10;
  #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
    circle_radius = 9;
  #else
    circle_radius = 7;
  #endif

  // Calculate line segment points - stop before circle and resume after
  int16_t line_start_distance = circle_distance + circle_radius + 2;
  int16_t line_end_distance = circle_distance - circle_radius - 2;
  
  GPoint line_start = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  };
  
  GPoint line_before_circle = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)line_start_distance / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)line_start_distance / TRIG_MAX_RATIO) + center.y,
  };
  
  GPoint line_after_circle = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)line_end_distance / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)line_end_distance / TRIG_MAX_RATIO) + center.y,
  };
  
  // Draw line from tip to before circle, and from after circle to center
  graphics_draw_line(ctx, line_start, line_before_circle);
  graphics_draw_line(ctx, line_after_circle, center);
  
  GPoint second_hand_opp = {
    .x = (int16_t)(-sin_lookup(angle) * (int32_t)second_hand_opp_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(cos_lookup(angle) * (int32_t)second_hand_opp_length / TRIG_MAX_RATIO) + center.y,
  };
  
  graphics_draw_line(ctx, second_hand_opp, center);

  // Draw the hollow circle
  graphics_draw_circle(ctx, second_circle, circle_radius);
  
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_context_set_stroke_color(ctx, GColorRed);
  #else
    hands_set_color_for_dial(ctx);
  #endif
  
  #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 7);
  #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 6);
  #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 5);
  #else
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 4);
  #endif
}

void hands_draw_center_circle(GContext *ctx, GPoint center) {
  // Draw a center circle to cover the base of the hands
  // This ensures the rounded edge illusion is maintained at all angles
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorFromHEX(0x000055));
  #else
    hands_set_color_for_dial(ctx);
  #endif
  
  #if PBL_DISPLAY_WIDTH == 260 && PBL_DISPLAY_HEIGHT == 260
    graphics_fill_circle(ctx, center, 8);
  #elif PBL_DISPLAY_WIDTH == 200 && PBL_DISPLAY_HEIGHT == 228
    graphics_fill_circle(ctx, center, 7);
  #elif PBL_DISPLAY_WIDTH == 180 && PBL_DISPLAY_HEIGHT == 180
    graphics_fill_circle(ctx, center, 6);
  #else
    graphics_fill_circle(ctx, center, 5);
  #endif
}

void hands_draw(Layer *layer, GContext *ctx, struct tm *t) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  
  GPath *hour_path;
  GPath *minute_path;
  hands_get_current_paths(&hour_path, &minute_path);
  
  hands_draw_date_box(ctx, bounds);
  hands_draw_hour_hand(ctx, hour_path, center, t);
  hands_draw_minute_hand(ctx, minute_path, center, t);
  hands_draw_second_hand(ctx, center, t);
  hands_draw_center_circle(ctx, center);
}
