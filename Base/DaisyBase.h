#pragma once

#include "daisy_seed.h"
#include "daisysp.h"

#include "HID.h"
#include "../DSP/ModuleCore.h"

#define TEST_MODE 0

using namespace daisy;
using namespace daisysp;
using namespace ydaisy;

static MidiUartHandler midi;

class DaisyBase {
public:
    DaisyBase(DaisySeed* hw, ModuleCore *core);

    ~DaisyBase();

    void init(AudioHandle::AudioCallback cb);
    void listen();
    void process(float** buf, int frameCount);
    void readHID();

private:
    void initMidi();
    void handleMidiMessage(MidiEvent m);

private:
    ModuleCore* core;
    HID* hid;
    DaisySeed* hw;
#if TEST_MODE
    Oscillator testOsc;
#endif
};