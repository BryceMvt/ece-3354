// Bryce Mooney
// main.cpp
// Last Modified: Bryce Mooney, 10/16/24
// This program provides feedback control to a buck-boost converter circuit. (5VDC -> 2.5VDC)

#include <Arduino.h>
#include <Button.h>

// ADC measurement setpoints for both the 2.5V and 6V output cases are recorded from supplying 2.5V and 6V to both outputs manually.
const int setpoint_2V5 = 350;
const int setpoint_6V0 = 800;
const int min_duty_cycle = 13;  // Approximately 5%
const int max_duty_cycle = 212; // Approximately 80%

// A button is placed across GPIO #4 to quickly swap between 2.5V and 6V setpoints
Button setpoint_button(4);

void setup()
{
  // GPIO #4 Is a button that toggles the setpoint to either 2.5V or 6V when pressed
  pinMode(4, INPUT_PULLUP);

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

  // Set the initial duty cycle. (27/265 is about 10%)
  OCR1A = 27;

  // Enable the PWM output on GPIO #9.
  TCCR1A |= (1 << COM1A1);

  delay(100);
}

void loop()
{
  static int current_setpoint = setpoint_2V5;

  if (setpoint_button.pressed())
  {
    if (current_setpoint == setpoint_2V5)
    {
      current_setpoint = setpoint_6V0;
    }
    else
    {
      current_setpoint = setpoint_2V5;
    }
  }
  
  // Measure the buck converter output voltage (1023 is about 5V)
  int ADC_reading = analogRead(A0);

  if (ADC_reading > current_setpoint)
  {
    OCR1A = constrain(OCR1A - 1, min_duty_cycle, max_duty_cycle);
  }
  else if (ADC_reading < current_setpoint)
  {
    OCR1A = constrain(OCR1A + 1, min_duty_cycle, max_duty_cycle);
  }
}