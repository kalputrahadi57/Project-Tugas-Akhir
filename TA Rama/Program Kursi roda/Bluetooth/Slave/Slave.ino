#define ledPin 9
char state = ' ';
int potValue = 0;
int count;
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}
void loop() {
 if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
    
 }
 // Controlling the LED
 if (state == '1') {
  digitalWrite(ledPin, HIGH); // LED ON
//  Serial.println("Lampu Hidup");
  state = 0;
 }
 else if (state == '0') {
  digitalWrite(ledPin, LOW); // LED ON
//  Serial.println("Lampu Mati");
  state = 0;
 }
 // Reading the potentiometer
 count++;
 Serial.write(49);
 delay(100);
}
