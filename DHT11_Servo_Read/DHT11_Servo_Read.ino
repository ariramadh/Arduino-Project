/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservoIn;  // create servo object to control a servo
Servo myservoOut;  // create servo object to control a servo

String in;
int val = 0;
int count;
int potPin = 0;  // analog pin used to connect the potentiometer
int pot;

bool kondisi = false;

void setup() {
  myservoIn.attach(9);  // attaches the servo on pin 9 to the servo object
  myservoOut.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  myservoIn.write(0);
  myservoOut.write(7);
}

void loop() {
  if (Serial.available()) {
    in = Serial.readString();
    val = in.substring(0, in.indexOf('*')).toInt();
  }

  pot = analogRead(potPin);

  digitalWrite(LED_BUILTIN, kondisi);

  Serial.println(pot);

  int In, Out;

  if (pot > 1000) {
    In = 46;      Out = 74;
    kondisi = false;
  } else if (pot > 500) {
    In = map(pot,501,1000,0,46);   Out = map(pot,501,1000,4,74);
    kondisi = false;
  } else if (pot > 300) {
    In = 0;   Out = 7;
    kondisi = false;
  }else{
    kondisi = true;
  }
  
  if (kondisi) {
    if (val == 10) {
      In = 14;        Out = 27;
    } else if (val == 20) {
      In = 21;        Out = 36;
    } else if (val == 30) {
      In = 27;        Out = 43;
    } else if (val == 40) {
      In = 32;        Out = 49;
    } else if (val == 50) {
      In = 35;        Out = 57;
    } else if (val == 60) {
      In = 39;        Out = 64;
    } else if (val == 0) {
      In = 0;         Out = 7;
    }
  }

  myservoIn.write(In);
  myservoOut.write(Out);

  delay(15);
}
