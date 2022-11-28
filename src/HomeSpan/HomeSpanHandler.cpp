//
// Created by Niklas Koll on 16.07.22.
//

#include <DNSServer.h>
#include <SD.h>
#include "HomeSpan/HomeSpanHandler.h"
#include "HomeSpan.h"
#include "HomeSpan/DEV_Identify.h"
#include "HomeSpan/DEV_LEDStrip.h"
#include "Globals.h"
#include "ArduinoJson.h"

WebServer apServer(80);

void handle_wifi_request() {
    String ssid = apServer.arg("ssid");
    String pass = apServer.arg("pass");

    if(ssid == "") {
        apServer.send(400, "application/json", R"({error: Bad Request (Missing SSID)})");
        return;
    }

    homeSpan.setWifiCredentials(ssid.c_str(), pass.c_str());

    File file = SD.open(WIFI_CREDENTIALS_FILE, "w");
    StaticJsonDocument<96> doc;
    doc["ssid"] = ssid;
    doc["pass"] = pass;
    serializeJson(doc, file);
    file.close();

    Serial.println("Connected");
    apServer.send(200);

    ESP.restart();
}

void wificonnect() {
    WiFiClient client = 0;
    DNSServer dns;
    IPAddress ip(192, 168, 4, 1);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    dns.start(53, "*", ip);

    apServer.on(WIFI_CONNECT_ROUTE, HTTP_POST, handle_wifi_request);

    apServer.begin();

    while(true) {
        apServer.handleClient();
    }
}

void HomeSpanHandler::setup_homespan(void(*wifiCallback)(), LED_API *ledApi) {
    File file = SD.open(WIFI_CREDENTIALS_FILE);
    if(file) {
        StaticJsonDocument<96> doc;
        DeserializationError error = deserializeJson(doc, file);

        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        const char* ssid = doc["ssid"];
        const char* pass = doc["pass"];

        homeSpan.setWifiCredentials(ssid, pass);
    }

    homeSpan.enableOTA();
    homeSpan.setApFunction(wificonnect);
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