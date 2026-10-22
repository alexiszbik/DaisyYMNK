#pragma once

#include "Env/AttackDecay.h"

namespace ydaisy {

class KickSidechainController {
public:
    static constexpr float kAttackTimeMs = 5.f;

    void init(float sampleRate);
    void setTempo(double tempo);

    void trigger();
    void process(float** buf, int channelCount, int frameCount, const float* depthBuf);

private:
    float releaseTimeSec() const;

    AttackDecay env;
    double tempo = 120.0;
};

}
