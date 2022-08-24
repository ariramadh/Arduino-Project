#define alarmPin = 5;

int PLAT_MIN = 5, // 5 cmH2O
    P_RESIST_MAX = 10,
    P_TIDAL_MIN = 5,
    MIN_PRESSURE = 5,
    MAX_PRESSURE = 40,
    Vtreshold;

const int normalState = 2200;                   //mV
const int maxCurrentState = 832.5;            //4.5 A * 185 mV

String currentSensor() {
  int analogCurrent = analogRead(A7);
  int voltage = 5000 * analogCurrent / 1023; //mV
  int arus = -0.0054 * voltage + 11.892;    //mA
  return (String)voltage;
}

String bacaAlarm() {
  int alarm;

  if (bacaPressure().toFloat() >= MAX_PRESSURE) { //Alarm tekanan tinggi
    alarm = 2;
  } else  if (bacaPressure().toFloat() < MIN_PRESSURE && plateau < 5) {
    //Alarm tekanan rendah{
    alarm = 1;
  } else if (bacaPressure().toFloat() - plateau > P_RESIST_MAX) {
    alarm = 3;
  } else if (bacaVolume().toInt() < sendTV() - Vtreshold) {
    alarm = 4;
  } else if (bacaPressure().toFloat() - peep < P_TIDAL_MIN) {
    alarm = 5;
  } else if (currentSensor().toInt() <= (normalState - maxCurrentState)) {
    alarm = 6;
  }

  if(btnSnooze == HIGH){
    alarm = 0;    
  }
  
  digitalWrite(buzzerPin, alarm>0);
  return (String)alarm;
}
