#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <Sensirion.h>
#include <SD.h>
#include <SPI.h>

File myFile;

#define LED_INDICATOR D3
#define CS_PIN D8
#define SENSOR A0

String apiKey = "5A22HURORHO8W21S";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "yola";     // replace with your wifi ssid and wpa2 key
const char *pass =  "yola12345";
const char* server = "api.thingspeak.com";
unsigned long myChannelNumber = 564338;
WiFiClient client;
int dataPin  =  4;
int clockPin =  5;
float temperature;
float humidity;
float dewpoint;

Sensirion tempSensor = Sensirion(dataPin, clockPin);
//int temperature = random( 16,28);
//int humidity = random(56,80);
void setup()
{
  Serial.begin(115200);
  delay(10);
  
  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization");
    while(1);
  }

  Serial.println("initialization sd card done");
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{

  tempSensor.measure(&temperature, &humidity, &dewpoint);

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    saveData("Suhu : ", "\tKelembapan : ", temperature, humidity);
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%. Send to Thingspeak.");
  }
  // client.stop();

  // Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(1000);
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
