#include <Fuzzy.h>
#include "DHT.h"
#include "OneButton.h"
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Ewma.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

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

OneButton btnP(PA0, false);
OneButton btnM(PA1, false);
OneButton btnT(PA2, false);
OneButton btnSS(PA3, false);
OneButton btnRec(PB0, false);

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

File dataRecord;
RTC_DS3231 rtc;

Fuzzy *fuzzy = new Fuzzy(); //inisialisasi Fuzzy

int sp[] = {40, 60};  //SetpointSuhu dan kelembaban
int hum = 60;  //Kadar air awal
bool running = false;
float valT[jumlahDHT];
float valH[jumlahDHT];
float total[1];
float avg[1];
bool val;
float average[1]; 
float output1;
int output2;

void setup()
{
  // Set the Serial output
  Serial.begin(9600);
  Serial.println("=========== Memulai mesin pengering ===========");

  pinMode(LED_BUILTIN, OUTPUT);

  if(rtc.lostPower()){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  lcd.init();                      // initialize the lcd
  lcd.backlight();

  dht0.begin();  //Inisialisasi dht
  dht1.begin();  //Inisialisasi dht
  dht2.begin();  //Inisialisasi dht
  dht3.begin();  //Inisialisasi dht

  pinMode(heater, OUTPUT);

  btnP.attachClick(clickP);
//  btnP.attachDuringLongPress(longPressP);

  btnM.attachClick(clickM);
//  btnM.attachDuringLongPress(longPressM);

  btnT.attachClick(clickT);

  btnSS.attachLongPressStart(longPressSS);

  btnRec.attachLongPressStart(recStart);
  btnRec.attachDuringLongPress(rec);
  btnRec.attachLongPressStop(recStop);

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
    lcd.print("Kelembaban: ");
    lcd.print(hum);
    lcd.print("%");

    lcd.setCursor(1, 0);
    lcd.print("Berat Awal:");
    lcd.print("xxx");
    lcd.print("KG");
    digitalWrite(LED_BUILTIN, LOW);
    
//    Serial.println("=========== Memulai mesin pengering ===========");

    Serial.print(hum);
  }
}
