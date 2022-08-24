// ====================================================
//                PARSING DATA SERIAL (string & int)
// ====================================================

String dataIn = "";
String dt[10];
bool parsing = false;

void readSerial() {
  while (Serial3.available() > 0)
  {
    char inChar = (char)Serial3.read();
    //        Serial.print("\tinChar: ");
    //        Serial.print(inChar);

    dataIn += inChar;

    if (inChar == '*') {
      //      Serial.print("OK");
      parsing = true;
    }
  }

  if (parsing) {
    parsingData();
    parsing = false;
    dataIn = "";
    Serial.print("\n");
  }
}

void parsingData() {
  int j = 0;
  //kirim data yang telah diterima sebelumnya
  Serial.print("\tdata masuk : ");
  Serial.print(dataIn);
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (int i = 0; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    //increment variabel j, digunakan untuk merubah index array penampung
    if (dataIn[i] == 'P') {
      j = 0;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    } else if (dataIn[i] == 'F') {
      j = 1;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }

    //    Serial.print("J:  ");      Serial.print(j);
    //    Serial.print("\tTeks:  ");      Serial.println(dataIn[i]);

    //proses tampung data saat pengecekan karakter selesai.
    dt[j] += dataIn[i];
    //    Serial.print("\tHasil:  ");      Serial.println(dt[j]);
  }

  for (int k = 0; k < 2; k++) {
    dt[k] = dt[k].substring(1);
  }

  //kirim data hasil parsing
  Serial.print("\tData Q1: ");
  Serial.print(dt[1].toFloat());
  Q1 = dt[0].toFloat();
  Serial.print("\tData P: ");
  Serial.print(dt[0].toFloat());
  pressure = dt[1].toFloat();
}
