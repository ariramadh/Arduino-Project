 #include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node00 = 00;

int x, y, z;
int count = 0;
float calories;
float distance;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}
void loop() {
  read_step();
  network.update();
  unsigned long pedometer = count;  // Read the potentiometer value
  RF24NetworkHeader header3(node00);     // (Address where the data is going)
  bool ok = network.write(header3, &pedometer, sizeof(pedometer)); // Send the data
}

void read_step() {
  x = analogRead(A5);
  y = analogRead(A4);
  z = analogRead(A3);

  //waktu++;
  //delay(1000);

  if (x <= 400 && x >= 390 && y <= 380 && y >= 355) {
    count++;
    delay(200);
  }

  calories = count * 0.035; // This value is taken depending upon my weight and height.
  distance = count * 0.0006; // My step it is on average 60cm.
//  Speed =  distance / waktu;
  Serial.print(x);
  Serial.print(" | ");
  Serial.print(y);
  Serial.print(" | ");
  Serial.print(z);
  Serial.print(" | ");
  Serial.print(count);
  Serial.print(" Steps");
  Serial.print(" | ");
  Serial.print(calories);
  Serial.print(" Calories");
  Serial.print(" | ");
  Serial.print(distance);
  Serial.println(" Meters");
  /*Serial.print(" | ");
  Serial.print(Speed);
  Serial.println(" m/s");*/
  delay(150);
}
