/*
  ==============================================================================

    Preset.h
    Created: 17 Jan 2024 9:41:20am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

using namespace std;

template<typename PresetT> class PresetParameter {
public:
    PresetT name;
    float value;
};

template<typename PresetT, size_t MaxParams = 64>
class Preset
{
public:
    PresetParameter<PresetT> params[MaxParams];
    size_t count = 0;

    Preset(std::initializer_list<PresetParameter<PresetT>> init)
    {
        for(const auto& p : init)
        {
            if(count < MaxParams)
                params[count++] = p;
        }
    }

    const PresetParameter<PresetT>* begin() const { return params; }
    const PresetParameter<PresetT>* end()   const { return params + count; }

    size_t size() const { return count; }
};

