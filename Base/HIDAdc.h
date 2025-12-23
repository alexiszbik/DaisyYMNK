#pragma once

#include "daisy_seed.h"

using namespace daisy;

struct HIDAdc
{
    HIDAdc(unsigned int index) : index(index) {}
    HIDAdc() {}

    bool setValue(float input)
    {
        // EMA
        float newValue = value + alpha * (input - value);

        if (fabs(newValue - value) < deadband)
        {
            if(deadBandTimer < deadbandHold)
                deadBandTimer++;
            else
                return false;
        }
        else
        {
            deadBandTimer = 0;
        }

        value = newValue;
        return true;
    }

    unsigned int index = 0;
    float value = 0.f;

private:
    const float alpha = 0.25f;      // ≈ N=4
    const float deadband = 0.002f;
    const int deadbandHold = 16;
    int deadBandTimer = 0;
};
