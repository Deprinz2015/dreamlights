//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/LEDStrip_Device.h"
#include "Globals.h"
#include "LED_API.h"

LEDStrip_Device::LEDStrip_Device(LED_API *ledApi) {
    this->ledApi = ledApi;

    Serial.print("Effect Range: 0 - ");
    Serial.println(ledApi->numEffect-1);

    power = new Characteristic::On();
    h = new Characteristic::Hue(0);
    s = new Characteristic::Saturation(0);
    v = new Characteristic::Brightness(100);
    effect = (new Characteristic::EffectNumber())
            ->setDescription("Effect Number")
            ->setRange(0, ledApi->numEffect-1, 1);
    // TODO Add Characteristic for Presets

    setInitValues();
}

boolean LEDStrip_Device::update() {
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
        Serial.println("Changed Effect through Homespan:");
        Serial.println(effectNumber);
    }

    return true;
}

void LEDStrip_Device::loop() {
    if(ledApi->updateCharacteristics[UPDATE_POWER]) {
        power->setVal(ledApi->turnedOn);

        ledApi->updateCharacteristics[UPDATE_POWER] = false;
    }

    if(ledApi->updateCharacteristics[UPDATE_COLOR]) {
        CHSV newColor = rgb2hsv_approximate(config.color);
        h->setVal(newColor.hue / 0.71);
        s->setVal(newColor.sat / 2.55);

        ledApi->updateCharacteristics[UPDATE_COLOR] = false;
    }

    if(ledApi->updateCharacteristics[UPDATE_COLOR]) {
        if (config.current_brightness == config.max_brightness) {
            v->setVal(100);
        } else {
            v->setVal(config.current_brightness / config.max_brightness);
        }

        ledApi->updateCharacteristics[UPDATE_COLOR] = false;
    }

    if(ledApi->updateCharacteristics[UPDATE_EFFECT]) {
        effect->setVal(ledApi->currentEffect.id.toInt());
        Serial.print("New Effect: ");
        Serial.println(ledApi->currentEffect.name);
        Serial.println("Updated Effect");
        ledApi->updateCharacteristics[UPDATE_EFFECT] = false;
    }
}

void LEDStrip_Device::setInitValues() const {
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