#include "DHT.h"
#include "OneButton.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Ewma.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

#define DHTPIN0 2  // Digital pin connected to the DHT sensor
#define DHTPIN1 3  // Digital pin connected to the DHT sensor
#define DHTPIN2 5  // Digital pin connected to the DHT sensor
#define DHTPIN3 6  // Digital pin connected to the DHT sensor

#define heater 7  // Kontrol relay

OneButton btnP(A0, true);
OneButton btnM(A1, true);
OneButton btnSS(A2, true);

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 4   //Jumlah dht yang dipakai

Ewma adcFilterT(0.01);  // More smoothing - less prone to noise, but slower to detect changes
Ewma adcFilterH(0.01);  // More smoothing - less prone to noise, but slower to detect changes

DHT dht0(DHTPIN0, DHTTYPE); //Inisialisasi DHT
DHT dht1(DHTPIN1, DHTTYPE); //Inisialisasi DHT
DHT dht2(DHTPIN2, DHTTYPE); //Inisialisasi DHT
DHT dht3(DHTPIN3, DHTTYPE); //Inisialisasi DHT

File dataRecord;

bool running = false;
float valT[jumlahDHT];
float valH[jumlahDHT];
float totalT;
float totalH;
float avgT;
float avgH;
bool rec = false;
bool relay = false;

int addrT = 0;
int addrH = 1;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

int ledState2 = LOW;             // ledState used to set the LED
unsigned long previousMillis2 = 0;        // will store last time LED was updated
const long interval2 = 60*1000;           // interval at which to blink (milliseconds)

int spT = 40;  //SetpointSuhu
int spH = 60;  //SetpointKelembaban
int err;

bool saveCtrl = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  lcd.init();                      // initialize the lcd
  lcd.backlight();

  Serial.print("Initializing DHT...");

  dht0.begin();  //Inisialisasi dht
  dht1.begin();  //Inisialisasi dht
  dht2.begin();  //Inisialisasi dht
  dht3.begin();  //Inisialisasi dht

  readDHT();

  pinMode(heater, OUTPUT);

  Serial.print("Initializing SD Card");

  if (!SD.begin(4)) {
    Serial.println("...initialization failed!");
    //    while (1);
  } else {
    Serial.print("...OK!");
    if (dataRecord) {
      dataRecord = SD.open("test.txt", FILE_WRITE);
      Serial.println("...Read OK!");
    } else {
      Serial.println("...Error opening file!");
    }
  }
  
  dataRecord.close();

  //  spT = EEPROM.read(addrT);
  //  spH = EEPROM.read(addrH);

  btnP.attachClick(clickP);
  btnP.attachDuringLongPress(longPressP);

  btnM.attachClick(clickM);
  btnM.attachDuringLongPress(longPressM);

  btnSS.attachClick(record);
  btnSS.attachLongPressStart(longPressSS);
}

int spTBef = 0;

void loop() {
  // put your main code here, to run repeatedly:
  btnM.tick();
  btnP.tick();
  btnSS.tick();

  if (running) {
    run();
    digitalWrite(LED_BUILTIN, blink());
  } else {
    if (spTBef != spT) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write("Setpoint Suhu: ");
      lcd.write(spT);
      lcd.write("C");

      Serial.println(spT);
      spTBef = spT;

      lcd.setCursor(1, 0);
      lcd.write("Berat Awal:");
      lcd.write("xxx");
      lcd.write("KG");
      //    EEPROM.write(addrT, spT);
      //    EEPROM.write(addrH, spH);
    }

    digitalWrite(LED_BUILTIN, LOW);
  }
}


bool blink() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    return ledState;
  }
}
