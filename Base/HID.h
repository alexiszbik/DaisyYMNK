#pragma once

#include "daisy_seed.h"
#include "../DSP/ModuleCore.h"

using namespace daisy;
using namespace std;
using namespace ydaisy;

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

struct HIDLed {
    dsy_gpio* led = nullptr;
    unsigned int index;
    bool previousState = false;

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

class HID {
public:
    HID(vector<HIDElement> desc);

    void init(DaisySeed &hw);
    void process(DaisySeed &hw, ModuleCore* core);

private:
    vector<HIDElement> desc;

    vector<HIDButton*> buttons;
    vector<HIDLed*> leds;
    vector<int> adcIndexes;

};