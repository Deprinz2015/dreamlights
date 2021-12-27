#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

// --------------------------------INCLUDES------------------------------
unsigned long last_moment; // Only for Debugging
unsigned long current_moment; // Only for Debugging

#include "HomeSpan.h"
#include "FastLED.h"
#include "WebServer.h"
#include "Update.h"
#include "WiFiManager.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "Clock.h"
#include "Configs.h"
#include "HTTPRoutes.h"
#include "LuaController.h"

//-------------------------------------------------------------------------------------------
#define LED_PIN 32
#define LED_CLK_PIN 33
#define CS_PIN 5
#define SK9822_TYPE
//#define WS2812B_TYPE

#define NO_UPDATE (-1)
#define UPDATE_ON 1
#define UPDATE_COLOR 2
#define UPDATE_BRIGHTNESS 3
//-------------------------------------------------------------------------------------------

MainConfig config;
Effect *effects;
Color_Preset_Key *presets;
int preset_amount;
int effect_amount;

WebServer server(80);
WiFiManager wm;

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

void update_device(int);

int to_update = NO_UPDATE;

#include "LEDController.h"
#include "HTTPController.h"
#include "DEV_LEDStrip.h"
#include "DEV_Identify.h"

void wificonnect() {
    wm.autoConnect(WIFI_SSID, WIFI_PASS);

    homeSpan.setWifiCredentials(wm.getWiFiSSID().c_str(), wm.getWiFiPass().c_str());
    ESP.restart();
}

void setup_server() {
    server.on(ROUTE_CURRENT_EFFECT, HTTP_GET, http_send_response);
    server.on(ROUTE_EFFECT, HTTP_GET, http_play_effect);
    server.on(ROUTE_CHANGE_COLOR, HTTP_GET, http_display_solid_color);
    server.on(ROUTE_OFF, HTTP_GET, http_turn_off);
    server.on(ROUTE_ON, HTTP_GET, http_turn_on);
    server.on(ROUTE_CHANGE_BRIGHTNESS, HTTP_GET, http_change_brightness);
    server.on(ROUTE_CHANGE_SEGMENT, HTTP_GET, http_color_segment);
    server.on(ROUTE_SAVE_PRESET, HTTP_GET, http_save_preset);
    server.on(ROUTE_LOAD_PRESET, HTTP_GET, http_load_preset);

    server.begin();
}

void setup_led() {
    leds = new CRGB[config.num_leds];
#ifdef SK9822_TYPE
    FastLED.addLeds<SK9822, LED_PIN, LED_CLK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, config.num_leds);
#endif
#ifdef WS2812B_TYPE
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, config.num_leds);
#endif
    FastLED.setBrightness(config.current_brightness);
    FastLED.clear(true);

    find_effect_by_id(&currentEffect, config.last_id);
    clockAmount = loadRefreshRates(currentEffect, clocks);

    Color_Preset_Key preset_key{};
    if (find_preset_by_id(&preset_key, config.segmented_preset_id) != -1) {
        set_new_preset(preset_key);
    }

    //TODO save last preset into a tmp file (id=-1), to get same colors after restart, if not already saved

    turn_on();
    update_device(UPDATE_ON);
}

void setup_homespan() {
    homeSpan.enableOTA();
    homeSpan.setApFunction(wificonnect);
    homeSpan.enableAutoStartAP();
    homeSpan.setHostNameSuffix("");
    homeSpan.setPortNum(1201);
    homeSpan.setWifiCallback(setup_server);

    homeSpan.begin(Category::Lighting, "Dreamlight", host);

    new SpanAccessory();
        new DEV_Identify("Bridge", "Nako", sn, "Bridge", "0.9", 3);
        new Service::HAPProtocolInformation();
            new Characteristic::Version("1.1.0");

    new SpanAccessory();
        new DEV_Identify(name, "Nako", sn, model, "0.9", 3);
        new DEV_LED_Strip();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    if (!SD.begin(CS_PIN)) {
        delay(2000);
        ESP.restart();
    }

    effect_amount = loadEffectList(effects);
    preset_amount = loadPresetList(presets);
    loadMainConfig(&config);

    saveMainConfig(config);
    setup_homespan();
    setup_led();

    Serial.println("Finished Setup");
}

void loop() {
    homeSpan.poll();

    if (!config.is_solid_color && !config.is_segmented_color && !turned_off) {
        runPattern();
    }

    server.handleClient();
}

void update_device(int characteristic) {
    if (to_update == NO_UPDATE) {
        to_update = characteristic;
    }
}

#endif
