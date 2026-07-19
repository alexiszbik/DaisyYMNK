//
//  SignalProcessing.cpp
//  JarreAudioUnit
//
//  Created by Alexis ZBIK on 15/01/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//


#include "SignalProcessing.h"
#include <cmath>
#include <cstring>
#include "ConstValues.h"
#include "InlineMath.h"

class CppBiquadState : public BiquadState {
public:
    CppBiquadState() {
        SignalProcessing::clear(inBuf, MAX_BUFFER_SIZE+2);
        SignalProcessing::clear(outBuf, MAX_BUFFER_SIZE+2);
    }

    virtual ~CppBiquadState() {}
    
public:
    virtual void clear() override {
        SignalProcessing::clear(inBuf, 2);
        SignalProcessing::clear(outBuf, 2);
    };

    virtual void clearBadStateValue() override {
        inBuf[0] = convertBadValuesToZero(inBuf[0]);
        inBuf[1] = convertBadValuesToZero(inBuf[1]);
        outBuf[0] = convertBadValuesToZero(outBuf[0]);
        outBuf[1] = convertBadValuesToZero(outBuf[1]);
    };
    
public:
    float inBuf[MAX_BUFFER_SIZE+2];
    float outBuf[MAX_BUFFER_SIZE+2];

};

void SignalProcessing::init() {
    srand(time(NULL));
}



//Addition : z = x + y
void SignalProcessing::add(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] + y[i];
    }
}

//Scalar Addition : x + y = z
void SignalProcessing::add_s(float* x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] + y;
    }
}

void SignalProcessing::sub(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] - y[i];
    }
}

//Multiplication : x * y = z
void SignalProcessing::mul(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] * y[i];
    }
}

//Multiplication & Scalar Addition : z = w * x + y
void SignalProcessing::mul_add(float* w, float *x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x[i] + y[i];
    }
}

void SignalProcessing::mul_add_s(float *w, float *x, float y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x[i] + y;
    }
}

//Scalar Multiplication & Addition : z = w * x + y
void SignalProcessing::mul_s_add(float* w, float x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x + y[i];
    }
}

void SignalProcessing::mul_mul_add(float* v, float* w, float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = v[i] * w[i] + x[i] * y[i];
    }
}

void SignalProcessing::div(float *x, float *y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] / y[i];
    }
}

void SignalProcessing::div_s(float *x, float y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] / y;
    }
}

//Scalar Multiplication : x * y = z
void SignalProcessing::mul_s(float* x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] * y;
    }
}

void SignalProcessing::mul_s_add_s(float* w, float x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x + y;
    }
}

void SignalProcessing::s_div(float x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x / y[i];
    }
}

void SignalProcessing::truncToInt(const float *x, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        //was using floor here, but this is not accurate, right?
        //z[i] = floor(x[i]);
        z[i] = trunc(x[i]);
    }
}

//Pow 2, in-place
void SignalProcessing::powTwo(float* x, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = x[i] * x[i];
    }
}

//Clipping :
void SignalProcessing::clip(float* x, float low, float high, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        if (x[i] < low) {
            z[i] = low;
        } else if (x[i] > high) {
            z[i] = high;
        } else {
            z[i] = x[i];
        }
    }
}

//Ramp :
void SignalProcessing::ramp(float startingValue, float increment, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = startingValue + i * increment;
    }
}

//Fractionnal part : - in place
void SignalProcessing::frac(float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = z[i] - floorf(z[i]);
    }
}

//Fill vector with value : x = a
void SignalProcessing::fill(float* x, const float a, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = a;
    }
}

//Clear vector : x = 0
void SignalProcessing::clear(float* x, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = 0;
    }
}

//Table read
void SignalProcessing::tableRead(float* indexes, float* table, const size_t tableLength, float* z, const size_t frameCount) {

    for (size_t i = 0; i < frameCount; i++) {
        const float p = indexes[i];
        const float q = floorf(p);
        const float r = p - q;
        z[i] = (1.0 - r) * table[(int)q] + (r * table[(int)q + 1]);
    }
}

void SignalProcessing::cos(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::cos(x[i]);
    }
}

void SignalProcessing::sin(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::sin(x[i]);
    }
}

void SignalProcessing::exp(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::exp(x[i]);
    }
}

void SignalProcessing::log(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::log(x[i]);
    }
}

void SignalProcessing::abs(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::abs(x[i]);
    }
}

void SignalProcessing::tanh(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::tanh(x[i]);
    }
}


void SignalProcessing::peak(float* data, float* value, const size_t frameCount) {
    *value = -INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value < std::abs(data[i])) {
            *value = std::abs(data[i]);
        }
    }
}

void SignalProcessing::max(float* data, float* value, const size_t frameCount) {
    *value = -INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value < data[i]) {
            *value = data[i];
        }
    }
}

void SignalProcessing::min(float* data, float* value, const size_t frameCount) {
    *value = INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value > data[i]) {
            *value = data[i];
        }
    }
}

void SignalProcessing::min_compare(float* array1, float* array2, float* arrayOut, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        arrayOut[i] = array1[i] <= array2[i] ? array1[i] : array2[i];
    }
}

void SignalProcessing::limit(float* array, float limit, float outScalar, float* arrayOut, const size_t frameCount) {
    
    for (size_t i = 0; i < frameCount; i++) {
        if (array[i] > limit)
        {
            arrayOut[i] = outScalar;
        }
        else {
            arrayOut[i] = -outScalar;
        }
    }
}

void SignalProcessing::threshold(float *array, float threshold, float *arrayOut, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        if(array[i] >= threshold) {
            arrayOut[i] = array[i];
        } else {
            arrayOut[i] = 0;
        }
    }
}

void SignalProcessing::biquad(float* inData, float* coefficients, float *outData, const size_t frameCount, BiquadState* biquadState) {
    
    //TODO : I do this blindly! Please test it!
    
    CppBiquadState* state = (CppBiquadState*)biquadState;
    
    memcpy(state->inBuf + 2, inData, frameCount * sizeof(float));
    
    for (size_t i = 2; i < frameCount + 2; i++) {
        state->outBuf[i] = (state->inBuf[i] * coefficients[0] + state->inBuf[i - 1] * coefficients[1] + state->inBuf[i - 2] * coefficients[2]) - (state->outBuf[i - 1] * coefficients[3] + state->outBuf[i - 2] * coefficients[4]) ;
    }
    
    memcpy(outData, state->outBuf + 2, frameCount * sizeof(float));
    
    memcpy(state->inBuf, state->inBuf + frameCount, 2 * sizeof(float));
    memcpy(state->outBuf, state->outBuf + frameCount, 2 * sizeof(float));
}

BiquadState* SignalProcessing::biquad_createState()  {
    return new CppBiquadState();
}

void SignalProcessing::biquad_destroyState(BiquadState* biquadState) {
    delete (CppBiquadState*)biquadState;
}
