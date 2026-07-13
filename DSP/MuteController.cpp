#include "MuteController.h"

namespace ydaisy {

void MuteController::init(float sampleRate) {
    rampDuration = static_cast<long>(sampleRate * kFadeTimeSec);
    gain.setImmediate(1.f);
    muted = false;
}

bool MuteController::processMIDI(MIDIMessageType type, int dataA, int dataB) {
    if (type == kControlChange && dataA == kMuteCC) {
        muted = dataB > kMuteThreshold;
        return true;
    }
    return false;
}

void MuteController::process(float** buf, int channelCount, int frameCount) {
    if (frameCount <= 0 || channelCount <= 0) {
        return;
    }

    gain.setValue(muted ? 0.f : 1.f);
    gain.dezipperCheck(rampDuration);

    for (int i = 0; i < frameCount; i++) {
        const float g = gain.getAndStep();

        for (int ch = 0; ch < channelCount; ch++) {
            buf[ch][i] *= g;
        }
    }
}

}
