#pragma once

#include "daisy_seed.h"

using namespace daisy;

struct HIDLed {
    GPIO gpio;
    unsigned int index;

    HIDLed(unsigned int index, Pin pin) : index(index) {
        gpio.Init(pin, GPIO::Mode::OUTPUT);
    }
};
