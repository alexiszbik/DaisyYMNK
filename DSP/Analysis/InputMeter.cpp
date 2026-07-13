#include "InputMeter.h"

#include <algorithm>
#include <cmath>

namespace ydaisy {

void InputMeter::Init(float sampleRate, int channelCount) {
    smoother.Init(kSmoothingMs, sampleRate);
    clipHoldDuration = static_cast<int>(sampleRate * kClipHoldMs * 0.001f);
    smoothedRms = 0.f;
    clipHoldSamples = 0;
    this->channelCount = channelCount;
}

void InputMeter::ProcessBlock(float** buf, int frameCount) {
    if (frameCount <= 0 || channelCount <= 0) {
        return;
    }

    float sumSq = 0.f;
    float blockPeak = 0.f;

    for (int i = 0; i < frameCount; i++) {
        for (int ch = 0; ch < channelCount; ch++) {
            const float sample = fabsf(buf[ch][i]);
            blockPeak = fmaxf(blockPeak, sample);
            sumSq += sample * sample;
        }
    }

    const float sampleCount = static_cast<float>(frameCount * channelCount);
    const float blockRms = sqrtf(sumSq / sampleCount);
    smoothedRms = smoother.Process(blockRms);

    if (blockPeak > kClipThreshold) {
        clipHoldSamples = clipHoldDuration;
    } else {
        clipHoldSamples = std::max(0, clipHoldSamples - frameCount);
    }
}

}
