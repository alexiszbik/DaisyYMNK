#pragma once

#include "daisy_seed.h"

using namespace daisy;

struct HIDLed {
    dsy_gpio* led = nullptr;
    unsigned int index;

    HIDLed(unsigned int index, dsy_gpio_pin pin) {
        led = new dsy_gpio();
        led->pin = pin;
        led->mode = DSY_GPIO_MODE_OUTPUT_PP;
        led->pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(led);
        this->index = index;
    }

    ~HIDLed() {
        delete led;
    }
};