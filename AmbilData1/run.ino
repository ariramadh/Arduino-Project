#include "DHT.h"

#define DHTPIN0 2     // Digital pin connected to the DHT sensor
#define DHTPIN1 3     // Digital pin connected to the DHT sensor
#define DHTPIN2 4     // Digital pin connected to the DHT sensor
#define DHTPIN3 5     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht0(DHTPIN0, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

void runsetup() {

  dht0.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
}

void runloop() {
  // Wait a few seconds between measurements.
  delay(2000);

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
      Serial.print("Failed to read from DHT sensor number");
      Serial.print(i);
      Serial.println("!!");
      return;
    }

    Serial.print("DHT ");
    Serial.print(i);
    Serial.print("  ");
    //    Serial.print(dhtTFilter(t[i]));
    Serial.print(t[i]);
    Serial.print("C  ");
    //    Serial.print(dhtTFilter(h[i]));
    Serial.print(h[i]);
    Serial.println("%  ");

    if (i == jmlDHT-1) {
      if (interval == jmlDHT-1) {
        interval = 0;
      } else {
        interval += 1;
      }
    }
  }
}
