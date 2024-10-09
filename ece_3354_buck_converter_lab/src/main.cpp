// Bryce Mooney
// main.cpp
// Last Modified: Bryce Mooney, 10/09/24
// This program provides feedback control to a simple buck converter circuit. (5VDC -> 2.5VDC)

#include <Arduino.h>

const int SETPOINT = 512;       // Approximately 2.5V
const int min_duty_cycle = 53;  // Approximately 20%
const int max_duty_cycle = 212; // Approximately 80%


void setup()
{
  // GPIO #9 is our PWM output.
  pinMode(9, OUTPUT);

  // Timer1 configuration is split between the 2 registers TCCR1A and TCCR1B.
  // Wavegen mode #14 "fast PWM" is assigned by setting bits WGM11, WGM12, and WGM13.
  // Output-compare mode #2 "non-inverting" is assigned by setting bit COM1A1.
  // Non-prescaled clock source (prescaler = 1) is assigned by setting bit CS10.
  TCCR1A = (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);

  // Timer1 TOP value must be set to 266 (265.6667) for 60kHz frequency.
  ICR1 = 265;

  // Set the initial duty cycle. (133/265 is about 50%)
  OCR1A = 133;

  // Enable the PWM output on GPIO #9.
  TCCR1A |= (1 << COM1A1);

  delay(100);
}

void loop()
{
  // Measure the buck converter output voltage (1023 is about 5V)
  int ADC_reading;
  ADC_reading = analogRead(A0);

  if (ADC_reading > SETPOINT)
  {
    OCR1A = constrain(OCR1A - 1, min_duty_cycle, max_duty_cycle);
  }
  else if (ADC_reading < SETPOINT)
  {
    OCR1A = constrain(OCR1A + 1, min_duty_cycle, max_duty_cycle);
  }
}