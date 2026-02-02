#pragma once

#include "daisy_seed.h"

using namespace daisy;

struct HIDButton {
    Switch* sw = nullptr;
    unsigned int index;
    bool isToggle = false;

    bool previousState = false;
    static const int updateRate = 1000;

    HIDButton(unsigned int index, dsy_gpio_pin pin, bool isToggle = true) {
        sw = new Switch();
        sw->Init(pin, updateRate, Switch::TYPE_MOMENTARY, Switch::POLARITY_NORMAL, Switch::PULL_UP);
        this->index = index;
        this->isToggle = isToggle;
    }

    ~HIDButton() {
        delete sw;
    }
};