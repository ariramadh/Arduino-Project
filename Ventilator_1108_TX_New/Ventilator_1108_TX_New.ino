#include "OneButton.h"
#include <SimpleTimer.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define setPin 10
#define buzzerPin 11
#define btnSnooze 12
#define Pwr 2

SimpleTimer sendData(20);
SimpleTimer receiveData(50);

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

float a;
float plateau, peep;
float pressurem;
float tIn;
float AVf_1;
float vol;

int count;

int currentVin;
int zPressure[2];
int maxPress;

unsigned long tprev;

String dataIn, dataArray[10], cmd, cmd2;

bool start = false, mode = false, getVol = false, sendRun = false;

String volum;

OneButton setButton(setPin, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);

  pinMode(buzzerPin, OUTPUT);
  pinMode(btnSnooze, INPUT_PULLUP);

  zPressure[0] = analogRead(A4) - 320;  //Mengkalibrasi ADC flow
  zPressure[1] = analogRead(A5);  //Mengkalibrasi ADC tekanan
  float fc = 15.167;
  float RC = 1.0 / (2 * 3.14159 * fc);
  a = RC / 0.02;

  setButton.attachClick(clickSet);
  setButton.attachLongPressStart(longStartSet);

  ads.begin();
}

void loop() {
  bool parsing = false;

  if (!start) {
    sendRun = false;  //Untuk mendeteksi apakah homing selesai
  }

  setButton.tick();

  //  if (sendRun) Serial.println("Kirim Pressure");  //Jika homing selesai, kirim data pressure flow dll

  if (start && sendRun && sendData.isReady()) {
    //    Serial.println("===========================");
    //    Serial.print(bacaFlow());  //=========Kirim data pressure============
    //    Serial.print("\t");  //=========Kirim data pressure============
    //    volum = bacaVolume();
    //    Serial.print(volum);  //=========Kirim data pressure============
    //    Serial.println(bacaPressure());  //=========Kirim data pressure============
    sendHere('p', bacaPressure());  //=========Kirim data pressure============
    sendHere('f', bacaFlow());  //=========Kirim data flow============
    sendHere('a', bacaAlarm()); //=========Kirim data alarm============
    sendHere('v', bacaVolume()); //=========Kirim data volume============
    sendData.reset();
    Serial.println("");
  } else if (!start && sendData.isReady()) {
    bacaPotensio();
    sendData.reset();
    currentSensor();
  }

  while (Serial.available() > 0) {
    //    delay(10);
    Serial.print("");
    //    Serial.println(Serial.available());
    char inChar = (char)Serial.read();
    //    Serial.print(inChar);
    dataIn += inChar;

    //mulai parsing jika pada data terdapat tanda "/" dan "#"
    if (dataIn.indexOf("/") >= 0 && inChar == '#') {
      parsing = true;
      //      Serial.println(dataIn);
    }

    if (parsing) {
      parsingData();
      parsing = false;
      dataIn = "";
    }
  }
  //  Serial.println();
}

void parsingData() {
  int j = 0;
  dataArray[j] = "";
  dataIn = dataIn.substring(dataIn.indexOf("/"));

    Serial.println(dataIn);

  for (int i = 1; i < dataIn.length(); i++) {
    if (dataIn[i] == '/')
    {
      j++;
      //      Serial.print("Data Array: "); Serial.println(dataArray[j]);
      dataArray[j] = "";
    } else {
      dataArray[j] = dataArray[j] + dataIn[i];
    }
  }

  //  cmd = dataArray[0];
  cmd = dataArray[0].substring(0, 1);
  //  Serial.print(" Perintah : "); Serial.println(cmd);
  //  Serial.println("");

  if (cmd == "i") {
    //    Serial.print("get vol:"); Serial.println(getVol);
    getVol = true;
    count ++;
    peep = bacaPressure().toFloat();
    //    Serial.print("/");Serial.print("p"); Serial.print(pressure);Serial.print("#");
  } else if (cmd == "I") {
    getVol = false;
    pressurem = 0;
    //    Serial.print("============ get vol:"); Serial.println(getVol);
    //    Serial.print("/");Serial.print("f"); Serial.print(Q);Serial.print("#");
  } else if (cmd == "h") {
    sendRun = true;
  } else if (cmd == "L") {
    plateau = bacaPressure().toFloat();
    //    Serial.print("/");Serial.print("k"); Serial.print(TV);
    //    Serial.print("*");Serial.print(RR);  Serial.print("*");Serial.print(IE);
    //    Serial.print("*");Serial.print(AC);  Serial.print("#");
  }
}
