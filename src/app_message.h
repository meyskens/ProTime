#pragma once
#include <pebble.h>

// AppMessage initialization and callbacks
void app_message_init(void);
void app_message_deinit(void);

// Callback for when settings change
typedef void (*SettingsChangedCallback)(void);
void app_message_set_settings_changed_callback(SettingsChangedCallback callback);
