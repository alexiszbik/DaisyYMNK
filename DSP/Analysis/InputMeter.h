#pragma once

#include "../Filters/OnePoleSmoother.h"

namespace ydaisy {

class InputMeter {
private:
    static constexpr float kLevelThreshold = 0.002f;
    static constexpr float kClipThreshold = 0.85f;
    static constexpr float kSmoothingMs = 5.f;
    static constexpr float kClipHoldMs = 500.f;

public:
    void Init(float sampleRate, int channelCount);
    void ProcessBlock(float** buf, int frameCount);

    bool hasSignal() const { return smoothedRms > kLevelThreshold; }
    bool isClipping() const { return clipHoldSamples > 0; }
    float getRms() const { return smoothedRms; }

private:
    OnePoleSmoother smoother;
    float smoothedRms = 0.f;
    int channelCount = 1;
    int clipHoldSamples = 0;
    int clipHoldDuration = 0;
};

}
