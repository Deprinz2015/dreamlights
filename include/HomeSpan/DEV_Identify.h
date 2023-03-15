//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_DEV_IDENTIFY_H
#define MAINCODE_DEV_IDENTIFY_H

#include "HomeSpan.h"

struct DEV_Identify : Service::AccessoryInformation {
    DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version);

    SpanCharacteristic *identify;
};


#endif //MAINCODE_DEV_IDENTIFY_H
