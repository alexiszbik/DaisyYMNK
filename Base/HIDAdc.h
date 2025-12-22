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
        for (float v : buffer) {
            sum += v;
        }

        float avg = sum / N;

        if (fabs(avg - value) < deadband) {
            if (deadBandTimer < N*4) { //16 => best value I found for normal ADC, 4 for Mux ...
                deadBandTimer++;
            } else {
                return false;
            }
        } else {
            deadBandTimer = 0;
        }

        if (value != avg) {
            value = avg;
            return true;
        }
        return false;
    }

    unsigned int index = 0;
    float value = 0;

private:
    static constexpr uint8_t N = 4;
    float buffer[N] = {}; //Do we really need a buff
    uint8_t pos = 0;

    const float deadband = 0.008f; 
    int deadBandTimer = 0;
};

