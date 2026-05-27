#include "settings.h"
#include "config.h"

ClaySettings g_settings;

// Helper function to convert hex char to value
uint8_t hex_char_to_val(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return 0;
}

// Helper function to parse hex string to RGB values
void parse_hex_color(const char *hex, uint8_t *r, uint8_t *g, uint8_t *b) {
  *r = 0; *g = 0; *b = 0;
    
  if (hex == NULL) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "parse_hex_color: hex is NULL");
    return;
  }
  
  size_t len = strlen(hex);
  if (len < 6) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "parse_hex_color: string too short");
    return;
  }
  
  const char *ptr = hex;
  if (ptr[0] == '#') {
    ptr++;
    len--;
  } else if (len >= 8 && ptr[0] == '0' && ptr[1] == 'x') {
    ptr += 2;
    len -= 2;
  }
  
  if (len < 6) return;
  
  *r = (hex_char_to_val(ptr[0]) << 4) | hex_char_to_val(ptr[1]);
  *g = (hex_char_to_val(ptr[2]) << 4) | hex_char_to_val(ptr[3]);
  *b = (hex_char_to_val(ptr[4]) << 4) | hex_char_to_val(ptr[5]);  
}

// Helper to check if string contains only valid hex chars
bool settings_validate_hex_color(const char *str, size_t expected_len) {
  if (str == NULL) return false;
  size_t len = strlen(str);
  if (len != expected_len) return false;
  for (size_t i = 0; i < len; i++) {
    char c = str[i];
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
      return false;
    }
  }
  return true;
}

void settings_set_defaults(void) {
  strcpy(g_settings.dialcolor, "round");
  strcpy(g_settings.secondhandoption, "quartz");
  strcpy(g_settings.dateoption, "nodate");
  strcpy(g_settings.hourlyvibration, "off");
  g_settings.bluetoothstatusdetection = 0;
  strcpy(g_settings.roundbackgroundcolor, "FFFFFF");
}

void settings_save(void) {
  persist_write_data(SETTINGS_KEY, &g_settings, sizeof(g_settings));
}

void settings_load(void) {
  settings_set_defaults();
  if (persist_exists(SETTINGS_KEY)) {
    ClaySettings saved_settings;
    int bytes_read = persist_read_data(SETTINGS_KEY, &saved_settings, sizeof(saved_settings));
    
    if (bytes_read >= (int)(sizeof(saved_settings) - sizeof(saved_settings.roundbackgroundcolor))) {
      g_settings = saved_settings;
      g_settings.dialcolor[sizeof(g_settings.dialcolor) - 1] = '\0';
      g_settings.secondhandoption[sizeof(g_settings.secondhandoption) - 1] = '\0';
      g_settings.dateoption[sizeof(g_settings.dateoption) - 1] = '\0';
      g_settings.hourlyvibration[sizeof(g_settings.hourlyvibration) - 1] = '\0';
      g_settings.roundbackgroundcolor[sizeof(g_settings.roundbackgroundcolor) - 1] = '\0';
      
      if (!settings_validate_hex_color(g_settings.roundbackgroundcolor, 6)) {
        APP_LOG(APP_LOG_LEVEL_WARNING, "settings_load: invalid color '%s', resetting to default", g_settings.roundbackgroundcolor);
        strcpy(g_settings.roundbackgroundcolor, "000000");
      }
    } 
  }
}

void settings_init(void) {
  settings_load();
}

// Quick accessors
bool settings_is_round_mode(void) {
  return strcmp(g_settings.dialcolor, "round") == 0 ||
         strcmp(g_settings.dialcolor, "roundlogo") == 0;
}

bool settings_is_round_logo_mode(void) {
  return strcmp(g_settings.dialcolor, "roundlogo") == 0;
}

bool settings_is_white_dial(void) {
  // NS plate has white background, round mode is always white
  return settings_is_round_mode();
}

bool settings_is_black_dial(void) {
  // NS plate is not black
  return false;
}

bool settings_show_date(void) {
  return strcmp(g_settings.dateoption, "nodate") != 0;
}

bool settings_second_hand_enabled(void) {
  return strcmp(g_settings.secondhandoption, "off") != 0;
}

bool settings_is_stop2go(void) {
  return strcmp(g_settings.secondhandoption, "stop2go") == 0;
}

bool settings_is_quartz(void) {
  return strcmp(g_settings.secondhandoption, "quartz") == 0;
}

bool settings_hourly_vibration_enabled(void) {
  return strcmp(g_settings.hourlyvibration, "off") != 0;
}

bool settings_bluetooth_detection_enabled(void) {
  return g_settings.bluetoothstatusdetection == 1;
}
