#include <Servo.h>

Servo pusherservo;

const int pusherPin = 7;
const int extendedPos = 5;
const int retractedPos = 90;

void setup() {
  pinMode(pusherPin, OUTPUT);
  pusherservo.attach(pusherPin);
}

void loop() {
  // Move from extendedPos to retractedPos

  for (int pos = extendedPos; pos <= retractedPos; pos += 1) { // Increase angle in small steps
    pusherservo.write(pos);
<<<<<<< Updated upstream
    delay(40);  // Speed control
=======
    delay(1);  // Small delay for smoother motion
>>>>>>> Stashed changes
  }
  
  delay(1000); // Pause at 140 degrees

  // Move back from 90 to 0 degrees slowly
  for (int pos = retractedPos; pos >= extendedPos; pos -= 1) { 
    pusherservo.write(pos);
<<<<<<< Updated upstream
    delay(40); // Speed control
=======
    delay(1);
>>>>>>> Stashed changes
  }

  delay(1000); // Pause after each cycle
}
