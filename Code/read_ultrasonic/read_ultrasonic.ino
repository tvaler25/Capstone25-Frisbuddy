// Define the pins for the ultrasonic sensor
const int trigPin = 9; // Trig pin of the ultrasonic sensor
const int echoPin = 10; // Echo pin of the ultrasonic sensor

// Variables for the duration of the pulse and the distance
long duration;
int distance_cm;


void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication
  Serial.begin(9600);

  // Define pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
// Clear the trigPin by setting it LOW for 2 microseconds
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the duration of the echo pulse from the echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  distance_cm = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  // Wait before taking the next measurement
  delay(500); // Adjust delay as needed for your application
}
