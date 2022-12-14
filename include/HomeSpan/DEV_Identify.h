//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_DEV_IDENTIFY_H
#define MAINCODE_DEV_IDENTIFY_H

#include "HomeSpan.h"

struct DEV_Identify : Service::AccessoryInformation {
    DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version,
                 int nBlinks);

    int nBlinks;
    SpanCharacteristic *identify;

    boolean update() override;
};


#endif //MAINCODE_DEV_IDENTIFY_H
