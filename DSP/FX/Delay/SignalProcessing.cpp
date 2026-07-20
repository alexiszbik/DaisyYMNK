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
#include "InlineMath.h"


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
