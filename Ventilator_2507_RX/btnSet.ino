void modeSet() {
  ACMode = !ACMode;
  zeroingDisplay();
  Serial.print("\tPencet\n");
}

void setStart() {
  start = true;
  Serial.print("\tOTW Start\n");
  hitung(); //Panggil function hitung
  //  start = !start;
  //  btnTick = millis();
}

//void setCount() {
//  Serial.print("\tKalem ");
//  Serial.print(millis () - btnTick);
//  if (millis() - btnTick >= 2000) {
//    Serial.print("\tSip");
//  }
//}
