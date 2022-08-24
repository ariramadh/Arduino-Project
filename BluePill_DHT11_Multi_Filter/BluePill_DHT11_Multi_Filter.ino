#include "DHT.h"

#define DHTPIN0 PB9     // Digital pin connected to the DHT sensor
#define DHTPIN1 PB8     // Digital pin connected to the DHT sensor
#define DHTPIN2 PB5     // Digital pin connected to the DHT sensor
#define DHTPIN3 PA8     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht0(DHTPIN0, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

const int jmlDHT = 4;

float h[jmlDHT];
float t[jmlDHT];

float hTotal;
float tTotal;

void setup() {
  Serial.begin(115200);
  Serial.println(("Multi DHT11 test!"));

  dht0.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(500);

  // Read Humidity as Percent
  h[0] = dht0.readHumidity();
  h[1] = dht1.readHumidity();
  h[2] = dht2.readHumidity();
  h[3] = dht3.readHumidity();
  // Read temperature as Celsius (the default)
  t[0] = dht0.readTemperature();
  t[1] = dht1.readTemperature();
  t[2] = dht2.readTemperature();
  t[3] = dht3.readTemperature();

  for (int i = 0; i < jmlDHT; i++) {
    if (isnan(h[i]) || isnan(t[i])) {
      Serial.print("Failed to read from DHT sensor ");
      Serial.print(i);
      Serial.println("!!");
      return;
    }

    Serial.print("DHT ");
    Serial.print(i);
    Serial.print("  ");
    Serial.print(t[i]);
    Serial.print("C  ");
    Serial.print(h[i]);
    Serial.println("%  ");

    tTotal += t[i];
    hTotal += h[i];

    if (i == jmlDHT - 1) {
      Serial.print("DHT T");
      Serial.print("  ");
      Serial.print(tTotal/jmlDHT);
      Serial.print("C  ");
      Serial.print(hTotal/jmlDHT);
      Serial.println("%  ");

      Serial.println("=======================");
    }
  }

  tTotal = 0;
  hTotal = 0;
}
