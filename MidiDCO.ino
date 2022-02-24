#include <MIDI.h>
#include <List.hpp>
#include "MidiTimer.h"
#include "MidiDacTimer.h"

MidiTimer clockOutput(9, TCCR1A, TCCR1B, OCR1A);
MidiDacTimer4 integratorCV(7);
constexpr uint8_t gatePin{ 8 };

midi::SerialMIDI<HardwareSerial> midiSerial(Serial1);
midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI((midi::SerialMIDI<HardwareSerial>&)midiSerial);

List<byte> heldNotes;

void setup()
{
	clockOutput.begin();
	integratorCV.begin();

	pinMode(gatePin, OUTPUT);

	MIDI.setHandleNoteOn(noteOn);
	MIDI.setHandleNoteOff(noteOff);
	MIDI.begin(MIDI_CHANNEL_OMNI);

    noteSet(69);
}

void loop()
{
	MIDI.read();
}

void noteOn(midi::Channel channel, byte note, byte velocity)
{
	heldNotes.add(note);
    noteSet(note);
	digitalWrite(gatePin, HIGH);
}

void noteOff(midi::Channel channel, byte note, byte velocity)
{
    // Find the note and remove it
    for (int i = 0; i < heldNotes.getSize(); i++)
    {
        if (heldNotes[i] == note)
        {
            heldNotes.remove(i);
            break;
        }
    }

    if (heldNotes.getSize() > 0)
    {
        noteSet(heldNotes[heldNotes.getSize() - 1]);
    }
    else
    {
        digitalWrite(gatePin, LOW);
    }
}

void noteSet(byte note)
{
    clockOutput.setNote(note);
    integratorCV.setFrequency(clockOutput.getFrequency());
}
