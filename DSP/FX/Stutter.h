#pragma once

#include "../SmoothValue.h"

class Stutter {
public:
    static constexpr int kMaxChannels = 2;
    static constexpr int kGateRampSamples = 50;
    static constexpr float kDepthRampSec = 0.02f;

    void Init(int channelCount, float sampleRate);
    void SetDepth(float depth);
    void SetGate(bool open);

    void Process(float** buf, int frameCount);

private:
    ydaisy::SmoothValue gateSmoother;
    ydaisy::SmoothValue depthSmoother;

    int channelCount = kMaxChannels;
    float depth = 0.f;
    long depthRampDuration = 0;
};
