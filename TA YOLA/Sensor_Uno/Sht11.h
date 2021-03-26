/*
   Sht11.h for sensor sht11
   Created by Kalputra Hadi, February 12, 2020
   email : kalputrahadi57official@gmail.com
*/

#include <Sensirion.h>

class Sht11 : public Sensirion{
  private:
    byte stat;
    byte error;
    byte measActive = false;
    byte measType = TEMP;
    unsigned int rawData;
    float temperature;
    float humidity;
    float dewpoint;
    unsigned long trhMillis;

  public:
    
    Sht11(byte dataPin_, byte clockPin_) : Sensirion (dataPin_, clockPin_)
    {
    }
    
    void setUp() {
      readSR(&stat);                     // Read sensor status register
      Serial.print("Status reg = 0x");
      Serial.println(stat, HEX);
      writeSR(LOW_RES);                  // Set sensor to low resolution
      readSR(&stat);                     // Read sensor status register again
      Serial.print("Status reg = 0x");
      Serial.println(stat, HEX);
      // Demonstrate blocking calls
      measTemp(&rawData);                // sht.meas(TEMP, &rawData, BLOCK)
      temperature = calcTemp(rawData);
      measHumi(&rawData);                // sht.meas(HUMI, &rawData, BLOCK)
      humidity = calcHumi(rawData, temperature);
      dewpoint = calcDewpoint(humidity, temperature);
    }

    void getSensor(float &temperature_, float &humidity_, unsigned long TRHSTEP) {

      unsigned long curMillis = millis();          // Get current time
      // Demonstrate non-blocking calls
      if (curMillis - trhMillis >= TRHSTEP) {      // Time for new measurements?
        measActive = true;
        measType = TEMP;
        meas(TEMP, &rawData, NONBLOCK);        // Start temp measurement
        trhMillis = curMillis;
      }
      if (measActive && measRdy()) {           // Check measurement status
        if (measType == TEMP) {                    // Process temp or humi?
          measType = HUMI;
          temperature = calcTemp(rawData);     // Convert raw sensor data
          meas(HUMI, &rawData, NONBLOCK);      // Start humi measurement
        } else {
          measActive = false;
          humidity = calcHumi(rawData, temperature); // Convert raw sensor data
          dewpoint = calcDewpoint(humidity, temperature);

          temperature_ = temperature;
          humidity_ = humidity;
          
          if (temperature_ < 10 || humidity_ < 0 ){
            temperature_  = 15;
          }
          if (temperature_ > 100 || humidity_ > 100 ){
            temperature_  = 99;
          }
          
        }
      }
    }
};
