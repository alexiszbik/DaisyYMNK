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
#include "Filters/BiquadFilter.h"

//Damp algorithm is lowPass filter signal - signal

class DelayLineDamped : public YDelayLine {
private:
    std::vector<BiquadFilter> lpFilter;

    Buffer workBuf;
    
public:
    DelayLineDamped(float maxDelayTimeInMs) : YDelayLine(maxDelayTimeInMs) {
    }
    
    void init(int inChannelCount, double inSampleRate) override {
        YDelayLine::init(inChannelCount, inSampleRate);
        
        for (size_t i = 0; i < inChannelCount; i++) {
            lpFilter.push_back(BiquadFilter());
        }
        
        for (size_t i = 0; i < inChannelCount; i++) {
            lpFilter[i].Init(sampleRate);
            lpFilter[i].SetLowpass(9000, 0.707f);
        }
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
        
        for (size_t i = 0; i < n; i++) {
            workBuf[i] = lpFilter[channel].Process(workBuf[i]);
        }
        
        BufferMath::sub(workBuf, dataIn, workBuf, n);
        
        BufferMath::mul_s_add(workBuf, 0.35, dataIn, dataIn, n);
    }
};


#endif /* DelayLineDamped_h */
