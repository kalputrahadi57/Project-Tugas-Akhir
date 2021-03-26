#include <SD.h>
#include <SPI.h>
File myFile;

#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   6
#define DATA_PIN  4
#define CS_PIN    5

#define MQ_PIN A0

#define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

// HARDWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);/
// SOFTWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed = 100;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 100; // in milliseconds

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  75
char* curMessage = { "" };
char* newMessage = { "Stevanny" };
bool firstTime = true;

float ppm;

void setup()
{
  Serial.begin(9600);
  Serial.print("\n[Parola Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline");

  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization");
    while (1);
  }

  Serial.println("initialization sd card done");

  pinMode (MQ_PIN, INPUT);
}

void loop()
{
  displayAnimateMax();
  getSensorMq135();
  saveData("\n\n PPM: ", ppm);
}

unsigned long timeNow, timePrev;

void getSensorMq135() {
  timeNow = millis();
  if (timeNow - timePrev >= 100) {
    int val = analogRead(MQ_PIN);
    float resistance = ((1023. / (float)val) * 5. - 1.) * RLOAD;
    ppm = PARA * pow((resistance/RZERO), -PARB);//

    timePrev = timeNow;
  }
  rangeValue(ppm, 0, 2, "\tSEHAT"); // 0 - 50
  rangeValue(ppm, 3, 4, "\tSEDANG"); // 51 - 150
  rangeValue(ppm, 151, 350, "\tTIDAK SEHAT");
  rangeValue(ppm, 351, 420, "\tSANGAT TIDAK SEHAT");
  rangeValue(ppm, 420, 1000, "\tBERBAHAYA");


}

void rangeValue(float value, int minValue, int maxValue, char* msg) {
  if (value >= minValue && value <= maxValue) {
    Serial.println(msg);
    newMessage = msg;
  }
}

void displayAnimateMax() {
  if (P.displayAnimate())
  {
    if (newMessage != curMessage)
    {
      Serial.println("new message");
      strcpy(curMessage, newMessage);
    }
    P.displayReset();
  }
}

void saveData(String str1, float data1) {
  myFile = SD.open("DATA_N.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to sd card");
    myFile.print(str1 + String(data1, 2));

    myFile.close();
    Serial.println("done");
  } else {
    Serial.println("error opening text");
  }
}
