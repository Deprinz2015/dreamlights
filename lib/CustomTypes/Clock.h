//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_CLOCK_H
#define MAINCODE_CLOCK_H

#include "Arduino.h"
#include "lib8tion.h"

struct Clock {
    String name;
    CEveryNMillis timer;
};

#endif //MAINCODE_CLOCK_H
