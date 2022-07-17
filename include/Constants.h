//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_CONSTANTS_H
#define MAINCODE_CONSTANTS_H

#define SK9822_TYPE
//#define WS2812B_TYPE

//--------------------------------------------------------------------------------------------------

#define WIFI_SSID "Dreamlights"
#define WIFI_PASS "12345678"

//--------------------------------------------------------------------------------------------------

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

#endif //MAINCODE_CONSTANTS_H
