//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_DEV_LEDSTRIP_H
#define MAINCODE_DEV_LEDSTRIP_H

#include "HomeSpan.h"
#include "LED_API.h"

struct DEV_LED_Strip : Service::LightBulb {
    DEV_LED_Strip(LED_API *ledApi);

    SpanCharacteristic *power;
    SpanCharacteristic *h;
    SpanCharacteristic *s;
    SpanCharacteristic *v;

    LED_API *ledApi;

    boolean update() override;
    void loop() override;
    void setInitValues();
};

#endif //MAINCODE_DEV_LEDSTRIP_H
