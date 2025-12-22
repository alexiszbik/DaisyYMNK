#pragma once

#include "daisy_seed.h"
#include "../DSP/ModuleCore.h"
#include "../Mux/Mux16.h"
#include "HIDButton.h"
#include "HIDLed.h"
#include "HIDAdc.h"

using namespace daisy;
using namespace ydaisy;

class HID {
public:
    HID(vector<HIDElement> desc);

    void init(DaisySeed &hw);
    void process(DaisySeed &hw, ModuleCore* core);

private:
    void readMux(uint8_t channel, ModuleCore* core);

private:
    vector<HIDElement> desc;

// we dont need pointers ..
    vector<HIDButton*> buttons;
    vector<HIDLed*> leds;
    vector<HIDAdc> adcs;

// --- Mux part

    bool useMux = false;
    static constexpr uint8_t muxSize = 16;
    uint8_t muxIdxRead = 0;

    float muxValues[muxSize];
    int muxIndexes[muxSize];
    uint8_t muxIndexCount = 0; 

    Mux16* mux = nullptr;

    AdcHandle adcHandle;

};