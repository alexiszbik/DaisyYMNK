//
//  Buffer.h
//  AudioStuff
//
//  Created by Alexis ZBIK on 17/08/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//


#ifndef Buffer_h
#define Buffer_h

#include "Memory.h"
#include "InlineMath.h"

class Buffer {
public:
    Buffer() {
        initBuf(&data, 0.0f);
    }
    
    Buffer(float initValue) {
        initBuf(&data, initValue);
    }
    
    ~Buffer() {
        free(data);
    }
    
    operator float*() { return data; }
private:
    float* data;
    
};

#endif /* Buffer_h */
