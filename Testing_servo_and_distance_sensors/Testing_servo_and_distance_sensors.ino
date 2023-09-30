#include <Servo.h>
#include <Ultrasonic.h>
Servo servo;
Ultrasonic ultrasonic(17, 16);

void setup() {
  servo.attach(15);
  Serial.begin(9600);
}

void loop() {
  float a = ultrasonic.distanceRead(CM);
  Serial.println(a);
  if(a>50) {
    servo.write(45);
    delay(1000);
    servo.write(90);
    delay(1000);
  }
  delay(1000);
}
