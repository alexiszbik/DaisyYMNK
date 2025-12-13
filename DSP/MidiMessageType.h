/*
  ==============================================================================

    MidiMessageType.h
    Created: 19 Jan 2024 9:52:27am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

namespace ydaisy {

enum MIDIMessageType {
    kNoteOn = 0,
    kNoteOff,
    kControlChange,
    kPitchBend,
    kProgramChange,
    
    kCount
};

}
