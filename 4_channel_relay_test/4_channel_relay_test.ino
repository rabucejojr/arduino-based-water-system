#define PIN_RELAY_1  2 // the Arduino pin, which connects to the IN1 pin of relay module
#define PIN_RELAY_2  3 // the Arduino pin, which connects to the IN2 pin of relay module
#define PIN_RELAY_3  4 // the Arduino pin, which connects to the IN3 pin of relay module
#define PIN_RELAY_4  5 // the Arduino pin, which connects to the IN4 pin of relay module
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin as an output.
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_RELAY_3, OUTPUT);
  pinMode(PIN_RELAY_4, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
  Serial.println("Turn on all");
  digitalWrite(PIN_RELAY_1, HIGH);
  digitalWrite(PIN_RELAY_2, HIGH);
  digitalWrite(PIN_RELAY_3, HIGH);
  digitalWrite(PIN_RELAY_4, HIGH);
  delay(1000);
  Serial.println("Turn off all");
  digitalWrite(PIN_RELAY_1, LOW);
  digitalWrite(PIN_RELAY_2, LOW);
  digitalWrite(PIN_RELAY_3, LOW);
  digitalWrite(PIN_RELAY_4, LOW);
  delay(1000);
}