// ====================================================
//                PARSING DATA SERIAL (string & int)
// ====================================================
// Format request " /perintah/# ",
// Perintah kirim P = req Pressure
//                F = req Flow
//                E = req Posisi

// Format terima " /perintah/data/# "
// perintah terima p = get pressure
//                 f = get flow
//                 e = get posisi
//                 a = get alarm


String dataIn = "", cmd;
String dataArray[10];
bool parsing = false;

void readSerial() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    dataIn += inChar;
        Serial.print(" Data innn: "); Serial.println(dataIn);
    if (inChar == '#') {
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
  //    Serial.print(" Data masuk: "); Serial.println(dataIn);
  int j = 0;
  dataArray[j] = "";
  dataIn = dataIn.substring(dataIn.indexOf("/"));
  //    Serial.print(" Data masuk2: "); Serial.print(dataIn.indexOf("/"));
  //  Serial.print(" Data parsing: "); Serial.println(dataIn);
  for (int i = 1; i < dataIn.length(); i++) {
    //        Serial.print("data parsing: ");Serial.println(dataIn[i]);

    if (dataIn[i] == 'p') {
      j = 1;
    } else if (dataIn[i] == 'f') {
      j = 2;
    } else if (dataIn[i] == 'e') {
      j = 3;
    } else if (dataIn[i] == 'k') {
      j = 4;
    } else if (dataIn[i] == 'l') {
      j = 5;
    } else if (dataIn[i] == '/') {
      j = 0;
      //      Serial.print("\t Terdelete: ");  Serial.println(dataArray[1]);
      dataArray[j] = "";
    } else if (dataIn[i] == '#') {
      //      dataArray[j] = "";
      //      Serial.print("\t J: ");  Serial.print(j);
      //      Serial.print("\t Tampil: ");  Serial.println(dataArray[j]);
      dataSwitch(j);
      dataArray[j] = "";
      j = 0;
      //      dataArray[j] = "";
    } else {
      dataArray[j] += dataIn[i];
      //      Serial.print("\t J: ");  Serial.println(j);
      //            Serial.print("\t Data: ");  Serial.println(dataArray[4]);
    }
  }

  for (int i = 0; i < 5; i ++) {
    dataArray[i] = "";
  }
}

void dataSwitch(int select) {
  switch (select) {
    case 1:
      pressure = dataArray[select].toFloat();
      //      Serial.print(" Pressure2: "); Serial.println(pressure);
      pressureAlarm();
      break;
    case 2:
      Q1 = dataArray[select].toFloat();
      //      Serial.print(" FLow: "); Serial.println(Q1);
      break;
    case 3:
      count = dataArray[select].toFloat();
      break;
    case 4:
      potensio = dataArray[select];
      break;
    case 5:
      limSwich_Procedure();
      break;
  }
}
//  cmd = dataArray[0];
//    Serial.print(" Perintah \t: ");Serial.println(cmd);
//  //  Serial.println("");
//
//  if (cmd == "a") {
//    //    digitalWrite(ALARM_PIN, HIGH);
//  } else if (cmd == "f") {
//    String flow = dataArray[1];
//    Serial.print(" flow \t: "); Serial.println(flow);
//  }
//  else if (cmd == "p") {
//    tekanan = dataArray[1];
//    pressure = tekanan;
//    Serial.print(" tekanan \t: "); Serial.println(tekanan);
//
//    //      Serial.print(" pressure \t: ");Serial.println(pressure);
//  }
//  else if (cmd == "e") {
//    String posisi = dataArray[1];
//    count = posisi;
//    Serial.print(" posisi \t: "); Serial.println(posisi);
//  }
