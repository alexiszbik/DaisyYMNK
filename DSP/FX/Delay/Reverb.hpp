//
//  Reveb.hpp
//  AudioStuff
//
//  Created by Alexis ZBIK on 27/02/2019.
//  Copyright © 2019 Bleass. All rights reserved.
//

#ifndef Reveb_h
#define Reveb_h

#include "DelayLineDamped.hpp"
#include "DelayGroup.hpp"

static const size_t reflectionCount = /*6*/4;
static const size_t delayGroupCount = 4;

static float delayInitTimes[delayGroupCount] = {58.6435f, 69.4325f, 74.5234f, 86.1244f};

class Reverb : public DelayGroup<DelayLineDamped> {
public:
    float gainOut = 0.501;

private:
    Buffer inBuf;
    Buffer inBuf2;
    
    Buffer feedbackBuf = 0.2;
    
    Buffer earlyBuf[2];
    Buffer chanOutBuf[delayGroupCount];
    Buffer rewriteBuf[delayGroupCount];
    Buffer lastWriteBuf[delayGroupCount];
    Buffer workBuf;
    
    bool freezeState = false;
    
private:
    
    void processReadDelayLine(size_t lineId, float* dataIn, size_t n) {
        lines.at(lineId)->delayLine->DelayLineDamped::process(dataIn, n, 0, lines.at(lineId)->timeBuf, feedbackBuf, false);
    }
    
    float computeFeedback(float reverbTime, float delayLineLengthInMs) {
        float power = 1.0f/(reverbTime/(delayLineLengthInMs * 0.001f));
        return powf(0.001f,power);
    }
    
    void processFeedbackBuf(float reverbInSeconds, size_t n) {
        reverbInSeconds = fmax(reverbInSeconds, 0.6);
        float targetFeedback = computeFeedback(reverbInSeconds, lines.at(3)->timeRef) / 2.0;
        BufferMath::fill(feedbackBuf, targetFeedback, n);
    }
    
protected:

    virtual void processWriteDelayLine(size_t lineId, float* dataIn, size_t n) {
        lines.at(lineId)->delayLine->YDelayLine::write(dataIn, n, 0);
    }
    
public:
    Reverb(float delayTime = 120.0f) : DelayGroup<DelayLineDamped>(delayInitTimes, delayGroupCount, delayTime) {
    }
    
    ~Reverb() {
    }
    
    void init(int inChannelCount, double inSampleRate) override {
        DelayGroup<DelayLineDamped>::init(1, inSampleRate);
    }
    
    void freeze(bool inFreezeState) {
        freezeState = inFreezeState;
    }
    

    void setRoomSize(const float roomSize, float* modulations, size_t n) {

        float adding = 3.0;
        BufferMath::fill(workBuf, roomSize + adding, n);
        float divid = 41.66;
        BufferMath::div_s(workBuf, divid, workBuf, n);
        
        for (size_t i = 0; i < count; i++) {
            BufferMath::mul_s(workBuf, delayInitTimes[i], lines.at(i)->timeBuf, n);
            lines.at(i)->timeRef = lines.at(i)->timeBuf[0];
            if (modulations) {
                //float factor = 1.25f * (((i % 2) == 0) ? 0 : -1);
                float factor = 1.25;
                BufferMath::mul_s_add(modulations, factor, lines.at(i)->timeBuf, lines.at(i)->timeBuf, n);
                BufferMath::abs(lines.at(i)->timeBuf, lines.at(i)->timeBuf, n);
            }
        }
    }

    void doReverb(size_t n, float* earlyReflectionsLevel = NULL) {
        
        //Process damped 0, no reinjection
        for (size_t i = 0; i < delayGroupCount; i++) {
            BufferMath::clear(chanOutBuf[i], n);
            BufferMath::clear(rewriteBuf[i], n);
            BufferMath::clear(lastWriteBuf[i], n);
            processReadDelayLine(i, chanOutBuf[i], n);
        }
        
        //if (!freezeState) {
            BufferMath::add(chanOutBuf[0], earlyBuf[0], chanOutBuf[0], n);
            BufferMath::add(chanOutBuf[1], earlyBuf[1], chanOutBuf[1], n);
        //}
     
        //1+2
        BufferMath::add(chanOutBuf[0], chanOutBuf[1], rewriteBuf[0], n);
        
        //1-2
        BufferMath::sub(chanOutBuf[0], chanOutBuf[1], rewriteBuf[1], n);
        
        //3+4
        BufferMath::add(chanOutBuf[2], chanOutBuf[3], rewriteBuf[2], n);
        
        //3-4
        BufferMath::sub(chanOutBuf[2], chanOutBuf[3], rewriteBuf[3], n);

        // -- Output Gain
        for (size_t i = 0; i < delayGroupCount; i++) {
            BufferMath::mul_s(chanOutBuf[i], gainOut, chanOutBuf[i], n);
        }
        
        // -- Output Stage
        
        BufferMath::add(chanOutBuf[0], chanOutBuf[2], chanOutBuf[0], n);
        BufferMath::add(chanOutBuf[1], chanOutBuf[3], chanOutBuf[1], n);
         
        // -- Second Stage for rewriting
        //1+3
        BufferMath::add(rewriteBuf[0], rewriteBuf[2], lastWriteBuf[0], n);
        
        //2+4
        BufferMath::add(rewriteBuf[1], rewriteBuf[3], lastWriteBuf[1], n);
        
        //1-3
        BufferMath::sub(rewriteBuf[0], rewriteBuf[2], lastWriteBuf[2], n);
        
        //2-4
        BufferMath::sub(rewriteBuf[1], rewriteBuf[3], lastWriteBuf[3], n);
        
        for (size_t i = 0; i < delayGroupCount; i++) {
            processWriteDelayLine(i, lastWriteBuf[i], n);
        }
    }
    
    //STEREO CHANNEL PROCESSING
    void process(float* dataInOutLeft, float* dataInOutRight, size_t n, float* reverbTimeInSec, float* earlyReflectionsLevel = NULL) {
        
        processFeedbackBuf(reverbTimeInSec[0], n);
        
        if (0) {
            //Get first channel early reflection
            /*memcpy(inBuf, dataInOutLeft, sizeof(float)*n);
            memcpy(inBuf2, dataInOutRight, sizeof(float)*n);

            //earlyReflections->process(inBuf, inBuf2, n);
            memcpy(earlyBuf[0], inBuf, sizeof(float)*n);
            memcpy(earlyBuf[1], inBuf2, sizeof(float)*n);*/
        } else {
            memcpy(earlyBuf[0], dataInOutLeft, sizeof(float)*n);
            memcpy(earlyBuf[1], dataInOutRight, sizeof(float)*n);
        }
        
        doReverb(n, earlyReflectionsLevel);
        
        memcpy(dataInOutLeft, chanOutBuf[0], sizeof(float)*n);
        memcpy(dataInOutRight, chanOutBuf[1], sizeof(float)*n);
    }
    
    //PER-CHANNEL PROCESSING
    void process(float* dataInOut, size_t n, size_t channel, float* reverbTimeInSec, float* earlyReflectionsLevel = NULL) {
        
        if (channel == 0) {
            
            processFeedbackBuf(reverbTimeInSec[0], n);
            
            if (0) {
                //Get first channel early reflection
                /*memcpy(inBuf, dataInOut, sizeof(float)*n);
                
                //earlyReflections->process(inBuf, n, 0);
                memcpy(earlyBuf[0], inBuf, sizeof(float)*n);
                
                //Get second channel early reflection
                memcpy(inBuf, dataInOut, sizeof(float)*n);
                
                //earlyReflections->process(inBuf, n, 1);
                memcpy(earlyBuf[1], inBuf, sizeof(float)*n);*/
            } else {
                memcpy(earlyBuf[0], dataInOut, sizeof(float)*n);
                memcpy(earlyBuf[1], dataInOut, sizeof(float)*n);
            }
            
            doReverb(n, earlyReflectionsLevel);
            
            //channel buf -> OUT
            memcpy(dataInOut, chanOutBuf[0], sizeof(float)*n);
            
        } else if (channel == 1) {
            //channel buf -> OUT
            memcpy(dataInOut, chanOutBuf[1], sizeof(float)*n);
        }
    }
};

#endif /* Reveb_h */
