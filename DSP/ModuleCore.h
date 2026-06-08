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
    float value = 0; //lockedValue
    float lastValue = 0;
    bool isLock = false;
    // Preset value X (0..1) when locked after load; used for soft takeover.
    float lockPresetValue = 0.f;
    // Physical knob at lock K (0..1); -1 = capture on first HID sample after lock.
    float lockKnobPos = -1.f;

    void lock() {
        isLock = true;
    }
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
    
    virtual void setHIDValue(unsigned int index, float value);
    float getHIDValue(unsigned int index);
    void setDSPValue(unsigned int index, float value);
    
    void dumpAllParameters();
    float getDSPValue(unsigned int index);
    
    vector<HIDElement>& getHIDDescription();
    
    const std::vector<Parameter*>& getAllParameters() const noexcept;
    
protected:
    void lockHID(unsigned int index);
    //virtual bool unlockCondition(unsigned int index, float value, HIDState* hidState);
    // If true while locked, writes *outMapped to the DSP instead of raw hardware (soft takeover).
    virtual bool trySoftTakeover(unsigned int index, float hwValue, HIDState* state, float* outMapped);
    static float softTakeoverMap(float k, float K, float X) noexcept;
    static bool physicalKnobCrossedPreset(float prev, float curr, float X) noexcept;
    
protected:
    //This is the method to override to transmit things to the DSP
    virtual void updateHIDValue(unsigned int index, float value) = 0;

protected:
    DSPKernel* dspKernel;
    int midiChannel = -1;
    
    IPresetManager* presetManager = nullptr;
    IDisplayManager* displayManager = nullptr;

    vector<HIDState> hidState;
    
private:
    vector<HIDElement> hidDesc;
    
#if !defined _SIMULATOR_
   
#endif
    
};

}
