/*
Blink an external LED
*/
// ledPin refers to ESP32 GPIO 23
const int ledPin = 23;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin ledPin as an output.
  pinMode(ledPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledPin, HIGH);
  // turn the LED on (HIGH is the voltage level)
  delay(500);
  // wait for a second
  digitalWrite(ledPin, LOW);
  // turn the LED off by making the voltage LOW
  delay(500);
  // wait for a second
}