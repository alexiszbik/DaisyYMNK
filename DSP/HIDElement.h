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

struct HIDElement {
    unsigned int index;
    HIDType type;
    unsigned int pin;
    const char* name;
    
    bool requireAdc() {
        return type == kKnob || type == kRotary8;
    }
};

}
