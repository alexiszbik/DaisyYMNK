/*
  ==============================================================================

    SyncedDSPKernel.cpp
    Created: 22 Oct 2026
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "SyncedDSPKernel.h"

namespace ydaisy {

SyncedDSPKernel::SyncedDSPKernel(vector<ParameterDesc> parameterDescriptors)
    : DSPKernel(parameterDescriptors) {
}

SyncedDSPKernel::~SyncedDSPKernel() {
}

void SyncedDSPKernel::setTempo(double tempo) {
    this->tempo = tempo;
}

double SyncedDSPKernel::getTempo() const {
    return tempo;
}

void SyncedDSPKernel::trigKick() {

}

}
