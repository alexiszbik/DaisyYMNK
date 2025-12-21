/*
  ==============================================================================

    DSPKernel.cpp
    Created: 8 Nov 2023 4:50:34pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "DSPKernel.h"
#include "DaisyYMNK/DSP/SignalProcessing.h"
#include <cmath>

namespace ydaisy {

DSPKernel::DSPKernel(vector<ParameterDesc> parameterDescriptors)
{
    for (auto& paramDesc : parameterDescriptors) {
        Parameter* p = new Parameter(paramDesc);
        parameters.push_back(p);
    }
    if (parameters.size()) {
        lastChangedParameter = parameters.front();
    }
}

DSPKernel::~DSPKernel() {
    for (auto& p : parameters) {
        delete p;
    }
    parameters.clear();
}

void DSPKernel::setParameterValue(int index, float value) {
    auto param = parameters.at(index);
//TODO => should be done on HID side !
    if (fabs(param->getValue() - value) > 0.005) {
        lastChangedParameter = param;
    }

    param->setValue(value);
    
    if (parameters.at(index)->useSmoothValue() == false) {
        updateParameter(index, parameters.at(index)->getValue());
    }
}

Parameter* DSPKernel::getLastChangedParameter() {
    return lastChangedParameter;
}
/*
void DSPKernel::loadPreset(Preset<const char*>* preset) {
    for (auto presetParam : *preset) {
        int index = 0;
        for (auto p : parameters) {
            if (presetParam.name == p->getName()) {
                setParameterValue(index, presetParam.value);
                break;
            }
            index++;
        }
    }
}
*/

void DSPKernel::loadPreset(const float* preset) {
    uint8_t k = getParameterCount();
    while(k--) {
        setParameterValue(k, clamp(preset[k], 0.f, 1.f));
    }
}

float DSPKernel::getValue(int index) {
    return parameters.at(index)->getValue();
}

const char* DSPKernel::getParameterName(int index) {
    return parameters.at(index)->getName();
}

float DSPKernel::getUIValue(int index) {
    return parameters.at(index)->getUIValue();
}

int DSPKernel::getParameterCount() {
    return (int)parameters.size();
}

void DSPKernel::dumpParameters() {
#if defined _SIMULATOR_
    cout << "{" << endl;
    for (auto& param : parameters) {
        cout << "{\"" << param->getName() << "\", " << param->getValue() << "}," << endl;
    }
    cout << "}" << endl;
#endif
}

void DSPKernel::init(int channelCount, double sampleRate) {
    this->channelCount = channelCount;
    this->sampleRate = sampleRate;
}

void DSPKernel::process(float** buf, int frameCount) {
}

void DSPKernel::processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB) {
    switch (messageType) {
        case MIDIMessageType::kControlChange : {
            int parameterIdx = dataA - MIDI_CTRL_START;
            if (parameterIdx >= 0 && parameterIdx < parameters.size()) {
                setParameterValue(parameterIdx, ((float)dataB)/127.f);
            }
        }
            break;
        
        default:
            break;
    }
}

void DSPKernel::updateParameter(int parameterIndex, float value) {
}

void DSPKernel::updateParameters() {
    for (auto& parameter : parameters) {
        parameter->update();
    }
}

const std::vector<Parameter*>& DSPKernel::getAllParameters() const noexcept {
    return parameters;
}

}
