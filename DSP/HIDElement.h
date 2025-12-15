/*
  ==============================================================================

    HIDElement.h
    Created: 8 Nov 2023 10:35:59pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <string>

using namespace std;

namespace ydaisy {

enum HIDType {
    kKnob = 0,
    kRotary8,
    kSwitch,
    kButton,
    kLed,
    kCVInput,
    kGateInput,
};

struct HIDPin {
    unsigned int pin;
    bool isMux = false;
    unsigned int muxId = 0;
    
    HIDPin(unsigned int pin) : pin(pin) {
    }
    HIDPin(unsigned int muxId, unsigned int pin) : pin(pin), muxId(muxId) {
        isMux = true;
    }
};

struct HIDElement {
    unsigned int index;
    HIDType type;
    HIDPin pin;
    const char* name;
    
    bool requireAdc() {
        return !pin.isMux && (type == kKnob || type == kRotary8);
    }
};

}
