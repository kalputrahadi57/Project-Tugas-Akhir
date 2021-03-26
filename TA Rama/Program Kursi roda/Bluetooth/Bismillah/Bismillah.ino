#include <Wire.h>
#include "MPU6050_tockn.h"



MPU6050 mpu6050(Wire);



char data;
float roll;

//const int pattern[][2] = {
//  {100, 50},
//  {70, 90},
//  {300, 200},
//  {100, 50},
//  {70, 90},
//  {200, 200},
//  {100, 50},
//  {70, 90},
//  {100, 20},
//  {70, 70},
//  {200, 200}
//};

void setup() {

  Serial.begin(9600);
  //pinMode(buzzer, OUTPUT);


  /*
    for (byte i = 0; i < sizeof(pattern) / sizeof(int) / 2; i++) {
    digitalWrite(buzzer, HIGH);
    delay(pattern[i][0]);
    digitalWrite(buzzer, LOW);
    delay(pattern[i][1]);
    }
  */
  Wire.begin();
  mpu6050.begin();
 
  mpu6050.calcGyroOffsets(true);
  


  Serial.println("CLEARDATA");
  Serial.println("LABEL,Date,Time,Flex1,Flex2,MPU,Waktu");
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

  Serial.println( (String) "DATA,DATE,TIME," + teg1 + "," + teg2 + "," + roll + "," + (millis() / 1000) );



  mpu6050.update();
  roll =  mpu6050.getAngleX();}
