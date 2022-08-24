void setAlarm(int sttsAlarm) {
  if (sttsAlarm > 0) {
    if (sttsAlarm == 1) {
      Serial.print("\tLow Pressure  ");
      teksAlarm = "Low Pressure DC?    ";
//      alarm = 1;
    }
    if (sttsAlarm == 2) {
      Serial.print("\tHigh Pressure  ");
      teksAlarm = "High Pressure       ";
//      alarm = 2;
    }
    if (sttsAlarm == 3) {
      Serial.print("\tHigh Current  ");
      teksAlarm = "High Current        ";
//      alarm = 3;
    }
    if (sttsAlarm == 4) {
      Serial.print("\tHigh Resist Press ");
      teksAlarm = " High Resist Press  ";
//      alarm = 4;
    }
    start = false;
  } else {
//    Serial.print("\tSystem OK ");
    teksAlarm = "System OK";
  }
  digitalWrite(ALARM_PIN, sttsAlarm > 0);
}
