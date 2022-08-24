#include "OneButton.h"
#include <EEPROM.h>

#define heater 7  // Kontrol relay

OneButton btnP(A0, true);
OneButton btnM(A1, true);
OneButton btnSS(A2, true);

#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 4  //Jumlah dht yang dipakai

//File dataRecord;

bool running = false;
float valT[jumlahDHT];
float valH[jumlahDHT];
float totalT;
float totalH;
float avgT;
float avgH;
bool relay = false;

int addrT = 0;
int addrH = 1;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

int ledState2 = LOW;             // ledState used to set the LED
unsigned long previousMillis2 = 0;        // will store last time LED was updated
const long interval2 = 60 * 1000;         // interval at which to blink (milliseconds)

int spT = 40;  //SetpointSuhu
int spH = 60;  //SetpointKelembaban
int err;

bool rec = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Initializing DHT...");

  run_setup();
  readDHT();

  pinMode(heater, OUTPUT);


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
    EEPROM.write(addrT, spT);
    EEPROM.write(addrH, spH);
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
