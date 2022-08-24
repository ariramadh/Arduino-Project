

void check_CurrentAlarm() {
  analogValue3 = analogRead(A7);
  voltage = 5000*analogValue3/1023;     //mV
  normalState = 2200;                   //mV
  maxCurrentState = 832,5;              //4.5 A * 185 mV
  if (voltage <= (normalState - maxCurrentState))
  {
    Serial.print(" Over Current  ");
    start = 1;  //start alarm
    alarm = 3;
  }
  else{
    start = 0;
  }

  
  // print out the value you read:
  
  Serial.print("  Voltage:  ");
  Serial.print(voltage);
  delay(100);        // delay in between reads for stability
}
