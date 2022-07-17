//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/HomeSpanHandler.h"

void wificonnect() {
    wm.autoConnect(WIFI_SSID, WIFI_PASS);

    homeSpan.setWifiCredentials(wm.getWiFiSSID().c_str(), wm.getWiFiPass().c_str());
    ESP.restart();
}

void setup_homespan(void(*wifiCallback)()) {
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
    new DEV_LED_Strip();
}