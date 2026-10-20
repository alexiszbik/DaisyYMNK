//
//  Memory.cpp
//  AudioStuff
//
//  Created by Alexis ZBIK on 12/02/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//


#include "Memory.h"

void Memory::copy(float* _in, float* _out, const size_t frameCount) {
    memcpy(_out, _in, sizeof(float)*frameCount);
}
