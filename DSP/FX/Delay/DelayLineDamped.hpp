//
//  DelayLineDamped.h
//  AudioStuff
//
//  Created by Alexis ZBIK on 27/02/2019.
//  Copyright © 2019 Bleass. All rights reserved.
//

#ifndef DelayLineDamped_h
#define DelayLineDamped_h

#include "DelayLine.h"
#include "BiquadFilter.h"

//Damp algorithm is lowPass filter signal - signal

class DelayLineDamped : public YDelayLine {
public:
    float crossoverFrequency = 9000.0f;
    float res = -3.f;
    
    float inverseNyquist;
private:
    BiquadFilter lpFilter = BiquadFilter(BiquadFilterType::lowPass);
    
    Buffer workBuf;
    
public:
    DelayLineDamped(float maxDelayTimeInMs) : YDelayLine(maxDelayTimeInMs) {
    }
    
    void init(int inChannelCount, double inSampleRate) override {
        YDelayLine::init(inChannelCount, inSampleRate);
        
        lpFilter.init(channelCount, sampleRate);
        
        lpFilter.windowSize = 128;
    }
    
    void clear() override {
        YDelayLine::clear();
        lpFilter.clear();
    }
    
    void process(float* dataIn, size_t n, size_t channel, float* timeInMs, float* feedback, bool reinject = true) {
        
        YDelayLine::process(dataIn, n, channel, timeInMs, feedback, reinject);
    }
    
    void preFeedbackProcess(float* dataIn, size_t n, size_t channel) override {
        
        memcpy(workBuf, dataIn, sizeof(float)*n);
        
        lpFilter.process(workBuf, n, channel, BiquadFilterParameters(crossoverFrequency, res));
        
        BufferMath::sub(workBuf, dataIn, workBuf, n);
        
        BufferMath::mul_s_add(workBuf, 0.35, dataIn, dataIn, n);
    }
};


#endif /* DelayLineDamped_h */
