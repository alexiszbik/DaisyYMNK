/*
  ==============================================================================

    DSP.h
    Created: 15 Nov 2023 3:57:31pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once
#include <math.h>

namespace ydaisy {

inline float dryWet(float inA, float inB, float value) {
    value = value*value;
    return value * inA + (1 - value) * inB;
}

inline float valueMap(float value, float min, float max) {
    float range = (max - min);
    return value * range + min;
}

inline int valueMap(float value, int min, int max) {
    int range = (max - min);
    return round(value * range + min);
}

inline float clamp(float value, float min, float max) {
    return fmaxf(fminf(value, max), min);
}

}
