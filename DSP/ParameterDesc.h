/*
  ==============================================================================

    ParameterDesc.h
    Created: 19 Jan 2024 9:13:21am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

struct ParameterDesc {
    unsigned int index;
    const char* name;
    bool isSmoothed = false;
};
