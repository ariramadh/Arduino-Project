#include <Arduino.h>
#include <Ewma.h>

Ewma adcFilter(0.01);  // More smoothing - less prone to noise, but slower to detect changes

#include "DHT.h"

#define DHTPIN0 2  // Digital pin connected to the DHT sensor
#define DHTPIN1 3  // Digital pin connected to the DHT sensor
#define DHTPIN2 4  // Digital pin connected to the DHT sensor
#define DHTPIN3 5  // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 4   //Jumlah dht yang dipakai

DHT dht0(DHTPIN0, DHTTYPE); //Inisialisasi DHT
DHT dht1(DHTPIN1, DHTTYPE); //Inisialisasi DHT
DHT dht2(DHTPIN2, DHTTYPE); //Inisialisasi DHT
DHT dht3(DHTPIN3, DHTTYPE); //Inisialisasi DHT

float valT[jumlahDHT];
float valH[jumlahDHT];
float totalT;
float totalH;
float avgT;
float avgH;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht0.begin();  //Inisialisasi dht
  dht1.begin();  //Inisialisasi dht
  dht2.begin();  //Inisialisasi dht
  dht3.begin();  //Inisialisasi dht
}

void loop() {
  delay(2000);

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

  // Baca kelembaban %
  valH[3] = dht3.readHumidity();
  // Baca temperatur celcius
  valT[3] = dht3.readTemperature();

  for (int i = 0; i < jumlahDHT; i++) {
    if (isnan(valT[i]) || isnan(valH[i])) {
      Serial.print(F("Failed to read from DHT sensor number "));
      Serial.print(F(i));
      Serial.println(F("!"));
      return;
    }

    totalT += valT[i];
    totalH += valH[i];

    if (i == jumlahDHT - 1) {
      avgT = totalT / jumlahDHT;
      avgH = totalH / jumlahDHT;
    }
  }

//  for (int i = 0; i < jumlahDHT; i++) {
//    Serial.print(valT[i]);
//    Serial.print("°C ");
//    Serial.print(" ");
//    if (i == jumlahDHT - 1) {
//      Serial.print("AvT: ");
//      Serial.print(avgT);
//      Serial.println("°C ");
//    }
//  }
//
//  for (int i = 0; i < jumlahDHT; i++) {
//    Serial.print(valH[i]);
//    Serial.print("% ");
//    Serial.print(" ");
//    if (i == jumlahDHT - 1) {
//      Serial.print("AvH: ");
//      Serial.print(avgH);
//      Serial.println("% ");
//      Serial.println("");
//    }
//  }

  float rawT = avgT;  if (inputT < 1) {
    output1 = 100;
    digitalWrite(heater, HIGH);
  } else {
    digitalWrite(heater, LOW);
  }

  float rawH = avgH;

  float filteredT = adcFilter.filter(rawT);
  float filteredH = adcFilter.filter(rawH);
  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

  totalT = 0;
  totalH = 0;
}
