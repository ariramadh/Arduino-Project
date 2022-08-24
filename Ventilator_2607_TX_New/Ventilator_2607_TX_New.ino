#include "OneButton.h"

#define btnPin 2
#define buzzerPin 5
#define btnSnooze 6

float a;
float plateau, peep;

int currentVin;
int zPressure[2];

unsigned long tprev;

String dataIn, dataArray[10], cmd, cmd2;

bool start = false, mode = false, getVol = false;

OneButton setButton(btnPin, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  pinMode(btnSnooze, INPUT);

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

  setButton.tick();

  if (longStartSet) {
    sendHere('p',bacaPressure());
    sendHere('f',bacaVolume());
    sendHere('v',bacaFlow());
    sendHere('a',bacaAlarm());
  } else {
    bacaPotensio();
  }

  while (Serial.available() > 0) {
    //    delay(10);
    char inChar = (char)Serial.read();
    //    Serial.print(inChar);
    dataIn += inChar;
    //      Serial.print(dataIn);

    //mulai parsing jika pada data terdapat tanda "/" dan "#"
    if (dataIn.indexOf("/") >= 0 && inChar == '#') {
      parsing = true;
    }

    if (parsing) {
      parsingData();
      parsing = false;
      dataIn = "";
    }
  }
}

void parsingData() {
  Serial.print(" Data masuk: "); Serial.println(dataIn);
  int j = 0;
  dataArray[j] = "";
  dataIn = dataIn.substring(dataIn.indexOf("/"));
  //  Serial.print(" Data parsing: "); Serial.println(dataIn);
  for (int i = 1; i < dataIn.length(); i++) {
    //    Serial.print("data parsing: ");Serial.println(dataIn[i]);
    if (dataIn[i] == '/')
    {
      j++;
      dataArray[j] = "";
    }
    else {
      dataArray[j] = dataArray[j] + dataIn[i];
    }
  }

  //  cmd = dataArray[0];
  cmd = dataArray[0];
  //  Serial.print(" Perintah \t: ");Serial.println(cmd);
  //  Serial.println("");

  if (cmd == 'i') {
    getVol = true;
    //    Serial.print("/");Serial.print("p"); Serial.print(pressure);Serial.print("#");
  } else if (cmd == 'I') {
    getVol = false;
    //    Serial.print("/");Serial.print("f"); Serial.print(Q);Serial.print("#");
  } else if (cmd == "E") {
    //    Serial.print("/");Serial.print("e"); Serial.print(count);Serial.print("#");
  } else if (cmd == "K") {
    //    Serial.print("/");Serial.print("k"); Serial.print(TV);
    //    Serial.print("*");Serial.print(RR);  Serial.print("*");Serial.print(IE);
    //    Serial.print("*");Serial.print(AC);  Serial.print("#");
  }
}
