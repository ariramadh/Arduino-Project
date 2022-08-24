#include "dhtKopi.h"
#include "fuzzyKopi.h"
#include "servoKopi.h"
#include <Fuzzy.h>
#include <Servo.h>

#define dhtpin 2
#define servopin 9

#define dhttype DHT11

int sp = 40;

int s;
int k;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dhtSetup(dhtpin, dhttype);
  fuzzySetup();
  servoSetup(9);    
}

void loop() {
  // put your main code here, to run repeatedly:
  if(! bacaDHT()){
    s = bacaSuhu();
    k = bacaKelembaban();
  }

  fuzzyProses(s);
  servoKontrol(map(fuzzyProses,0,100,0,255));
}
