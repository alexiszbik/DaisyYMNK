#pragma once

#include "daisy_seed.h"

using namespace daisy;

//Smoothed part should be in a different class

struct HIDAdc {
    HIDAdc(unsigned int index) : index(index) {}

    HIDAdc() {};

    bool setValue(float input) {
        
        buffer[pos] = input;
        pos = (pos + 1) % N;

        float sum = 0.0f;
        for (float v : buffer)
            sum += v;

        float avg = sum / N;

        if (fabs(avg - value) < deadband)
            return false;

        if (value != avg) {
            value = avg;
            return true;
        }
        return false;
    }

    unsigned int index = 0;
    float value = 0;

private:
    static constexpr int N = 4;
    float buffer[N] = {};
    int pos = 0;

    const float deadband = 0.001f; 
};

