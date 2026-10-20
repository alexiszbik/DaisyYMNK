//
//  Memory.hpp
//
//  Created by Alexis ZBIK on 12/02/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//

#ifndef Memory_hpp
#define Memory_hpp

#include <cstring>
#include <cstdlib>

#define MAX_BUFFER_SIZE 128

class Memory {
public:
    //Copy
    static void copy(float* _in, float* _out, const size_t frameCount);
};

static inline void allocBuf(float** buf, size_t size) {
    *buf = static_cast<float*>(malloc(sizeof(float) * size));
    if (*buf == nullptr) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        (*buf)[i] = 0.f;
    }
}

static inline void allocBuf(float** buf) {
    allocBuf(buf, MAX_BUFFER_SIZE);
}

static inline void initBuf(float** buf, const float initValue = 0.0f) {
    *buf = static_cast<float*>(malloc(sizeof(float) * MAX_BUFFER_SIZE));
    if (*buf == nullptr) {
        return;
    }
    for (size_t i = 0; i < MAX_BUFFER_SIZE; i++) {
        (*buf)[i] = initValue;
    }
}


#endif /* Memory_hpp */
