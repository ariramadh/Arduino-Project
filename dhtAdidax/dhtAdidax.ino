#include <dht11.h>

dht11 DHT11;
#define DHT11PIN 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT11.read(DHT11PIN);
 
  Serial.print("Temp: ");
  Serial.print((float)DHT11.temperature, 2);
  Serial.print(" C\t");
  
  Serial.print("RelF: ");
  Serial.print((float)DHT11.humidity, 2);
  Serial.println(" %");
  
  delay(2000);
}
