/*
  ==============================================================================

    ModuleCore.cpp
    Created: 19 Jan 2024 9:49:34am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "ModuleCore.h"

namespace ydaisy {

ModuleCore::ModuleCore(DSPKernel* kernel, vector<HIDElement> hidDescription, int midiChannel) : dspKernel(kernel), midiChannel(midiChannel), hidDesc(hidDescription) {
    hidState.resize(hidDescription.size());
}

ModuleCore::~ModuleCore() {
    delete dspKernel;
}

vector<HIDElement> ModuleCore::getHIDDescription() {
    return hidDesc;
}

void ModuleCore::init(int channelCount, double sampleRate) {
    dspKernel->init(channelCount, sampleRate);
}

void ModuleCore::process(float** buf, int frameCount) {
    dspKernel->process(buf, frameCount);
}

void ModuleCore::processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB) {
    if (midiChannel == -1 || midiChannel == channel) {
        dspKernel->processMIDI(messageType, channel, dataA, dataB);
    }
}

void ModuleCore::lockHID(unsigned int index) {
    if (index < hidState.size()) {
        hidState.at(index).isLock = true;
    }
}

float ModuleCore::getDSPValue(unsigned int index) {
    return dspKernel->getUIValue(index);
}

void ModuleCore::dumpAllParameters() {
    dspKernel->dumpParameters();
}

void ModuleCore::setHIDValue(unsigned int index, float value) {
    if (index < hidState.size()) {
        HIDState* state = &hidState.at(index);
        if (state->isLock && fabs(state->value - value) > 0.05) {
            state->isLock = false;
        }
        if (!state->isLock) {
            state->value = value;
            updateHIDValue(index, value);
        }
    }
}

float ModuleCore::getHIDValue(unsigned int index) {
    if (index < hidState.size()) {
        return hidState.at(index).value;
    } else {
        return 0;
    }
}

void ModuleCore::setDSPValue(unsigned int index, float value) {
    dspKernel->setParameterValue(index, value);
}

vector<Parameter*> ModuleCore::getAllParameters() {
    return dspKernel->getAllParameters();
}

}
