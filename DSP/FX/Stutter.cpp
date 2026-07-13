#include "Stutter.h"

#include <algorithm>

void Stutter::Init(int inChannelCount, float sampleRate) {
    channelCount = std::min(inChannelCount, kMaxChannels);
    depthRampDuration = static_cast<long>(sampleRate * kDepthRampSec);
    gateSmoother.setImmediate(0.f);
    depthSmoother.setImmediate(0.f);
    depth = 0.f;
}

void Stutter::SetDepth(float inDepth) {
    depth = inDepth;
}

void Stutter::SetGate(bool open) {
    gateSmoother.setValue(open ? 1.f : 0.f);
}

void Stutter::Process(float** buf, int frameCount) {
    if (frameCount <= 0 || channelCount <= 0) {
        return;
    }

    depthSmoother.setValue(depth);
    depthSmoother.dezipperCheck(depthRampDuration);
    gateSmoother.dezipperCheck(kGateRampSamples);

    for (int i = 0; i < frameCount; i++) {
        const float gate = gateSmoother.getAndStep();
        const float depthValue = depthSmoother.getAndStep();
        const float volume = 1.f - gate * depthValue;

        for (int ch = 0; ch < channelCount; ch++) {
            buf[ch][i] *= volume;
        }
    }
}
