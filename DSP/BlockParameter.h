//
//  BlockParameter.h
//  AudioStuff
//
//  Created by Alexis ZBIK on 18/08/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//

#ifndef BlockParameter_h
#define BlockParameter_h

class BlockParameter {
public:
    BlockParameter() = default;
    
    BlockParameter ( const float &otherValue ) {
        this->value = otherValue;
    }
    
    BlockParameter ( float* &reference ) {
        this->reference = reference;
    }
    
    BlockParameter& operator=(float* reference)
    {
        this->reference = reference;
        return *this;
    }
    
    bool isBuffer() {
        return reference != nullptr;
    }
    
    operator float() {
        if (isBuffer()) {
            return reference[0];
        }
        return value;
    }
    
    operator float*() { return reference; }
    
    float &operator[](int i) {
        if (isBuffer()) {
            return reference[i];
        }
        return value;
    }
    
    float &operator[](size_t i) {
        if (isBuffer()) {
            return reference[i];
        }
        return value;
    }
    
private:
    float value = 0;
    float* reference = nullptr;
};


#endif /* BlockParameter_h */
