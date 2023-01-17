#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#include "Arduino.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "HomeSpan/HomeSpanHandler.h"
#include "LEDServer.h"
#include "LED_API.h"
#include "Globals.h"
#include "Config.h"

void setup_server() {
    LEDServer::setup();
    LEDServer::begin();
}

void setup_leds() {
    leds = LED_API();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    Serial.print("LED Software Version: ");
    Serial.println(LED_VERSION);

    if(!SD.begin(CS_PIN)) {
        Serial.println("SD Card failed to initialize.");
        delay(2000);
        ESP.restart();
    }

    Config::loadMainConfig(&config);
    Serial.println("Loaded main config");

    HomeSpanHandler::setup_homespan(setup_server, &leds);
    Serial.println("Setup homespan");

    setup_leds();
    Serial.println("Setup leds");

    Serial.println("Setup Complete");
}

void loop() {
    HomeSpanHandler::homespan_loop();

    if (!config.is_solid_color && !config.is_segmented_color && leds.turnedOn) {
        leds.runPattern();
    }

    LEDServer::handle();
}

#endif
