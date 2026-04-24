#include "StereoChorus.h"
#include <daisysp.h>
#include <math.h>

using namespace daisysp;

void StereoChorus::Init(float sampleRate)
{
    sampleRate_ = sampleRate;

    for (int i = 0; i < 2; i++) {
        del_[i].Init();
        lfoPhase_[i] = 0.f;
        lfoFreq_[i]  = 0.f;
    }

    feedback_ = 0.2f;
    SetDelay(0.75f);
    SetLfoFreq(0.3f);
    SetLfoDepth(0.9f);
}

float StereoChorus::Process(float in, int channel)
{
    float lfo = ProcessLfo(channel);
    del_[channel].SetDelay(lfo + delay_);

    float wet = del_[channel].Read();
    del_[channel].Write(in + wet * feedback_);

    return (in + wet) * 0.707f;
}

void StereoChorus::SetFeedback(float feedback)
{
    feedback_ = fclamp(feedback, 0.f, 1.f);
}

void StereoChorus::SetLfoDepth(float depth)
{
    depth    = fclamp(depth, 0.f, 0.93f);
    lfoAmp_ = depth * delay_;
}

void StereoChorus::SetLfoFreq(float freq)
{
    float inc = 4.f * freq / sampleRate_;

    float sign = (lfoFreq_[0] < 0.f) ? -1.f : 1.f;
    lfoFreq_[0] = fclamp(inc * sign, -0.25f, 0.25f);

    // Right channel starts in opposite direction
    lfoFreq_[1] = -lfoFreq_[0];
}

void StereoChorus::SetDelay(float delay)
{
    delay = 0.1f + delay * 7.9f; // 0.1 to 8 ms
    SetDelayMs(delay);
}

void StereoChorus::SetDelayMs(float ms)
{
    ms     = daisysp::fmax(0.1f, ms);
    delay_ = ms * 0.001f * sampleRate_;
    lfoAmp_ = daisysp::fmin(lfoAmp_, delay_);
}

float StereoChorus::ProcessLfo(int channel)
{
    lfoPhase_[channel] += lfoFreq_[channel];

    if (lfoPhase_[channel] > 1.f)
    {
        lfoPhase_[channel] = 1.f - (lfoPhase_[channel] - 1.f);
        lfoFreq_[channel] *= -1.f;
    }
    else if (lfoPhase_[channel] < -1.f)
    {
        lfoPhase_[channel] = -1.f - (lfoPhase_[channel] + 1.f);
        lfoFreq_[channel] *= -1.f;
    }

    return lfoPhase_[channel] * lfoAmp_;
}
