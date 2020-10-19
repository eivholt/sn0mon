#define DEBUG

#include <LowPower.h>

const int usPwPin = 5;
const int usPowerPin = 3;
long sensor1, cm, inches;
unsigned long Timeout_Duration = 5100000; //5.1 seconds

void setup () {
  SetAllPinsLow();
  Serial.begin(9600);
  Serial.println("setup");
  Serial.flush();
  pinMode(usPwPin, INPUT);
  pinMode(usPowerPin, OUTPUT);
  digitalWrite(usPowerPin, LOW);
}

void read_sensor(){
  digitalWrite(usPowerPin, HIGH);
  sensor1 = pulseIn(usPwPin, HIGH, Timeout_Duration);
  cm = sensor1/10; // converts the range to cm
  digitalWrite(usPowerPin, LOW);
  //digitalWrite(usRangingPin, LOW);
}

void printall(){         
  Serial.println(sensor1); //This can be made to match the measurement type you wish to have.
  Serial.flush();
}

void loop () {
  Sleep();
  Serial.println("wake");
  Serial.flush();
  read_sensor();
  printall();
}

void Sleep()
{
  #ifdef DEBUG
  Serial.println("Sleep 4s");
  Serial.flush();
  #endif
  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

void SetAllPinsLow()
{
  for (byte i = 0; i <= A5; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }
}
