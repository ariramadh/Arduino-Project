int analogValue3, normalState;
float voltage, maxCurrentState;

bool check_CurrentAlarm() {
  analogValue3 = analogRead(A0);
  voltage = 5000*analogValue3/1023;     //mV
  normalState = 2200;                   //mV
  maxCurrentState = 832,5;              //4.5 A * 185 mV
  if (voltage <= (normalState - maxCurrentState))
  {
    Serial.print("OVER CURRENT");
//    alarm = true;
  }
  
  // print out the value you read:
  
//  Serial.print("  Voltage:  ");
//  Serial.print(voltage);

  return (voltage <= (normalState - maxCurrentState));
}
