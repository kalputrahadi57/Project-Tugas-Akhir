#include <SD.h>
#include <SPI.h>

File myFile;

#define LED_INDICATOR 3
#define CS_PIN 9
#define SENSOR A0

float temperature, humidity;

void setup() {
  Serial.begin(9600);
  pinMode(LED_INDICATOR, OUTPUT);
  pinMode(SENSOR, INPUT);
  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization");
    while(1);
  }

  Serial.println("initialization done");
}

void loop() {
//  getSensor();
  delay(1000);
  saveData("Suhu : ", "\tKelembapan : ", temperature, humidity);
  

}

void saveData(String str1, String str2, float data1, float data2) {
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to sd card");
    myFile.print(str1 + String(data1));
    myFile.println(str2 + String(data2,2));
    Serial.print(str1 + String(data1,2));
    Serial.println(str2 + String(data2,2));

    myFile.close();
    Serial.println("done");
  } else {
    Serial.println("error opening text");
  }
}

void getSensor() {
  temperature = map(analogRead(SENSOR), 0, 1023, 0, 200);
  humidity = map(analogRead(SENSOR)/2, 0, 511, 0, 100);
}
