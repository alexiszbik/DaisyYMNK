#include "InputMeter.h"
#include <cmath>

namespace ydaisy {

void InputMeter::Init(float sampleRate, int channelCount) {
    smoother.Init(kSmoothingMs, sampleRate);
    smoothedRms = 0.f;
    this->channelCount = channelCount;
}

void InputMeter::ProcessBlock(float** buf, int frameCount) {
    if (frameCount <= 0 || channelCount <= 0) {
        return;
    }

    float sumSq = 0.f;

    for (int i = 0; i < frameCount; i++) {
        float peak = 0.f;

        for (int ch = 0; ch < channelCount; ch++) {
            peak = fmaxf(peak, fabsf(buf[ch][i])); //Hmm right now we merge the 2 channels, we could have a peak per channel
        }

        sumSq += peak * peak;
    }

    const float blockRms = sqrtf(sumSq / static_cast<float>(frameCount));
    smoothedRms = smoother.Process(blockRms);
}

}
