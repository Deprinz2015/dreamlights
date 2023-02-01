//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/DEV_LEDStrip.h"
#include "Globals.h"
#include "LED_API.h"

DEV_LED_Strip::DEV_LED_Strip(LED_API *ledApi) {
    power = new Characteristic::On();
    h = new Characteristic::Hue(0);
    s = new Characteristic::Saturation(0);
    v = new Characteristic::Brightness(100);
    effect = new Characteristic::EffectNumber();
    this->ledApi = ledApi;

    setInitValues();
}

boolean DEV_LED_Strip::update() {
    if (power->updated()) {
        ledApi->set_power(power->getNewVal<bool>());
    }

    auto hue = h->getVal<float>();
    auto sat = s->getVal<float>();
    bool colorUpdated = false;
    auto effectNumber = effect->getVal<int>();

    if (h->updated()) {
        hue = h->getNewVal<float>();
        colorUpdated = true;
    }
    if (s->updated()) {
        sat = s->getNewVal<float>();
        colorUpdated = true;
    }
    if (v->updated()) {
        auto val = v->getNewVal<uint8_t>();
        ledApi->setBrightness(val);
    }
    if(effect->updated()) {
        effectNumber = effect->getNewVal<int>();
    }

    if (colorUpdated && effectNumber == 0) {
        uint8_t convHue = hue * 0.71;
        uint8_t convSat = sat * 2.55;
        CHSV colorHSV = CHSV(convHue, convSat, 255);
        CRGB color{};
        hsv2rgb_spectrum(colorHSV, color);
        uint32_t color_raw = ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
        ledApi->display_solid_color(color_raw);
    }
    else if(effectNumber != 0) {
        // TODO Update effect
    }

    return true;
}

void DEV_LED_Strip::loop() {
    if (ledApi->toUpdateHomespan == NO_UPDATE) {
        return;
    }

    switch (ledApi->toUpdateHomespan) {
        case UPDATE_POWER: {
            power->setVal(ledApi->turnedOn);
            break;
        }
        case UPDATE_COLOR: {
            CHSV newColor = rgb2hsv_approximate(config.color);
            h->setVal(newColor.hue / 0.71);
            s->setVal(newColor.sat / 2.55);
            break;
        }
        case UPDATE_BRIGHTNESS: {
            if (config.current_brightness == config.max_brightness) {
                v->setVal(100);
            } else {
                v->setVal(config.current_brightness / config.max_brightness);
            }
            break;
        }
        case UPDATE_EFFECT: {
            // TODO Update effectNumber
        }
    }

    ledApi->toUpdateHomespan = NO_UPDATE;
}

void DEV_LED_Strip::setInitValues() {
    power->setVal(ledApi->turnedOn);
    CHSV newColor = rgb2hsv_approximate(config.color);
    h->setVal(newColor.hue / 0.71);
    s->setVal(newColor.sat / 2.55);
    if (config.current_brightness == config.max_brightness) {
        v->setVal(100);
    } else {
        v->setVal(config.current_brightness / config.max_brightness);
    }
}