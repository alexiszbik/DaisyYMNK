#include "StereoFlanger.h"
#include <daisysp.h>
#include <math.h>
#include "DaisyYMNK/DSP/SignalProcessing.h"

using namespace daisysp;

void StereoFlanger::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    SetFeedback(.2f);

    for (int i = 0; i < 2; i++) {
        del_[i].Init();

        hpFilter[i].Init(sample_rate);
        hpFilter[i].SetHighpass(90);
    }

    lfo_amp_ = 0.f;
    SetDelay(.75);

    lfo_phase_ = 0.f;
    SetLfoFreq(.3);
    SetLfoDepth(.9);
}

float StereoFlanger::Process(float in, int channel, float mix)
{
    float lfo_sig = ProcessLfo();
    if (channel == 1) {
        lfo_sig *= -1;
    }
    del_[channel].SetDelay(1.f + lfo_sig + delay_);

    float out = del_[channel].Read();
    del_[channel].Write(in - hpFilter[channel].Process(out * feedback_));

    float inMix = in * (1 - mix);
    float outMix = out * mix * 0.707f;

    return outMix + inMix;
}

void StereoFlanger::SetFeedback(float feedback)
{
    feedback_ = fclamp(feedback, 0.f, 1.f);
    feedback_ *= .97f;
}

void StereoFlanger::SetLfoDepth(float depth)
{
    depth    = fclamp(depth, 0.f, .93f);
    lfo_amp_ = depth * delay_;
}

void StereoFlanger::SetLfoFreq(float freq)
{
    freq = 4.f * freq / sample_rate_;
    freq *= lfo_freq_ < 0.f ? -1.f : 1.f;  //if we're headed down, keep going
    lfo_freq_ = fclamp(freq, -.001f, .001f); //clip at +/- .125 * sr
}

void StereoFlanger::SetDelay(float delay)
{
    delay = (.1f + delay * 6.9f); //.1 to 7 ms
    SetDelayMs(delay);
}

void StereoFlanger::SetDelayMs(float ms)
{
    ms     = fmaxf(.1f, ms);
    delay_ = ms * .001f * sample_rate_; //ms to samples

    lfo_amp_ = fminf(lfo_amp_, delay_); //clip this if needed
}

float StereoFlanger::ProcessLfo()
{
    lfo_phase_ += lfo_freq_;

    //wrap around and flip direction
    if(lfo_phase_ > 1.f)
    {
        lfo_phase_ = 1.f - (lfo_phase_ - 1.f);
        lfo_freq_ *= -1.f;
    }
    else if(lfo_phase_ < -1.f)
    {
        lfo_phase_ = -1.f - (lfo_phase_ + 1.f);
        lfo_freq_ *= -1.f;
    }

    return lfo_phase_ * lfo_amp_;
}
