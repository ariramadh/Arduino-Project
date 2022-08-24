#include <SoftwareSerial.h>
SoftwareSerial HC12(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HC12.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    HC12.write(Serial.read());
  }
  if (HC12.available()) {
    Serial.write(HC12.read());
  }
}
