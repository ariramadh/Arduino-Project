#include <Servo.h>

Servo myservoIn;  // create servo object to control a servo
Servo myservoOut;  // create servo object to control a servo


void setup() {
  // put your setup code here, to run once:
  myservoOut.attach(11);  // attaches the servo on pin 9 to the servo object
  myservoIn.attach(10);  // attaches the servo on pin 9 to the servo object

}

void loop() {
  // put your main code here, to run repeatedly:
  myservoIn.write(0);
  myservoOut.write(0);

}
