#include "OneButton.h"
#include <SimpleTimer.h>

#define setPin 10
#define buzzerPin 11
#define btnSnooze 12
#define Pwr 2

SimpleTimer sendData(100);
SimpleTimer receiveData(50);

float a;
float plateau, peep;
float pressurem;

int count;

int currentVin;
int zPressure[2];

unsigned long tprev;

String dataIn, dataArray[10], cmd, cmd2;

bool start = false, mode = false, getVol = false, sendRun = false;

OneButton setButton(setPin, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);

  pinMode(buzzerPin, OUTPUT);
  pinMode(btnSnooze, INPUT_PULLUP);

  zPressure[0] = analogRead(A4) - 465;  //Mengkalibrasi ADC tekanan
  zPressure[1] = analogRead(A5) - 559 ;  //Mengkalibrasi ADC tekanan
  float fc = 15.167;
  float RC = 1.0 / (2 * 3.14159 * fc);
  a = RC / 0.02;

  setButton.attachClick(clickSet);
  setButton.attachLongPressStart(longStartSet);
}

void loop() {
  bool parsing = false;

  if (!start) {
    sendRun = false;  //Untuk mendeteksi apakah homing selesai
  }

  setButton.tick();

  //  if (sendRun) Serial.println("Kirim Pressure");  //Jika homing selesai, kirim data pressure flow dll

  if (start && sendRun  && sendData.isReady()) {
    Serial.println("===========================");
    sendHere('p', bacaPressure());  //=========Kirim data pressure============
    sendHere('f', bacaFlow());  //=========Kirim data flow============
    sendHere('v', bacaVolume()); //=========Kirim data volume============
    sendHere('a', bacaAlarm()); //=========Kirim data alarm============
    sendData.reset();
  } else if (!start && sendData.isReady()) {
    bacaPotensio();
    sendData.reset();
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
    //    Serial.print("/");Serial.print("p"); Serial.print(pressure);Serial.print("#");
  } else if (cmd == "I") {
    getVol = false;
    pressurem = 0;
    //    Serial.print("============ get vol:"); Serial.println(getVol);
    //    Serial.print("/");Serial.print("f"); Serial.print(Q);Serial.print("#");
  } else if (cmd == "h") {
    sendRun = true;
  } else if (cmd == "p") {
    //    Serial.print("/");Serial.print("k"); Serial.print(TV);
    //    Serial.print("*");Serial.print(RR);  Serial.print("*");Serial.print(IE);
    //    Serial.print("*");Serial.print(AC);  Serial.print("#");
  }
}
