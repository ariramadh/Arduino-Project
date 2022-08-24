#include "DHT.h"
#include <Arduino.h>
#include <Ewma.h>

#define DHTPIN0 2  // Digital pin connected to the DHT sensor
#define DHTPIN1 3  // Digital pin connected to the DHT sensor
#define DHTPIN2 5  // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

void run()
{
  readDHT();

  float rawT = avgT;
  float rawH = avgH;

  float averageT = adcFilterT.filter(rawT);
  float averageH = adcFilterH.filter(rawH);
  //  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

  float inputH = spH - averageH;
  float inputT = spT - averageT;

  if (inputT <= 0) {
    relay = HIGH;
  } else if (inputT >= 2) {
    relay = LOW;
  }

  digitalWrite(heater, relay);

  Serial.print("SetPoint:");
  Serial.print(spT);
  Serial.print("\tSuhu: ");
  Serial.print(averageT);
  Serial.print("\tKelembaban: ");
  Serial.print(averageH);
  Serial.print("\tRelay: ");
  Serial.println(relay);
}

void readDHT() {
  delay(500);

  totalT = 0;
  totalH = 0;

  // Baca kelembaban %
  valH[0] = dht0.readHumidity();
  // Baca temperatur celcius
  valT[0] = dht0.readTemperature();

  // Baca kelembaban %
  valH[1] = dht1.readHumidity();
  // Baca temperatur celcius
  valT[1] = dht1.readTemperature();

  // Baca kelembaban %
  valH[2] = dht2.readHumidity();
  // Baca temperatur celcius
  valT[2] = dht2.readTemperature();

  for (int i = 0; i < jumlahDHT; i++) {
    if (isnan(valT[i]) || isnan(valH[i])) {
      Serial.print(("Failed to read from DHT sensor "));
      Serial.print(i);
      Serial.println("!");
      return;
    }

    totalT += valT[i];
    totalH += valH[i];

    if (i == jumlahDHT - 1) {
      avgT = totalT / jumlahDHT;
      avgH = totalH / jumlahDHT;
    }
  }
}
