// Pin Definitions
const int limitSwitchPin = 4; // Limit switch pin (digital input)

void setup() {
  // Start the serial communication at 9600 baud rate
  Serial.begin(9600);

   // Initialize limit switch pin as input with internal pull-up resistor
  pinMode(limitSwitchPin, INPUT_PULLUP);

}

void loop() {
  // Read the state of the limit switch (LOW when pressed)
  int limitSwitchState = digitalRead(limitSwitchPin);

  // If the limit switch is HIGH (not pressed), output "Pressed"
  if (limitSwitchState == HIGH) {
    Serial.println("Pressed");

  } 
  // If the limit switch is LOW (pressed), output "Not Pressed"
  else {
    Serial.println("Not Pressed");

  }
  
  // Small delay to prevent bouncing and give time for serial output
  delay(50);
}

