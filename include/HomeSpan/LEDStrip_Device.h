//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_DEV_LEDSTRIP_H
#define MAINCODE_DEV_LEDSTRIP_H

#define CUSTOM_CHAR_HEADER

#include "HomeSpan.h"
#include "LED_API.h"
#include "CustomCharacteristics.h"

struct LEDStrip_Device : Service::LightBulb {
    explicit LEDStrip_Device(LED_API *ledApi);

    SpanCharacteristic *power;
    SpanCharacteristic *h;
    SpanCharacteristic *s;
    SpanCharacteristic *v;
    SpanCharacteristic *effect;

    LED_API *ledApi;

    boolean update() override;
    void loop() override;
    void setInitValues() const;
};

#endif //MAINCODE_DEV_LEDSTRIP_H
