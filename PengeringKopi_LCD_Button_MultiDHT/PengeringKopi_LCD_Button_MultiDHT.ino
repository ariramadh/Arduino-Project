#include <Fuzzy.h>
#include "DHT.h"
#include <Servo.h>
#include "OneButton.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Ewma.h>
#include <SPI.h>
#include <SD.h>

#define DHTPIN0 2  // Digital pin connected to the DHT sensor
#define DHTPIN1 3  // Digital pin connected to the DHT sensor
#define DHTPIN2 4  // Digital pin connected to the DHT sensor
#define DHTPIN3 5  // Digital pin connected to the DHT sensor
#define heater 6


#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 4   //Jumlah dht yang dipakai

Ewma adcFilterT(0.01);  // More smoothing - less prone to noise, but slower to detect changes
Ewma adcFilterH(0.01);  // More smoothing - less prone to noise, but slower to detect changes


DHT dht0(DHTPIN0, DHTTYPE); //Inisialisasi DHT
DHT dht1(DHTPIN1, DHTTYPE); //Inisialisasi DHT
DHT dht2(DHTPIN2, DHTTYPE); //Inisialisasi DHT
DHT dht3(DHTPIN3, DHTTYPE); //Inisialisasi DHT

OneButton btnP(A0, false);
OneButton btnM(A1, false);
OneButton btnT(A2, false);
OneButton btnSS(A3, false);
OneButton btnRec(A6, false);

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

Servo myservo;  // create servo object to control a servo

File dataRecord;

Fuzzy *fuzzy = new Fuzzy(); //inisialisasi Fuzzy

int spT = 40;  //SetpointSuhu
int spH = 60;  //SetpointSuhu
int hum = 60;  //Kadar air awal
bool running = false;
float valT[jumlahDHT];
float valH[jumlahDHT];
float totalT;
float totalH;
float avgT;
float avgH;
bool val;

void setup()
{
  // Set the Serial output
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  lcd.init();                      // initialize the lcd
  lcd.backlight();

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  dht0.begin();  //Inisialisasi dht
  dht1.begin();  //Inisialisasi dht
  dht2.begin();  //Inisialisasi dht
  dht3.begin();  //Inisialisasi dht

  pinMode(heater, OUTPUT);

  btnP.attachClick(clickP);
  btnP.attachDuringLongPress(longPressP);

  btnM.attachClick(clickM);
  btnM.attachDuringLongPress(longPressM);

  btnT.attachClick(clickT);

  btnSS.attachLongPressStart(longPressSS);

  btnRec.attachLongPressStart(record);

  fuzzySetup();
}

void loop(){
  btnP.tick();
  btnM.tick();
  btnSS.tick();
  btnT.tick();
  btnRec.tick();
  
  Serial.println(running);
  
  if(running){
    run();
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Kelembaban: ");
    lcd.write(hum);
    lcd.write("%");

    lcd.setCursor(1, 0);
    lcd.write("Berat Awal:");
    lcd.write("xxx");
    lcd.write("KG");
    digitalWrite(LED_BUILTIN, LOW);
  }
}
