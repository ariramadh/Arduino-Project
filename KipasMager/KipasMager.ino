// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // Pin Digital Sensor
#define RELAYPIN 8   // Pin Digital Relay

#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  Serial.print(F("Relay: "));
  if (t > 32){
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("Nyala");
  }else{
    digitalWrite(RELAYPIN, LOW);
    Serial.println("Mati");
  }
}
