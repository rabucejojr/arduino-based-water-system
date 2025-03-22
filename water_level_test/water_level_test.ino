// //Arduino water level sensor code
// // Sensor pins pin D6 LED output, pin A0 analog Input
// #define ledPin 6
// #define sensorPin A0
// void setup() {
//   Serial.begin(9600);
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);
// }
// void loop()
// {
//   unsigned int sensorValue = analogRead(sensorPin);
//   if (sensorValue < 540)
//     return;
//   uint8_t outputValue = map(sensorValue, 540, 800, 0, 255);
//   Serial.print(sensorValue);
//   Serial.print(" ");
//   Serial.println(outputValue);
//   analogWrite(ledPin, outputValue); // generate PWM signal
// }

//Arduino water level sensor code
// Sensor pins pin D6 LED output, pin A0 analog Input
#define sensorPin A0
#define lowOutput  2
#define mediumOutput  3
#define highOutput  4
void setup() {
  Serial.begin(9600);
  pinMode(lowOutput , OUTPUT);
  pinMode(mediumOutput , OUTPUT);
  pinMode(highOutput , OUTPUT);
  digitalWrite(lowOutput, LOW);
  digitalWrite(mediumOutput, LOW);
  digitalWrite(highOutput, LOW);
}
void loop() {
  int sensorValue = analogRead(sensorPin);
  if (280 > sensorValue) {
    Serial.println(sensorValue);
  }  else if (564 > sensorValue) {
    Serial.println(sensorValue);
  } else if (640 > sensorValue) {
    Serial.println(sensorValue);
  } else {
    Serial.println(sensorValue);
  }
  delay(1000);
}