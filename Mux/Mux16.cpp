#include "Mux16.h"

Mux16::Mux16() {
    
}

void Mux16::Init(AdcHandle* adc_handle,
                 Pin s0,
                 Pin s1,
                 Pin s2,
                 Pin s3)
{
    adc_ = adc_handle;

    adc_idx_ = 0;

    s0_.Init(s0, GPIO::Mode::OUTPUT);
    s1_.Init(s1, GPIO::Mode::OUTPUT);
    s2_.Init(s2, GPIO::Mode::OUTPUT);
    s3_.Init(s3, GPIO::Mode::OUTPUT);
}

void Mux16::Select(uint8_t ch)
{
    s0_.Write(ch & 0x01);
    s1_.Write((ch >> 1) & 0x01);
    s2_.Write((ch >> 2) & 0x01);
    s3_.Write((ch >> 3) & 0x01);
}

float Mux16::Read(uint8_t channel, bool& valueChanged)
{
    if(channel > 15)
        return 0.0f;

    Select(channel);
    System::DelayUs(550);

    float v = adc_->GetFloat(adc_idx_);

    valueChanged = values[channel].setValue(v);

    /*// Filtering ?
    last_[channel] += 0.15f * (v - last_[channel]);*/

    return values[channel].value;//last_[channel];
}

