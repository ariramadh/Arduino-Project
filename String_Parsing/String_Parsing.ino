// ====================================================
//                PARSING DATA SERIAL (string & int)
// ====================================================

String dataIn;
String dt[10];
int i;
boolean parsing = false;
boolean request = false;
boolean send = true;

void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200);

  Serial.println("RUNNING....");
  dataIn = "";
}

void loop()
{
  request = true;
  send = request;

  if (request) {
    if (send) {
      Serial3.write("#");
      //      Serial.println("Requesting Data...");
    }
    //  Serial.print("Serial Stat: ");
    //  Serial.print(Serial3.available() > 0);
    while (Serial3.available() > 0) {
      char inChar = (char)Serial3.read();
      //      String inString = Serial3.readStringUntil('*');
      //    Serial.print("\tinChar: ");
      //    Serial.print(inChar);

      dataIn += inChar;
      //      dataIn = inString;

      if (inChar == '*') {
        Serial.println("OK");
        parsing = true;
      }
    }

    if (parsing) {
      parsingData();
      parsing = false;
      send = false;
      dataIn = "";
      Serial.print("\n");
    }
    request = false;
  }
}

void parsingData()
{
  int j = 0;
  //kirim data yang telah diterima sebelumnya
  Serial.print("\tdata masuk : ");
  Serial.print(dataIn);
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 0; i < dataIn.length(); i++)
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
  Serial.print("\t\tdata 1 : ");
  Serial.print(dt[0].toFloat());
  Serial.print("\tdata 2 : ");
  Serial.print(dt[1].toFloat());
}
