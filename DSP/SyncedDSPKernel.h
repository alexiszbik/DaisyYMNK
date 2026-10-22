/*
  ==============================================================================

    SyncedDSPKernel.h
    Created: 22 Oct 2026
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "DSPKernel.h"

namespace ydaisy {

class SyncedDSPKernel : public DSPKernel {

public:
    SyncedDSPKernel(vector<ParameterDesc> parameterDescriptors);
    virtual ~SyncedDSPKernel();

    void setTempo(double tempo);
    double getTempo() const;

    virtual void trigKick();

private:
    double tempo = 120.0;

};

}
