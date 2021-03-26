#pragma once
#include <Arduino.h>

class MidiTimer
{
public:
	MidiTimer (
		uint8_t outputPin,
		volatile uint8_t &controlRegisterA,
		volatile uint8_t &controlRegisterB,
		volatile uint16_t &compareRegister);

	void begin ();
	void setNote(int midiNote);
	uint16_t getFrequency() const;

private:
	static double midiNoteToFrequency(int midiNote);
	static int midiNoteToPrescaler(int midiNote);
	static int prescalerToPrescalerBits(int prescaler);

private:
	uint8_t m_outputPin{};
	double m_frequency{};
	volatile uint8_t &m_controlRegisterA;
	volatile uint8_t &m_controlRegisterB;
	volatile uint16_t &m_compareRegister;
};

