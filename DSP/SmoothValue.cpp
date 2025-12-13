/*
  ==============================================================================

    SmoothValue.cpp
    Created: 8 Nov 2023 10:43:30pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "SmoothValue.h"

namespace ydaisy {

SmoothValue::SmoothValue(float value) {
    setImmediate(value);
}

void SmoothValue::setImmediate(float value) {
    goal = this->value = value;
    inverseSlope = 0.0;
    samplesRemaining = 0;
}

void SmoothValue::setValue(float value) {
    if (this->value != value) {
        this->value = value;
        changeCounter = changeCounter + 1;
    }
}

void SmoothValue::dezipperCheck(long rampDuration)
{
    long changeCounterSnapshot = changeCounter;
    if (updateCounter != changeCounterSnapshot) {
        updateCounter = changeCounterSnapshot;
        startRamp(value, rampDuration);
    }
}

void SmoothValue::startRamp(float newGoal, long duration) {
    value = newGoal;
    
    if (duration == 0) {
        setImmediate(newGoal);
    }
    else {
        inverseSlope = (get() - newGoal) / float(duration);
        samplesRemaining = duration;
        goal = newGoal;
    }
}

float SmoothValue::get() {
    return inverseSlope * float(samplesRemaining) + goal;
}

float SmoothValue::getGoal() {
    return goal;
}

float SmoothValue::getAndStep() {
    if (samplesRemaining > 0) {
        float currentValue = get();
        --samplesRemaining;
        return currentValue;
    }
    else {
        return goal;
    }
}

}
