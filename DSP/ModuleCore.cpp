/*
  ==============================================================================

    ModuleCore.cpp
    Created: 19 Jan 2024 9:49:34am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "ModuleCore.h"
#include <cmath>
#include "daisysp.h"

namespace ydaisy {

ModuleCore::ModuleCore(DSPKernel* kernel, vector<HIDElement> hidDescription, int midiChannel) : dspKernel(kernel), midiChannel(midiChannel), hidDesc(hidDescription) {
    hidState.resize(hidDescription.size());
}

ModuleCore::~ModuleCore() {
    delete dspKernel;
}

void ModuleCore::setDisplayManager(IDisplayManager* inDisplayManager) { this->displayManager = inDisplayManager; }
void ModuleCore::setPresetManager(IPresetManager* inPresetManager)  { this->presetManager = inPresetManager; }

vector<HIDElement>& ModuleCore::getHIDDescription() {
    return hidDesc;
}

void ModuleCore::init(int channelCount, double sampleRate) {
    dspKernel->init(channelCount, sampleRate);
}

void ModuleCore::process(float** buf, int frameCount) {
    dspKernel->process(buf, frameCount);
}

void ModuleCore::processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB) {
    const bool isSystemRealTime = messageType == kTimingClock
        || messageType == kMidiStart
        || messageType == kMidiStop;

    if (isSystemRealTime || midiChannel == -1 || midiChannel == channel) {
        dspKernel->processMIDI(messageType, channel, dataA, dataB);
    }
}

void ModuleCore::lockHID(unsigned int index) {
    if (index < hidState.size()) {
        hidState.at(index).lock();
    }
}

float ModuleCore::getDSPValue(unsigned int index) {
    return dspKernel->getUIValue(index);
}

void ModuleCore::dumpAllParameters() {
    dspKernel->dumpParameters();
}
/*
bool ModuleCore::unlockCondition(unsigned int index, float value, HIDState* hidState) {
    return fabs(hidState->value - value) > 0.05;
}
*/
float ModuleCore::softTakeoverMap(float k, float K, float X) noexcept {
    constexpr float eps = 1e-2f;//1e-5f;
    k = daisysp::fclamp(k, 0.f, 1.f);
    K = daisysp::fclamp(K, 0.f, 1.f);
    X = daisysp::fclamp(X, 0.f, 1.f);

    if (k <= K + eps) {
        if (K <= eps) {
            return k;
        }
        return (k / K) * X;
    }
    if (K >= 1.f - eps) {
        return X + (k - K);
    }
    return X + (k - K) / (1.f - K) * (1.f - X);
}

bool ModuleCore::physicalKnobCrossedPreset(float prev, float curr, float X) noexcept {
    constexpr float eps = 1e-2f;//1e-4f;
    curr = daisysp::fclamp(curr, 0.f, 1.f);
    prev = daisysp::fclamp(prev, 0.f, 1.f);
    X    = daisysp::fclamp(X,    0.f, 1.f);
    if (std::fabs(curr - X) <= eps || std::fabs(prev - X) <= eps) {
        return true;
    }
    return (prev - X) * (curr - X) < 0.f;
}

bool ModuleCore::trySoftTakeover(unsigned int index, float hwValue, HIDState* state, float* outMapped) {
    if (!state->isLock) {
        return false;
    }
    if (index >= hidDesc.size()) {
        return false;
    }
    const HIDElement& element = hidDesc.at(index);
    if (element.type != kKnob) {
        return false;
    }
    const float X = state->lockPresetValue;
    float prev = state->lastValue;
    if (prev < 0.f) {
        prev = hwValue;
    }
    if (state->lockKnobPos < 0.f) {
        state->lockKnobPos = hwValue;
    }
    constexpr float kEdgeUnlockEps = 1e-2f;
    if (hwValue <= kEdgeUnlockEps || hwValue >= 1.f - kEdgeUnlockEps) {
        state->isLock = false;
        return false;
    }
    if (physicalKnobCrossedPreset(prev, hwValue, X)) {
        // Keep continuity at unlock point: output the preset value once,
        // then switch to raw hardware value on next call.
        *outMapped = X;
        state->isLock = false;
        return true;
    }
    *outMapped = softTakeoverMap(hwValue, state->lockKnobPos, X);
    return true;
}

void ModuleCore::setHIDValue(unsigned int index, float value) {
    if (index < hidState.size()) {
        HIDState* state = &hidState.at(index);
        
        if (state->isLock) {
            float mapped = value;
            if (trySoftTakeover(index, value, state, &mapped)) {
                state->value = value;
                updateHIDValue(index, mapped);
                state->lastValue = value;
                return;
            }
        } else {
            state->value = value;
            updateHIDValue(index, value);
        }
        
        state->lastValue = value;
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

const std::vector<Parameter*>& ModuleCore::getAllParameters() const noexcept {
    return dspKernel->getAllParameters();
}

}
