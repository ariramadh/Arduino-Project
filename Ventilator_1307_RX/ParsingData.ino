// ====================================================
//                PARSING DATA SERIAL (string & int)
// ====================================================

String dataIn = "";
String dt[10];
bool parsing = false;

void readSerial() {
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '\n') {
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
  int j = 0;
  //kirim data yang telah diterima sebelumnya
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";

  Serial.print("\tData masuk : ");
  Serial.print(dataIn);

  //proses parsing data
  for (int i = 0; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (*) dan (,)
    if (dataIn[i] == 'P') {
      j = 0;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    } else if (dataIn[i] == 'F') {
      j = 1;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    dt[j] += dataIn[i];
  }

  for (int i = 0; i < 2; i++) {
    dt[i] = dt[i].substring(1);
  }

  //kirim data hasil parsing
  Serial.print("\tData Q1: ");
  Serial.print(dt[0].toFloat());
  Q1 = dt[0].toFloat();
  Serial.print("\tData P: ");
  Serial.print(dt[1].toFloat());
  pressure = dt[1].toFloat();
}
