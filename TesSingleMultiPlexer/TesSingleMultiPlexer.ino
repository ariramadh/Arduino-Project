#include "DHT.h"

#define DHTPIN 2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE); //Inisilisasi DHT

#define pin1 7  //S0 MultiPlexer
#define pin2 8  //S1 MultiPlexer
#define pin3 9  //S2 MultiPlexer
#define pin4 10  //S3 MultiPlexer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Tes DHT Multiplexer");
  dht.begin();

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(2000);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);

  float h = dht.readHumidity(); //Baca kelembaban
  float t = dht.readTemperature();  //Baca suhu celcius

  if (isnan(h) || isnan(t)) {
    Serial.println(F("DHT Tidak Terbaca!"));
    return;
  }
  
  Serial.print(F("Sensor ke: "));
  Serial.print("1");
  Serial.print(F("%  Kelembaban: "));
  Serial.print(h);
  Serial.print(F("%  Suhu: "));
  Serial.print(t);
  Serial.println(F("°C "));
  }  
