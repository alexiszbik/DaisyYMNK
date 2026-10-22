/*
  ==============================================================================

    SyncedModuleCore.h
    Created: 22 Oct 2026
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "ModuleCore.h"
#include "SyncedDSPKernel.h"

namespace ydaisy {

static constexpr uint8_t kCommonChannel = 12; //Channel 13
static constexpr uint8_t kDrumChannel = 3; //Channel 4
static constexpr uint8_t kKickNote = 36;

class SyncedModuleCore : public ModuleCore {

public:
    SyncedModuleCore(SyncedDSPKernel* kernel, vector<HIDElement> hidDescription, int midiChannel = -1);
    virtual ~SyncedModuleCore();

    void processMIDI(MIDIMessageType messageType, int channel, int dataA, int dataB) override;

private:
    SyncedDSPKernel* syncKernel = nullptr;

};

}
