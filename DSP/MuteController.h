#pragma once

#include "MidiMessageType.h"
#include "SmoothValue.h"

namespace ydaisy {

class MuteController {
public:
    static constexpr int kMuteCC = 80;
    static constexpr int kMuteThreshold = 60;
    static constexpr float kFadeTimeSec = 0.005f;

    void init(float sampleRate);
    bool processMIDI(MIDIMessageType type, int dataA, int dataB);
    void process(float** buf, int channelCount, int frameCount);

    bool isMuted() const { return muted; }

private:
    SmoothValue gain;
    bool muted = false;
    long rampDuration = 0;
};

}
