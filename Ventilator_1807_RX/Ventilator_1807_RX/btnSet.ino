void modeSet() {
  ACMode = !ACMode;
  zeroingDisplay();
}

void setStart() {
  Serial.print("\tOTW Start\n");
  hitung(); //Panggil function hitung
  start = !start;
  //  btnTick = millis();
}

//void setCount() {
//  Serial.print("\tKalem ");
//  Serial.print(millis () - btnTick);
//  if (millis() - btnTick >= 2000) {
//    Serial.print("\tSip");
//  }
//}
