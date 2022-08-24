#include <SPI.h>
#include "RF24.h"
RF24 radio(10, 9);
int timer;
int dataTransmitted;
const uint64_t pipe = 0x123456789ABC;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(110);
  radio.openWritingPipe(pipe);
  radio.powerUp();
}

void loop() {
  // put your main code here, to run repeatedly:

  dataTransmitted = 100;
  radio.write( &dataTransmitted, sizeof(dataTransmitted) );
  delay(1000);
  Serial.println(dataTransmitted);
}
