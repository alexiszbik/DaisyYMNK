#pragma once

#include "Filters/OnePoleSmoother.h"

namespace ydaisy {

class InputMeter {
private:
    static constexpr float kLevelThreshold = 0.002f;
    static constexpr float kClipThreshold = 0.975f;
    static constexpr float kSmoothingMs = 5.f;
public:
    void Init(float sampleRate, int channelCount);
    void ProcessBlock(float** buf, int frameCount);

    bool hasSignal() const { return smoothedRms > kLevelThreshold; }
    bool isClipping() const { return smoothedRms > kClipThreshold; }
    float getRms() const { return smoothedRms; }

private:
    OnePoleSmoother smoother;
    float smoothedRms = 0.f;
    int channelCount = 1;
};

}
