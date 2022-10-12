#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;

float humidity;
float temperature;

void setup_sensor()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);

  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
  // use this instead: 
  dht.setup(14, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
}

void bacaSensor()
{
  delay(dht.getMinimumSamplingPeriod());
}

float bacaKelembaban(){
  humidity = dht.getHumidity();
  Serial.print(humidity, 1);
  Serial.print("\t");
  return humidity;
}
 
float bacaSuhu(){
  temperature = dht.getTemperature();
  Serial.println(temperature, 1);
  return temperature;
}