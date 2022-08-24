// ====================================================
//            PARSING DATA SERIAL (string & int)
// ====================================================
// '/' Untuk memulai perintah, '#' untuk mengakhiri perintah
// Format request " /perintah/# ",
// Perintah kirim P = req Pressure
//                F = req Flow
//                V = req volume

// Format terima " /perintah/data/# "
// perintah terima p = get pressure
//                 f = get flow * get volume
//                 s = get set button
//                 a = get alarm
//                 k = get potensio

//  Yang mesti di request   : Pressure, Flow, Volume*
//  Yang mesti di broadcast : Alarm, Set Button, Potensio
//  *Khusus volume, kirimkan perintah di awal inhale dan akhir inhale

String dataIn = "", cmd;
String dataArray[10];
bool parsing = false;

void readSerial() {
  //  Serial.println(" READ SERIAL ");
  while (Serial.available() > 0) {
    //      Serial.println(" SERIAL AVAILABLE ");
    char inChar = (char)Serial.read();
    dataIn += inChar;
    //    Serial.print(" Data inn: "); Serial.print(dataIn);
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
  //  Serial.print(" Data masuk: "); Serial.println(dataIn);
  int j = 0;

  dataIn = dataIn.substring(dataIn.indexOf("/"));

  //  Serial.print(" Data masuk2: "); Serial.print(dataIn.indexOf("/"));
  //  Serial.print(" Data parsing: "); Serial.println(dataIn);

  for (int i = 1; i < dataIn.length(); i++) {
    //    Serial.print("data parsing: ");Serial.println(dataIn[i]);

    if (dataIn[i] == 'p') { //Pressure
      j = 1;
    } else if (dataIn[i] == 'f') {  //Flow
      j = 2;
    } else if (dataIn[i] == 'v') {  //Volume
      j = 3;
    } else if (dataIn[i] == 'a') {
      j = 4;
    } else if (dataIn[i] == 'm') {
      j = 5;
    } else if (dataIn[i] == 's') {
      j = 6;
    } else if (dataIn[i] == '/') {
      j = 0;
      //      Serial.print("\t Terdelete: ");  Serial.println(dataArray[1]);
      dataArray[j] = "";
    } else if (dataIn[i] == '#') {
      //      dataArray[j] = "";
      //      Serial.print("\t J: ");  Serial.print(j);
      //      Serial.print("\t Tampil: ");  Serial.println(dataArray[j]);
      dataSwitch(j);
      j = 0;
      //      dataArray[j] = "";
    } else {
      dataArray[j] += dataIn[i];
      //      Serial.print("\t J: ");  Serial.println(j);
      //      Serial.print("\t Data: ");  Serial.println(dataArray[1]);
    }
  }

  for (int i = 0; i <= 5; i ++) {
    dataArray[i] = "";
  }
}

void dataSwitch(int select) {
  //  int k = 0;
  //  String daPot[4];
  switch (select) {
    case 1:
      pressure = dataArray[select].toFloat();
      //      Serial.print(" Pressure2: "); Serial.println(pressure);
      break;
    case 2:
      Q1 = dataArray[select].toFloat();
      //      Serial.print(" FLow: "); Serial.println(Q1);
      break;
    case 3:
      volume = dataArray[select].toInt();
      break;
    case 4:
      alarm = dataArray[select].toInt();
      break;
    case 5:
      mode = dataArray[select].toInt();
      break;
    case 6:
      start = dataArray[select].toInt();
      break;
  }
}
