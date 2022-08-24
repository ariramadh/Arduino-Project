#include "DHT.h"

#define DHTPIN 2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE); //Inisilisasi DHT

#define pin1 7  //S0 MultiPlexer
#define pin2 8  //S1 MultiPlexer
#define pin3 9  //S2 MultiPlexer
#define pin4 10  //S3 MultiPlexer

#define jumlahDHT 6   //Jumlah dht yang dipakai

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Tes DHT Multiplexer");
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 0; i < jumlahDHT; i++){

    delay(2000);

    Serial.print(F("Sensor ke: "));
    Serial.print(i+1);  

    bcd(i);

    float h = dht.readHumidity(); //Baca kelembaban
    float t = dht.readTemperature();  //Baca suhu celcius

    if (isnan(h) || isnan(t)) {
      Serial.println(F(" DHT Tidak Terbaca!"));
    }

    Serial.print(F("  Kelembaban: "));
    Serial.print(h);
    Serial.print(F("%  Suhu: "));
    Serial.print(t);
    Serial.println(F("°C "));
  }
}

void bcd(int pin){
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  if (bitRead(pin,0)){
    digitalWrite(pin1, HIGH);
  }else{
    digitalWrite(pin1, LOW);    
  }

  if (bitRead(pin,1)){
    digitalWrite(pin2, HIGH);
  }else{
    digitalWrite(pin2, LOW);    
  }
  
  if (bitRead(pin,2)){
    digitalWrite(pin3, HIGH);
  }else{
    digitalWrite(pin3, LOW);    
  }
  
  if (bitRead(pin,3)){
    digitalWrite(pin4, HIGH);
  }else{
    digitalWrite(pin4, LOW);    
  }
}
