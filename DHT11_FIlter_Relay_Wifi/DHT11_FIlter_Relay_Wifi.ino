#include <Arduino.h>
#include <Ewma.h>

#include "DHT.h"

#include "CTBot.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

Ewma adcFilter1(0.01);
Ewma adcFilter2(0.01);

#define relay 4

CTBot myBot;

String ssid = "Rvs";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "jashujan"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "1047412026:AAE5s748p-ja_510Q2Jhv1c8xpuPODBS_-4";   // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));

  Serial.println("Starting TelegramBot...");

  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  pinMode(relay, OUTPUT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  int err = 0;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  float filtered1;
  float filtered2;
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    err = 1;
  } else {
    filtered1 = adcFilter1.filter(t);
    filtered2 = adcFilter2.filter(h);
  }

  Serial.print("T");
  Serial.print(t);
  Serial.print("H");
  Serial.print(h);
  Serial.println("");

  Serial.print("*T");
  Serial.print(filtered1);
  Serial.print("*H");
  Serial.print(filtered2);
  Serial.println("*");

  String errText = (String)"Waduh bos, maaf sensornya lagi error";

  TBMessage msg;

  if (myBot.getNewMessage(msg)) {
    Serial.println(msg.text);
    if (msg.text.equalsIgnoreCase("/gerah")) {              // if the received message is "LIGHT ON"...
      digitalWrite(relay, HIGH);
      myBot.sendMessage(msg.sender.id, "Saya nyalain kipas nya ya bos");  // notify the sender
    }
    else if (msg.text.equalsIgnoreCase("/dingin")) {        // if the received message is "LIGHT OFF"...
      digitalWrite(relay, LOW);
      myBot.sendMessage(msg.sender.id, "Sebentar bos, saya matiin dulu kipasnya"); // notify the sender
    }
    else if (msg.text.equalsIgnoreCase("/suhu")) {        // if the received message is "LIGHT OFF"...
      String suhunya = (String)"Suhu sekarang " + filtered1 + (String)"°C bos!";
      if (err == 0) {
        myBot.sendMessage(msg.sender.id, suhunya); // notify the sender
      } else {
        myBot.sendMessage(msg.sender.id, errText); // notify the sender
      }
    }
    else if (msg.text.equalsIgnoreCase("/lembab")) {        // if the received message is "LIGHT OFF"...
      String lembabnya = (String)"Kelembaban sekarang " + filtered2 + (String)"% bos!";
      if (err == 0) {
        myBot.sendMessage(msg.sender.id, lembabnya); // notify the sender
      } else {
        myBot.sendMessage(msg.sender.id, errText); // notify the sender
      }
    }
    else {                                                    // otherwise...
      // generate the message for the sender
      String reply;
      reply = (String)"Selamat Datang bos " + msg.sender.username + (String)". Silahkan masukan perintah yang tersedia.";
      myBot.sendMessage(msg.sender.id, reply);             // and send it
    }
  }

  //  if (Serial.available()) {
  //    char inByte = Serial.read();
  //
  //    Serial.print(inByte);
  //    if (inByte == 'R') {
  //      digitalWrite(relay, HIGH);
  //    } else if (inByte == 'r') {
  //      digitalWrite(relay, LOW);
  //    }
  //  }
}
