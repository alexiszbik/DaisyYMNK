#include "DaisyBase.h"

DaisyBase::DaisyBase(ModuleCore *core) : core(core) {
    hid = new HID(core->getHIDDescription());
}

DaisyBase::~DaisyBase() {
    delete hid;
}

void DaisyBase::initMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}

// Typical Switch case for Message Type.
void DaisyBase::handleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
		
            core->processMIDI(MIDIMessageType::kNoteOn, p.channel, p.note, p.velocity);
            hw.SetLed(p.velocity > 0);
        }
        break;

		case NoteOff:
        {
            NoteOffEvent p = m.AsNoteOff();
            
            core->processMIDI(MIDIMessageType::kNoteOff, p.channel, p.note, 0);
			hw.SetLed(false);
        }
        break;

        case PitchBend:
        {
            PitchBendEvent p = m.AsPitchBend();
            core->processMIDI(MIDIMessageType::kPitchBend, p.channel, p.value + 8192, 0);
        }
        break;

        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            core->processMIDI(MIDIMessageType::kControlChange, p.channel, p.control_number, p.value);
        }
        break;

        case ProgramChange:
        {
            ProgramChangeEvent p = m.AsProgramChange();
            core->processMIDI(MIDIMessageType::kProgramChange, p.channel, p.program, 0);
        }
        break;

        default: break;
    }
}

void DaisyBase::init(AudioHandle::AudioCallback cb) {

    hw.Init();
    hw.SetLed(true);

	hw.SetAudioBlockSize(128); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	core->init(2, hw.AudioSampleRate());

    hid->init(hw);
#if TEST_MODE
    testOsc.Init(hw.AudioSampleRate());
    testOsc.SetFreq(440);
#endif

    System::Delay(1000); // as if something wrong happenend ....

	hw.StartAudio(cb);

    //int updateRate = 1000;
	
	initMidi();
    midi.StartReceive();
}

void DaisyBase::listen() {
    midi.Listen();

    while(midi.HasEvents())
    {
        handleMidiMessage(midi.PopEvent());
    }
    System::Delay(1);
}

void DaisyBase::process(float** buf, int frameCount) {

    hid->process(hw, core);

	core->process(buf, frameCount);
#if TEST_MODE
    for (int i = 0; i < frameCount; i++) {
        buf[0][i] += testOsc.Process();
    }
#endif
}
