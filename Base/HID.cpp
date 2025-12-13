#include "HID.h"

HID::HID(vector<HIDElement> desc) : desc(desc) {
}

void HID::init(DaisySeed &hw) {

    unsigned int adcCount = 0;
    for (auto& hidElmt : desc) {
        if (hidElmt.requireAdc()) {
            adcCount++;
        }
    }

    AdcChannelConfig adcConfig[adcCount];

    for (auto& hidElmt : desc) {
        auto pin = hw.GetPin(hidElmt.pin);

        if (hidElmt.type == kLed) {
            leds.push_back(new HIDLed(hidElmt.index, pin));
        } else if (hidElmt.requireAdc()) {
            adcConfig[adcIndexes.size()].InitSingle(pin);
            adcIndexes.push_back(hidElmt.index);
        } else {
            buttons.push_back(new HIDButton(hidElmt.index, pin));
        }
    }

    hw.adc.Init(adcConfig, adcCount);
    hw.adc.Start();
}

void HID::process(DaisySeed &hw, ModuleCore* core) {

    int k = 0;
    for (auto &adcIdx : adcIndexes) {
        float value = hw.adc.GetFloat(k++);
        core->setHIDValue(adcIdx, value);
    }

    for (auto button : buttons) {
        button->sw->Debounce();
        bool state = button->sw->Pressed();
        if (state != button->previousState && state) {
            core->setHIDValue(button->index, 1);
        }
        button->previousState = state;
    }

    for (auto led : leds) {
        bool state = core->getHIDValue(led->index) > 0.f;
        dsy_gpio_write(led->led, state);
    }
}
