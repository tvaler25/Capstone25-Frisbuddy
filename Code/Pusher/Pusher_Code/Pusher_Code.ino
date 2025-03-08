#include <Servo.h>

Servo pusherservo;

const int pusherPin = 1;

void setup() {
  pinMode(pusherPin, OUTPUT);
  pusherservo.attach(pusherPin);
}

void loop() {
  // Move from 0 to 120 degrees slowly
  for (int pos = 30; pos <= 150; pos += 1) { // Increase angle in small steps
    pusherservo.write(pos);
    delay(20);  // Small delay for smoother motion
  }
  
  delay(1000); // Pause at 140 degrees

  // Move back from 140 to 0 degrees slowly
  for (int pos = 150; pos >= 30; pos -= 1) { 
    pusherservo.write(pos);
    delay(20);
  }

  delay(1000); // Pause at 0 degrees
}
