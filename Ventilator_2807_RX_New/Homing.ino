void homing() {
  limState = false;

  Serial.print("HOMING !! INITIALIZING");

  //  lcd.clear();

  //  lcd.setCursor(0, 0); lcd.print("HOMING !!");
  //  lcd.setCursor(0, 1); lcd.print("INITIALIZING !!");

  delay(2000);
  //


  Serial.print(".");

  if (inOk) {
    //  if (!Hom.isReady()) {
    rise = true;
    analogWrite(Min2, 70);
    digitalWrite(Min1, LOW);
    delay (3000);
  }

  //  Serial.end();
  Serial.print(".");
  //  }

  while (!limState) {
    //membuka hingga limSwitch pressed
    //    Serial.println(count);
    rise = false;
    digitalWrite(Min2, LOW);
    analogWrite(Min1, 70);
    //    lcd.setCursor(0, 2); lcd.print(limState);
  }

  Serial.print(".");

  //Post Home
  indikator[0] = 1; //Indikator apabila homing  selesai

  //  lcd.setCursor(0, 2); lcd.print(limState);
  rise = true;
  analogWrite(Min2, 70);
  digitalWrite(Min1, LOW);
  delay(700);

  Serial.print(".");

  //finger diam
  digitalWrite(Min2, LOW);
  digitalWrite(Min1, LOW);

  Serial.print(".");

  delay(1000);
  //  lcd.clear();
  //  limState = 0;
  Serial.println("/h#");
  Serial.println("/h#");
  Serial.println("/h#");

  Serial.print(".");

  delay(1000);
}
