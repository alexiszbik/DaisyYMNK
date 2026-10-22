#include "KickSidechainController.h"

namespace ydaisy {

void KickSidechainController::init(float sampleRate) {
    env.Init(sampleRate);
    env.SetAttack(kAttackTimeMs * 0.001f);
    env.SetDecay(releaseTimeSec());
}

void KickSidechainController::setTempo(double tempo) {
    this->tempo = tempo;
    env.SetDecay(releaseTimeSec());
}

float KickSidechainController::releaseTimeSec() const {
    return static_cast<float>(60.0 / tempo);
}

void KickSidechainController::trigger() {
    env.SetDecay(releaseTimeSec());
    env.Retrig();
}

void KickSidechainController::process(float** buf, int channelCount, int frameCount, const float* depthBuf) {

    for (int i = 0; i < frameCount; i++) {
        const float g = 1.f - depthBuf[i] * env.Process();

        for (int ch = 0; ch < channelCount; ch++) {
            buf[ch][i] *= g * g;
        }
    }
}

}
