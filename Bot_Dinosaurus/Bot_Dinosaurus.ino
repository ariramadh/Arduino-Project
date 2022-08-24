#include <Servo.h>
Servo myservo;  // create servo object to control a servo

int sensorPin = A0, sensorValue = 0;
int pos = 90;
bool jump;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(sensorPin);
//  Serial.println(sensorValue);

  if (sensorValue > 645) {
    jump = false;              // tell servo to go to position in variable 'pos'
  } else {
    jump = true;              // tell servo to go to position in variable 'pos'
  }

  if(jump){
    myservo.write(90);
    delay(200);
  }else{
    myservo.write(100);
  }
  
  delay(10);
}
