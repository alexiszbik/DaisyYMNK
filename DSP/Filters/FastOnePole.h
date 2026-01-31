
#pragma once

#include "math.h"

class FastOnePole
{
public:
    enum Mode
    {
        LOWPASS,
        HIGHPASS
    };

    void Init(float sampleRate)
    {
        sr_ = sampleRate;
        Reset();
        mode_ = LOWPASS;
    }

    void Reset()
    {
        state_ = 0.0f;
    }

    void SetLowpass(float cutoffHz)
    {
        mode_ = LOWPASS;
        SetCutoff(cutoffHz);
    }

    void SetHighpass(float cutoffHz)
    {
        mode_ = HIGHPASS;
        SetCutoff(cutoffHz);
    }

    inline float Process(float in)
    {
        float lp = (g_ * in + state_) * gi_;
        state_   = g_ * (in - lp) + lp;
        return (mode_ == LOWPASS) ? lp : (in - lp);
    }

private:
    void SetCutoff(float cutoffHz)
    {
        float f = cutoffHz / sr_;
        f = fminf(f, 0.497f);

        // fast tan(pi * f) approximation
        float x = 3.14159265f * f;
        g_  = x + 0.5f * x * x * x;
        gi_ = 1.0f / (1.0f + g_);
    }

private:
    float sr_    = 44100.0f;
    float g_     = 0.0f;
    float gi_    = 1.0f;
    float state_ = 0.0f;
    Mode  mode_  = LOWPASS;
};
