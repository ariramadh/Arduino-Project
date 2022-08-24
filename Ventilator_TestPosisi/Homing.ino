void homing() {
  delay(2000);

  rise = true;
  analogWrite(Min2, 70);
  digitalWrite(Min1, LOW);
  delay (2000);

  limState = 0;

  do {
    //membuka hingga limSwitch pressed
    Serial.print(count);
    Serial.println("\tEX");
    rise = false;
    digitalWrite(Min2, LOW);
    analogWrite(Min1, 70);
    count = 0;
    //    lcd.setCursor(0, 2); lcd.print(limState);
  } while (limState == 0);

  Serial.print(".");

  //Post Home
  //  indikator[0] = 1; //Indikator apabila homing  selesai

  rise = true;
  analogWrite(Min2, 70);
  digitalWrite(Min1, LOW);
  delay(700);

  Serial.print(".");

  //finger diam
  digitalWrite(Min2, LOW);
  digitalWrite(Min1, LOW);

  delay(1000);
}
