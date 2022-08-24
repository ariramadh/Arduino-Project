int PLAT_MIN = 5, // 5 cmH2O
P_RESIST_MAX = 10,
P_TIDAL_MIN = 5;
int analogValue3;
float voltage, normalState, maxCurrentState;
void check_alarm(){
  //============= BACA ARUS ==============
  analogValue3 = analogRead(A7);
  voltage = 5000*analogValue3/1023;     //mV
  normalState = 2200;                   //mV
  maxCurrentState = 832,5;              //4.5 A * 185 mV
  Serial.print("  Voltage:  ");
  Serial.print(voltage);
//  delay(100);
  
  //=============Alarm tekanan==============
  if (pressure >= MAX_PRESSURE) { //Alarm tekanan tinggi
    startAlarm = 1;
    alarm = 1;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
    Serial.print("  MAX PRESS!!!");
  }
  else if ((plateau  < PLAT_MIN) && (tp - tp_1 >= timer_pressure_limit) ) //Alarm tekanan rendah
  {
    startAlarm = 1;
    alarm = 2;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else if (pressure - plateau > P_RESIST_MAX){
    startAlarm = 1;
    alarm = 3;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else if (noTidalVolume == true){
    startAlarm = 1;
    alarm = 4;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else if (pressure - peep < P_TIDAL_MIN){
    startAlarm = 1;
    alarm = 5;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else if (voltage <= (normalState - maxCurrentState))
  {
    Serial.print(" Over Current  ");
    start = 1;  //start alarm
    alarm = 6;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else{
    start = 0;
    set_alarm(0);
    lcd_alarm(0);
  }

  //========================================
}

void set_alarm(int startAlarm) {
  Serial.print("start= ");
  Serial.println(startAlarm);

  if (startAlarm == 1) {
    Serial.println("alarm on");
    digitalWrite(ALARM_PIN, HIGH);
  }
  else {
    Serial.println("alarm off");
    digitalWrite(ALARM_PIN, LOW);
  }
}
