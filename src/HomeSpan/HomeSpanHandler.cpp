//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/HomeSpanHandler.h"
#include "HomeSpan.h"
#include "HomeSpan/LEDStrip_Device.h"
#include "HomeSpan/LEDStrip_Identify.h"
#include "HomeSpan/CustomCharacteristics.h"

void HomeSpanHandler::setup_homespan(void(*wifiCallback)(), LED_API *ledApi) {
    homeSpan.enableOTA();
    homeSpan.enableAutoStartAP();
    homeSpan.setHostNameSuffix("");
    homeSpan.setPortNum(1201);
    homeSpan.setWifiCallback(wifiCallback);

    homeSpan.begin(Category::Lighting, "Dreamlight", host);

    new SpanAccessory();
    new LEDStrip_Identify(name, "Nako", sn, model, "0.9");
    new LEDStrip_Device(ledApi);
}

void HomeSpanHandler::homespan_loop() {
    homeSpan.poll();
}