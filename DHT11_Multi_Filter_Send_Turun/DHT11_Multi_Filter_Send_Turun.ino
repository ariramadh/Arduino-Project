#include "DHT.h"

#define DHTPIN0 2     // Digital pin connected to the DHT sensor
#define DHTPIN1 3     // Digital pin connected to the DHT sensor
#define DHTPIN2 4     // Digital pin connected to the DHT sensor
//#define DHTPIN3 5     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht0(DHTPIN0, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
//DHT dht3(DHTPIN3, DHTTYPE);

const int jmlDHT = 3;

float h[jmlDHT];
float t[jmlDHT];

float lastOutput[jmlDHT];
float lastHOutput[jmlDHT];

bool start = false;

void setup() {
  Serial.begin(115200);
  Serial.println(("\tMulti DHT11 test!"));

  dht0.begin();
  dht1.begin();
  dht2.begin();
  //  dht3.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  start = false;

}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read Humidity as Percent
  h[0] = dht0.readHumidity();
  h[1] = dht1.readHumidity();
  h[2] = dht2.readHumidity();
  //  h[3] = dht3.readHumidity();

  //   Read temperature as Celsius (the default)
  t[0] = dht0.readTemperature();
  t[1] = dht1.readTemperature();
  t[2] = dht2.readTemperature();
  //  t[3] = dht3.readTemperature();

  float nil = dhtTFilter(t[1], 1);

  if (nil >= 42) {
    start = true;
  }

  digitalWrite(LED_BUILTIN, start);

  if (start) {
    if (nil <= 40) {
      Serial.print("\t20*");
    } else if (nil <= 41) {
      Serial.print("\t40*");
    } else if (nil <= 42) {
      Serial.print("\t60*");
    } else {
      Serial.print("\t60*");
    }
  }

  for (int i = 0; i < jmlDHT; i++) {
    if (isnan(h[i]) || isnan(t[i])) {
      Serial.print("\tFailed to read from DHT sensor number");
      Serial.print(i);
      Serial.println("!!");
      return;
    }

    Serial.print("\tDHT");
    Serial.print(i);
    Serial.print("\t");
    Serial.print(t[i]);
    Serial.print("\t");
    Serial.print(dhtTFilter(t[i], i));
    Serial.print("\t");
    //    Serial.print(dhtTFilter(h[i]));
    Serial.print(h[i]);
    Serial.print("%\t");

    //    Serial.print(lastOutput[i]);

    if (i == jmlDHT - 1) {
      Serial.print(nil);
      Serial.println();
    }
  }


}

float dhtTFilter(float rawT, int iteration) {
  float alpha = 0.1;
  float averageT = alpha * rawT + (1 - alpha) * lastOutput[iteration];

  lastOutput[iteration] = averageT;

  averageT = round(averageT * 100) / (float)100;

  return averageT;
}

float dhtHFilter(float rawH, int iteration) {
  float alpha = 0.1;
  float averageH = alpha * rawH + (1 - alpha) * lastHOutput[iteration];

  lastHOutput[iteration] = averageH;

  return averageH;
}
