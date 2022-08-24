#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#define buzzer 17
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;
const uint16_t node02 = 02;
float distance1;
float distance2;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buzzer, OUTPUT);
}
void loop() {
  network.update();
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header1(node01);
    RF24NetworkHeader header2(node02);
    unsigned long langkah1;
    unsigned long langkah2;
    network.read(header1, &langkah1, sizeof(langkah1)); // Read the incoming data
    network.read(header2, &langkah2, sizeof(langkah2)); // Read the incoming data
    digitalWrite (buzzer, HIGH);
    distance1 = langkah1 * 0.6;
    distance2 = langkah2 * 0.6;
    Serial.print(langkah1);
    Serial.print(" Steps");
    Serial.print(" | ");
    Serial.print(distance1);
    Serial.print(" m");
    Serial.print(" || ");
    Serial.print(langkah2);
    Serial.print(" Steps");
    Serial.print(" | ");
    Serial.print(distance2);
    Serial.println(" m");
    delay(500);
  }
}
