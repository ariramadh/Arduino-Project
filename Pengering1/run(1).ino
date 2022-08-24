#include "DHT.h"
#include <Arduino.h>
#include <Ewma.h>

#define DHTPIN0 2  // Digital pin connected to the DHT sensor
#define DHTPIN1 3  // Digital pin connected to the DHT sensor
#define DHTPIN2 4  // Digital pin connected to the DHT sensor
#define DHTPIN3 5  // Digital pin connected to the DHT sensor
#define heater 7  // Kontrol relay

#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 3   //Jumlah dht yang dipakai

Ewma adcFilterT(0.01);  // More smoothing - less prone to noise, but slower to detect changes
Ewma adcFilterH(0.01);  // More smoothing - less prone to noise, but slower to detect changes

DHT dht0(DHTPIN0, DHTTYPE); //Inisialisasi DHT
DHT dht1(DHTPIN1, DHTTYPE); //Inisialisasi DHT
DHT dht2(DHTPIN2, DHTTYPE); //Inisialisasi DHT

float valT[jumlahDHT];
float valH[jumlahDHT];
float totalT;
float totalH;
float avgT;
float avgH;
bool relay = false;

int ledState2 = LOW;             // ledState used to set the LED
unsigned long previousMillis2 = 0;        // will store last time LED was updated
const long interval2 = 60 * 1000;         // interval at which to blink (milliseconds)

int err;

void run_setup() {
  Serial.print("Initializing DHT...");

  dht0.begin();  //Inisialisasi dht
  dht1.begin();  //Inisialisasi dht
  dht2.begin();  //Inisialisasi dht

  pinMode(heater, OUTPUT);
}

float dhtTFilter(float rawT) {
  float averageT = adcFilterT.filter(rawT);

  return averageT;
}

float dhtHFilter(float rawH) {
  float averageH = adcFilterH.filter(rawH);

  return averageH;
}

void run()
{
  readDHT();

  //  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

    float inputT = spT - dhtTFilter(avgT);
    float inputH = spH - dhtHFilter(avgH);
  
    if (inputT <= 0) {
      relay = HIGH;
    } else if (inputT >= 2) {
      relay = LOW;
    }

  digitalWrite(heater, relay);

  Serial.print("SetPoint:");
  Serial.print(spT);

  Serial.print("\tSuhu: ");
  for (int i = 0; i < jumlahDHT; i++) {
    Serial.print(dhtTFilter(valT[i]));
    Serial.print("/");
  }

  Serial.print("\tKelembaban: ");
  for (int i = 0; i <= jumlahDHT; i++) {
    Serial.print(dhtHFilter(valH[i]));
    Serial.print("/");
  }

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
