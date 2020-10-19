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
  //SetAllPinsLow();
  //pinMode(LED_BUILTIN, OUTPUT); //LED Connected to Pin 7
  pinMode(usPwPin, INPUT);
  pinMode(usPowerPin, OUTPUT);
  digitalWrite(usPowerPin, LOW);
  
  Serial.begin(9600);
  Serial.println("setup");
  Serial.print(F("Connecting to NB-IoT module...\n"));
  //digitalWrite(LED_BUILTIN, HIGH);
  ublox.begin(9600);
  nbiot.begin(ublox);
  nbiot.createSocket();
  //digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("NB-IoT module connected."));
  Serial.flush();
} 
 
void loop() 
{
  
  if (nbiot.isConnected()) {
    // Successfully connected to the network
    // Send message to remote server
    ReadUsSensor();
    PrintRangeReading();
    Serial.println(F("Sending data"));
    //digitalWrite(LED_BUILTIN, HIGH);
    String payload = 
        String(rangeReadingMm);
    if (true == nbiot.sendString(remoteIP, REMOTE_PORT, payload)) {
      Serial.println(F("Successfully sent data: "));
    } else {
      Serial.println(F("Failed sending data: "));
    }
    //digitalWrite(LED_BUILTIN, LOW);
  }

  for(int i = 0; i < 2; i++)
  {
    Sleep();
  }
}

void ReadUsSensor(){
  digitalWrite(usPowerPin, HIGH);
  rangeReadingMm = pulseIn(usPwPin, HIGH, Timeout_Duration);
  digitalWrite(usPowerPin, LOW);
}

void PrintRangeReading(){         
  Serial.println(rangeReadingMm);
  Serial.flush();
}

void Sleep()
{
  #ifdef DEBUG
  Serial.println("Sleep 8s");
  Serial.flush();
  #endif
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void SetAllPinsLow()
{
  for (byte i = 0; i <= A5; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }
}
