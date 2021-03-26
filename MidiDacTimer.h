#pragma once
#include <Arduino.h>

class MidiDacTimer4
{
public:
	MidiDacTimer4(uint8_t highFreqPin);
	void begin();
	void setFrequency(double midiNote);

private:
	uint8_t m_highFreqPin{7};
};

