/*
  ==============================================================================

    SmoothValue.h
    Created: 8 Nov 2023 10:43:30pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

namespace ydaisy {

class SmoothValue {
public:
    SmoothValue() {};
    ~SmoothValue() {};
    
    SmoothValue(float value);
    
    void setImmediate(float value);

    void setValue(float value);
    
    void dezipperCheck(long rampDuration);

    float getAndStep();
    
    float getGoal();
    
private:
    void startRamp(float newGoal, long duration);
    float get();
    
private:
    float value = 0.0f;
    float goal = 0.0f;
    float inverseSlope = 0.0f;
    long samplesRemaining = 0;
    long changeCounter = 0;
    long updateCounter = 0;
};

}
