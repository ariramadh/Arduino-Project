void setAlarm(int sttsAlarm) {
  if (sttsAlarm > 0) {
    if (sttsAlarm == 1) {
      Serial.print("\tLow Pressure  ");
      teksAlarm = "Lo Press   ";
//      alarm = 1;
    }
    if (sttsAlarm == 2) {
      Serial.print("\tHigh Pressure  ");
      teksAlarm = "Hi Press   ";
//      alarm = 2;
      homing();
      start = false;
      inOk = false;
    }
    if (sttsAlarm == 3) {
      Serial.print("\tHigh Current  ");
      teksAlarm = "Hi Current ";
//      alarm = 3;
      homing();
      start = false;
      inOk = false;
    }
    if (sttsAlarm == 4) {
      Serial.print("\tHigh Resist Press ");
      teksAlarm = "Hi Resist ";
//      alarm = 4;
    }
    if (sttsAlarm == 5) {
      teksAlarm = "No Tidal V";
//      alarm = 5;
    }
    if (sttsAlarm == 6) {
      teksAlarm = "Lo Tidal P";
//      alarm = 6;
    }
  } else {
//    Serial.print("\tSystem OK ");
    teksAlarm = "   Running";
  }
  
  digitalWrite(ALARM_PIN, sttsAlarm > 0);
}
