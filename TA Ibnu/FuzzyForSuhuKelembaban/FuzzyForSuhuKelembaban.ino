#include <dht11.h>

#define DHT11_PIN A0
#define FAN1 4 //FANection motor kiri
#define FAN2 5 //FANection motor kanan
#define PIR 6

dht11 DHT11;
float pwm;
float suhuError, suhu;
float readSuhu, kelembapan ;
float defuz, output;
float setPoint = 27.00;
float suhuA [5];
float rule [5];
float rule00, rule01, rule02, rule03, rule04;

int readPir;

void setup() {
  Serial.begin(9600);
  Serial.println("Fuzzy Suhu");
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);
  pinMode(PIR, INPUT);

}

void loop() {
  getPir();
  getSuhu();
  getError(readSuhu);
  FuzzySuhu(suhuError);
  Defuzzy();
  (readPir == 1)? (onFan(output)) : (offFan());
//  if (readPir == 1){
//    onFan(output);
//  }else{
//    offFan();
//  }
  

//  Serial.println(time);
  Serial.print("\t");
  Serial.print(readSuhu);
  Serial.print(" C\t");
  Serial.print(kelembapan);
  Serial.print(" %\t\t");
  Serial.print(readPir);
  Serial.print("  \t\t");
  Serial.println(output);
}

void getPir (){
//  readPir = digitalRead(PIR);
    readPir = 1;
}
void onFan(unsigned char speed_fan) {
  analogWrite(FAN1, speed_fan);
  analogWrite(FAN2, 0);
  Serial.print(" \tPeople Detect\t");
}
void offFan(){
  analogWrite(FAN1, 0);
  analogWrite(FAN2, 0);
  Serial.print(" \tPeople Undetect\t");
}

void getSuhu()
{
  float chk = DHT11.read(DHT11_PIN);
  readSuhu = DHT11.temperature;
  readSuhu -= 4;
  kelembapan = DHT11.humidity;
 
}
void getError (float suhuTerbaca){
  suhuError = suhuTerbaca - setPoint;
}

void FuzzySuhu(float suhu) {
  // error sangat kecil
  if (suhu <= 0)
  {
    suhuA [0] = 1;
  }
  else if (suhu > 0 && suhu <= 1)
  {
    suhuA [0] = (1 - suhu) / (1 - 0);
  }
  else
  {
    suhuA [0] = 0;
  }

  // error kecil
  if (suhu <= 0)
  {
    suhuA [1] = 0;
  }
  else if (suhu > 0 && suhu <= 1)
  {
    suhuA [1] = (suhu - 0) / (1 - 0);
  }
  else if (suhu > 1 && suhu <= 2)
  {
    suhuA [1] = (2 - suhu) / (2 - 1);
  }
  else
  {
    suhuA [1] = 0;
  }

  // normal
  if (suhu <= 1)
  {
    suhuA [2] = 0;
  }
  else if (suhu > 1 && suhu <= 2)
  {
    suhuA [2] = (suhu - 1) / (2 - 1);
  }
  else if (suhu > 2 && suhu <= 3)
  {
    suhuA [2] = (3 - suhu) / (33 - 2);
  }
  else
  {
    suhuA [2] = 0;
  }
   // error besar
  if (suhu <= 2)
  {
    suhuA [3] = 0;
  }
  else if (suhu > 2 && suhu <= 3)
  {
    suhuA [3] = (suhu - 2) / (3 - 2);
  }
  else if (suhu > 3 && suhu <= 4)
  {
    suhuA [3] = (4 - suhu) / (4 - 3);
  }
  else
  {
    suhuA [3] = 0;
  }

  // error sangat besar
  if (suhu <= 3)
  {
    suhuA [4] = 0;
  }
  else if (suhu > 3 && suhu <= 4)
  {
    suhuA [4] = (suhu - 3) / (4 - 3);
  }
  else
  {
    suhuA [4] = 1;
  }
  
}
void RuleEva () {
  for ( int i = 0; i <= 4; i++)
  {
    rule [i] = suhuA[i];
  }
  rule00 = rule [0]; 
  rule01 = rule [1]; 
  rule02 = rule [2]; 
  rule03 = rule [3]; 
  rule04 = rule [4]; 
//  Serial.println("rul eva");
}
void Defuzzy () {
  // metode sugeno (weighted average)
  float mati = 50;
  float pelan = 95;
  float normal = 130;
  float cepat = 180;
  float sangat_cepat = 255;
  
  RuleEva();
  output = (rule00 * mati) + (rule01 * pelan) + (rule02 * normal) + (rule03 * cepat) + (rule04 * sangat_cepat);
  
  defuz = 0;
//  Serial.println("defuzzy");
  
  for ( int i = 0; i <= 4; i++)
  {
    defuz = defuz + rule[i];
  }

  //    Serial.println(defuz);
  output = output / defuz;
  
}
