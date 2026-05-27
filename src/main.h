#pragma once
#include <pebble.h>

// Tick handler - called by hands module
void handle_tick(struct tm *t, TimeUnits units_changed);
