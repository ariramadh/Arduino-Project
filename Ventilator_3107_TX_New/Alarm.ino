#define alarmPin = 5;

int alarm,
    PLAT_MIN = 5, // 5 cmH2O
    P_RESIST_MAX = 10,
    P_TIDAL_MIN = 5,
    MIN_PRESSURE = 5,
    Vtreshold;

const int normalState = 2200;                   //mV
const int maxCurrentState = 832.5;            //4.5 A * 185 mV

String currentSensor() {
  int analogCurrent = analogRead(A6);
  int voltage = 5000 * analogCurrent / 1023; //mV
  int arus = -0.0054 * voltage + 11.892;    //mA
  return (String)voltage;
}

String bacaAlarm() {
  //  alarm = 0;

  Serial.println(plateau);
  Serial.println(peep);

  if (bacaPressure().toFloat() >= maxPress) { //Alarm tekanan tinggi
    alarm = 2;
    start = false;
    sendHere('s', (String)start);
    //HIGH Pressure
  }

  if (bacaPressure().toFloat() < MIN_PRESSURE && plateau < 5 && count > 2) {
    alarm = 1;
    //LOW Pressure
  }

  if (bacaPressure().toFloat() - plateau > P_RESIST_MAX) {
    alarm = 3;
    //HIGH Resist
    start = false;
    sendHere('s', (String)start);
  }

  if (bacaVolume().toInt() < sendTV() - Vtreshold && count > 2) {
    alarm = 4;
    //VOLUME not delivered
  }

  if (bacaPressure().toFloat() - peep < P_TIDAL_MIN && count > 2) {
    alarm = 5;
    //TIDAL Not Delivered
  }

  if (currentSensor().toInt() <= (normalState - maxCurrentState)) {
    alarm = 6;
    //HIGH Current
    start = false;
    sendHere('s', (String)start);
  }

  if (digitalRead(btnSnooze) == LOW) {
    Serial.println("Snooze");
    alarm = 0;
  }

  digitalWrite(buzzerPin, alarm > 0);
  return (String)alarm;
}
