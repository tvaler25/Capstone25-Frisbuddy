#include <Arduino.h>
#include <Servo.h>
//#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library
#include <ezButton.h>

int PWMPin = 5; int DirPin = 4;
int power = 100;
int val;

int aimerIn1Pin = 12; int aimerIn2Pin = 13;
int aimerEnablePin = A5;

enum Position {LEFT, MIDDLE, RIGHT};
Position currentPosition = MIDDLE;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PWMPin, OUTPUT);
  pinMode(DirPin, OUTPUT);

  pinMode(aimerIn1Pin, OUTPUT); pinMode(aimerIn2Pin, OUTPUT); //aimer
    pinMode(aimerEnablePin, OUTPUT);

  
  digitalWrite(DirPin, HIGH);
  analogWrite(PWMPin, power);
  Serial.print(power);

  digitalWrite(aimerEnablePin, HIGH);
  moveMotor(1);
  delay(1400);
  moveMotor(0);
  delay(700);
  stopMotor();
  digitalWrite(aimerEnablePin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    val = Serial.read();

    if(val == '-') {
      power = power - 5;
      if(power<0) {
        power=0; 
      }
      Serial.println(power);
      analogWrite(PWMPin, power);
    }

    if(val == '+') {
      power = power + 5;
      if(power>255) {
        power=255; 
      }
      Serial.println(power);
      analogWrite(PWMPin, power);
    }


  }
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