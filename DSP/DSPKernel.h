/*
  ==============================================================================

    DSPKernel.h
    Created: 8 Nov 2023 4:50:34pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <vector>
#include "MidiMessageType.h"
#include "Parameter.h"
#include "Preset.h"

#define MIDI_CTRL_START 10

#if defined _SIMULATOR_
#include <iostream>
#endif

using namespace std;

namespace ydaisy {

class DSPKernel {
public:
    DSPKernel(vector<ParameterDesc> parameterDescriptors);
    virtual ~DSPKernel();
    
public:
    virtual void init(int channelCount, double sampleRate);
    virtual void process(float** buf, int frameCount);
    
    virtual void processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB);
    
protected:
    virtual void updateParameter(int parameterIndex, float value);
    
public:
    void setParameterValue(int index, float value);
    const char* getParameterName(int index);
    float getUIValue(int index);
    
    vector<Parameter*> getAllParameters();
    Parameter* getLastChangedParameter();
    
    //void loadPreset(Preset<const char*>* preset);
    void loadPreset(const float* preset);
    
    void dumpParameters();
    
protected:
    int getParameterCount();
    float getValue(int index);
    void updateParameters();
    
protected:
    int channelCount = 0;
    double sampleRate = 0;
    
private:
    vector<Parameter*> parameters;
    Parameter* lastChangedParameter = nullptr;
};

}
