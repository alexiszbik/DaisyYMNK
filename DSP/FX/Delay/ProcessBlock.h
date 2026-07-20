

#ifndef ProcessBlock_h
#define ProcessBlock_h

#include <cstdlib>
#include <cmath>

#include "BufferMath.h"
#include "Buffer.h"
#include "BlockParameter.h"

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
    
};

#endif /* ProcessBlock_h */
