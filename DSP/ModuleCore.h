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

#include "DaisyYMNK/QSPI/IPresetManager.h"
#include "DaisyYMNK/Display/IDisplayManager.h"

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
    void init(int channelCount, double sampleRate);
    void process(float** buf, int frameCount);
    
    void setDisplayManager(IDisplayManager* inDisplayManager);
    void setPresetManager(IPresetManager* inPresetManager);
    
    virtual void processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB);
    
    void setHIDValue(unsigned int index, float value);
    float getHIDValue(unsigned int index);
    void setDSPValue(unsigned int index, float value);
    
    void dumpAllParameters();
    float getDSPValue(unsigned int index);
    
    vector<HIDElement>& getHIDDescription();
    
    vector<Parameter*> getAllParameters();
    Parameter* getLastChangedParameter();

    //Do we keep this ?
    void setValueChangedCallback(void (*cb)(uint8_t, float)) { valueChangedCallback = cb; }
    
protected:
    void lockHID(unsigned int index);
    
protected:
    //This is the method to override to transmit things to the DSP
    virtual void updateHIDValue(unsigned int index, float value) = 0;

protected:
    DSPKernel* dspKernel;
    int midiChannel = -1;
    
    IPresetManager* presetManager = nullptr;
    IDisplayManager* displayManager = nullptr;
    
    void (*valueChangedCallback)(uint8_t, float) = nullptr;
    
private:
    vector<HIDElement> hidDesc;
    vector<HIDState> hidState;
    
#if !defined _SIMULATOR_
   
#endif
    
};

}
