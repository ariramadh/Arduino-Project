/** 
 * The main parameters to configure this sketch accordingly to your hardware setup are:
 *  - syncPin, that is the pin listening to AC zero cross signal
 *  - light, the pin which is connected to the thyristor
 */ 

#include "dimmable_light.h"

const int syncPin = 7;
const int thyristorPin = 5;

DimmableLight light(thyristorPin);

// Delay between a brightness changement in millisecond
int period = 100;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println("Dimmable Light for Arduino: first example");
  
  Serial.print("Init the dimmable light class... ");
  DimmableLight::setSyncPin(syncPin);
  // VERY IMPORTANT: Call this method to start internal light routine
  DimmableLight::begin();
  Serial.println("Done!");
}

void loop() {
  int input = analogRead(A5);
  int value = map(input, 0, 1024, 0, 255);
  Serial.print(input);
  Serial.print("\t");
  Serial.println(value);
  light.setBrightness(value);
  delay(period);
}
