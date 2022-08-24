#include <HCSR04.h>
#include <Servo.h>

UltraSonicDistanceSensor distanceSensor(52, 53);  // Initialize sensor that uses digital pins 13 and 12.

int dis_depan;

Servo ser1;  // create servo object to control a servo
Servo ser2;  // create servo object to control a servo
Servo ser3;  // create servo object to control a servo
Servo ser4;  // create servo object to control a servo

int serA = 0;
int serB = 180;
int serC = 90;

bool geser = false;
bool turun = false;
bool capit = false;

void setup () {
  Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  ser1.attach(8);  // attaches the servo on pin 9 to the servo object
  ser2.attach(9);  // attaches the servo on pin 9 to the servo object
  ser3.attach(10);  // attaches the servo on pin 9 to the servo object
  ser4.attach(11);  // attaches the servo on pin 9 to the servo object
}

void loop () {
  // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
  dis_depan = distanceSensor.measureDistanceCm();
  Serial.println(dis_depan);

  if (dis_depan < 5) {
    if (!geser) {
      for (int i = serA; i < 150; i++) {
        ser1.write(i);              // tell servo to go to position in variable 'pos'
        delay(15);
        serA = i;
      }
      geser = true;
    }
    if (geser && !turun) {
      for (int i = serB; i > 0; i--) {
        ser2.write(i);              // tell servo to go to position in variable 'pos'
        delay(15);
        serB = i;
      }
      turun = true;
    }
    if (turun && !capit) {
      for (int i = serC; i > 0; i--) {
        ser3.write(i);              // tell servo to go to position in variable 'pos'
        ser4.write(map(i, 0, 90, 90, 0));          // tell servo to go to position in variable 'pos'
        delay(15);
        serC = i;
      }
      capit = true;
    }
    if (capit && turun) {
      for (int i = serB; i < 180; i++) {
        ser2.write(i);              // tell servo to go to position in variable 'pos'
        delay(15);
        serB = i;
      }
      turun = false;
    }
  }
}
