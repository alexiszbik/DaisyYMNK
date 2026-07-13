#pragma once

#include "MidiMessageType.h"
#include "SmoothValue.h"

namespace ydaisy {

class MuteControllerDelegate {
public:
    virtual ~MuteControllerDelegate() = default;
    virtual void muteChanged(bool muted) = 0;
};

class MuteController {
public:
    static constexpr int kMuteCC = 80;
    static constexpr int kMuteThreshold = 60;
    static constexpr float kFadeTimeSec = 0.005f;

    MuteControllerDelegate* delegate = nullptr;

    void init(float sampleRate);
    bool processMIDI(MIDIMessageType type, int dataA, int dataB);
    void process(float** buf, int channelCount, int frameCount);

    bool isMuted() const { return muted; }
    void setMuted(bool value);
    void unmute() { setMuted(false); }

private:
    SmoothValue gain;
    bool muted = false;
    long rampDuration = 0;
};

}
