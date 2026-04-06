
#pragma once

#include <math.h>

/** Exponential one-pole lowpass, configured by time constant in milliseconds. */
class OnePoleSmoother
{
public:
    void Init(float timeMs, float sr)
    {
        float x = expf(-1.f / (timeMs * 0.001f * sr));
        a = x;
        b = 1.f - x;
    }

    float Process(float in)
    {
        z = a * z + b * in;
        return z;
    }

private:
    float a = 0.f;
    float b = 0.f;
    float z = 0.f;
};
