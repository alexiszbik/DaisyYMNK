/*
  ==============================================================================

    InlineMath.h
    Created: 11 May 2025 2:39:08pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <limits>

#include "BufferMath.h"

#define LOG2  0.693147180559945
#define LOGTEN 2.302585092994
#define PI 3.14159265358979323846

#if defined arc4random

#else
    #define arc4random rand
#endif

static const float twopi = 2.f * PI;

static inline float ftom(float f)
{
    return (f > 0 ? 17.3123405046f * logf(.12231220585f * f) : -1500.f);
}

inline float fast_mtof(float m)
{
    constexpr float inv12 = 0.0833333333333f;
    constexpr float a440  = 440.f;
    return a440 * exp2f((m - 69.f) * inv12);
}

static inline float convertBadValuesToZero(float x) {
    float absx = fabsf(x);

    if (absx > 1e-15f && absx < 1e15f) {
        return x;
    }

    return 0.0f;
}

static inline double squared(double x) {
    return x * x;
}

static inline void ftom(float* f, const int n)
{
    const float a = .12231220585f;
    BufferMath::mul_s(f, a, f, n);

    BufferMath::log(f, f, n);

    const float b = 17.3123405046f;
    BufferMath::mul_s(f, b, f, n);
}

static inline float dbtorms(float f)
{
    return expf((LOGTEN * 0.05f) * (f - 100.f));
}

static inline void dbtorms(float *f, const int n)
{
    static float a = (LOGTEN * 0.05f);
    static float b = -100.f;
    BufferMath::add_s(f, b, f, n);
    BufferMath::mul_s(f, a, f, n);
    BufferMath::exp(f, f, n);
}

static inline float rmstodb(float f)
{
    if (f <= 0) return 0.f;
    float val = 100.f + 20.f / LOGTEN * logf(f);
    return (val < 0.f ? 0.f : val);
}

static inline float powToDb(float f)
{
    if (f <= 0) return 0.f;
    float val = 100.f + 10.f / LOGTEN * logf(f);
    return (val < 0.f ? 0.f : val);
}

static inline float randomFloat()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

static inline float randomSignedFloat()
{
    float fvalue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return (fvalue - 0.5f) * 2.0f;
}

static inline int randomInt(int min, int max) {
    if (min >= max) {
        return min;
    }
    return arc4random() % ((max + 1) - min) + min;
}

static inline float db2lin(float db) {
    return powf(10.0f, 0.05f * db);
}

static inline float lin2db(float lin) {
    return 20.0f * log10f(lin);
}

static inline int ilog2(int n)
{
    int r = -1;
    if (n <= 0) return 0;
    while (n)
    {
        r++;
        n >>= 1;
    }
    return r;
}

template <typename T>
static inline T clamp(T input, T low, T high) {
    return std::min(std::max(input, low), high);
}

static inline unsigned long floor_power_of_two(unsigned long v)
{
    int p = (int)log2(v);
    return (unsigned long)pow(2, p);
}

static inline float amplitudeToDb(float amplitude) {
    amplitude = fmaxf(amplitude, 0.001f);
    return 20.f * log10f(amplitude);
}

static inline void amplitudeToDb(float* amplitude, size_t frameCount) {
    BufferMath::clip(amplitude, 0.001f, std::numeric_limits<float>::max(), amplitude, frameCount);
    BufferMath::log_ten(amplitude, amplitude, frameCount);
    BufferMath::mul_s(amplitude, 20.f, amplitude, frameCount);
}

static inline float dBToAmplitude(float db) {
    return powf(10.f, db / 20.f);
}
