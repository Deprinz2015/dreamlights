//
// Created by Niklas Koll on 16.07.22.
//

#include "HomeSpan/DEV_Identify.h"

DEV_Identify::DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version,
                           int nBlinks) : Service::AccessoryInformation() {

    new Characteristic::Name(name);
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify = new Characteristic::Identify();

    this->nBlinks = nBlinks;

    pinMode(homeSpan.getStatusPin(), OUTPUT);
}

boolean DEV_Identify::update() {
    for (int i = 0; i < nBlinks; i++) {
        digitalWrite(homeSpan.getStatusPin(), LOW);
        delay(250);
        digitalWrite(homeSpan.getStatusPin(), HIGH);
        delay(250);
    }
    return true;
}