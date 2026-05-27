#include "app_message.h"
#include "config.h"
#include "settings.h"

static SettingsChangedCallback s_settings_changed_callback = NULL;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "inbox_received: START");
  
  bool settings_changed = false;
  
  Tuple *dial_color_t = dict_find(iterator, MESSAGE_KEY_DialColor);
  if (dial_color_t) {
    strncpy(g_settings.dialcolor, dial_color_t->value->cstring, sizeof(g_settings.dialcolor) - 1);
    g_settings.dialcolor[sizeof(g_settings.dialcolor) - 1] = '\0';
    settings_changed = true;
  }
  
  Tuple *second_hand_t = dict_find(iterator, MESSAGE_KEY_SecondHandOption);
  if (second_hand_t) {
    strncpy(g_settings.secondhandoption, second_hand_t->value->cstring, sizeof(g_settings.secondhandoption) - 1);
    g_settings.secondhandoption[sizeof(g_settings.secondhandoption) - 1] = '\0';
    settings_changed = true;
  }
  
  Tuple *date_option_t = dict_find(iterator, MESSAGE_KEY_DateOption);
  if (date_option_t) {
    strncpy(g_settings.dateoption, date_option_t->value->cstring, sizeof(g_settings.dateoption) - 1);
    g_settings.dateoption[sizeof(g_settings.dateoption) - 1] = '\0';
    settings_changed = true;
  }
  
  Tuple *hourly_vib_t = dict_find(iterator, MESSAGE_KEY_HourlyVibration);
  if (hourly_vib_t) {
    strncpy(g_settings.hourlyvibration, hourly_vib_t->value->cstring, sizeof(g_settings.hourlyvibration) - 1);
    g_settings.hourlyvibration[sizeof(g_settings.hourlyvibration) - 1] = '\0';
    settings_changed = true;
  }
  
  Tuple *bt_status_t = dict_find(iterator, MESSAGE_KEY_BluetoothStatusDetection);
  if (bt_status_t) {
    g_settings.bluetoothstatusdetection = bt_status_t->value->uint8;
    settings_changed = true;
  }

  Tuple *round_bg_color_t = dict_find(iterator, MESSAGE_KEY_RoundBackgroundColor);
  if (round_bg_color_t) {
    uint32_t value = (uint32_t)round_bg_color_t->value->int32;
    snprintf(g_settings.roundbackgroundcolor, sizeof(g_settings.roundbackgroundcolor), "%06lX", value);
    settings_changed = true;
  }
  
  if (settings_changed) {
    settings_save();
    if (s_settings_changed_callback) {
      s_settings_changed_callback();
    }
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! Reason: %d", (int)reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", (int)reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void app_message_set_settings_changed_callback(SettingsChangedCallback callback) {
  s_settings_changed_callback = callback;
}

void app_message_init(void) {
  const int inbox_size = 256;
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}

void app_message_deinit(void) {
  app_message_deregister_callbacks();
}
