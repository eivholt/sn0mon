#include <LowPower.h>

const int usPowerPin = 3;
bool usToggle = 0;

void setup() 
{
  for (byte i = 0; i <= A5; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }

  pinMode(usPowerPin, OUTPUT);
}

void loop() 
{
  digitalWrite(usPowerPin, usToggle);
  usToggle = !usToggle;
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
