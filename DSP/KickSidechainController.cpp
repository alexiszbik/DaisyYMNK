#include "KickSidechainController.h"
#include "SignalProcessing.h"

namespace ydaisy {

void KickSidechainController::init(float sampleRate) {
    this->sampleRate = sampleRate;
    gain.setImmediate(1.f);
}

void KickSidechainController::setTempo(double tempo) {
    this->tempo = tempo;
}

void KickSidechainController::setDepth(float depth) {
    this->depth = clamp01(depth);
}

long KickSidechainController::releaseSamplesForTempo() const {
    // One quarter note at current tempo.
    const double beatSec = 60.0 / tempo;
    return static_cast<long>(sampleRate * beatSec);
}

void KickSidechainController::trigger() {
    gain.setImmediate(1.f - depth);
    gain.setValue(1.f);
    gain.dezipperCheck(releaseSamplesForTempo());
}

void KickSidechainController::process(float** buf, int channelCount, int frameCount) {

    for (int i = 0; i < frameCount; i++) {
        const float g = gain.getAndStep();

        for (int ch = 0; ch < channelCount; ch++) {
            buf[ch][i] *= g;
        }
    }
}

}
