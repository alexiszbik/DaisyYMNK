#pragma once

#include "SmoothValue.h"

namespace ydaisy {

class KickSidechainController {
public:
    void init(float sampleRate);
    void setTempo(double tempo);
    void setDepth(float depth);
    float getDepth() const { return depth; }

    void trigger();
    void process(float** buf, int channelCount, int frameCount);

private:
    long releaseSamplesForTempo() const;

    SmoothValue gain;
    float sampleRate = 48000.f;
    double tempo = 120.0;
    float depth = 0.f;
};

}
