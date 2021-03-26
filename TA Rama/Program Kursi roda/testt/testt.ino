#define m1 5
#define m2 3
#define m3 9
#define m4 6

char baca = ' ';

void setup() {

  Serial.begin(38400); // Default communication rate of the Bluetooth module
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
}

void loop() { // run over and over
  if (Serial.available()) {
    baca = Serial.read();
    Serial.println(baca);
  }
    Serial.println(baca);
    if (baca == '1') {
      Serial.println("MAJU");
      //motor kiri
      analogWrite(m1, 145);
      analogWrite(m2, 0);
      //motor kanan
      analogWrite(m3, 145);
      analogWrite(m4, 0);
    }
    else if (baca == '2') {
      Serial.print("MUNDUR");
      //motor kiri
      analogWrite(m1, 0);
      analogWrite(m2, 120);
      //motor kanan
      analogWrite(m3, 0);
      analogWrite(m4, 120);
    }
    else if (baca == '3') {
      Serial.println("KANAN");
      //motor kiri
      analogWrite(m1, 100);
      analogWrite(m2, 0);
      //motor kanan
      analogWrite(m3, 0);
      analogWrite(m4, 100);
    }
    else if (baca == '4') {
      Serial.println("KIRI");
      //motor kiri
      analogWrite(m1, 0);
      analogWrite(m2, 100);
      //motor kanan
      analogWrite(m3, 100);
      analogWrite(m4, 0
      
      
      );
    }
    else {
      Serial.println("Normal");
      analogWrite(m1, 0);
      analogWrite(m2, 0);
      //motor kanan
      analogWrite(m3, 0);
      analogWrite(m4, 0);
    }
 
  }
