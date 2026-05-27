#pragma once
#include <pebble.h>

#define DIALCOLOR_MAX_LEN 12
#define SECONDHANDOPTION_MAX_LEN 10
#define DATEOPTION_MAX_LEN 10
#define HOURLYVIBRATION_MAX_LEN 10
#define ROUNDBGCOLOR_MAX_LEN 7

typedef struct ClaySettings {
  char dialcolor[DIALCOLOR_MAX_LEN];
  char secondhandoption[SECONDHANDOPTION_MAX_LEN];
  char dateoption[DATEOPTION_MAX_LEN];
  char hourlyvibration[HOURLYVIBRATION_MAX_LEN];
  uint8_t bluetoothstatusdetection;
  char roundbackgroundcolor[ROUNDBGCOLOR_MAX_LEN];
} ClaySettings;

// Global settings instance
extern ClaySettings g_settings;

// Settings management functions
void settings_init(void);
void settings_save(void);
void settings_load(void);
void settings_set_defaults(void);
bool settings_validate_hex_color(const char *str, size_t expected_len);

// Helper functions for color parsing
uint8_t hex_char_to_val(char c);
void parse_hex_color(const char *hex, uint8_t *r, uint8_t *g, uint8_t *b);

// Quick accessors for common checks
bool settings_is_round_mode(void);
bool settings_is_round_logo_mode(void);
bool settings_is_white_dial(void);
bool settings_is_black_dial(void);
bool settings_show_date(void);
bool settings_second_hand_enabled(void);
bool settings_is_stop2go(void);
bool settings_is_quartz(void);
bool settings_hourly_vibration_enabled(void);
bool settings_bluetooth_detection_enabled(void);
