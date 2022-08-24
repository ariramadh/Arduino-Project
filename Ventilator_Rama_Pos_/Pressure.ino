float RC, fc, a;
int AVf, AVf_1;

void read_pressure() {
  tp = millis();

  fc = 15.167;
  RC = 1.0 / (2 * 3.14159 * fc);
  a = RC / 0.02;

  int analogValue = analogRead(A6) - 103; //Baca sensor tekanan1
  int analogValue2 = analogRead(A5) - 93;  //Baca sensor tekanan2

  AVf = (analogValue + a * AVf_1) / (a + 1);

  //=============Pressure 1==============
  pres = ((-0.0603 * analogValue)  + 28.228);    // kPa
  pres *= 10.1971;
  pres = constrain(pres, 0, 100); //kPa

  v1 = sqrt((2 * pres / rho) / (pow((a1 / a2), 2) - 1)); //kecepatan udara 1 //
  v2 = sqrt((2 * pres / rho) / (1 - (pow((a1 / a2), 2)))); //kecepatan udara 2 //

  Q1 = (0.407 * AVf - 180.65);
  if (Q1 < 9) {
    Q1 = 0; //   l/m
  }

  //  Q1 = a1 * v1 * 1000000; //debit udara 1
  //  Q2 = a2 * v2 * 1000000; //debit udara 2
  //=====================================

  //=============Pressure 2==============
  pressure = (analogValue2 * 0.00512) - 2.8672;   // kPa
  pressure *= 10.1971;                            // cmH2O
  pressure = constrain(pressure, 0, 100);

  AVf_1 = AVf;
  //=====================================


  //=============Alarm tekanan==============
  if (pressure >= MAX_PRESSURE) { //Alarm tekanan tinggi
    startAlarm = 1;
    alarm = 1;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
    Serial.print("  MAX PRESS!!!");
  }
  else if ((pressure  < MIN_PRESSURE) && (tp - tp_1 >= timer_pressure_limit) ) //Alarm tekanan rendah
  {
    tp_1 = tp;
    //    Serial.print("  LOW PRESS");
    startAlarm = 1;
    alarm = 2;
    set_alarm(startAlarm);
    lcd_alarm(alarm);
  }
  else {
    set_alarm(0);
    lcd_alarm(0);
  }
  //========================================

  Serial.print("  ADC: "); Serial.print(analogValue); //Tampilkan tekanan
  Serial.print("  PRESSURE: "); Serial.print(pres, 4); //Tampilkan tekanan
  Serial.print("  V1: "); Serial.print(v1, 4); //Tampilkan tekanan
  Serial.print("  Flow 1: "); Serial.print(Q1, 4); //Tampilkan debit
  Serial.print("  PRESSURE2: "); Serial.print(pressure, 4); //Tampilkan tekanan
  //  Serial.println("  Flow 2: "); Serial.println(Q2);
  //  Serial.println("t: "); Serial.printlnln(td - td_1);
  //  Serial.println("TIMER PRESS: ");
  //  Serial.printlnln(tp-tp_1);
//  return Q1;
//  return pressure;
}
