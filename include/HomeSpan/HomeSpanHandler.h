//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_HOMESPANHANDLER_H
#define MAINCODE_HOMESPANHANDLER_H

#include "HomeSpan.h"
#include "HomeSpan/DEV_LEDStrip.h"
#include "HomeSpan/DEV_Identify.h"
#include "Constants.h"
#include "Globals.h"

void wificonnect();
void setup_homespan(void(*)());

#endif //MAINCODE_HOMESPANHANDLER_H