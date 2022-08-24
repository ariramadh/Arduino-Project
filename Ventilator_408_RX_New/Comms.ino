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
bool done;

void readSerial() {
  done = false;

  if (Serial.available() > 0 && !done) {
    //    Serial.print("SERIAL AVAILABLE  ");
    //    Serial.println(Serial.available());
    char inChar = Serial.read();
    dataIn += inChar;

    //    Serial.print(inChar);

    if (inChar == '#') {
      parsingData();
      dataIn = "";
    }
  }
}

void parsingData() {
  int j = 0;

  String dataArray[8];

  dataIn = dataIn.substring(dataIn.indexOf("/"), dataIn.length());

  for (int i = 1; i < dataIn.length(); i++) {

    if (dataIn[i] == 'p') { //Pressure
      j = 1;
    } else if (dataIn[i] == 'f') {  //Flow
      j = 2;
    } else if (dataIn[i] == 'v') {  //Volume
      j = 3;
    } else if (dataIn[i] == 'a') {  //Alarm
      j = 4;
    } else if (dataIn[i] == 'm') {  //Mode
      j = 5;
    } else if (dataIn[i] == 's') {  //Setbutton
      j = 6;
    } else if (dataIn[i] == 'k') {  //Potensio
      j = 7;
    } else if (dataIn[i] == '/') {  //Start Character
      j = 0;
      //      Serial.print("\t Terdelete: ");  Serial.println(dataArray[1]);
      dataArray[j] = "";
    } else if (dataIn[i] == '#') {  //End Character
      //      dataArray[j] = "";
      //      Serial.print("\t J: ");  Serial.print(j);
      //      Serial.print("\t Tampil: ");  Serial.println(dataArray[j]);
      //      dataSwitch(j);
      Serial.println(dataArray[j]);
      //      Serial.println(dataArray[j]);

      switch (j) {
        case 1:
          pressure = dataArray[j].toFloat();
          //      Serial.print(" Pressure2: "); Serial.println(pressure);
          done = true;
          break;
        case 2:
          Q1 = dataArray[j].toFloat();
          //      Serial.print(" FLow: "); Serial.println(Q1);
          done = true;
          break;
        case 3:
          volume = dataArray[j].toInt();
          //          Serial.print(" volume "); Serial.println(volume);

          if (volume >= 0) {
            vol = volume;
          }

          done = true;
          break;
        case 4:
          alarm = dataArray[j].toInt();
          done = true;
          break;
        case 5:
          ACMode = dataArray[j].toInt();
          done = true;
          break;
        case 6:
          start = dataArray[j].toInt();
          done = true;
          break;
        case 7:
          potensio = dataArray[j];
          done = true;
          break;
        default:
          dataArray[j] = "";
          break;
      }

      j = 0;
    } else {
      dataArray[j] += dataIn[i];
      //      Serial.print("\t J: ");  Serial.println(j);
      //      Serial.print("\t Data: ");  Serial.println(dataArray[1]);
    }
  }

  for (int i = 0; i <= 7; i ++) {
    dataArray[i] = "";
  }
}

//void dataSwitch(int select) {
//  //  int k = 0;
//  //  String daPot[4];
//
//  switch (select) {
//    case 1:
//      pressure = dataArray[select].toFloat();
//      //      Serial.print(" Pressure2: "); Serial.println(pressure);
//      done = true;
//      break;
//    case 2:
//      Q1 = dataArray[select].toFloat();
//      //      Serial.print(" FLow: "); Serial.println(Q1);
//      done = true;
//      break;
//    case 3:
//      volume = dataArray[select].toInt();
//      done = true;
//      break;
//    case 4:
//      alarm = dataArray[select].toInt();
//      done = true;
//      break;
//    case 5:
//      ACMode = dataArray[select].toInt();
//      done = true;
//      break;
//    case 6:
//      start = dataArray[select].toInt();
//      Serial.println(start);
//      //      done = true;
//      break;
//    case 7:
//      potensio = dataArray[select];
//      done = true;
//      break;
//  }
//}
