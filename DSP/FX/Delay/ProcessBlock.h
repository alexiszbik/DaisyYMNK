//
//  ProcessBlock.h
//  AudioStuff
//
//  Created by Alexis ZBIK on 31/12/2018.
//

#ifndef ProcessBlock_h
#define ProcessBlock_h

#include <cstdlib>
#include <cmath>

#include "InlineMath.h"
#include "SignalProcessing.h"
#include "Buffer.h"
#include "BlockParameter.h"

class ProcessBlockParameters {
};

class ProcessBlock {
public:
    virtual ~ProcessBlock() {};
    
protected:
    double sampleRate = 44100.0;
    int channelCount = 2;
    
public:
    virtual void init(int inChannelCount, double inSampleRate) {
        channelCount = inChannelCount;
        sampleRate = inSampleRate;
    };
    
    virtual void clear() = 0;
    size_t windowSize = 64;
    
protected:
    float getIncrementFor(float timeInMs) {
        if (timeInMs == 0.0) {
            return 1.0;
        }
        return 1.0f / (sampleRate * fmaxf(timeInMs, 0.01f) * 0.001f);
    }
};

template <class T = ProcessBlockParameters> class MonoProcessBlock : public ProcessBlock {
public:
    virtual void process(float* dataInOut, size_t frameCount, T parameters) = 0;
};

template <class T = ProcessBlockParameters> class MultiChannelProcessBlock : public ProcessBlock {

public:
    virtual void process(float* dataInOut, size_t frameCount, size_t channel, T parameters) = 0;
};

#endif /* ProcessBlock_h */
