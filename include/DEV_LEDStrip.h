//
// Created by Niklas Koll on 25.12.21.
//

#ifndef MAINCODE_DEV_LEDSTRIP_H
#define MAINCODE_DEV_LEDSTRIP_H

struct DEV_LED_Strip : Service::LightBulb {

    SpanCharacteristic *power;
    SpanCharacteristic *h;
    SpanCharacteristic *s;
    SpanCharacteristic *v;

    DEV_LED_Strip() : Service::LightBulb() {
        power = new Characteristic::On();
        h = new Characteristic::Hue(0);
        s = new Characteristic::Saturation(0);
        v = new Characteristic::Brightness(100);

        setInitValues();
    }

    boolean update() override {
        if (power->updated()) {
            if (power->getNewVal() == 0) {
                turn_off();
            } else if (power->getNewVal() == 1) {
                turn_on();
            }
        }

        auto hue = h->getVal<float>();
        auto sat = s->getVal<float>();
        bool colorUpdated = false;

        if (h->updated()) {
            hue = h->getNewVal<float>();
            colorUpdated = true;
        }
        if (s->updated()) {
            sat = s->getNewVal<float>();
            colorUpdated = true;
        }
        if (v->updated()) {
            auto val = v->getNewVal<float>();
            setBrightness(val/100);
        }

        if (colorUpdated) {
            uint8_t convHue = hue * 0.71;
            uint8_t convSat = sat * 2.55;
            CHSV colorHSV = CHSV(convHue, convSat, 255);
            CRGB color;
            hsv2rgb_spectrum(colorHSV, color);
            uint32_t color_raw = ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
            display_solid_color(color_raw);
        }


        return true;
    }

    void loop() override {
        if (to_update == NO_UPDATE) {
            return;
        }

        switch (to_update) {
            case UPDATE_ON: {
                power->setVal(!turned_off);
                break;
            }
            case UPDATE_COLOR: {
                CHSV newColor = rgb2hsv_approximate(config.color);
                h->setVal(newColor.hue / 0.71);
                s->setVal(newColor.sat / 2.55);
                break;
            }
            case UPDATE_BRIGHTNESS: {
                if(config.current_brightness == config.max_brightness) {
                    v->setVal(100);
                }
                else {
                    v->setVal(config.current_brightness / config.max_brightness);
                }
                break;
            }
        }

        to_update = NO_UPDATE;
    }

    void setInitValues() {
        CHSV newColor = rgb2hsv_approximate(config.color);
        h->setVal(newColor.hue / 0.71);
        s->setVal(newColor.sat / 2.55);
        if(config.current_brightness == config.max_brightness) {
            v->setVal(100);
        }
        else {
            v->setVal(config.current_brightness / config.max_brightness);
        }
        power->setVal(!turned_off);
    }

};

#endif //MAINCODE_DEV_LEDSTRIP_H
