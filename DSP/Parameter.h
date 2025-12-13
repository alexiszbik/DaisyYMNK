/*
  ==============================================================================

    Parameter.h
    Created: 8 Nov 2023 10:59:38pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "Macros.h"
#include "ParameterDesc.h"
#include "SmoothValue.h"

namespace ydaisy {

class Parameter {
public:
    Parameter(ParameterDesc desc);
    ~Parameter();
    
public:
    const char* getName();
    void setValue(float value);
    void update();
    float getValue();
    float getUIValue();
    
    bool useSmoothValue();
    
private:
    ParameterDesc desc;
    SmoothValue* smoothValue = nullptr;
    float currentValue = 0;
};

}
