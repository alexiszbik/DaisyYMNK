#pragma once
#include "daisy_seed.h"

#include "../Base/HIDAdc.h"

using namespace daisy;

class Mux16
{
public:
    Mux16();
    void Init(AdcHandle* adc_handle,
              Pin s0,
              Pin s1,
              Pin s2,
              Pin s3);

    float Read(uint8_t channel, bool& valueChanged);
  private:
    void Select(uint8_t ch);

    AdcHandle* adc_;
    uint16_t   adc_idx_;

    dsy_gpio s0_, s1_, s2_, s3_;

    //float last_[16];
    HIDAdc values[16];
};
