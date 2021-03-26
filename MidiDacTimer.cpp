#include "MidiDacTimer.h"
#include <Arduino.h>


MidiDacTimer4::MidiDacTimer4(uint8_t highFreqPin) :
	m_highFreqPin(highFreqPin)
{
}

void MidiDacTimer4::begin()
{
	// These outputs are summed in hardware
	pinMode(10, OUTPUT); // DAC output
	pinMode(m_highFreqPin, OUTPUT);

	TCCR4A = _BV(COM4B0)	// Toggle B on Compare Match
		| _BV(PWM4B);		// Enable PWM on B

	TCCR4B = 1;				// Prescaler: 1

	OCR4C = 255;
}

void MidiDacTimer4::setFrequency(double frequency)
{
	constexpr auto magic = 0.425; // This value was determined from testing the circuit
	constexpr auto maxMilliVolts = 5000;

	auto milliVolts = constrain(round(frequency / magic), 0, maxMilliVolts * 2);
	if (milliVolts >= maxMilliVolts)
	{
		milliVolts -= maxMilliVolts;
		digitalWrite(m_highFreqPin, HIGH);
	}
	else
	{
		digitalWrite(m_highFreqPin, LOW);
	}

	OCR4B = map(milliVolts, 0, maxMilliVolts, 0, 255);
}
