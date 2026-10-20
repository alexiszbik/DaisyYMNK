//
//  Reveb.hpp


#ifndef Reveb_h
#define Reveb_h

#include "DelayLineDamped.hpp"
#include "DelayGroup.hpp"
#include "Memory.h"

static const size_t delayGroupCount = 4;

static float delayInitTimes[delayGroupCount] = {58.6435f, 69.4325f, 74.5234f, 86.1244f};

namespace ydaisy {

class Reverb : public DelayGroup<DelayLineDamped> {
public:
    float gainOut = 0.501;

private:
    Buffer feedbackBuf = 0.2;

    Buffer inBuf[2];
    Buffer chanOutBuf[delayGroupCount];
    Buffer rewriteBuf[delayGroupCount];
    Buffer lastWriteBuf[delayGroupCount];
    Buffer workBuf;

    void processReadDelayLine(size_t lineId, float* dataIn, size_t n) {
        lines.at(lineId)->delayLine->DelayLineDamped::process(
            dataIn, n, 0, lines.at(lineId)->timeBuf, feedbackBuf, false);
    }

    float computeFeedback(float reverbTime, float delayLineLengthInMs) {
        float power = 1.0f / (reverbTime / (delayLineLengthInMs * 0.001f));
        return powf(0.001f, power);
    }

    void processFeedbackBuf(float reverbInSeconds, size_t n) {
        reverbInSeconds = fmaxf(reverbInSeconds, 0.6f);
        float targetFeedback = computeFeedback(reverbInSeconds, lines.at(3)->timeRef) / 2.0f;
        BufferMath::fill(feedbackBuf, targetFeedback, n);
    }

protected:
    virtual void processWriteDelayLine(size_t lineId, float* dataIn, size_t n) {
        lines.at(lineId)->delayLine->DelayLine::write(dataIn, n, 0);
    }

public:
    Reverb(float delayTime = 120.0f)
        : DelayGroup<DelayLineDamped>(delayInitTimes, delayGroupCount, delayTime) {
    }

    void init(int inChannelCount, double inSampleRate) override {
        DelayGroup<DelayLineDamped>::init(1, inSampleRate);
    }

    void setRoomSize(const float roomSize, float* modulations, size_t n) {
        float adding = 3.0f;
        BufferMath::fill(workBuf, roomSize + adding, n);
        float divid = 41.66f;
        BufferMath::div_s(workBuf, divid, workBuf, n);

        for (size_t i = 0; i < count; i++) {
            BufferMath::mul_s(workBuf, delayInitTimes[i], lines.at(i)->timeBuf, n);
            lines.at(i)->timeRef = lines.at(i)->timeBuf[0];
            if (modulations) {
                const float factor = 1.25f;
                BufferMath::mul_s_add(modulations, factor, lines.at(i)->timeBuf, lines.at(i)->timeBuf, n);
                BufferMath::abs(lines.at(i)->timeBuf, lines.at(i)->timeBuf, n);
            }
        }
    }

    void doReverb(size_t n) {
        for (size_t i = 0; i < delayGroupCount; i++) {
            BufferMath::clear(chanOutBuf[i], n);
            BufferMath::clear(rewriteBuf[i], n);
            BufferMath::clear(lastWriteBuf[i], n);
            processReadDelayLine(i, chanOutBuf[i], n);
        }

        BufferMath::add(chanOutBuf[0], inBuf[0], chanOutBuf[0], n);
        BufferMath::add(chanOutBuf[1], inBuf[1], chanOutBuf[1], n);

        BufferMath::add(chanOutBuf[0], chanOutBuf[1], rewriteBuf[0], n);
        BufferMath::sub(chanOutBuf[0], chanOutBuf[1], rewriteBuf[1], n);
        BufferMath::add(chanOutBuf[2], chanOutBuf[3], rewriteBuf[2], n);
        BufferMath::sub(chanOutBuf[2], chanOutBuf[3], rewriteBuf[3], n);

        for (size_t i = 0; i < delayGroupCount; i++) {
            BufferMath::mul_s(chanOutBuf[i], gainOut, chanOutBuf[i], n);
        }

        BufferMath::add(chanOutBuf[0], chanOutBuf[2], chanOutBuf[0], n);
        BufferMath::add(chanOutBuf[1], chanOutBuf[3], chanOutBuf[1], n);

        BufferMath::add(rewriteBuf[0], rewriteBuf[2], lastWriteBuf[0], n);
        BufferMath::add(rewriteBuf[1], rewriteBuf[3], lastWriteBuf[1], n);
        BufferMath::sub(rewriteBuf[0], rewriteBuf[2], lastWriteBuf[2], n);
        BufferMath::sub(rewriteBuf[1], rewriteBuf[3], lastWriteBuf[3], n);

        for (size_t i = 0; i < delayGroupCount; i++) {
            processWriteDelayLine(i, lastWriteBuf[i], n);
        }
    }

    void process(float* dataInOutLeft, float* dataInOutRight, size_t n, float* reverbTimeInSec) {
        processFeedbackBuf(reverbTimeInSec[0], n);

        Memory::copy(dataInOutLeft, inBuf[0], n);
        Memory::copy(dataInOutRight, inBuf[1], n);

        doReverb(n);

        Memory::copy(chanOutBuf[0], dataInOutLeft, n);
        Memory::copy(chanOutBuf[1], dataInOutRight, n);
    }

    void process(float* dataInOut, size_t n, size_t channel, float* reverbTimeInSec) {
        if (channel == 0) {
            processFeedbackBuf(reverbTimeInSec[0], n);

            Memory::copy(dataInOut, inBuf[0], n);
            Memory::copy(dataInOut, inBuf[1], n);

            doReverb(n);

            Memory::copy(chanOutBuf[0], dataInOut, n);
        } else if (channel == 1) {
            Memory::copy(chanOutBuf[1], dataInOut, n);
        }
    }
};

} // namespace ydaisy

#endif /* Reveb_h */
