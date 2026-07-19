//
//  InlineMath.hpp
//
//
//  Created by Alexis ZBIK on 27/12/2018.
//

#ifndef InlineMath_h
#define InlineMath_h

#include <algorithm>
#include <float.h>

#include "math.h"
#include <limits>
#include "ConstValues.h"
#include "SignalProcessing.h"

#define LOG2  0.693147180559945
#define LOGTEN 2.302585092994
#define PI 3.14159265358979323846

#if defined arc4random

#else
    #define arc4random rand
#endif

static const float twopi = 2.f * PI;

static inline void allocBuf(float** buf, size_t size) {
    *buf = (float*)malloc(sizeof(float)*size);
    SignalProcessing::clear(*buf, size);
}

static inline void allocBuf_d(double** buf, size_t size) {
    *buf = (double*)malloc(sizeof(double)*size);
    SignalProcessing::clear(*buf, size);
}

static inline void allocBuf(float** buf) {
    allocBuf(buf, MAX_BUFFER_SIZE);
}

static inline void allocBuf_d(double** buf) {
    allocBuf_d(buf, MAX_BUFFER_SIZE);
}

static inline void initBuf(float** buf, const float initValue = 0.0f) {
    *buf = (float*)malloc(sizeof(float)*MAX_BUFFER_SIZE);
    SignalProcessing::fill(*buf, initValue, MAX_BUFFER_SIZE);
}

static inline float convertBadValuesToZero(float x) {
    /*
     Eliminate denormals, not-a-numbers, and infinities.
     Denormals will fail the first test (absx > 1e-15), infinities will fail
     the second test (absx < 1e15), and NaNs will fail both tests. Zero will
     also fail both tests, but since it will get set to zero that is OK.
     */
    
    float absx = fabs(x);
    
    if (absx > 1e-15 && absx < 1e15) {
        return x;
    } 
    
    return 0.0;
}

static inline double squared(double x) {
    return x * x;
}
/*
static inline float mtof(float f)
{
    if (f <= -1500) return(0);
    else if (f > 1499) return(mtof(1499));
    else return (8.17579891564f * exp(.0577622650f * f));
}

static inline void mtof(float* f, const int n)
{
    const float a = .0577622650f;
    SignalProcessing::mul_s(f, a, f, n);
    
    SignalProcessing::exp(f, f, n);
    
    const float b = 8.17579891564f;
    SignalProcessing::mul_s(f, b, f, n);
}*/

static inline void ftom(float* f, const int n)
{
    const float a = .12231220585f;
    SignalProcessing::mul_s(f, a, f, n);
    
    SignalProcessing::log(f, f, n);
    
    const float b = 17.3123405046;
    SignalProcessing::mul_s(f, b, f, n);
}

static inline float dbtorms(float f)
{
    return exp((LOGTEN * 0.05) * (f-100.));
}

static inline void dbtorms(float *f, const int n)
{
    static float a = (LOGTEN * 0.05);
    static float b = -100.;
    SignalProcessing::add_s(f, b, f, n);
    SignalProcessing::mul_s(f, a, f, n);
    SignalProcessing::exp(f, f, n);
}

static inline float rmstodb(float f)
{
    if (f <= 0) return (0);
    else
    {
        float val = 100 + 20./LOGTEN * log(f);
        return (val < 0 ? 0 : val);
    }
}

static inline float powToDb(float f)
{
    if (f <= 0) return (0);
    else
    {
        float val = 100 + 10./LOGTEN * log(f);
        return (val < 0 ? 0 : val);
    }
}

static inline float randomFloat()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static inline float randomSignedFloat()
{
    float fvalue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return (fvalue-0.5) * 2.0;
}

static inline int randomInt(int min, int max) {
    if (min >= max) {
        return min;
    }
    //return arc4random()%(max - min) + min + 1;
    return arc4random()%((max + 1) - min) + min;
}
static inline float db2lin(float db){ // dB to linear
    return powf(10.0f, 0.05f * db);
}

static inline float lin2db(float lin){ // linear to dB
    return 20.0f * log10f(lin);
}

static inline int ilog2(int n)
{
    int r = -1;
    if (n <= 0) return(0);
    while (n)
    {
        r++;
        n >>= 1;
    }
    return (r);
}

template <typename T>
static inline T clamp(T input, T low, T high) {
    return std::min(std::max(input, low), high);
}

static inline unsigned long floor_power_of_two(unsigned long v)
{
    int p = (int)log2(v);
    return (int)pow(2, p);

}

static inline float amplitudeToDb(float amplitude) {
    amplitude = fmaxf(amplitude, 0.001f);
    float db = 20 * log10f(amplitude);
    return db;
}

static inline void amplitudeToDb(float* amplitude, size_t frameCount) {
    SignalProcessing::clip(amplitude, 0.001f, std::numeric_limits<float>::max(), amplitude, frameCount);
    SignalProcessing::log_ten(amplitude, amplitude, frameCount);
    SignalProcessing::mul_s(amplitude, 20.f, amplitude, frameCount);
}

static inline float dBToAmplitude(float db) {
    float amplitude = powf(10.f, db / 20.f);
    return amplitude;
}


#endif /* InlineMath_h */

