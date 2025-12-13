/*
  ==============================================================================

    Preset.h
    Created: 17 Jan 2024 9:41:20am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

using namespace std;

struct PresetParameter {
    const char* name;
    float value;
};

typedef vector<PresetParameter> Preset;
