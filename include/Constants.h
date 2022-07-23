//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_CONSTANTS_H
#define MAINCODE_CONSTANTS_H

// Definition of LED-Type

#define SK9822_TYPE
//#define WS2812B_TYPE

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

// WiFi Configuration for AP-Mode

#define WIFI_SSID "Dreamlights"
#define WIFI_PASS "12345678"

//--------------------------------------------------------------------------------------------------

// Status Constants for HomeSpan controlling

#define NO_UPDATE (-1)
#define UPDATE_ON 1
#define UPDATE_COLOR 2
#define UPDATE_BRIGHTNESS 3

//--------------------------------------------------------------------------------------------------

// Definition of HomeSpan-Device Parameters

#ifdef SK9822_TYPE
extern const char *host;
extern const char *model;
extern const char *sn;
extern const char *name;
#endif
#ifdef WS2812B_TYPE
extern const char *host;
extern const char *model;
extern const char *sn;
extern const char *name;
#endif

//--------------------------------------------------------------------------------------------------

// Lua Library Names

#define LUA_LED_ARRAY_METATABLE "led_array.mt"
#define LUA_LED_ARRAY_LIB_NAME "leds"

//--------------------------------------------------------------------------------------------------

#endif //MAINCODE_CONSTANTS_H
