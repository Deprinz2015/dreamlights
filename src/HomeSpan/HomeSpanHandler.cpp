//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/HomeSpanHandler.h"
#include "HomeSpan.h"
#include "HomeSpan/DEV_Identify.h"
#include "HomeSpan/DEV_LEDStrip.h"

void HomeSpanHandler::wificonnect() {
    //TODO
    // Die Standard AP-Function von Homespan speichert die WiFi-Daten nicht korrekt ab.
    // Da die LEDs sowieso eine eigene App bekommen sollen, kann eine einfach HTTP-Anfrage
    // mit SSID, Passwort und Setup-Code genügen, um die dynamische Verbindung einzurichten
    // Dabei werden die WiFi Daten wenn möglich im ESP eigenen NVS gespeichert,
    // ansonsten (verschlüsselt) auf der SD Karte
    ESP.restart();
}

void HomeSpanHandler::setup_homespan(void(*wifiCallback)(), LED_API *ledApi) {
    homeSpan.enableOTA();
    //homeSpan.setApFunction(wificonnect);
    homeSpan.enableAutoStartAP();
    homeSpan.setHostNameSuffix("");
    homeSpan.setPortNum(1201);
    homeSpan.setWifiCallback(wifiCallback);

    homeSpan.begin(Category::Lighting, "Dreamlight", host);

    new SpanAccessory();
    new DEV_Identify("Bridge", "Nako", sn, "Bridge", "0.9", 3);
    new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");

    new SpanAccessory();
    new DEV_Identify(name, "Nako", sn, model, "0.9", 3);
    new DEV_LED_Strip(ledApi);
}

void HomeSpanHandler::homespan_loop() {
    homeSpan.poll();
}