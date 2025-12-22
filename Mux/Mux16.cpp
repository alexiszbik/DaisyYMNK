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

    auto init_gpio = [](dsy_gpio& g, Pin p)
    {
        g.pin  = p;
        g.mode = DSY_GPIO_MODE_OUTPUT_PP;
        g.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&g);
    };

    init_gpio(s0_, s0);
    init_gpio(s1_, s1);
    init_gpio(s2_, s2);
    init_gpio(s3_, s3);
}

void Mux16::Select(uint8_t ch)
{
    dsy_gpio_write(&s0_, ch & 0x01);
    dsy_gpio_write(&s1_, (ch >> 1) & 0x01);
    dsy_gpio_write(&s2_, (ch >> 2) & 0x01);
    dsy_gpio_write(&s3_, (ch >> 3) & 0x01);
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

