void setAlarm(int sttsAlarm) {
  if (sttsAlarm > 0) {
    if (sttsAlarm == 1) {
      Serial.println("  Low Pressure  ");
      teksAlarm = "Low Pressure DC?    ";
//      alarm = 1;
    }
    if (sttsAlarm == 2) {
      Serial.println("  High Pressure  ");
      teksAlarm = "High Pressure       ";
//      alarm = 2;
    }
    if (sttsAlarm == 3) {
      Serial.println("  High Current  ");
      teksAlarm = "High Current        ";
//      alarm = 3;
    }
    if (sttsAlarm == 4) {
      Serial.println("  High Resist Press ");
      teksAlarm = " High Resist Press  ";
//      alarm = 4;
    }
    start = false;
  } else {
    Serial.print("  System OK ");
    teksAlarm = "System OK";
  }
  digitalWrite(ALARM_PIN, sttsAlarm > 0);
}
