#include <Fuzzy.h>
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

#include <Servo.h>

Servo myservoIn;  // create servo object to control a servo
Servo myservoOut;  // create servo object to control a servo

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

const int jmlDHT = 3;

float h[jmlDHT];
float t[jmlDHT];

float lastOutput[jmlDHT];
float lastHOutput[jmlDHT];

int output;

unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 2000;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(115200);
  Serial.println(("\tMulti DHT11 test!"));

  myservoIn.attach(9);  // attaches the servo on pin 9 to the servo object
  myservoOut.attach(10);  // attaches the servo on pin 9 to the servo object

  myservoIn.write(0);
  myservoOut.write(0);

  dht0.begin();
  dht1.begin();
  dht2.begin();
  //  dht3.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

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

  // Wait a few seconds between measurements.

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

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
      //      Serial.print(dhtTFilter(h[i], i));
      Serial.print(h[i]);
      Serial.print("%");

      //      if (i == jmlDHT - 1) {
      //        Serial.println();
      //      }
    }

    fuzzy->setInput(1, dhtTFilter(t[1], 1));
    fuzzy->fuzzify();
    output = fuzzy->defuzzify(1);

    Serial.print("\t||  Input\t");
    Serial.print(dhtTFilter(t[1], 1));
    Serial.print("\tOutput\t");
    Serial.println(output);

    ServoInput(output);
    ServoOutput(output);
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

  averageH = round(averageH * 100) / (float)100;

  return averageH;
}

void ServoInput(int val) {
  int derajat = 0;

  if (val <= 10) {
    derajat = map(val, 0, 10, 0, 14);
  } else if (val <= 20) {
    derajat = map(val, 10, 20, 14, 21);
  } else if (val <= 30) {
    derajat = map(val, 20, 30, 21, 27);
  } else if (val <= 40) {
    derajat = map(val, 30, 40, 27, 32);
  } else if (val <= 50) {
    derajat = map(val, 40, 50, 32, 35);
  } else if (val <= 60) {
    derajat = map(val, 50, 60, 35, 39);
  } else {
    derajat = map(val, 0, 90, 39, 46);
  }

  myservoIn.write(derajat);
}


void ServoOutput(int val) {
  int derajat = 0;

  if (val <= 10) {
    derajat = map(val, 0, 10, 4, 27);
  } else if (val <= 20) {
    derajat = map(val, 10, 20, 27, 36);
  } else if (val <= 30) {
    derajat = map(val, 20, 30, 36, 43);
  } else if (val <= 40) {
    derajat = map(val, 30, 40, 43, 49);
  } else if (val <= 50) {
    derajat = map(val, 40, 50, 49, 57);
  } else if (val <= 60) {
    derajat = map(val, 50, 60, 57, 64);
  } else {
    derajat = map(val, 0, 90, 64, 74);
  }

  myservoOut.write(derajat);
}
