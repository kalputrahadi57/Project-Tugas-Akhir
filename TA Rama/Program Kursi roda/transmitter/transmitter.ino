#include <Wire.h>
#include "MPU6050_tockn.h"

MPU6050 mpu6050(Wire);

char data;
float roll;
long distanceA, distanceB, distanceC;
long hasil1A, hasil2A, hasil1B, hasil2B, hasil1C, hasil2C;


void setup() {

  Serial.begin(38400);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}
void loop() {
  /*
     list of command:
     1. Forward = '1'
     2. Backward = '2'
     3. Right = '3'
     4. Left = '4'
     5. Stop = '5'
  */
  int val1 = analogRead(A0); // index finger
  int val2 = analogRead(A1); // thumb finger

  float teg1 = (val1 * (5.0 / 1023.0));
  float teg2 = (val2 * (5.0 / 1023.0));

  //Serial.println( (String) "DATA,DATE,TIME," + teg1 + "," + teg2 + "," + roll + "," + (millis() / 1000) );
  mpu6050.update();
  roll =  mpu6050.getAngleX();

  if (roll > -24.0) {
    hasil1A = abs(teg1 - 3.60);
    hasil2A = abs(teg2 - 2.42);
    distanceA = sqrt(pow(2, hasil1A) + pow(2, hasil2A));
    hasil1B = abs(teg1 - 2.47);
    hasil2B = abs(teg2 - 3.54);
    distanceB = sqrt(pow(2, hasil1B) + pow(2, hasil2B));
    hasil1C = abs(teg1 - 2.77);
    hasil2C = abs(teg2 - 2.64);
    distanceC = sqrt(pow(2, hasil1C) + pow(2, hasil2C));


    if (distanceA < distanceB) { // atas
      Serial.write('1');
      
    }
    else if (distanceB < distanceA) {//bawah
      Serial.write('2');
    }
    else {      
      Serial.write('0'); //normal
    }
  }
  else if (roll <= -24.0) {
    hasil1A = abs(teg1 - 3.60);
    hasil2A = abs(teg2 - 2.42);
    distanceA = sqrt(pow(2, hasil1A) + pow(2, hasil2A));
    hasil1B = abs(teg1 - 2.47);
    hasil2B = abs(teg2 - 3.54);
    distanceB = sqrt(pow(2, hasil1B) + pow(2, hasil2B));
    hasil1C = abs(teg1 - 2.77);
    hasil2C = abs(teg2 - 2.64);
    distanceC = sqrt(pow(2, hasil1C) + pow(2, hasil2C));


    if (distanceA < distanceB) {
      Serial.write('3'); //kanan
    }
    else if (distanceB < distanceA) { //kiri
      Serial.write('4');
    }
    else {
      Serial.write('0'); //
    }
  }
}
