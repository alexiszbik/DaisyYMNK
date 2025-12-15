#include "HID.h"

HID::HID(vector<HIDElement> desc) : desc(desc) {
}

void HID::init(DaisySeed &hw) {

    unsigned int adcCount = 0;
    for (auto& hidElmt : desc) {
        if (hidElmt.requireAdc()) {
            adcCount++;
        }
        if (hidElmt.pin.isMux) {
            useMux = true;
        }
    }

    if (useMux) {
        adcCount++;
    }

    AdcChannelConfig adcConfig[adcCount];

    for (auto& hidElmt : desc) {

        //Create Mux if we need one !
        //Right now it only works with 1 unique mux
        if (hidElmt.pin.isMux) {    
            if (mux == nullptr) {
                mux = new Mux16();
                mux->Init(&adcHandle,
                    seed::D0,// S0
                    seed::D1,// S1
                    seed::D2,// S2
                    seed::D3 // S3
                );
                adcConfig[0].InitSingle(seed::A0);
            }
            muxIndexes[hidElmt.pin.pin] = hidElmt.index;  
            muxIndexCount++;                
        } else {
            auto pin = hw.GetPin(hidElmt.pin.pin);

            if (hidElmt.type == kLed) {
                leds.push_back(new HIDLed(hidElmt.index, pin));
            } else if (hidElmt.requireAdc()) {
                adcConfig[adcIndexes.size() + (useMux ? 1 : 0)].InitSingle(pin);
                adcIndexes.push_back(hidElmt.index);
            } else {
                buttons.push_back(new HIDButton(hidElmt.index, pin));
            }
        }
    }

    hw.adc.Init(adcConfig, adcCount);
    hw.adc.Start();
}

void HID::process(DaisySeed &hw, ModuleCore* core) {
    
    mux->ReadAll(muxValues);
    
    uint8_t i = muxIndexCount;
    while (i--) {
        core->setHIDValue(muxIndexes[i], muxValues[i]);
    }

    int k = 0;
    int offset = useMux ? 1 : 0;
    for (auto &adcIdx : adcIndexes) {
        float value = hw.adc.GetFloat(k + offset);
        core->setHIDValue(adcIdx, value);
        k++;
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
