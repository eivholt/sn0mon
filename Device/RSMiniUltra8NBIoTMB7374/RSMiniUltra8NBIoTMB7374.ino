#define DEBUG

#include <TelenorNBIoT.h>
#include <LowPower.h>

#ifdef SERIAL_PORT_HARDWARE_OPEN
/*
 * For Arduino boards with a hardware serial port separate from USB serial.
 * This is usually mapped to Serial1. Check which pins are used for Serial1 on
 * the board you're using.
 */
#define ublox SERIAL_PORT_HARDWARE_OPEN
#else
/*
 * For Arduino boards with only one hardware serial port (like Arduino UNO). It
 * is mapped to USB, so we use SoftwareSerial on pin 10 and 11 instead.
 */
#include <SoftwareSerial.h>
SoftwareSerial ublox(10, 11);
#endif

TelenorNBIoT nbiot("mda.ee", 242, 01);
// The remote IP address to send data packets to
// u-blox SARA N2 does not support DNS
IPAddress remoteIP(172, 16, 15, 14);
unsigned int REMOTE_PORT = 1234;
unsigned long INTERVAL_MS = (unsigned long) 15 * 1000;

const int usPwPin = 5;
const int usPowerPin = 3;
long rangeReadingMm;
unsigned long Timeout_Duration = 5100000; //5.1 seconds

void setup() 
{
  pinMode(usPwPin, INPUT);
  pinMode(usPowerPin, OUTPUT);
  digitalWrite(usPowerPin, LOW);

  #ifdef DEBUG
  Serial.begin(9600);
  Debugln(F("Setup"));
  Debugln(F("Connecting to NB-IoT module..."));
  #endif
  ublox.begin(9600);
  nbiot.begin(ublox);
  nbiot.createSocket();

  Debugln(F("NB-IoT module connected."));
  
  Sleep(4);
} 
 
void loop() 
{
  if (nbiot.isConnected()) 
  {
    ReadUsSensor();
    PrintRangeReading();

    Debugln(F("Sending data"));

    String payload = 
        String(rangeReadingMm);
    if (true == nbiot.sendString(remoteIP, REMOTE_PORT, payload)) {
      Debug(F("Successfully sent data: "));
    } else {
      Debug(F("Failed sending data: "));
    }
    Debugln(payload);
  }

  Sleep(4);
}

void ReadUsSensor(){
  Debugln(F("Enable sensor"));
  digitalWrite(usPowerPin, HIGH);
  Sleep(1);
  Debugln(F("Take reading"));
  rangeReadingMm = pulseIn(usPwPin, HIGH, Timeout_Duration);
  digitalWrite(usPowerPin, LOW);
}

void PrintRangeReading(){         
  Debugln(String(rangeReadingMm));
}

void Sleep(int iterations)
{
  for(int i = 0; i < iterations; i++)
  {
    Debugln(F("Sleep 8s"));
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

void Debugln(String message)
{
  #ifdef DEBUG
  Serial.println(message);
  Serial.flush();
  #endif
}

void Debug(String message)
{
  #ifdef DEBUG
  Serial.print(message);
  Serial.flush();
  #endif
}

void SetAllPinsLow()
{
  for (byte i = 0; i <= A5; i++)
  {
    pinMode (i, OUTPUT);
    digitalWrite (i, LOW);
  }
}
