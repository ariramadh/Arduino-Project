void limSwich_Procedure() {
    Serial.println (" limSwitch Pressed!!");
    limState = 1;
    count = 0;
}

void motorEncoder() {
  if (rise) {
    count++;
  } else {
    count--;
  }
}
