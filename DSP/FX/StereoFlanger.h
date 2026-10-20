#pragma once
#ifndef DSY_STEREO_FLANGER_H
#define DSY_STEREO_FLANGER_H
#ifdef __cplusplus

#include <stdint.h>
#include "Utility/delayline.h"

#include "Filters/FastOnePole.h"

/** @file StereoFlanger.h */

/** @brief Stereo flanging audio effect
 *
 * Generates a modulating phase shifted copy of a signal, and recombines
 * with the original to create a 'flanging' sound effect.
 */
class StereoFlanger
{
  public:
    /** Initialize the modules
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate);

    /** Get the next sample
        \param in Sample to process
        \param channel 0 = left, 1 = right
        \param mix Dry/wet mix 0-1
    */
    float Process(float in, int channel, float mix);

    /** How much of the signal to feedback into the delay line.
        \param feedback Works 0-1.
    */
    void SetFeedback(float feedback);

    /** How much to modulate the delay by.
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
        \param freq Frequency in Hz
    */
    void SetLfoFreq(float freq);

    /** Set the internal delay rate.
        \param delay Tuned for 0-1. Maps to .1 to 7 ms.
    */
    void SetDelay(float delay);

    /** Set the delay time in ms.
        \param ms Delay time in ms.
    */
    void SetDelayMs(float ms);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength = 960; // 20 ms at 48kHz = .02 * 48000

    float feedback_;

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float delay_;

    daisysp::DelayLine<float, kDelayLength> del_[2];

    float ProcessLfo();

    FastOnePole hpFilter[2];
};
#endif
#endif
