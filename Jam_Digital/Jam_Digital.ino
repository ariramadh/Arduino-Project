// Date and time functions using just software, based on millis() & timer

//#include <Arduino.h>
#include "RTClib.h"

//#if defined(ARDUINO_ARCH_SAMD)
//// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
//#define Serial SerialUSB
//#endif

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int secBefore = 0;

void setup () {
  Serial.begin(115200);
  // following line sets the RTC to the date & time this sketch was compiled
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  dht.begin();
}

void loop () {
  DateTime now = rtc.now();

  int h = dht.readHumidity();
  int t = dht.readTemperature();

  if (now.second() != secBefore) {
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.println();


    int m = 2;
    int y = 4;

    if (now.day() >= 10) {
      m++;
      y++;
    }

    if (now.month() >= 10) {
      y++;
    }

    lcd.setCursor(0, 0);
    lcd.print(now.day());
    lcd.print("/");

    lcd.setCursor(m, 0);
    lcd.print(now.month());
    lcd.print("/");

    lcd.setCursor(y, 0);
    lcd.print(now.year());

    lcd.setCursor(13, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    int mi = 2;
    int se = 4;

    if (now.hour() >= 10) {
      mi++;
      se++;
    }

    if (now.minute() >= 10) {
      se++;
    }

    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(":");

    lcd.setCursor(mi, 1);
    lcd.print(now.minute());
    lcd.print(":");

    lcd.setCursor(se, 1);
    lcd.print(now.second());

    lcd.setCursor(9, 1);
    lcd.print(t);
    lcd.print("C");

    lcd.setCursor(13, 1);
    lcd.print(h);
    lcd.print("%");

    Serial.println();
    secBefore = now.second();
  }
}
