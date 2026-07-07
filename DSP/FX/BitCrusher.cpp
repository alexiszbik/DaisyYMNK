#include "BitCrusher.h"

#include "../SignalProcessing.h"

#include <algorithm>
#include <cmath>

using namespace ydaisy;

void BitCrusher::Init(int channelCount) {
    this->channelCount = std::min(channelCount, kMaxChannels);
    SetBitDepth(16.f);
    SetRepeatRate(1.f);
    Reset();
}

void BitCrusher::SetBitDepth(float inBitDepth) {
    bitDepth = inBitDepth;
    UpdateDerivedValues();
}

void BitCrusher::SetRepeatRate(float rate) {
    repeatRate = clamp(rate, 0.f, 1.f);
    UpdateDerivedValues();
}

void BitCrusher::UpdateDerivedValues() {
    bypass = (bitDepth >= 16.f && repeatRate >= 1.f);

    const float clampedDepth = clamp(bitDepth - 1.f, 0.f, 15.f);
    crushFactor = powf(2.f, clampedDepth);
    holdSamples = clamp(truncf(64.f - repeatRate * 64.f), 0.f, 64.f);
}

void BitCrusher::Reset() {
    for (int i = 0; i < kMaxChannels; i++) {
        lastSampleValue[i] = 0.f;
        holdCounter[i] = 0.f;
    }
}

float BitCrusher::Process(float in, int channel) {
    if (bypass || channel < 0 || channel >= channelCount) {
        return in;
    }

    if (holdCounter[channel] > 0.f) {
        holdCounter[channel] -= 1.f;
    } else {
        lastSampleValue[channel] = truncf(in * crushFactor) / crushFactor;
        holdCounter[channel] = holdSamples;
    }

    return lastSampleValue[channel];
}
