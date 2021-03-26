#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <SD.h>
#include <SPI.h>
File myFile;

#include <SoftwareSerial.h>
SoftwareSerial mySerial (D4, D3);

#define LED_INDICATOR D3
#define CS_PIN D8
#define SENSOR A0

#define TIMEUPLOAD 300000
#define TIMELOGGER 300000 //milisecond
unsigned long prevTiUpload, prevTiDataLog;

String apiKey = "5A22HURORHO8W21S";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "yola";     // replace with your wifi ssid and wpa2 key
const char *pass =  "yola12345";
const char* server = "api.thingspeak.com";
unsigned long myChannelNumber = 564338;
WiFiClient client;
String strData;

float temperature_1, temperature_2, temperature_3, temperatureAvg;
float humidity_1, humidity_2, humidity_3, humidityAvg;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(10);

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization");
    while (1);
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
  unsigned long timeNow = millis();
  getSerialData();
  if (timeNow - prevTiUpload > TIMEUPLOAD) {
    uploadToThinkspeak();
    prevTiUpload = timeNow;
  }
  if (timeNow - prevTiDataLog > TIMELOGGER) {
    dataLoggerSdCard();
    prevTiDataLog = timeNow;
  }

}

void getSerialData() {
  if (mySerial.available() != 0) {
    delay(50);
    while (mySerial.available()) {
      char a = mySerial.read();
      strData += a;
    }
    //    Serial.println(strData);
    String tempeStr_1 = strData.substring(0, strData.indexOf('a'));
    String humiStr_1 = strData.substring((strData.indexOf('a') + 1), strData.indexOf('b'));
    String tempeStr_2 = strData.substring((strData.indexOf('b') + 1), strData.indexOf('c'));
    String humiStr_2 = strData.substring((strData.indexOf('c') + 1), strData.indexOf('d'));
    String tempeStr_3 = strData.substring((strData.indexOf('d') + 1), strData.indexOf('e'));
    String humiStr_3 = strData.substring((strData.indexOf('e') + 1), strData.indexOf('f'));

    temperature_1 = tempeStr_1.toFloat();
    humidity_1 = humiStr_1.toFloat();
    temperature_2 = tempeStr_2.toFloat();
    humidity_2 = humiStr_2.toFloat();
    temperature_3 = tempeStr_3.toFloat();
    humidity_3 = humiStr_3.toFloat();

    temperatureAvg = (temperature_1 + temperature_2 + temperature_3) / 3;
    humidityAvg = (humidity_1 + humidity_2 + humidity_3) / 3;

    //    Serial.println(tempeStr_2);
    strData = "";
  }
}

void uploadToThinkspeak() {
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    if (temperatureAvg < 10) {
      postStr += "&field1=";
      postStr += String(temperature_1, 3);
      postStr += "&field2=";
      postStr += String(humidity_1, 2);
      postStr += "&field3=";
      postStr += String(temperature_2, 3);
      postStr += "&field4=";
      postStr += String(humidity_2, 2);
      postStr += "&field5=";
      postStr += String(temperature_3, 3);
      postStr += "&field6=";
      postStr += String(humidity_3, 2);
      postStr += "&field7=";
      postStr += String(temperatureAvg, 3);
      postStr += "&field8=";
      postStr += String(humidityAvg, 2);
      postStr += "\r\n\r\n";
      Serial.println(String(temperature_1, 3));
    } else {
      postStr += "&field1=";
      postStr += String(temperature_1, 2);
      postStr += "&field2=";
      postStr += String(humidity_1, 2);
      postStr += "&field3=";
      postStr += String(temperature_2, 2);
      postStr += "&field4=";
      postStr += String(humidity_2, 2);
      postStr += "&field5=";
      postStr += String(temperature_3, 2);
      postStr += "&field6=";
      postStr += String(humidity_3, 2);
      postStr += "&field7=";
      postStr += String(temperatureAvg, 2);
      postStr += "&field8=";
      postStr += String(humidityAvg, 2);
      postStr += "\r\n\r\n";
    }

    if (humidityAvg < 10) {
      postStr += "&field1=";
      postStr += String(temperature_1, 2);
      postStr += "&field2=";
      postStr += String(humidity_1, 3);
      postStr += "&field3=";
      postStr += String(temperature_2, 2);
      postStr += "&field4=";
      postStr += String(humidity_2, 3);
      postStr += "&field5=";
      postStr += String(temperature_3, 2);
      postStr += "&field6=";
      postStr += String(humidity_3, 3);
      postStr += "&field7=";
      postStr += String(temperatureAvg, 2);
      postStr += "&field8=";
      postStr += String(humidityAvg, 3);
      postStr += "\r\n\r\n";
    } else {
      postStr += "&field1=";
      postStr += String(temperature_1, 2);
      postStr += "&field2=";
      postStr += String(humidity_1, 2);
      postStr += "&field3=";
      postStr += String(temperature_2, 2);
      postStr += "&field4=";
      postStr += String(humidity_2, 2);
      postStr += "&field5=";
      postStr += String(temperature_3, 2);
      postStr += "&field6=";
      postStr += String(humidity_3, 2);
      postStr += "&field7=";
      postStr += String(temperatureAvg, 2);
      postStr += "&field8=";
      postStr += String(humidityAvg, 2);
      postStr += "\r\n\r\n";
    }


    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("%. Send to Thingspeak.");
  }
}

void saveData(String str1, String str2, String str3, String str4, String str5, String str6, String str7, String str8, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8) {
  myFile = SD.open("DATA_N.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to sd card");
    myFile.print(str1 + String(data1, 2));
    myFile.print(str2 + String(data2, 2));
    myFile.print(str3 + String(data3, 2));
    myFile.print(str4 + String(data4, 2));
    myFile.print(str5 + String(data5, 2));
    myFile.print(str6 + String(data6, 2));
    myFile.print(str7 + String(data7, 2));
    myFile.print(str8 + String(data8, 2));

    myFile.close();
    Serial.println("done");
  } else {
    Serial.println("error opening text");
  }
}

void dataLoggerSdCard() {
  saveData("\n\nSuhu-1 : ", "\t|Kelembapan-1 : ", "\t|Suhu-2 : ", "\t|Kelembapan-2 : ", "\t|Suhu-3 : ", "\t|Kelembapan-3 : ", "\t|Suhu Rata-Rata : ", "\t|Kelembaban Rata-Rata : ", temperature_1, humidity_1, temperature_2, humidity_2 , temperature_3, humidity_3, temperatureAvg, humidityAvg);
}
