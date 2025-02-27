#include <ezButton.h>

// Define the pins for limit switches (using analog pins A0 and A1)
const int limitSwitch1Pin = 4;  // Limit switch 1 connected to A0
const int limitSwitch2Pin = 3;  // Limit switch 2 connected to A1

// Create ezButton objects for each limit switch
ezButton limitSwitch1(limitSwitch1Pin);  // Initialize ezButton object for limit switch 1
ezButton limitSwitch2(limitSwitch2Pin);  // Initialize ezButton object for limit switch 2

void setup() {
  // Start serial communication
  Serial.begin(9600);

}

void loop() {
  // Update the button states
  limitSwitch1.loop();
  limitSwitch2.loop();

  // Output the state of the switches
  Serial.print("Switch 1 (A0) state: ");
  if (limitSwitch1.isPressed()) {
    Serial.println("Pressed");
  } else {
    Serial.println("Not Pressed");
  }

  Serial.print("Switch 2 (A1) state: ");
  if (limitSwitch2.isPressed()) {
    Serial.println("Pressed");
  } else {
    Serial.println("Not Pressed");
  }

  // Add a small delay for readability
  delay(500);
}
