#define alarmPin = 5;

int alarm,
    PLAT_MIN = 5, // 5 cmH2O
    P_RESIST_MAX = 10,
    P_TIDAL_MIN = 5,
    MIN_PRESSURE = 5,
    Vtreshold;

int lastOutput[]= {443,443};

const int normalState = 2155; //mV
const int maxCurrent = 500;  //mA

String currentSensor() {
  int analogCurrent = ewma(analogRead(A6), 0);
  int voltage = (analogCurrent / 1023.0) * 5000; //mV
  int arus = (normalState - voltage) / 0.110;    //mA
//    Serial.print(" ADC: "); Serial.println(analogCurrent);
//    Serial.print(" Voltage: "); Serial.println(voltage);
//    Serial.print(" Arus: "); Serial.println(arus);
  return (String)arus;
}

int ewma(int raw, int num) {
  float alpha = 0.1;
  int average = alpha * raw + (1 - alpha) * lastOutput[num];

  lastOutput[num] = average;

  return average;
}

String bacaAlarm() {
  //  alarm = 0;

  if (bacaPressure().toFloat() < MIN_PRESSURE && plateau < 5 && count > 2) {
    alarm = 1;
    //LOW Pressure
  }

  if (bacaPressure().toFloat() >= maxPress) { //Alarm tekanan tinggi
    alarm = 2;
    //    start = false;
    //    sendHere('s', (String)start);
    //HIGH Pressure
  }

  if (bacaPressure().toFloat() - plateau > P_RESIST_MAX) {
    alarm = 3;
    //HIGH Resist
    //    start = false;
    //    sendHere('s', (String)start);
  }

  if (bacaVolume().toInt() < sendTV() - Vtreshold && count > 2) {
    alarm = 4;
    //VOLUME not delivered
  }

  if (bacaPressure().toFloat() - peep < P_TIDAL_MIN && count > 2) {
    alarm = 5;
    //TIDAL Not Delivered
  }

  if (currentSensor().toInt() <= maxCurrent) {
    alarm = 6;
    //HIGH Current
    //    start = false;
    //    sendHere('s', (String)start);
  }

  if (digitalRead(btnSnooze) == LOW) {
    //    Serial.println("Snooze");
    alarm = 0;
  }


  digitalWrite(buzzerPin, alarm > 0);
  return (String)alarm;
}
