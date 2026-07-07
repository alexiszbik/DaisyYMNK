#pragma once

class FrequencyShifter {
public:
    static constexpr int kMaxChannels = 2;

    void Init(float sampleRate, int channelCount = kMaxChannels);
    void SetFrequency(float frequencyHz);
    void Reset();

    float Process(float in, int channel);

private:
    struct BiquadStage {
        float b0 = 0.f;
        float b1 = 0.f;
        float b2 = 0.f;
        float a1 = 0.f;
        float a2 = 0.f;
        float in1 = 0.f;
        float in2 = 0.f;
        float out1 = 0.f;
        float out2 = 0.f;

        void SetCoefficients(const float coeff[5]);
        float Process(float in);
        void Reset();
    };

    struct ChannelState {
        float phase = 0.f;
        BiquadStage dir1;
        BiquadStage dir2;
        BiquadStage phase1;
        BiquadStage phase2;
    };

    int channelCount = kMaxChannels;
    float sampleRate = 48000.f;
    float phaseIncrement = 0.f;
    ChannelState channels[kMaxChannels];
};
