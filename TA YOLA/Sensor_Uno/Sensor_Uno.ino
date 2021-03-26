#include <SoftwareSerial.h>
SoftwareSerial mySerial (6, 7);

#include "Sht11.h"
byte dataPin1 = A2;
byte clockPin1 = A3;

byte dataPin2 = A0;
byte clockPin2 = A1;

byte dataPin3 = A4;
byte clockPin3 = A5;

//
float calfacT_a = 0.98; // calibration suhu sensor A
float calfacT_b = 1; // calibration suhu sensor B
float calfacT_c = 1; // calibration suhu sensor C

float calfacH_a = 1; // calibration humidity sensor A
float calfacH_b = 1; // calibration humidity sensor B
float calfacH_c = 1; // calibration humidity sensor C

Sht11 sht11_1(dataPin1, clockPin1); //data pin, clock pin
Sht11 sht11_2(dataPin2, clockPin2); //data pin, clock pin
Sht11 sht11_3(dataPin3, clockPin3); //data pin, clock pin


const unsigned long period = 500UL; // must larger than 100
unsigned long prevTime;
float temperature1, humidity1;
float temperature2, humidity2;
float temperature3, humidity3;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  sht11_1.setUp();
  sht11_2.setUp();
  sht11_3.setUp();
}

void loop() {
  unsigned long timeNow = millis();
  getSensorSHT11();
  if (timeNow - prevTime >= 1000) {
    sendToNodeMcu();
    Serial.print(temperature1 * calfacT_a);
    Serial.print("\t");
    Serial.print(humidity1 * calfacH_a);
    Serial.print("\t");
    Serial.print(temperature2 * calfacT_b);
    Serial.print("\t");
    Serial.print(humidity2 * calfacH_b);
    Serial.print("\t");
    Serial.print(temperature3 * calfacT_c);
    Serial.print("\t");
    Serial.print(humidity3 * calfacH_c);
    Serial.print("\t");
    Serial.println();

    prevTime = timeNow;
  }

}
float ppm;
void getSensorSHT11() {
  sht11_1.getSensor(temperature1, humidity1, period);
  sht11_2.getSensor(temperature2, humidity2, period);
  sht11_3.getSensor(temperature3, humidity3, period);
}

void sendToNodeMcu() {
  mySerial.print(temperature1 * calfacT_a);
  mySerial.print("a");
  mySerial.print(humidity1 * calfacH_a);
  mySerial.print("b");
  mySerial.print(temperature2 * calfacT_b);
  mySerial.print("c");
  mySerial.print(humidity2 * calfacH_b);
  mySerial.print("d");
  mySerial.print(temperature3 * calfacT_c);
  mySerial.print("e");
  mySerial.print(humidity3 * calfacH_c);
  mySerial.print("f");
  mySerial.println();
}
