#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

#define SENSOR_PIN 4 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.

float suhu;

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void suhuSetup() {
  sensors.begin();
}

float readSuhu() {
  Serial.println("**Baca Suhu**");
  sensors.requestTemperatures();

  if (isnan(sensors.getTempCByIndex(0))) {
    suhu = 0;
  } else {
    suhu = sensors.getTempCByIndex(0);
  }

  return suhu;
}
