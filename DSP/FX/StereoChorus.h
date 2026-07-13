#pragma once

#include <stdint.h>
#include "Utility/delayline.h"

#include "Filters/FastOnePole.h"

class StereoChorus
{
public:
    void Init(float sampleRate);

    // Process one sample for the given channel (0 = left, 1 = right).
    // Channels must be called in order: 0 then 1 for each sample.
    float Process(float in, int channel);

    void SetFeedback(float feedback);
    void SetLfoDepth(float depth);
    void SetLfoFreq(float freq);
    void SetDelay(float delay);
    void SetDelayMs(float ms);

private:
    float sampleRate_;
    static constexpr int32_t kDelayLength = 2400; // 50 ms at 48kHz

    float feedback_;
    float delay_;

    // Per-channel triangle LFOs running in opposite directions
    float lfoPhase_[2];
    float lfoFreq_[2];
    float lfoAmp_;

    daisysp::DelayLine<float, kDelayLength> del_[2];

    float ProcessLfo(int channel);

    FastOnePole hpFilter[2];
};
