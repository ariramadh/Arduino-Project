String filename;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 10000;           // interval at which to blink (milliseconds)
unsigned long currentMillis;

void recStart() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  filename = (String)"Data" + waktu + (String)".txt";

  dataRecord = SD.open(filename, FILE_WRITE);
}

void rec() {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (dataRecord) {
      dataRecord.println(jmd + (String)" " + sp[0] + (String)" " + average[0] + (String)" " + sp[1] + (String)" " + average[1] + (String)" " + output1 + (String)" " + output2);
    } else {
      Serial.print((String)"Error opening " + filename + (String)"...");
    }
  }
}

void recStop() {
  dataRecord.close();
  Serial.println("done.");
}

void clickM() {
  if (hum > 0) {
    hum -= 1;
  }
}

void clickP() {
  if (hum < 100) {
    hum += 1;
  }
}

//void longPressM() {
//  if (hum > 0) {
//    hum -= 1;
//  } else {
//    hum = 0;
//  }
//  delay(50);
//}
//
//void longPressP() {
//  if (hum < 100) {
//    hum += 1;
//  } else {
//    hum = 100;
//  }
//  delay(50);
//}

void clickT() {
  digitalWrite(LED_BUILTIN, val);
  val = !val;

  //  btnTare();
}

void longPressSS() {
  running = !running;
}
