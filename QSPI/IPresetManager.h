#pragma once

#define MAX_PRESET_SIZE    64   // floats par preset
#define MAX_PRESETS        16   // can we do more ?

class IPresetManager
{
public:
    virtual bool Save(const float* preset, uint8_t size, uint8_t index) = 0;
    virtual const float* Load(uint8_t index) const = 0; 
};
