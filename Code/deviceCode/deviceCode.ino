//still need to do:
//implement IR remote into code
//make sure reorienter code is up to date (delays too long?)
//make sure all analog pins are working properly (is int A0/1/2/3 illegal?)

#include <Arduino.h>
#include <Servo.h>
//#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library
#include <ezButton.h>

//---------------------------------------PIN NUMBERS & OBJECTS----------------------------------

//Set digital pin numbers
int motorDirPin = 4; int motorPWMPin = 5;
int reorienterTriggerPin = 2; int reorienterEchoRightPin = 3; int reorienterEchoLeftPin = 6;
int reorienterServoRightPin = 8; int reorienterServoLeftPin = 7;
int pusherServoPin = 9;
int reserveGateServoPin = 10; int reserveLifterServoPin = 11;
int aimerIn1Pin = 12; int aimerIn2Pin = 13;

//set analog pin numbers
int aimerLeftSignalPin = A0; int aimerRightSignalPin = A1;
int buzzerPin = A3;
int IRReceiverPin = A4;
int aimerEnablePin = A5;

//Initialize objects
Servo servoLeft; Servo servoRight;
Servo servoPusher;
Servo servoGate; Servo servoLifter;

ezButton limitSwitch1(aimerLeftSignalPin); ezButton limitSwitch2(aimerRightSignalPin);

//------------------------------------------------CONSTANTS---------------------------------------

//Motor/H-Bridge constants:
int motorPower = 165;

//Reorienter constants:
const int ClearValRight = 0; //servo positions
const int downValRight = 70;
const int PushValLeft = 0;  
const int downValLeft = 60;
const int ClearValLeft = 180;
const int PushValRight = 160;

long duration_right, duration_left; //initialize sensor variables
long distance_mm_right, distance_mm_left;

long filteredDistanceRight = 0; //filter settings
long filteredDistanceLeft = 0;
const int numSamples = 5; // Number of readings to average

int rightDistanceBound = 37; int leftDistanceBound = 33;

bool signalReceived = false;

bool dispense = false;//variable for keyboard input to activate reserve 

// Pusher constants: 
int pusherBack = 5; int pusherForward = 82;

//Reserve stack constants:
int lifterDown = 10; int lifterUp = 70;
int gateDown = 70; int gateUp = 10;

//Aimer constants:
enum Position {LEFT, MIDDLE, RIGHT};
Position currentPosition = MIDDLE;

//IR Receiver/buzzer constants:

//--------------------------------------------SETUP-------------------------------------------------

void setup() {

  Serial.begin(9600);
  delay(2000);

  //Set pin modes or attach servos
  pinMode(motorDirPin, OUTPUT); pinMode(motorPWMPin, OUTPUT); //motor
  pinMode(reorienterTriggerPin, OUTPUT); pinMode(reorienterEchoLeftPin, INPUT); pinMode(reorienterEchoRightPin, INPUT); //reserve stack 
  servoRight.attach(reorienterServoRightPin); servoLeft.attach(reorienterServoLeftPin);
  servoPusher.attach(pusherServoPin); //pusher
  servoPusher.write(pusherBack);
  servoGate.attach(reserveGateServoPin); servoLifter.attach(reserveLifterServoPin); //reserve stack
  pinMode(aimerIn1Pin, OUTPUT); pinMode(aimerIn2Pin, OUTPUT); //aimer

  pinMode(aimerLeftSignalPin, INPUT); pinMode(aimerRightSignalPin, INPUT);
  pinMode(buzzerPin, OUTPUT); //buzzer
  pinMode(IRReceiverPin, INPUT_PULLUP); //IR signal
  pinMode(aimerEnablePin, OUTPUT);

  //Write default values to pins
  digitalWrite(aimerEnablePin, HIGH);
  moveMotor(true); //move to left
  delay(1600);
  stopMotor();
  moveMotor(false); //move to right
  delay(600);
  //delay(1500);
  //stopMotor();
  //digitalWrite(aimerEnablePin, LOW);


  digitalWrite(motorDirPin, HIGH); analogWrite(motorPWMPin, motorPower);
  servoLeft.write(downValLeft); servoRight.write(downValRight);
  servoPusher.write(pusherBack);
  servoGate.write(gateDown); servoLifter.write(lifterDown); //reserve stack

  IrReceiver.begin(IRReceiverPin, ENABLE_LED_FEEDBACK);
  
  Serial.println("Enter L, R, or C to set aimer position:");
  
  bool positionSet = false;
  while (!positionSet) {
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'L' || input == 'R' || input == 'C') {
        switch (input) {
          case 'L':
            moveMotor(true); //move to left
            delay(1600);
            break;
          case 'R':
            moveMotor(false); //move to right
            delay(1000);
            break;
          case 'C':
            break;
        }
        positionSet = true;
      }
    }
  }

}

//--------------------------------------------MAIN LOOP--------------------------------------------------------

void loop() {
  
  //wait for signal from either reorienter or remote
  do {
    //Get filtered distance readings
    filteredDistanceRight = getFilteredDistance(reorienterEchoRightPin);
    filteredDistanceLeft = getFilteredDistance(reorienterEchoLeftPin);

     
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'D') {
        dispense=true;
        break;  // Exit the loop if 'd' is pressed
      }
    }

    delay(200); //check every 200 ms
  }
  while(filteredDistanceRight > rightDistanceBound && filteredDistanceLeft > leftDistanceBound && dispense == false);
  
  //reorient right
  if(filteredDistanceRight <= rightDistanceBound) {
    Serial.println("Disc sensed right");
    delay(200);
    servoLeft.write(ClearValLeft);
    delay(500);
    servoLeft.write(downValLeft);
    delay(1500);
    
  }

  //reorient left
  else if(filteredDistanceLeft <= leftDistanceBound) {
    //Serial.println("Left sensor tirggered ");
    Serial.println("Disc sensed left");
    delay(200);
    servoRight.write(ClearValRight);
    servoLeft.write(downValLeft+10);
    delay(500);
    servoLeft.write(PushValLeft);
    delay(1000);
    servoLeft.write(downValLeft);
    servoRight.write(downValRight);
    delay(1500);
  }

  //pull from reserve stack
  else if(dispense == true) {
    Serial.println("IR pressed");
    servoLifter.write(lifterUp); //lift discs up
    delay(1000);
    servoGate.write(gateUp); //open gate
    delay(1500);
    servoGate.write(gateDown); //close gate
    delay(1000);
    servoLifter.write(lifterDown);//lower linkage
    delay(1500);
    
  }

  //delay to let disc settle, then push disc into wheel, then give warning with buzzer
  
  tone(buzzerPin, 750);   
  for (int pos = pusherBack; pos <= pusherForward; pos += 1) { // Increase angle in small steps
    servoPusher.write(pos);
    delay(35);  // Small delay for smoother motion
  } 
  noTone(buzzerPin); 
  delay(1000);
  for (int pos = pusherForward; pos >= pusherBack; pos -= 1) { // Decrease angle in small steps
    servoPusher.write(pos);
    delay(35);
  }
  

  //aim
  delay(2000); //wait for disc to pass the aimer
  digitalWrite(aimerEnablePin, HIGH);

  //int aimerAngle = (random(3) - 1) * 30; //random between -30, 0, 30
  
  int aimerAngle = 0;
  
  switch (aimerAngle) {
    case -30:
      moveToPosition(LEFT);
      break;
    case 0:
      moveToPosition(MIDDLE);
      break;
    case 30:
      moveToPosition(RIGHT);
      break;
  }
  digitalWrite(aimerEnablePin, LOW);
  //Serial.println("Loop complete");

  IrReceiver.resume();
}

//----------------------------------------ADDITIONAL FUNCTIONS------------------------------------------

// Function to get a filtered distance using multiple samples
long getFilteredDistance(int echoPin) {
  long sum = 0;
  long validSamples = 0;
  long previousReading = 0;

  for (int i = 0; i < numSamples; i++) {
    long distance = getDistance(echoPin);
    
    // Ignore outlier readings (change threshold as needed)
    if (i == 0 || abs(distance - previousReading) < 50) {
      sum += distance;
      validSamples++;
    }
    
    previousReading = distance;
    delay(10); // Small delay between samples
  }

  return validSamples > 0 ? sum / validSamples : previousReading; // Return average distance
}

// Function to get a single distance reading
long getDistance(int echoPin) {
  digitalWrite(reorienterTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(reorienterTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(reorienterTriggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.34 / 2.0; // Convert to mm
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
    if(currentPosition == MIDDLE) {
      delay(700);
    }
    else if(currentPosition == RIGHT) {
      delay(1400);
    }
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
    if(currentPosition == MIDDLE) {
      delay(750);
    }
    else if(currentPosition == LEFT) {
      delay(1400);
    }
    //  while (!limitSwitch2.isPressed()) {
    //    // Wait until the right limit switch is pressed
    //          if (limitSwitch2.isPressed()) {
    //   Serial.println(F("The limit switch 2: TOUCHED"));
    //   }
    // }
    stopMotor();
    Serial.println("Reached RIGHT position.");
    currentPosition = RIGHT;
  } 
  else if (targetPosition == MIDDLE) {
    // Move to middle position after interacting with the end switch
    Serial.println("Moving to MIDDLE...");
    if (currentPosition == LEFT) {  
      moveMotor(false);  // Move motor to the right from left  
      delay(600);
    } 
    else if (currentPosition == RIGHT) {
      moveMotor(true);  // Move motor to the left from right
      delay(650);
    }

    stopMotor();  
    Serial.println("Reached MIDDLE position.");
    currentPosition = MIDDLE;
  }
}

void moveMotor(bool direction) {
  if (direction) {
    // Motor moving left (Clockwise)
    digitalWrite(aimerIn1Pin, HIGH);
    digitalWrite(aimerIn2Pin, LOW);
  } else {
    // Motor moving right (Counter-clockwise)
    digitalWrite(aimerIn1Pin, LOW);
    digitalWrite(aimerIn2Pin, HIGH);
  }
}

void stopMotor() {
  // Stop the motor
  digitalWrite(aimerIn1Pin, LOW);
  digitalWrite(aimerIn2Pin, LOW);
}
