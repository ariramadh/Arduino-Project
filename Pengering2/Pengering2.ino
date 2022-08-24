#include "OneButton.h"
#include <EEPROM.h>

OneButton btnM(A0, true);
OneButton btnP(A1, true);
OneButton btnSS(A2, true);

bool running = false;
bool rec = false;

int spT = 40;  //SetpointSuhu
int spH = 60;  //SetpointKelembaban

int addrT = 0;
int addrH = 1;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  run_setup();

  spT = EEPROM.read(addrT);
  spH = EEPROM.read(addrH);

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
    if(rec){
      digitalWrite(LED_BUILTIN, blink());
    }else{
      digitalWrite(LED_BUILTIN, HIGH);      
    }
  } else {
    if (spTBef != spT) {
      Serial.println(spT);
      spTBef = spT;

      EEPROM.write(addrT, spT);
      EEPROM.write(addrH, spH);
      delay(100);
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
