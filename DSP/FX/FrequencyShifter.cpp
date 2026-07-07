#include "FrequencyShifter.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr float kTwoPi = 6.28318530718f;
constexpr float kPhaseOffset = -0.25f;

constexpr float kCoeffDir1[5] = {0.02569f, -0.260502f, -0.260502f, 0.02568f, 1.f};
constexpr float kCoeffDir2[5] = {-1.8685f, 0.870686f, 0.870686f, -1.8685f, 1.f};
constexpr float kCoeffPhase1[5] = {-1.94632f, 0.94657f, 0.94657f, -1.94632f, 1.f};
constexpr float kCoeffPhase2[5] = {-0.83774f, 0.06338f, 0.06338f, -0.83774f, 1.f};
}

void FrequencyShifter::BiquadStage::SetCoefficients(const float coeff[5]) {
    b0 = coeff[2];
    b1 = coeff[3];
    b2 = coeff[4];
    a1 = coeff[0];
    a2 = coeff[1];
}

float FrequencyShifter::BiquadStage::Process(float in) {
    const float out = in * b0 + in1 * b1 + in2 * b2 - out1 * a1 - out2 * a2;

    in2 = in1;
    in1 = in;
    out2 = out1;
    out1 = out;

    return out;
}

void FrequencyShifter::BiquadStage::Reset() {
    in1 = 0.f;
    in2 = 0.f;
    out1 = 0.f;
    out2 = 0.f;
}

void FrequencyShifter::Init(float inSampleRate, int inChannelCount) {
    sampleRate = inSampleRate;
    channelCount = std::min(inChannelCount, kMaxChannels);

    for (int i = 0; i < kMaxChannels; i++) {
        channels[i].dir1.SetCoefficients(kCoeffDir1);
        channels[i].dir2.SetCoefficients(kCoeffDir2);
        channels[i].phase1.SetCoefficients(kCoeffPhase1);
        channels[i].phase2.SetCoefficients(kCoeffPhase2);
    }

    SetFrequency(0.f);
    Reset();
}

void FrequencyShifter::SetFrequency(float frequencyHz) {
    phaseIncrement = frequencyHz / sampleRate;
}

void FrequencyShifter::Reset() {
    for (int i = 0; i < kMaxChannels; i++) {
        channels[i].phase = 0.f;
        channels[i].dir1.Reset();
        channels[i].dir2.Reset();
        channels[i].phase1.Reset();
        channels[i].phase2.Reset();
    }
}

float FrequencyShifter::Process(float in, int channel) {
    if (channel < 0 || channel >= channelCount) {
        return in;
    }

    ChannelState& ch = channels[channel];
    const float original = in;

    float iPath = ch.dir1.Process(in);
    iPath = ch.dir2.Process(iPath);

    float qPath = ch.phase1.Process(original);
    qPath = ch.phase2.Process(qPath);

    const float phase = ch.phase;
    const float cosTerm = cosf(kTwoPi * phase);
    const float sinTerm = cosf(kTwoPi * (phase + kPhaseOffset));

    ch.phase += phaseIncrement;
    ch.phase -= floorf(ch.phase);

    return iPath * cosTerm - qPath * sinTerm;
}
