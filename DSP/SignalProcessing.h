/*
  ==============================================================================

    DSP.h
    Created: 15 Nov 2023 3:57:31pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once
#include <math.h>
#include <daisysp.h>

namespace ydaisy {

inline float sqrtDryWet(float inA, float inB, float value) {
    float a = value;
    float b = 1.f - value;
    a = sqrtf(a);
    b = sqrtf(b);
    return a * inA + b * inB;
}

inline float dryWet(float inA, float inB, float value) {
    value = value*value;
    return value * inA + (1 - value) * inB;
}

inline float valueMap(float value, float min, float max) {
    float range = (max - min);
    return value * range + min;
}

inline float valueMapPow2(float value, float min, float max) {
    float range = (max - min);
    return value * value * range + min;
}

inline float valueMapPow3(float value, float min, float max) {
    float range = (max - min);
    return value * value * value * range + min;
}

inline int valueMap(float value, int min, int max) {
    int range = (max - min);
    return round(value * range + min);
}

inline float clamp(float value, float min, float max) noexcept {
    return fmaxf(fminf(value, max), min);
}

inline float clamp01(float x) noexcept
{
    return fminf(fmaxf(x, 0.f), 1.f);
}

}
