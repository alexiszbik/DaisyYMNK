#pragma once

#ifndef BufferMath_h
#define BufferMath_h

#include <cstdlib>

class BufferMath {
public:
    static void add(float* x, float* y, float* z, size_t frameCount);
    static void add_s(float* x, float y, float* z, size_t frameCount);
    static void sub(float* x, float* y, float* z, size_t frameCount);
    static void mul(float* x, float* y, float* z, size_t frameCount);
    static void mul_s(float* x, float y, float* z, size_t frameCount);
    static void mul_add(float* w, float* x, float* y, float* z, size_t frameCount);
    static void mul_add_s(float* w, float* x, float y, float* z, size_t frameCount);
    static void mul_s_add(float* w, float x, float* y, float* z, size_t frameCount);
    static void mul_s_add_s(float* w, float x, float y, float* z, size_t frameCount);
    static void mul_mul_add(float* v, float* w, float* x, float* y, float* z, size_t frameCount);
    static void div(float* x, float* y, float* z, size_t frameCount);
    static void s_div(float x, float* y, float* z, size_t frameCount);
    static void div_s(float* x, float y, float* z, size_t frameCount);
    static void powTwo(float* x, size_t frameCount);
    static void clip(float* x, float low, float high, float* z, size_t frameCount);
    static void ramp(float startingValue, float increment, float* z, size_t frameCount);
    static void frac(float* z, size_t frameCount);
    static void abs(float* x, float* z, size_t frameCount);
    static void fill(float* x, float a, size_t frameCount);
    static void clear(float* x, size_t frameCount);
    static void truncToInt(const float* x, float* z, size_t frameCount);
    static void tableRead(float* indexes, float* table, size_t tableLength, float* z, size_t frameCount);
    static void cos(float* x, float* z, size_t frameCount);
    static void sin(float* x, float* z, size_t frameCount);
    static void tanh(float* x, float* z, size_t frameCount);
    static void exp(float* x, float* z, size_t frameCount);
    static void log(float* x, float* z, size_t frameCount);
    static void log_ten(float* x, float* z, size_t frameCount);
    static void peak(float* data, float* value, size_t frameCount);
    static void max(float* data, float* value, size_t frameCount);
    static void min(float* data, float* value, size_t frameCount);
    static void min_compare(float* array1, float* array2, float* arrayOut, size_t frameCount);
    static void limit(float* array, float limit, float outScalar, float* arrayOut, size_t frameCount);
    static void threshold(float* array, float threshold, float* arrayOut, size_t frameCount);
};

#endif
