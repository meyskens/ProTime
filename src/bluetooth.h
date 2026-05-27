#pragma once
#include <pebble.h>

// Bluetooth splash screen management
void bluetooth_init(void);
void bluetooth_deinit(void);
void bluetooth_connection_callback(bool is_connected);
void bluetooth_startup_check(void);
