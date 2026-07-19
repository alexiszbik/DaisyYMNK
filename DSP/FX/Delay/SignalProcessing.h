//
//  SignalProcessing.h
//  AudioStuff
//
//  Created by Alexis ZBIK on 15/01/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//

#ifndef SignalProcessing_h
#define SignalProcessing_h

#include <cstdlib>
#include <time.h>

class BiquadState {
public:
    virtual ~BiquadState() {};
public:
    virtual void clear() = 0;
    virtual void clearBadStateValue() = 0;
};

class SignalProcessing {
public:

    //Initialization
    static void init();
    
    //Addition : z = x + y
    static void add(float* x, float* y, float* z, const size_t frameCount);
    
    //Scalar Addition : x + y = z
    static void add_s(float* x, float y, float* z, const size_t frameCount);
    static void add_s_D(double* x, double y, double* z, const size_t frameCount);
    
    //Substraction : z = x - y
    static void sub(float* x, float* y, float* z, const size_t frameCount);
    
    //Multiplication : x * y = z
    static void mul(float* x, float* y, float* z, const size_t frameCount);

    //Scalar Multiplication : x * y = z
    static void mul_s(float* x, float y, float* z, const size_t frameCount);
    
    //Addition & Multiplication : z = (w + x) * y
    static void add_mul(float* w, float *x, float *y, float* z, const size_t frameCount);
    
    //Multiplication & Addition : z = w * x + y
    static void mul_add(float* w, float *x, float *y, float* z, const size_t frameCount);
    
    //Multiplication & Scalar Addition : z = w * x + y
    static void mul_add_s(float* w, float *x, float y, float* z, const size_t frameCount);
    
    //Scalar Multiplication & Addition : z = w * x + y
    static void mul_s_add(float* w, float x, float *y, float* z, const size_t frameCount);
    
    //Scalar Multiplication & Scalar Addition : z = w * x + y
    static void mul_s_add_s(float* w, float x, float y, float* z, const size_t frameCount);
    
    //Multiplication, Multiplication, Add : z = v * w + x * y
    static void mul_mul_add(float* v, float* w, float* x, float* y, float* z, const size_t frameCount);
    
    //Divide array together : z = x / y
    static void div(float* x, float *y, float* z, const size_t frameCount);
    
    //Divide Scalar by Array : z = x / y
    static void s_div(float x, float *y, float* z, const size_t frameCount);
    
    //Divide Array by Scalare : z = x / y
    static void div_s(float *x, float y, float* z, const size_t frameCount);
    
    //Square root, y = sqrt(x)
    static void squareRoot(float* x, float* y, const size_t frameCount);
    
    //Pow 2, in-place
    static void powTwo(float* x, const size_t frameCount);

    //Pow, z = x^y
    static void pow(float* x, float* y, float* z, const size_t frameCount);
    
    //Clipping
    static void clip(float* x, float low, float high, float* z, const size_t frameCount);
    
    //Ramp
    static void ramp(float startingValue, float increment, float* z, const size_t frameCount);
    static void rampD(double startingValue, double increment, double* z, const size_t frameCount);
    
    //Fractionnal part in place
    static void frac(float* z, const size_t frameCount);
    
    //Absolute z = |x| 
    static void abs(float* x, float* z, const size_t frameCount);
    
    //Fill vector with value : x = a
    static void fill(float* x, const float a, const size_t frameCount);
    
    //Clear vector : x = 0
    static void clear(float* x, const size_t frameCount);
    static void clear(double* x, const size_t frameCount);
    
    //Truncate to integer
    static void truncToInt(const float* x, float* z, const size_t frameCount);
    
    //Table read
    static void tableRead(float* indexes, float* table, const size_t tableLength, float* z, const size_t frameCount);
    
    //Cos
    static void cos(float* x, float* z, const size_t frameCount);
    
    //Sin
    static void sin(float* x, float* z, const size_t frameCount);
    
    //Tan hyberbolic
    static void tanh(float* x, float* z, const size_t frameCount);
    
    //Exponential
    static void exp(float* x, float* z, const size_t frameCount);
    
    //Log
    static void log(float* x, float* z, const size_t frameCount);
    
    //Log10
    static void log_ten(float* x, float* z, const size_t frameCount);

    //Peak
    static void peak(float* data, float* value, const size_t frameCount);
    
    //Max value - find maximum value in an array
    static void max(float* data, float* value, const size_t frameCount);

    //Min value - find minimum value in an array
    static void min(float* data, float* value, const size_t frameCount);
    
    //Min compare each element of array : arrayOut[i] = array1[i] <= array2[i] ? array1[i] : array2[i]
    static void min_compare(float* array1, float* array2, float* arrayOut, const size_t frameCount);
    
    //Max compare each element of array : arrayOut[i] = array1[i] >= array2[i] ? array1[i] : array2[i]
    static void max_compare(float* array1, float* array2, float* arrayOut, const size_t frameCount);
    
    //Root mean square value
    static void rms(float* data, float* value, const size_t frameCount);
    
    //Limit test : arrayOut[i] = array[i] > limit ? outScalar : -outScalar
    static void limit(float* array, float limit, float outScalar, float* arrayOut, const size_t frameCount);
    
    //Threshold :
    static void threshold(float* array, float threshold, float* arrayOut, const size_t frameCount);
    
    //Modulo z = x % y
    static void mod(float* x, float* y, float* z, const size_t frameCount);
    
    //Interleave
    static void interleaveStereo(float* inDataLeft, float* inDataRight, float* outData, const size_t frameCount);
    
    //Convert double to float
    static void doubleToFloat(double* bufIn, float* bufOut, const size_t frameCount);
    
    //Reverse
    static void reverse(float* data, const size_t frameCount);
    
    //Biquad
    static void biquad(float* inData, float* coefficients, float* outData, const size_t frameCount, BiquadState* biquadState = nullptr);

    static BiquadState* biquad_createState();
    
    static void biquad_destroyState(BiquadState* biquadState);
};

#endif /* SignalProcessing_h */
