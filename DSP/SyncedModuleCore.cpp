/*
  ==============================================================================

    SyncedModuleCore.cpp
    Created: 22 Oct 2026
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "SyncedModuleCore.h"

namespace ydaisy
{

SyncedModuleCore::SyncedModuleCore(SyncedDSPKernel *kernel, vector<HIDElement> hidDescription, int midiChannel)
    : ModuleCore(static_cast<DSPKernel*>(kernel), hidDescription, midiChannel) {
        syncKernel = kernel;
}

SyncedModuleCore::~SyncedModuleCore() {
}

void SyncedModuleCore::processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB)
{
    ModuleCore::processMIDI(messageType, channel, dataA, dataB);

    if (channel == kCommonChannel)
    {
        if (dataA == 10)
        {
            double newTempo = dataB + 60;
            //mixBuses.setTempo(newTempo);
            syncKernel->setTempo(newTempo);
        }
        return;
    }
}

}
