//
//  DelayLine.hpp
//  AudioStuff
//

#ifndef DelayLine_h
#define DelayLine_h

#include "ProcessBlock.h"
#include "DelayWrite.h"
#include "DelayRead.h"
#include "BufferMath.h"

namespace ydaisy {

class DelayLineParameters {
public:
    BlockParameter timeInMs;
    BlockParameter feedback;
    bool reinject;
    bool wetOnly;
    
    DelayLineParameters(float timeInMs, float feedback = 0.f, bool reinject = true, bool wetOnly = false) :
    timeInMs(timeInMs), feedback(feedback), reinject(reinject), wetOnly(wetOnly) {}
    DelayLineParameters(float* timeInMs, float* feedback = nullptr, bool reinject = true, bool wetOnly = false) :
    timeInMs(timeInMs), feedback(feedback), reinject(reinject), wetOnly(wetOnly) {}
};

class DelayLine : public ProcessBlock {

private:
    DelayWrite* writers;
    DelayRead* readers;
    
    float maxDelayTime = 0;

    Buffer delayOutBuf = 0.f;
    Buffer workBuf = 0.f;
    
protected:
    virtual void preFeedbackProcess(float* dataIn, size_t n, size_t channel) {
    };
    
public:
    DelayLine(float maxDelayTimeInMs) {
        maxDelayTime = maxDelayTimeInMs;
    }
    
    ~DelayLine() {

    }
    
    void init(int inChannelCount, double inSampleRate) override {
        ProcessBlock::init(inChannelCount, inSampleRate);
        
        writers = new DelayWrite[channelCount];
        readers = new DelayRead[channelCount];
        
        for (int i = 0; i < channelCount; i++) {
            writers[i].init(maxDelayTime, sampleRate);
            readers[i].init(sampleRate);
        }
    }
    
    void reset() {
    }
    
    void clear() override {
        for (int i = 0; i < channelCount; i++) {
            writers[i].clear();
        }
    }
    
    void write(float* dataIn, size_t n, size_t channel) {
        writers[channel].process(dataIn, (int)n);
    }
    
    void process(float* dataInOut, size_t frameCount, size_t channel, DelayLineParameters parameters) {
        if (parameters.timeInMs.isBuffer()) {
            process(dataInOut, frameCount, channel, parameters.timeInMs, parameters.feedback, parameters.reinject, parameters.wetOnly);
        } else {
            BufferMath::fill(workBuf, 0.001f * (float)parameters.timeInMs, frameCount);
            
            readers[channel].process((float*)workBuf, delayOutBuf, &(writers[channel]), (int)frameCount);
            
            if (parameters.feedback != 0.f) {
                memcpy(workBuf, delayOutBuf, sizeof(float)*frameCount);
                
                preFeedbackProcess(workBuf, frameCount, channel);

                BufferMath::mul_s_add(workBuf, (float)parameters.feedback, dataInOut, workBuf, frameCount);
            } else {
                if(parameters.wetOnly) {
                    memcpy(workBuf, delayOutBuf, sizeof(float)*frameCount);
                } else {
                    memcpy(workBuf, dataInOut, sizeof(float)*frameCount);
                }
            }
            
            if (parameters.reinject) {
                write(workBuf, frameCount, channel);
                memcpy(dataInOut, delayOutBuf, sizeof(float)*frameCount);
            } else {
                memcpy(dataInOut, workBuf, sizeof(float)*frameCount);
            }
        }
    }
    
    void process(float* dataIn, size_t n, size_t channel, float* timeInMs, float* feedback, bool reinject = true, bool wetOnly = false) {

        BufferMath::mul_s(timeInMs, 0.001f, workBuf, n);
        
        //Get data from delay
        readers[channel].process((float*)workBuf, delayOutBuf, &(writers[channel]), (int)n);
        
        if (feedback) {
            memcpy(workBuf, delayOutBuf, sizeof(float)*n);
            
            preFeedbackProcess(workBuf, n, channel);

            BufferMath::mul_add(workBuf, feedback, dataIn, workBuf, n);
        
        } else { // no feedback? give the line only
            if(wetOnly) {
                memcpy(workBuf, delayOutBuf, sizeof(float)*n);
            } else {
                memcpy(workBuf, dataIn, sizeof(float)*n);
            }
        }
        
        //Reinject in delay
        if (reinject) {
            write(workBuf, n, channel);
            memcpy(dataIn, delayOutBuf, sizeof(float)*n);
        } else {
            memcpy(dataIn, workBuf, sizeof(float)*n);
        }
    }
    
    void process(float* dataIn, size_t n, size_t channel, float* timeInMs) {
        process(dataIn, n, channel, timeInMs, NULL);
    }
    
};

} // namespace ydaisy

#endif /* DelayLine_h */
