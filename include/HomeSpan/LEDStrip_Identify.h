//
// Created by Niklas  Koll on 19.03.23.
//

#ifndef DREAMLIGHTS_LEDSTRIP_IDENTIFY_H
#define DREAMLIGHTS_LEDSTRIP_IDENTIFY_H

#include "HomeSpan.h"

struct LEDStrip_Identify : Service::AccessoryInformation {
    LEDStrip_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version);

    SpanCharacteristic *identify;
};

#endif //DREAMLIGHTS_LEDSTRIP_IDENTIFY_H
