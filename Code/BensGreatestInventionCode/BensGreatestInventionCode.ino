#include <Servo.h>

Servo servoGate; Servo servoLinkage;

int servoGatePin = 10; int servoLinkagePin = 11;

int val;

void setup() {
  // put your setup code here, to run once:

  servoGate.attach(servoGatePin);
  servoLinkage.attach(servoLinkagePin);
  servoGate.write(70);
  servoLinkage.write(30);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    val = Serial.read();

    if (val == 'l') {
    // for (int angle = 0; angle <= 70; angle += 1) {  // rotate slowly from 0 degrees to 120 degrees, one by one degree
      servoLinkage.write(70);  // control servo to go to position in variable 'angle'
      delay(10);         // waits 10ms for the servo to reach the position
      
    }

    if (val == '.') {
    // for (int angle = 70; angle >= 0; angle -= 1) {  // rotate from 120 degrees to 0 degrees, one by one degree
      servoLinkage.write(30);                        // control servo to go to position in variable 'angle'
      delay(10);                               // waits 10ms for the servo to reach the position
    }

    if (val == 'g') {
    // for (int angle = 0; angle <= 70; angle += 1) {  // rotate slowly from 0 degrees to 120 degrees, one by one degree
      servoGate.write(90);  // control servo to go to position in variable 'angle'
      delay(10);         // waits 10ms for the servo to reach the position
      
    }

    if (val == 'b') {
    // for (int angle = 70; angle >= 0; angle -= 1) {  // rotate from 120 degrees to 0 degrees, one by one degree
      servoGate.write(30);                        // control servo to go to position in variable 'angle'
      delay(10);                               // waits 10ms for the servo to reach the position
    }

    if (val == 'd') {
      servoLinkage.write(70); //lift discs up
      delay(1000);
      servoGate.write(10); //open gate
      delay(1500);
      servoGate.write(70); //close gate
      delay(1000);
      servoLinkage.write(10);//lower linkage

    }
  }

}
