#include <SPI.h>
#include "RF24.h"
RF24 radio(10, 9);//ce,csn
int dataReceived;
const uint64_t pipe = 0x123456789ABC;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(110);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

}

void loop() {
  // put your main code here, to run repeatedly:
  if ( radio.available())
  {
    while (radio.available())
    {
      radio.read( &dataReceived, sizeof(dataReceived) );
    }
    radio.stopListening();
    Serial.print("Data received = ");
    Serial.println(dataReceived);
      
  }
}
