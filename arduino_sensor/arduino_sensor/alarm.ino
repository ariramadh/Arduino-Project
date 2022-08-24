String teksAlarm;
const int ALARM_PIN = 5;
int PLAT_MIN = 5, // 5 cmH2O
    P_RESIST_MAX = 10,
    P_TIDAL_MIN = 5,
    MIN_PRESSURE = 5,
    MAX_PRESSURE = 40,
    Vtreshold;
int analogValue3;
float voltage, normalState, maxCurrentState, arus;

void check_alarm(){
  //============= BACA ARUS ==============
  analogValue3 = analogRead(A7);
  voltage = 5000*analogValue3/1023;     //mV
  normalState = 2200;                   //mV
  maxCurrentState = 832,5;              //4.5 A * 185 mV
  arus = -0.0054*voltage + 11.892;      //mA
//  Serial.print("  Arus:  ");
//  Serial.println(arus);
//  delay(100);
  
  //=============Alarm tekanan==============
  

  if (pressure >= MAX_PRESSURE) { //Alarm tekanan tinggi
    alarm = 2; Serial.print("/a/#");
  }
  else  if (pressure  < MIN_PRESSURE && plateau < 5 && oneCycle){ 
    //Alarm tekanan rendah{
    alarm = 1; Serial.print("/a/#");
  }
  else if (pressure - plateau > P_RESIST_MAX){
    alarm = 3; Serial.print("/a/#");
  }
  else if (vol < TV - Vtreshold){
    alarm = 4; Serial.print("/a/#");
  }
  else if (pressure - peep < P_TIDAL_MIN){
    alarm = 5; Serial.print("/a/#");
  }
  else if (voltage <= (normalState - maxCurrentState)){
    alarm = 6; Serial.print("/a/#");
  }
}


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
    Serial.print("\tSystem OK ");
    teksAlarm = "System OK";
  }
  digitalWrite(ALARM_PIN, sttsAlarm > 0);
}
