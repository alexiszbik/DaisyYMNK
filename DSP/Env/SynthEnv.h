/*
  ADSR envelope — adapted from DaisySP (MIT license, Electrosmith).
  Adds Kill() for immediate stop and reset (not available on daisysp::Adsr).
*/

#pragma once

#ifndef SYNTH_ENV_H
#define SYNTH_ENV_H

#include <stdint.h>

/** Segment IDs (prefixed — daisysp also defines ADSR_SEG_* in the global scope). */
enum SynthEnvSeg : uint8_t
{
    SYNTH_ENV_SEG_IDLE    = 0,
    SYNTH_ENV_SEG_ATTACK  = 1,
    SYNTH_ENV_SEG_DECAY   = 2,
    SYNTH_ENV_SEG_RELEASE = 4
};

class SynthEnv
{
  public:
    SynthEnv() {}
    ~SynthEnv() {}

    void Init(float sample_rate, int blockSize = 1);

    void Retrigger(bool hard);

    /** Instant idle: output 0, no release tail. Call when stealing / killing a voice. */
    void Kill();

    float Process(bool gate);

    void SetTime(int seg, float time);
    void SetAttackTime(float timeInS, float shape = 0.0f);
    void SetDecayTime(float timeInS);
    void SetReleaseTime(float timeInS);

  private:
    void SetTimeConstant(float timeInS, float& time, float& coeff);

  public:
    inline void SetSustainLevel(float sus_level)
    {
        sus_level = (sus_level <= 0.f) ? -0.01f
                                       : (sus_level > 1.f) ? 1.f : sus_level;
        sus_level_ = sus_level;
    }

    inline uint8_t GetCurrentSegment() { return mode_; }

    inline bool IsRunning() const { return mode_ != SYNTH_ENV_SEG_IDLE; }

  private:
    float   sus_level_{0.f};
    float   x_{0.f};
    float   attackShape_{-1.f};
    float   attackTarget_{0.0f};
    float   attackTime_{-1.0f};
    float   decayTime_{-1.0f};
    float   releaseTime_{-1.0f};
    float   attackD0_{0.f};
    float   decayD0_{0.f};
    float   releaseD0_{0.f};
    int     sample_rate_;
    uint8_t mode_{SYNTH_ENV_SEG_IDLE};
    bool    gate_{false};
};

#endif
