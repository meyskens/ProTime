#pragma once
#include <pebble.h>

// Number of updates per second for stop2go, must be < 1000
#define STOP2GO_TICK_RESOLUTION 4

// Persistent storage key
#define SETTINGS_KEY 1

// Message keys from package.json
#define MESSAGE_KEY_DialColor 0
#define MESSAGE_KEY_SecondHandOption 1
#define MESSAGE_KEY_DateOption 2
#define MESSAGE_KEY_HourlyVibration 3
#define MESSAGE_KEY_BluetoothStatusDetection 4
#define MESSAGE_KEY_RoundBackgroundColor 5

// Hand path definitions for standard displays (144x168)
extern const GPathInfo MINUTE_HAND_POINTS;
extern const GPathInfo HOUR_HAND_POINTS;

// Scaled hand path definitions for round mode (round face on square displays)
extern const GPathInfo MINUTE_HAND_POINTS_ROUND;
extern const GPathInfo HOUR_HAND_POINTS_ROUND;

// Scaled hand path definitions for Chalk (180x180 round)
extern const GPathInfo MINUTE_HAND_POINTS_CHALK;
extern const GPathInfo HOUR_HAND_POINTS_CHALK;

// Scaled hand path definitions for Emery (200x228 rectangular)
extern const GPathInfo MINUTE_HAND_POINTS_EMERY;
extern const GPathInfo HOUR_HAND_POINTS_EMERY;

// Scaled hand path definitions for Gabbro (260x260 round)
extern const GPathInfo MINUTE_HAND_POINTS_GABBRO;
extern const GPathInfo HOUR_HAND_POINTS_GABBRO;
