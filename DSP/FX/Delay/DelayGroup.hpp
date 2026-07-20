

#ifndef DelayGroup_h
#define DelayGroup_h

#include "ProcessBlock.h"

template <class T> class DelayGroup : public ProcessBlock {
    
public:
    struct Dly {
        Dly(float maxDelayTime) {
            delayLine = new T(maxDelayTime);
        }
        
        T *delayLine;
        float timeRef;
        Buffer timeBuf;
    };
    
    std::vector<Dly*> lines;
    size_t count;
    
public:
    
    DelayGroup<T>(float* inDelayTimes, size_t inCount, float maxDelayTime) {
        count = inCount;
        
        for (size_t i = 0; i < count; i++) {
            lines.push_back(new Dly(maxDelayTime));
            lines.back()->timeRef = inDelayTimes[i];
            BufferMath::fill(lines.back()->timeBuf, inDelayTimes[i], MAX_BUFFER_SIZE);
        }
    }
    
    ~DelayGroup<T>() {
        for (size_t i = 0; i < count; i++) {
            delete lines[i]->delayLine;
            delete lines[i];
        }
    }
    
    void init(int inChannelCount, double inSampleRate) override {
        ProcessBlock::init(inChannelCount, inSampleRate);
        
        for (size_t i = 0; i < count; i++) {
            lines.at(i)->delayLine->init(inChannelCount, sampleRate);
        }
    }
    
    void clear() override {
        for (size_t i = 0; i < count; i++) {
            lines.at(i)->delayLine->clear();
        }
    }
};


#endif /* DelayGroup_h */
