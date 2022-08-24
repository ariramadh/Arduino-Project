#include "DHT.h"

float h = 0; 
float t = 0;

void dhtSetup(int DHTPIN, String DHTTYPE){
  DHT dht(DHTPIN, DHTTYPE);
  dht.begin();  
}

bool bacaDHT(){
  bool bacaDHTEr = true;

  // Baca kelembaban
  h = dht.readHumidity();
  // Baca temperatur celcius
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return bacaDHTEr = false;
    return;
  }
  
  return bacDHTEr;
}

float bacaSuhu(){
  h = dht.readHumidity();

  return h;
}

float bacaKelembaban(){
  t = dht.readTemperature();  

  return t;
}
