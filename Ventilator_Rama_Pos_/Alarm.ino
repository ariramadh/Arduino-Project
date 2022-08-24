void set_alarm(int startAlarm) {

//  Serial.print("start= ");
  Serial.println(startAlarm);

  if (startAlarm == 1) {
//    Serial.println("alarm on");
    digitalWrite(ALARM_PIN, HIGH);
  }
  else {
//    Serial.println("alarm off");
    digitalWrite(ALARM_PIN, LOW);
  }
}
