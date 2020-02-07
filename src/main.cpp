#include <Arduino.h>
#include <SoftwareSerial.h>

// #include <AltSoftSerial.h>

#define rxPin 3
#define txPin 2

String Arsp, Grsp;

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(rxPin, txPin ); //SIM800L Tx & Rx is connected to Arduino #3 & #2
// AltSoftSerial mySerial(rxPin, txPin);
// RX, TX
// 3, 2

int led = 13; //build in 17
int bound_rate = 9600;

int incomingByte = 0;

void updateSerial()
{
  delay(500);
  Serial.println(mySerial.available());
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void setup()
{
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(bound_rate);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(bound_rate);
  delay(500);
  Serial.println("Initializing...");
  mySerial.println("mySerial sending test...");
  delay(500);
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
}

void loop()
{
  // updateSerial();
  Serial.print(mySerial.available());
  delay(200);
  if (mySerial.available()) {
    digitalWrite(led, HIGH);
    Grsp = mySerial.readString();
    Serial.print(Grsp);
  }
  if (Serial.available()) {
    // mySerial.write(Serial.read());
    Arsp = Serial.readString();
    mySerial.println(Arsp);
  }

}
