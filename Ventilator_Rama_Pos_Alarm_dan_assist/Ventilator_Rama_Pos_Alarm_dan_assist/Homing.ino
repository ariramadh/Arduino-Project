void homing() {
  lcd.clear();
  Serial.println("HOMING !! INITIALIZING");
  lcd.setCursor(0, 0); lcd.print("HOMING !!");
  lcd.setCursor(0, 1); lcd.print("INITIALIZING !!");

  if (inOk) {
    rise = true;
    analogWrite(Min2, 70);
    digitalWrite(Min1, LOW);

    delay (3000);
  }

  limState = 0;
  
  while (limState == 0) {
    //membuka hingga limSwitch pressed
    rise = false;
    digitalWrite(Min2, LOW);
    analogWrite(Min1, 70);
    lcd.setCursor(0, 2); lcd.print(limState);
  }
  //Post Home
  indikator[0] = 1; //Indikator apabila homing  selesai

  lcd.setCursor(0, 2); lcd.print(limState);
  rise = true;
  analogWrite(Min2, 70);
  digitalWrite(Min1, LOW);
  delay(700);

  //finger diam
  digitalWrite(Min2, LOW);
  digitalWrite(Min1, LOW);

  delay(1000);
  lcd.clear();
  limState = 0;
  delay(1000);
}
