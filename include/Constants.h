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
const char *host = "ledsk9822";
const char *model = "SK9822";
const char *sn = "123-ABC-SK9822";
const char *name = "Dreamlight-SK9822";
#endif
#ifdef WS2812B_TYPE
const char *host = "ledws2812b";
const char *model = "WS2812B";
const char *sn = "123-ABC-WS2812B";
const char *name = "Dreamlight-WS2812B";
#endif

//--------------------------------------------------------------------------------------------------

// Lua Library Names

#define LUA_LED_ARRAY_METATABLE "led_array.mt"
#define LUA_LED_ARRAY_LIB_NAME "leds"

//--------------------------------------------------------------------------------------------------

#endif //MAINCODE_CONSTANTS_H
