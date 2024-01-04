//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_CONSTANTS_H
#define MAINCODE_CONSTANTS_H

// Definition of LED-Type

#define SK9822_TYPE
//#define WS2812B_TYPE

//--------------------------------------------------------------------------------------------------

#define LED_VERSION "1.0.0-dev3"

//--------------------------------------------------------------------------------------------------

// LED-Pin Definition

#define LED_PIN 32
#define LED_CLK_PIN 33
#define CS_PIN 5

//--------------------------------------------------------------------------------------------------

// Definition of LED POWER-States

#define POWER_ON true
#define POWER_OFF false

//--------------------------------------------------------------------------------------------------

// Status Constants (Indices) for HomeSpan controlling

#define UPDATE_POWER 0
#define UPDATE_COLOR 1
#define UPDATE_BRIGHTNESS 2
#define UPDATE_EFFECT 3

//--------------------------------------------------------------------------------------------------

// Definition of HomeSpan-Device Parameters
extern const char *host;
extern const char *model;
extern const char *sn;
extern const char *name;

//--------------------------------------------------------------------------------------------------

// Lua Library Names

#define LUA_LED_ARRAY_METATABLE "led_array.mt"
#define LUA_LED_ARRAY_LIB_NAME "leds"

//--------------------------------------------------------------------------------------------------

// Size for StaticJsonObject

#define STATIC_JSON_OBJ_SIZE 250

//--------------------------------------------------------------------------------------------------

// Dateinamen für Config-Dateien

extern const char *main_config_file_name;
extern const char *effect_list_file_name;
extern const char *preset_list_file_name;
extern const char *preset_file_template;
extern const char *effect_config_template;
extern const char *effect_file_template;

//--------------------------------------------------------------------------------------------------

// Default Werte für MainConfig

#define MAINCONFIG_NUM_LEDS 300
#define MAINCONFIG_MAX_BRIGHTNESS 150
#define MAINCONFIG_CURRENT_BRIGHTNESS 100
#define MAINCONFIG_LAST_ID "1"
#define MAINCONFIG_IS_SOLID_COLOR false
#define MAINCONFIG_IS_SEGMENTED_COLOR false
#define MAINCONFIG_COLOR 0xffffff
#define MAINCONFIG_SEGMENTED_PRESET_ID "NULL"

//--------------------------------------------------------------------------------------------------

// HTTP-Routes

#define ROUTE_EFFECT "/effect"
#define ROUTE_POWER "/power"
#define ROUTE_CHANGE_COLOR "/color"
#define ROUTE_CHANGE_BRIGHTNESS "/brightness"
#define ROUTE_CHANGE_SEGMENT "/segment"
#define ROUTE_LOAD_PRESET "/load_preset"
#define ROUTE_SAVE_PRESET "/save_preset"
#define ROUTE_SAVE_NEW_EFFECT "/new_effect"
#define ROUTE_SAVE_EFFECT "/update_effect"

#endif //MAINCODE_CONSTANTS_H
