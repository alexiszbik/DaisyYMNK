
#ifndef ParameterRamper_h
#define ParameterRamper_h

#include "InlineMath.h"
#include "SignalProcessing.h"
#include "ConstValues.h"

#include <atomic>

class ParameterRamper {
    float _uiValue = 0.0f;
    float _goal = 0.0f;
    float inverseSlope = 0.0f;
    size_t samplesRemaining = 0;
	std::atomic<int32_t> changeCounter = {0};
	int32_t updateCounter = 0;
    
    bool fillOnce = false;
    size_t previousBufferSize = 0;
    
    float buf[MAX_BUFFER_SIZE];

public:
    ParameterRamper() {};
    ~ParameterRamper() {};
    
	ParameterRamper(float value) {
		setImmediate(value);
        
	}
    
    void setImmediate(float value) {
        // only to be called from the render thread or when resources are not allocated.
        _goal = _uiValue = value;
        inverseSlope = 0.0;
        samplesRemaining = 0;
        fillOnce = false;
    }

    void setValue(float value) {
        if (_uiValue != value) {
            _uiValue = value;
            std::atomic_fetch_add(&changeCounter, 1);
        }
    }
	
	void dezipperCheck(size_t rampDuration)
	{
		// check to see if the UI has changed and if so, start a ramp to dezipper it.
		int32_t changeCounterSnapshot = changeCounter;
		if (updateCounter != changeCounterSnapshot) {
			updateCounter = changeCounterSnapshot;
			startRamp(_uiValue, rampDuration);
		}
	}

    void startRamp(float newGoal, size_t duration, bool isInteger = false) {
        
        _uiValue = newGoal;
        
        if (isInteger) {
            newGoal = ((int)roundf(newGoal));
        }
        
        if (duration == 0) {
            setImmediate(newGoal);
        }
        else {
            /*
            	Set a new ramp.
            	Assigning to inverseSlope must come before assigning to goal.
            */
            inverseSlope = (get() - newGoal) / float(duration);
            samplesRemaining = duration;
            _goal = newGoal;
        }
    }

    float get() const {
        /*
			For long ramps, integrating a sum loses precision and does not reach 
            the goal at the right time. So instead, a line equation is used. y = m * x + b.
		*/
        return inverseSlope * float(samplesRemaining) + _goal;
    }
	
    void step() {
        // Do this in each nner loop iteration after getting the value.
        if (samplesRemaining != 0) {
			--samplesRemaining;
		}
    }

    float getAndStep() {
        // Combines get and step. Saves a multiply-add when not ramping.
        if (samplesRemaining > 0) {
            float value = get();
            --samplesRemaining;
            return value;
        }
		else {
            return _goal;
        }
    }


    void stepBy(size_t n) {
        /*
            When a parameter does not participate in the current inner loop, you 
            will want to advance it after the end of the loop.
        */
        if (n >= samplesRemaining) {
			samplesRemaining = 0;
        }
		else {
			samplesRemaining -= n;
		}
    }
    
    float* getAndStep(size_t bufferSize, bool& updated) {
        if (samplesRemaining != 0) {
            float current = get();
            float slope = -inverseSlope;

            SignalProcessing::ramp(current, slope, buf, bufferSize);
            
            if (current > _goal) {
                SignalProcessing::clip(buf, _goal, current, buf, bufferSize);
            } else {
                SignalProcessing::clip(buf, current, _goal, buf, bufferSize);
            }
            
            updated = true;
            
            stepBy(bufferSize);
            
            fillOnce = false;
        }
        else {
            if (!fillOnce || previousBufferSize != bufferSize) {
                SignalProcessing::fill(buf, _goal, bufferSize);
                fillOnce = true;
                previousBufferSize = bufferSize;
                updated = true;
            }
        }
        return buf;
    }
};

#endif /* ParameterRamper_h */
