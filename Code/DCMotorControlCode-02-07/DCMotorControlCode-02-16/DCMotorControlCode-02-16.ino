#include <ezButton.h>

// Define pins for motor control (L298N)
const int enablePin = A5;  // Enable pin for motor (typically HIGH for enabling)
const int motorPin1 = 12;  // Motor A pin 1
const int motorPin2 = 13;  // Motor A pin 2

// Define pins for limit switches
const int limitSwitch1Pin = A0;  // Limit switch 1 (left side)
const int limitSwitch2Pin = A1;  // Limit switch 2 (right side)

// Initialize the ezButton objects
ezButton limitSwitch1(limitSwitch1Pin);
ezButton limitSwitch2(limitSwitch2Pin);

// Variable to track the current position
enum Position {LEFT, MIDDLE, RIGHT};
Position currentPosition = MIDDLE;

void setup() {
  // Set motor control pins as output
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Set limit switch pins as input
  pinMode(limitSwitch1Pin, INPUT);
  pinMode(limitSwitch2Pin, INPUT);

  limitSwitch1.setDebounceTime(100);  // Reduced debounce time to 10ms
  limitSwitch2.setDebounceTime(100);  // Reduced debounce time to 10ms

  // Initialize motor control
  digitalWrite(enablePin, HIGH);  // Enable the motor

  // Start serial communication
  Serial.begin(9600);
  Serial.println("Motor Control Ready.");
}

void loop() {
limitSwitch1.loop();  // Update limit switch states
limitSwitch2.loop();

  // Check for serial input to move to a position
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch (input) {
      case 'L':
        moveToPosition(LEFT);
        break;
      case 'M':
        moveToPosition(MIDDLE);
        break;
      case 'R':
        moveToPosition(RIGHT);
        break;
      default:
        Serial.println("Invalid input. Use 'L', 'M', or 'R'.");
        break;
    }
  }
}

void moveToPosition(Position targetPosition) {

  if (targetPosition == currentPosition) {
    Serial.println("Motor is already in the desired position.");
    return;
  }

  // Move towards the target position
  if (targetPosition == LEFT) {
    Serial.println("Moving to LEFT...");
    moveMotor(true);  // Move motor to the left
    delay(750);
    // while (!limitSwitch1.isPressed()) {
    //    // Wait until the left limit switch is pressed
    //        if (limitSwitch1.isPressed()) {
    //   Serial.println(F("The limit switch 1: TOUCHED"));
    //   }
    // }
    stopMotor();
    Serial.println("Reached LEFT position.");
    currentPosition = LEFT;
  } else if (targetPosition == RIGHT) {
    Serial.println("Moving to RIGHT...");
    moveMotor(false);  // Move motor to the right
    delay(750);
    //  while (!limitSwitch2.isPressed()) {
    //    // Wait until the right limit switch is pressed
    //          if (limitSwitch2.isPressed()) {
    //   Serial.println(F("The limit switch 2: TOUCHED"));
    //   }
    // }
    stopMotor();
    Serial.println("Reached RIGHT position.");
    currentPosition = RIGHT;
  } else if (targetPosition == MIDDLE) {
    // Move to middle position after interacting with the end switch
    if (currentPosition == LEFT) {
      Serial.println("Moving to MIDDLE...");
      moveMotor(false);  // Move motor to the right from left
      delay(750);  // Move for 2 seconds to find the middle
      stopMotor();  // Stop motor after 0.5 seconds
      Serial.println("Reached MIDDLE position.");
      currentPosition = MIDDLE;
    } else if (currentPosition == RIGHT) {
      Serial.println("Moving to MIDDLE...");
      moveMotor(true);  // Move motor to the left from right
      delay(750);  // Move for 2 seconds to find the middle
      stopMotor();  // Stop motor after 0.5 seconds
      Serial.println("Reached MIDDLE position.");
      currentPosition = MIDDLE;
    }
  }
}

void moveMotor(bool direction) {
  if (direction) {
    // Motor moving left (Clockwise)
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    // Motor moving right (Counter-clockwise)
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  }
}

void stopMotor() {
  // Stop the motor
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}
