
#include "BufferMath.h"
#include <cmath>
#include <cstring>


//Addition : z = x + y
void BufferMath::add(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] + y[i];
    }
}

//Scalar Addition : x + y = z
void BufferMath::add_s(float* x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] + y;
    }
}

void BufferMath::sub(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] - y[i];
    }
}

//Multiplication : x * y = z
void BufferMath::mul(float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] * y[i];
    }
}

//Multiplication & Scalar Addition : z = w * x + y
void BufferMath::mul_add(float* w, float *x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x[i] + y[i];
    }
}

void BufferMath::mul_add_s(float *w, float *x, float y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x[i] + y;
    }
}

//Scalar Multiplication & Addition : z = w * x + y
void BufferMath::mul_s_add(float* w, float x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x + y[i];
    }
}

void BufferMath::mul_mul_add(float* v, float* w, float* x, float* y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = v[i] * w[i] + x[i] * y[i];
    }
}

void BufferMath::div(float *x, float *y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] / y[i];
    }
}

void BufferMath::div_s(float *x, float y, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] / y;
    }
}

//Scalar Multiplication : x * y = z
void BufferMath::mul_s(float* x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x[i] * y;
    }
}

void BufferMath::mul_s_add_s(float* w, float x, float y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = w[i] * x + y;
    }
}

void BufferMath::s_div(float x, float *y, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = x / y[i];
    }
}

void BufferMath::truncToInt(const float *x, float *z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        //was using floor here, but this is not accurate, right?
        //z[i] = floor(x[i]);
        z[i] = trunc(x[i]);
    }
}

//Pow 2, in-place
void BufferMath::powTwo(float* x, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = x[i] * x[i];
    }
}

//Clipping :
void BufferMath::clip(float* x, float low, float high, float* z, const size_t frameCount) {
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
void BufferMath::ramp(float startingValue, float increment, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = startingValue + i * increment;
    }
}

//Fractionnal part : - in place
void BufferMath::frac(float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = z[i] - floorf(z[i]);
    }
}

//Fill vector with value : x = a
void BufferMath::fill(float* x, const float a, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = a;
    }
}

//Clear vector : x = 0
void BufferMath::clear(float* x, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        x[i] = 0;
    }
}

//Table read
void BufferMath::tableRead(float* indexes, float* table, const size_t tableLength, float* z, const size_t frameCount) {

    for (size_t i = 0; i < frameCount; i++) {
        const float p = indexes[i];
        const float q = floorf(p);
        const float r = p - q;
        z[i] = (1.0 - r) * table[(int)q] + (r * table[(int)q + 1]);
    }
}

void BufferMath::cos(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::cos(x[i]);
    }
}

void BufferMath::sin(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::sin(x[i]);
    }
}

void BufferMath::exp(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::exp(x[i]);
    }
}

void BufferMath::log(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::log(x[i]);
    }
}

void BufferMath::log_ten(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::log10(x[i]);
    }
}

void BufferMath::abs(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::abs(x[i]);
    }
}

void BufferMath::tanh(float* x, float* z, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        z[i] = std::tanh(x[i]);
    }
}


void BufferMath::peak(float* data, float* value, const size_t frameCount) {
    *value = -INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value < std::abs(data[i])) {
            *value = std::abs(data[i]);
        }
    }
}

void BufferMath::max(float* data, float* value, const size_t frameCount) {
    *value = -INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value < data[i]) {
            *value = data[i];
        }
    }
}

void BufferMath::min(float* data, float* value, const size_t frameCount) {
    *value = INFINITY;
    for (size_t i = 0; i < frameCount; i++) {
        if (*value > data[i]) {
            *value = data[i];
        }
    }
}

void BufferMath::min_compare(float* array1, float* array2, float* arrayOut, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        arrayOut[i] = array1[i] <= array2[i] ? array1[i] : array2[i];
    }
}

void BufferMath::limit(float* array, float limit, float outScalar, float* arrayOut, const size_t frameCount) {
    
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

void BufferMath::threshold(float *array, float threshold, float *arrayOut, const size_t frameCount) {
    for (size_t i = 0; i < frameCount; i++) {
        if(array[i] >= threshold) {
            arrayOut[i] = array[i];
        } else {
            arrayOut[i] = 0;
        }
    }
}
