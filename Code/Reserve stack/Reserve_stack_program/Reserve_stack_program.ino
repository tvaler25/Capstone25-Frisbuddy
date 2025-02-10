#include <Servo.h>

Servo servoGate; Servo servoLinkage;

int servoGatePin = 5; int servoLinkagePin = 11;

int val;

void setup() {
  // put your setup code here, to run once:

  servoGate.attach(servoGatePin);
  servoLinkage.attach(servoLinkagePin);
  servoGate.write(0);
  servoLinkage.write(0);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()){
  val = Serial.read();

if (val == 'h'){
  for (int angle = 0; angle <= 80; angle += 1) {  // rotate slowly from 0 degrees to 120 degrees, one by one degree
    // in steps of 1 degree
    servoLinkage.write(angle);  // control servo to go to position in variable 'angle'
    delay(10);         // waits 10ms for the servo to reach the position
  }
}

if (val == 'l'){
  for (int angle = 80; angle >= 0; angle -= 1) {  // rotate from 120 degrees to 0 degrees, one by one degree
    servoLinkage.write(angle);                        // control servo to go to position in variable 'angle'
    delay(10);                               // waits 10ms for the servo to reach the position
  }
  delay(10000);
  }
}
}