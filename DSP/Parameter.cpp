/*
  ==============================================================================

    Parameter.cpp
    Created: 8 Nov 2023 10:59:38pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "Parameter.h"

namespace ydaisy {

Parameter::Parameter(ParameterDesc desc) : desc(desc) {
    if (desc.isSmoothed) {
        smoothValue = new SmoothValue();
        smoothValue->setImmediate(0);
    }
}

Parameter::~Parameter() {
    delete smoothValue;
}

const char* Parameter::getName() {
    return desc.name;
}

void Parameter::setValue(float value) {
    if (smoothValue) {
        smoothValue->setValue(value);
    } else {
        currentValue = value;
    }
}

void Parameter::update() {
    if (smoothValue) {
        smoothValue->dezipperCheck(800);
        currentValue = smoothValue->getAndStep();
    }
}

float Parameter::getValue() {
    return currentValue;
}

float Parameter::getUIValue() {
    if (smoothValue) {
        return smoothValue->getGoal();
    } else {
        return currentValue;
    }
}

bool Parameter::useSmoothValue() {
    return smoothValue != nullptr;
}

}
