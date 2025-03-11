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
int buzzerPin = A2;
int IRReceiverPin = A3;
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
const int downValRight = 73;
const int PushValLeft = 30;  
const int downValLeft = 60;
const int ClearValLeft = 180;
const int PushValRight = 160;

long duration_right, duration_left; //initialize sensor variables
long distance_mm_right, distance_mm_left;

long filteredDistanceRight = 0; //filter settings
long filteredDistanceLeft = 0;
const int numSamples = 5; // Number of readings to average

int rightDistanceBound = 40; int leftDistanceBound = 35;



// Pusher constants: 
int pusherBack = 5; int pusherForward = 90;

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
  pinMode(IRReceiverPin, INPUT); //IR signal
  pinMode(aimerEnablePin, OUTPUT);

  //Write default values to pins
  digitalWrite(motorDirPin, HIGH); analogWrite(motorPWMPin, motorPower);
  servoLeft.write(downValLeft); servoRight.write(downValRight);
  servoPusher.write(pusherBack);
  servoGate.write(gateDown); servoLifter.write(lifterDown); //reserve stack

  digitalWrite(aimerEnablePin, HIGH);
  moveMotor(1);
  delay(1500);
  moveMotor(0);
  delay(600);
  stopMotor();
  digitalWrite(aimerEnablePin, LOW);

}

//--------------------------------------------MAIN LOOP--------------------------------------------------------

void loop() {
  
  //wait for signal from either reorienter or remote
  do {
    //Get filtered distance readings
    filteredDistanceRight = getFilteredDistance(reorienterEchoRightPin);
    filteredDistanceLeft = getFilteredDistance(reorienterEchoLeftPin);
    delay(200); //check every 100 ms
  }
  while(filteredDistanceRight > rightDistanceBound && filteredDistanceLeft > leftDistanceBound);
  Serial.println("Disc sensed");
  
  //reorient right
  if(filteredDistanceRight <= rightDistanceBound) {
    
    delay(200);
    servoLeft.write(ClearValLeft);
    delay(500);
    servoLeft.write(downValLeft);
  }

  //reorient left
  else if(filteredDistanceLeft <= leftDistanceBound) {
    //Serial.println("Left sensor tirggered ");
    delay(200);
    servoRight.write(ClearValRight);
    delay(500);
    servoLeft.write(PushValLeft);
    delay(1000);
    servoLeft.write(downValLeft);
    servoRight.write(downValRight);
  }

  //pull from reserve stack
  /*else if(IrReceiver.decodedIRData.command == 0xC) {
    servoLifter.write(lifterUp); //lift discs up
    delay(1000);
    servoGate.write(gateUp); //open gate
    delay(1500);
    servoGate.write(gateDown); //close gate
    delay(1000);
    servoLifter.write(lifterDown);//lower linkage
  }*/

  //push disc into wheel, give warning with buzzer
  delay(1500);
  tone(buzzerPin, 750);   
  for (int pos = pusherBack; pos <= pusherForward; pos += 1) { // Increase angle in small steps
    servoPusher.write(pos);
    delay(50);  // Small delay for smoother motion
  } 
  noTone(buzzerPin); 
  delay(1000);
  for (int pos = pusherForward; pos >= pusherBack; pos -= 1) { // Decrease angle in small steps
    servoPusher.write(pos);
    delay(50);
  }
  

  //aim
  //delay(2000); //wait for disc to pass the aimer
  digitalWrite(aimerEnablePin, HIGH);

  int aimerAngle = (random(3) - 1) * 30; //random between -30, 0, 30
  Serial.println(aimerAngle);
  //int aimerAngle = 0; int nextAngle = -30;
  
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
    //Serial.println("Motor is already in the desired position.");
    return;
  }
  

  // Move towards the target position
  if (targetPosition == LEFT) {
    Serial.println("Moving to LEFT...");
    moveMotor(true);  // Move motor to the left
    delay(1200);
    /*while (!limitSwitch1.isPressed()) {
       // Wait until the left limit switch is pressed
           if (limitSwitch1.isPressed()) {
      Serial.println(F("The limit switch 1: TOUCHED"));
      }
    }*/
    stopMotor();
    //Serial.println("Reached LEFT position.");
    currentPosition = LEFT;
  } 
  else if (targetPosition == RIGHT) {
    Serial.println("Moving to RIGHT...");
    moveMotor(false);  // Move motor to the right
    delay(1200);
    /*while (!limitSwitch2.isPressed()) {
      // Wait until the right limit switch is pressed
      if (limitSwitch2.isPressed()) {
         Serial.println(F("The limit switch 2: TOUCHED"));
      }
    }*/
    stopMotor();
    //Serial.println("Reached RIGHT position.");
    currentPosition = RIGHT;
  } 
  else if (targetPosition == MIDDLE) {
    // Move to middle position after interacting with the end switch
    Serial.println("Moving to MIDDLE...");
    if (currentPosition == LEFT) {
      moveMotor(false);  // Move motor to the right from left
    } 
    else if (currentPosition == RIGHT) {
      moveMotor(true);  // Move motor to the left from right
    }
    delay(600);  // Move for 2 seconds to find the middle
      stopMotor();  // Stop motor after 0.5 seconds
      //Serial.println("Reached MIDDLE position.");
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