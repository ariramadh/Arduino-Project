#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <ArduinoJson.h>

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network

const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;

int a, b, c, j, x, y, z;
int count = 0;

float Max = 0;
float Min = 100;
float Interval;
float calories;
float distance;
float Percepatan;
float Speed1 = 0;
float Speed2 = 0;
float Speed3 = 0;
float Speed4 = 0;
float Speed5 = 0;
float jumlahSpeed, rataSpeed;

unsigned long ulangi, ulangi2;

char input;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void  loop() {
  read_step();

  unsigned long currentMillis = millis();

  if (currentMillis - ulangi > 1000) {
    b += 1;
    ulangi = millis();
  }

  if (b == 5) {
    Speed1 = distance / 5;
    Serial.print(Speed1);
    Serial.print(" Speed1");
    Serial.print(" | ");
  }

  else if (b == 10) {
    Speed2 = distance / 5 - Speed1;
    Serial.print(Speed2);
    Serial.print(" Speed2");
    Serial.print(" | ");
  }

  else if (b == 15) {
    Speed3 = distance / 5 - Speed1 - Speed2;
    Serial.print(Speed3);
    Serial.print(" Speed3");
    Serial.print(" | ");
  }

  else if (b == 20) {
    Speed4 = distance / 5 - Speed1 - Speed2 - Speed3;
    Serial.print(Speed4);
    Serial.print(" Speed4");
    Serial.print(" | ");
  }

  else if (b == 25) {
    Speed5 = distance / 5 - Speed1 - Speed2 - Speed3 - Speed4;
    Serial.print(Speed5);
    Serial.print(" Speed5");
    Serial.print(" | ");
  }

  else if (b == 26) {
    Serial.print(" | ");
    jumlahSpeed = Speed1 + Speed2 + Speed3 + Speed4 + Speed5;
    rataSpeed = jumlahSpeed / 5;
    Serial.print(rataSpeed);
    Serial.print(" RataSpeed");
    Serial.print(" | ");
    percepatan();
    Interval = Max - Min;
    Percepatan = Interval / b;
    Serial.print(Percepatan);
    Serial.print(" percepatan");
    Serial.print(" | ");
  }

  else if (b > 26) {
    reset();
  }

  //============Bagian JSON============
  root["pedometer"] = count;
  root["speed"] = Speed1;
  //===================================

  network.update();
  //  unsigned long pedometer = count;  // Read the potentiometer value

  String output;
  root.printTo(output);
  Serial.print(output);

  RF24NetworkHeader header(other_node);     // (Address where the data is going)
  bool ok = network.write(header, &output, sizeof(output)); // Send the data
}

void read_step() {

  x = analogRead(A5);
  y = analogRead(A4);
  z = analogRead(A3);

  if (x <= 390 && x >= 370 && z <= 370 && z >= 360) {

    unsigned long currentMillis2 = millis();
    if (currentMillis2 - ulangi2 > 299) {
      count++;
      ulangi2 = millis();
    }
  }

  calories = count * 0.035; // This value is taken depending upon my weight and height.
  distance = count * 0.6; // My step it is on average 60cm.

  Serial.print(count);
  Serial.print(" Steps");
  Serial.print(" | ");
  Serial.print(calories);
  Serial.print(" Calories");
  Serial.print(" | ");
  Serial.print(distance);
  Serial.println(" Meters");

}

void reset() {
  b = 0;
  count = 0;
  distance = 0;
  Speed1 = 0;
  Speed2 = 0;
  Speed3 = 0;
  Speed4 = 0;
  Speed5 = 0;
}

void percepatan() {
  float nilai[5] = {Speed1, Speed2, Speed3, Speed4, Speed5};
  for (j = 0; j < 5; j++) {
    if (nilai[j] > Max) {
      Max  = nilai[j];
    }
    if (nilai[j] < Min) {
      Min  = nilai[j];
    }
  }
}
