#define DIR1 9 //direction motor kiri
#define DIR2 8 //direction motor kanan

int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
volatile long counterx = 0; // tipe data long nilai berkisat antar -2.147.483.648 hingga 2.147.483.647
unsigned char speedx;
unsigned long time;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

float pwm;
float sudut;
float defuz, output;
float sudutAngel ;
float temp;
float sudutA [12];
float rule [12];
float rule00, rule01, rule02, rule03, rule04, rule05, rule06, rule07, rule08, rule09, rule10, rule11, rule12;



void setup() {
  Serial.begin(9600);
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  attachInterrupt(digitalPinToInterrupt(encoderPin1), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder, CHANGE);

  pinMode(18, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), zsignal, HIGH);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);


  // pinMode(50, INPUT);

  //inisialisasi
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
}

void loop() {
  getSudut(encoderValue);
  sudutAngel = sudut;
  FuzzyAngel(sudutAngel);
  Defuzzy();
  rotasix();

  time=millis();

  Serial.println(time);
  Serial.print(sudut);
  Serial.print("\t");
  Serial.println(output);
}

//sudut
void rotasix() {
  if (sudut > 0) {
    rotasi2(output);
  }
  else if (sudut < 0) {
    rotasi1(output);
  }
  else if (sudut > 45 || sudut < -45) {
    output = 0;
    diam(output);
  }
  else {
    output = 0;
    diam(output);
  }
}

void rotasi1(unsigned char speed_motor) {
  analogWrite(DIR2, 0);
  speedx = speed_motor;
  analogWrite(DIR1, speedx);
}
void rotasi2(unsigned char speed_motor) {
  analogWrite(DIR1, 0);
  speedx = speed_motor;
  analogWrite(DIR2, speedx);
}
void diam(unsigned char speed_motor) {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101) encoderValue++;
  if (sum == 0b1110) encoderValue--;

  lastEncoded = encoded; //store this value for next time
}
void getSudut(float encoder)
{
  sudut = (encoder * 0.36);
  sudut = sudut - 180;
}
void ai2() {              //pastikan jika bertambah searah jarum jam
  if (digitalRead(20) == LOW) {
    counterx++;
  } else {
    counterx--;
  }
}

void ai3() {              //pastikan jika berkurang berlawanan arah jarum jam
  if (digitalRead(21) == LOW) {
    counterx--;
  } else {
    counterx++;
  }
}

void zsignal() {
  //encoderValue = 512;
}
void FuzzyAngel(float sudutAngel) {
  // untuk sudut kiri 30
  if (sudutAngel < -30)
  {
    sudutA [0] = 0;
  }
  else if (sudutAngel >= -30 && sudutAngel <= -25)
  {
    sudutA [0] = ((-25) - sudutAngel) / ((-25) - (-30));
  }
  else
  {
    sudutA [0] = 0;
  }

  // untuk sudut kiri 25
  if (sudutAngel <= -30)
  {
    sudutA [1] = 0;
  }
  else if (sudutAngel > -30 && sudutAngel <= -25)
  {
    sudutA [1] = (sudutAngel - (-30)) / ((-25) - (-30));
  }
  else if (sudutAngel > -25 && sudutAngel <= -20)
  {
    sudutA [1] = ((-20) - sudutAngel) / ((-20) - (-25));
  }
  else
  {
    sudutA [1] = 0;
  }

  // untuk sudut kiri 20
  if (sudutAngel <= -25)
  {
    sudutA [2] = 0;
  }
  else if (sudutAngel > -25 && sudutAngel <= -20)
  {
    sudutA [2] = (sudutAngel - (-25)) / ((-20) - (-25));
  }
  else if (sudutAngel > -20 && sudutAngel <= 15)
  {
    sudutA [2] = ((15) - sudutAngel) / ((15) - (-20));
  }
  else
  {
    sudutA [2] = 0;
  }
// untuk sudut kiri 15
  if (sudutAngel <= -20)
  {
    sudutA [3] = 0;
  }
  else if (sudutAngel > -20 && sudutAngel <= -15)
  {
    sudutA [3] = (sudutAngel - (-20)) / ((-15) - (-20));
  }
  else if (sudutAngel > -15 && sudutAngel <= 10)
  {
    sudutA [3] = ((10) - sudutAngel) / ((10) - (-15));
  }
  else
  {
    sudutA [3] = 0;
  }
  // untuk sudut kiri 10
  if (sudutAngel <= -15)
  {
    sudutA [4] = 0;
  }
  else if (sudutAngel > -15 && sudutAngel <= -10)
  {
    sudutA [4] = (sudutAngel - (-15)) / ((-10) - (-15));
  }
  else if (sudutAngel > -5 && sudutAngel <= -10)
  {
    sudutA [4] = ((-5) - sudutAngel) / ((-5) - (-10));
  }
  else
  {
    sudutA [4] = 0;
  }
  // untuk sudut kiri 5
  if (sudutAngel <= -10)
  {
    sudutA [5] = 0;
  }
  else if (sudutAngel > -10 && sudutAngel <= -5)
  {
    sudutA [5] = (sudutAngel - (-10)) / ((-5) - (-10));
  }
  else if (sudutAngel > -5 && sudutAngel <= 0)
  {
    sudutA [5] = ((0) - sudutAngel) / ((0) - (-5));
  }
  else
  {
    sudutA [5] = 0;
  }
  // untuk sudut tengah
  if (sudutAngel <= -5)
  {
    sudutA [6] = 0;
  }
  else if (sudutAngel > -5 && sudutAngel <= 0)
  {
    sudutA [6] = (sudutAngel - (-5)) / ((0) - (-5));
  }
  else if (sudutAngel > 0 && sudutAngel <= 5)
  {
    sudutA [6] = (5 - sudutAngel) / (5 - 0);
  }
  else
  {
    sudutA [6] = 0;
  }

// untuk posisi kanan 5
  if (sudutAngel <= 0)
  {
    sudutA [7] = 0;
  }
  else if (sudutAngel > 0 && sudutAngel <= 5)
  {
    sudutA [7] = (sudutAngel - 0) / (5 - 0);
  }
  else if (sudutAngel > 5 && sudutAngel <= 10)
  {
    sudutA [7] = ((10) - sudutAngel) / ((10) - (5));
  }
  else
  {
    sudutA [7] = 0;
  }
  // untuk posisi kanan 10
  if (sudutAngel <= 5)
  {
    sudutA [8] = 0;
  }
  else if (sudutAngel > 5 && sudutAngel <= 10)
  {
    sudutA [8] = (sudutAngel - 5) / (10 - 5);
  }
  else if (sudutAngel > 10 && sudutAngel <= 15)
  {
    sudutA [8] = ((15) - sudutAngel) / ((15) - (10));
  }
  else
  {
    sudutA [8] = 0;
  }

  // untuk posisi kanan 15
  if (sudutAngel <= 10)
  {
    sudutA [9] = 0;
  }
  else if (sudutAngel > 10 && sudutAngel <= 15)
  {
    sudutA [9] = (sudutAngel - 10) / (15 - 10);
  }
  else if (sudutAngel > 15 && sudutAngel <= 20)
  {
    sudutA [9] = ((20) - sudutAngel) / ((20) - (15));
  }
  else
  {
    sudutA [9] = 0;
  }
  // untuk posisi kanan 20
  if (sudutAngel <= 15)
  {
    sudutA [10] = 0;
  }
  else if (sudutAngel > 15 && sudutAngel <= 20)
  {
    sudutA [10] = (sudutAngel - 15) / (20 - 15);
  }
  else if (sudutAngel > 20 && sudutAngel <= 25)
  {
    sudutA [10] = ((25) - sudutAngel) / ((25) - (20));
  }
  else
  {
    sudutA [10] = 0;
  }
   // untuk posisi kanan 25
  if (sudutAngel <= 20)
  {
    sudutA [11] = 0;
  }
  else if (sudutAngel > 20 && sudutAngel <= 25)
  {
    sudutA [11] = (sudutAngel - 20) / (25 - 20);
  }
  else if (sudutAngel > 25 && sudutAngel <= 30)
  {
    sudutA [11] = ((30) - sudutAngel) / ((30) - (25));
  }
  else
  {
    sudutA [11] = 0;
  }

  // untuk posisi kanan 30
  if (sudutAngel <= 25)
  {
    sudutA [12] = 0;
  }
  else if (sudutAngel > 25 && sudutAngel <= 30)
  {
    sudutA [12] = (sudutAngel - 25) / (30 - 25);
  }
  else if (sudutAngel > 30 )
  {
    sudutA [12] = (0);
  }
  
}
void RuleEva () {
  for ( int j = 0; j <= 12; j++)
  {
    temp = sudutA[j];
    rule [j] = temp;

  }
  rule00 = rule [0]; 
  rule01 = rule [1]; 
  rule02 = rule [2]; 
  rule03 = rule [3]; 
  rule04 = rule [4]; 
  rule05 = rule [5]; 
  rule06 = rule [6]; 
  rule07 = rule [7]; 
  rule08 = rule [8]; 
  rule09 = rule [9]; 
  rule10 = rule [10]; 
  rule11 = rule [11]; 
  rule12 = rule [12]; 

}
void Defuzzy () {
  // metode sugeno (weighted average)
  float kiri30 = -180;
  float kiri25 = -150;
  float kiri20 = -120;
  float kiri15 = -90;
  float kiri10 = -60;
  float kiri5 = -30;
  float diam = 0;
  float kanan5 = 30;
  float kanan10 = 60;
  float kanan15 = 90;
  float kanan20 = 120;
  float kanan25 = 150;
  float kanan30 = 180;
  
  RuleEva();
  output = (rule00 * kiri30) + (rule01 * kiri25) + (rule02 * kiri20)+ (rule03 * kiri15)+ (rule04 * kiri10)+ (rule05 * kiri5) + (rule06 * diam) + (rule07 * kanan5) + (rule08 * kanan10) + (rule09 * kanan15)+ (rule10 * kanan20)+ (rule11 * kanan25)+ (rule12 * kanan30);
  //  Serial.println(output);
  defuz = 0;

  for ( int j = 0; j <= 12; j = j + 1)
  {
    defuz = defuz + rule[j];
  }

  //    Serial.println(defuz);
  output = output / defuz;
  
}
