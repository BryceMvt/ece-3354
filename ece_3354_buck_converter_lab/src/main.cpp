// Bryce Mooney
// main.cpp
// Last Modified: Bryce Mooney, 10/09/24
// This program provides feedback control to a simple buck converter circuit. (5VDC -> 2.5VDC)

#include <Arduino.h>

void setup()
{
  // GPIO #9 is our PWM output.
  pinMode(9, OUTPUT);

  // Timer1 configuration is split between the 2 registers TCCR1A and TCCR1B.
  // Wavegen mode #14 "fast PWM" is assigned by setting bits WGM11, WGM12, and WGM13.
  // Output-compare mode #2 "non-inverting" is assigned by setting bit COM1A1.
  // Non-prescaled clock source (prescaler = 1) is assigned by setting bit CS10.
  TCCR1A = (1 << WGM11);
  TCCR1B = (1 << WGM13)  | (1 << WGM12) | (1 << CS10);

  // Timer1 TOP value must be set to 266 (265.6667) for 60kHz frequency.
  ICR1 = 265;

  // Set the initial duty cycle. (133/266 = 50%)
  OCR1A = 133;

  // Enable the PWM output on GPIO #9.
  TCCR1A |= (1 << COM1A1);
}

void loop()
{

}