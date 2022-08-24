float RC, fc, a;
int AVf, AVf_1;

void read_pressure() {
  tp = millis();

  fc = 15.167;
  RC = 1.0 / (2 * 3.14159 * fc);
  a = RC / 0.02;

  int analogValue = analogRead(A6) - zPressure[0]; //Baca sensor tekanan1
  int analogValue2 = analogRead(A5) - zPressure[1];  //Baca sensor tekanan2

  AVf = (analogValue + a * AVf_1) / (a + 1);

  //  AVf = analogValue;

  //=============Pressure 1==============
  Q1 = (0.407 * AVf - 180.65);
  Serial.print("  Flow 1: "); Serial.print(Q1, 4); //Tampilkan debit

  if (Q1 < 9) {
    Q1 = 0; //   ml/m
  }

  //  pres = ((-0.0603 * analogValue)  + 28.228);    // kPa
  //  pres *= 10.1971;
  //  pres = constrain(pres, 0, 100); //kPa

  //  v1 = sqrt((2 * pres / rho) / (pow((a1 / a2), 2) - 1)); //kecepatan udara 1 //
  //  v2 = sqrt((2 * pres / rho) / (1 - (pow((a1 / a2), 2)))); //kecepatan udara 2 //


  //  Q1 = a1 * v1 * 1000000; //debit udara 1
  //  Q2 = a2 * v2 * 1000000; //debit udara 2

  AVf_1 = AVf;
  //=====================================

  //=============Pressure 2==============
  pressure = ((-0.0603 * analogValue2) + 28.228);    // kPa
  //  pressure *= 10.1971;
  pressure = constrain(pressure, 0, 100);
  if (pressure < 1) {
    pressure = 0;
  }
  //=====================================

  //=============Alarm tekanan==============
  if (pressure >= MAX_PRESSURE) { //Alarm tekanan tinggi
    Serial.print("  MAX PRESS!!!");
    alarm = 2;
  }
  else if (pressure  < MIN_PRESSURE && plateau < 5 && oneCycle) //Alarm tekanan rendah
  {
    Serial.print("  LOW PRESS");
    //    alarm = 1;
  }
  //========================================

  Serial.print("  ADC2: "); Serial.print(analogRead(A5)); //Tampilkan tekanan
  Serial.print("  ADC2F: "); Serial.print(analogValue2); //Tampilkan tekanan
  Serial.print("  PRESSURE2: "); Serial.print(pressure, 4); //Tampilkan tekanan
  //  Serial.print("  V1: "); Serial.print(v1, 4); //Tampilkan tekanan
  Serial.print("  ADC1F: "); Serial.print(analogValue); //Tampilkan tekanan
  Serial.print("  ZPressure: "); Serial.print(zPressure[0]); //Tampilkan tekanan
  Serial.print("  PRESSURE1: "); Serial.print(AVf); //Tampilkan tekanan
  //  Serial.print("  PRESSURE2: "); Serial.print(pressure, 4); //Tampilkan tekanan
  //  Serial.println("  Flow 2: "); Serial.println(Q2);
  //  Serial.println("t: "); Serial.printlnln(td - td_1);
  //  Serial.println("TIMER PRESS: ");
  //  Serial.printlnln(tp-tp_1);
  //  return Q1;
  //  return pressure;
}
