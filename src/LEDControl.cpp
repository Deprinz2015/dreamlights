#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#include "HomeSpan.h"
#include "FastLED.h"
#include "WebServer.h"
#include "Update.h"
#include "WiFiManager.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "HomeSpan/HomeSpanHandler.h"
#include "LEDServer.h"

#include "CustomTypes/Clock.h"
#include "CustomTypes/Color_Preset.h"
#include "CustomTypes/Color_Preset_Key.h"
#include "CustomTypes/Effect.h"
#include "CustomTypes/Main_Config.h"
#include "CustomTypes/Segmented_Color.h"

LED_API *leds;

void setup_server() {
    LEDServer::setup();
    LEDServer::begin();
}

void setup_leds() {
    leds = new LED_API();
    leds->init();
}

void setup() {
    Serial.println("Started Setup");

    HomeSpanHandler::setup_homespan(setup_server);
    setup_leds();

    Serial.println("Setup Complete");
}

void loop() {
    HomeSpanHandler::homespan_loop();

    if (!config.is_solid_color && !config.is_segmented_color && leds->turnedOn) {
        leds->runPattern();
    }

    LEDServer::handle();
}

#endif
