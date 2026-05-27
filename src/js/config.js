module.exports = [
  {
    "type": "heading",
    "defaultValue": "ProTime Settings"
  },
  {
    "type": "text",
    "defaultValue": "Customize your ProRail-style watchface."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Dial Appearance"
      },
      {
        "type": "select",
        "messageKey": "DialColor",
        "defaultValue": "round",
        "label": "Dial Style",
        "options": [
          {
            "label": "Round (white)",
            "value": "round"
          },
          {
            "label": "Round with Logo",
            "value": "roundlogo"
          }
        ]
      },
      {
        "type": "color",
        "messageKey": "RoundBackgroundColor",
        "defaultValue": "FFFFFF",
        "label": "Round background color (square Pebble only)",
        "description": "Background color shown around the round face on square Pebble watches"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Second Hand"
      },
      {
        "type": "select",
        "messageKey": "SecondHandOption",
        "defaultValue": "quartz",
        "label": "Second Hand Style",
        "options": [
          { 
            "label": "Quartz", 
            "value": "quartz" 
          },
          { 
            "label": "Stop2Go", 
            "value": "stop2go" 
          },
          { 
            "label": "Off", 
            "value": "off" 
          }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Date Display"
      },
      {
        "type": "select",
        "messageKey": "DateOption",
        "defaultValue": "nodate",
        "label": "Date Display",
        "options": [
          { 
            "label": "No Date", 
            "value": "nodate" 
          },
          { 
            "label": "Show Date", 
            "value": "showdate" 
          }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Hourly Vibration"
      },
      {
        "type": "select",
        "messageKey": "HourlyVibration",
        "defaultValue": "off",
        "label": "Hourly Vibration",
        "options": [
          { 
            "label": "Off", 
            "value": "off" 
          },
          { 
            "label": "Short Pulse", 
            "value": "short" 
          },
          { 
            "label": "Long Pulse", 
            "value": "long" 
          },
          { 
            "label": "Double Pulse", 
            "value": "double" 
          }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Bluetooth"
      },
      {
        "type": "toggle",
        "messageKey": "BluetoothStatusDetection",
        "label": "Show Bluetooth Status",
        "defaultValue": false,
        "description": "Show splash screen and vibrate on connect/disconnect"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
