/*
  ==============================================================================

    ModuleCore.h
    Created: 19 Jan 2024 9:49:34am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "HIDElement.h"
#include "DSPKernel.h"

namespace ydaisy {

struct HIDState {
    float value = 0;
    bool isLock = false;
};

class ModuleCore {
public:
    ModuleCore(DSPKernel* kernel, vector<HIDElement> hidDescription, int midiChannel = -1);
    virtual ~ModuleCore();
    
public:

    vector<HIDElement>& getHIDDescription();
    
    void init(int channelCount, double sampleRate);
    void process(float** buf, int frameCount);
    
    virtual void processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB);
    
    void setHIDValue(unsigned int index, float value);
    float getHIDValue(unsigned int index);
    void setDSPValue(unsigned int index, float value);
    
    void dumpAllParameters();
    float getDSPValue(unsigned int index);
    
    vector<Parameter*> getAllParameters();
    
protected:
    void lockHID(unsigned int index);
    
protected:
    virtual void updateHIDValue(unsigned int index, float value) = 0;

protected:
    DSPKernel* dspKernel;
    int midiChannel = -1;
    
private:
    vector<HIDElement> hidDesc;
    vector<HIDState> hidState;
    
};

}
