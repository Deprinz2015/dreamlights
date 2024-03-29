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

    setup_leds();
    Serial.println("Setup leds");

    HomeSpanHandler::setup_homespan(setup_server, &leds);
    Serial.println("Setup homespan");

    Serial.println("Setup Complete");
}

void loop() {
    HomeSpanHandler::homespan_loop();

    // TODO Warum muss das?
    // Sowohl Presets, als auch solide Farben können auch über die Lua-Skripte ausgeführt werden.
    // der Timer kann auch auf hohe Werte (zb > 10sek) gesetzt werden, um performance zu schonen
    // Vorteil: es gibt nur die Effekte, die über die Namen den Farben/Presets zugeordnet werden kann.
    // -> keine Sonderbehandlung mehr => einfacher, lesbarer Code
    // -> Das meiste funktioniert schon, die besondere Logik für die Presets kann in App ausgelagert werden
    // -> Für Farben sollte trotzdem (aufgrund des Hex-Wert) eigene Logik gelten
    if (!config.is_solid_color && !config.is_segmented_color && leds.turnedOn) {
        leds.runPattern();
    }

    LEDServer::handle();
}

#endif
