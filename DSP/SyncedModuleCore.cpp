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

    if (messageType == MIDIMessageType::kControlChange && channel == kCommonChannel) {
        if (dataA == 10) {
            double newTempo = dataB + 60;
            syncKernel->setTempo(newTempo);
        }
    } else if (messageType == MIDIMessageType::kNoteOn && channel == kDrumChannel) {
        if (dataA == kKickNote && dataB > 0) {
            syncKernel->trigKick();
        }
    }
}

}
