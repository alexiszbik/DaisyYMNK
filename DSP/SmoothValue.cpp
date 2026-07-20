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
    fillOnce = false;
}

void SmoothValue::setValue(float value) {
    if (this->value != value) {
        this->value = value;
        valueChanged = true;
    }
}

void SmoothValue::dezipperCheck(long rampDuration)
{
    if (valueChanged) {
        valueChanged = false;
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

void SmoothValue::stepBy(size_t n) {

    if (n >= samplesRemaining) {
        samplesRemaining = 0;
    }
    else {
        samplesRemaining -= n;
    }
}

float* SmoothValue::getAndStep(size_t bufferSize, bool& updated) {
    if (samplesRemaining != 0) {
        float current = get();
        float slope = -inverseSlope;

        BufferMath::ramp(current, slope, buf, bufferSize);
        
        if (current > goal) {
            BufferMath::clip(buf, goal, current, buf, bufferSize);
        } else {
            BufferMath::clip(buf, current, goal, buf, bufferSize);
        }
        
        updated = true;
        
        stepBy(bufferSize);
        
        fillOnce = false;
    }
    else {
        if (!fillOnce || previousBufferSize != bufferSize) {
            BufferMath::fill(buf, goal, bufferSize);
            fillOnce = true;
            previousBufferSize = bufferSize;
            updated = true;
        }
    }
    return buf;
}

}
