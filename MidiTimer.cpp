#include "MidiTimer.h"

MidiTimer::MidiTimer (
	uint8_t outputPin,
	volatile uint8_t &controlRegisterA,
	volatile uint8_t &controlRegisterB,
	volatile uint16_t &compareRegister)
	:
	m_outputPin(outputPin),
	m_controlRegisterA(controlRegisterA),
	m_controlRegisterB(controlRegisterB),
	m_compareRegister(compareRegister)
{
}

void MidiTimer::begin ()
{
	pinMode (m_outputPin, OUTPUT);
	m_controlRegisterA = _BV(COM1A0);
}

void MidiTimer::setNote(int midiNote)
{
	m_frequency = midiNoteToFrequency(midiNote);
	auto prescaler = midiNoteToPrescaler(midiNote);
	auto compare = F_CPU >> 1; // divide by two for high and low portion of wave
	compare /= prescaler;
	compare = round(compare / m_frequency);
	compare--; // subtract one for an extra instruction

	// Set the registers
	m_compareRegister = compare;
	m_controlRegisterB = prescalerToPrescalerBits(prescaler);
}

uint16_t MidiTimer::getFrequency() const
{
	return m_frequency;
}

double MidiTimer::midiNoteToFrequency(int midiNote)
{
	return 440 * pow(2.0, (midiNote - 69) / 12.0);
}

int MidiTimer::midiNoteToPrescaler(int midiNote)
{
	// supported prescalers are 1, 8, 64, 256 or 1024
	if (midiNote < 11)
	{
		return 64;
	}
	else if (midiNote < 47)
	{
		return 8;
	}
	else
	{
		return 1;
	}
}

int MidiTimer::prescalerToPrescalerBits(int prescaler)
{
	switch (prescaler)
	{
		default:
		case 1:		return 0b1001;
		case 8:		return 0b1010;
		case 64:	return 0b1011;
		case 256:	return 0b1100;
		case 1024:	return 0b1101;
	}
}
