#pragma once

#include "daisy_seed.h"

using namespace daisy;

struct HIDButton {
    Switch* sw = nullptr;
    unsigned int index;
    bool previousState = false;

    HIDButton(unsigned int index, dsy_gpio_pin pin) {
        sw = new Switch();
        int updateRate = 1000;
        sw->Init(pin, updateRate, Switch::TYPE_MOMENTARY, Switch::POLARITY_NORMAL, Switch::PULL_UP);
        this->index = index;
    }

    ~HIDButton() {
        delete sw;
    }
};