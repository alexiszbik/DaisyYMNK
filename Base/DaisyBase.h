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
static DaisySeed hw;

class DaisyBase {
public:
    DaisyBase(ModuleCore *core);

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
#if TEST_MODE
    Oscillator testOsc;
#endif
};