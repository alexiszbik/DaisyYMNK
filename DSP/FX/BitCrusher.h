#pragma once

class BitCrusher {
public:
    static constexpr int kMaxChannels = 2;

    void Init(int channelCount = kMaxChannels);
    void SetBitDepth(float bitDepth);
    void SetRepeatRate(float rate);
    void Reset();

    float Process(float in, int channel);

private:
    void UpdateDerivedValues();

    int channelCount = kMaxChannels;
    float bitDepth = 16.f;
    float repeatRate = 1.f;
    float crushFactor = 32768.f;
    float holdSamples = 0.f;
    bool bypass = true;

    float lastSampleValue[kMaxChannels] = {};
    float holdCounter[kMaxChannels] = {};
};
