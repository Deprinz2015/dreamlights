//
// Created by Niklas  Koll on 19.03.23.
//

#include "HomeSpan/LEDStrip_Identify.h"

LEDStrip_Identify::LEDStrip_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version) : Service::AccessoryInformation() {
    new Characteristic::Name(name);
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify = new Characteristic::Identify();
}