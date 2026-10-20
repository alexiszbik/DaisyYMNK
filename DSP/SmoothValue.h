/*
  ==============================================================================

    SmoothValue.h
    Created: 8 Nov 2023 10:43:30pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "Buffer.h"
#include "BufferMath.h"

namespace ydaisy {

class SmoothValue {
public:
    SmoothValue() {};
    ~SmoothValue() {};
    
    SmoothValue(float value);
    
    void setImmediate(float value);

    void setValue(float value);
    
    void dezipperCheck(long rampDuration);

    void stepBy(size_t n);

    float getAndStep();
    
    float getGoal();

    float* getAndStep(size_t bufferSize, bool& updated);
    
private:
    void startRamp(float newGoal, long duration);
    float get();
    
private:
    float value = 0.0f;
    float goal = 0.0f;
    float inverseSlope = 0.0f;
    long samplesRemaining = 0;
    
    bool valueChanged = true;

    //TODO : Smooth value use a buffer for smooth ramped value but maybe smooth value is not used this way
    Buffer buf; 
    bool fillOnce = false;
    size_t previousBufferSize = 0;
};

}
