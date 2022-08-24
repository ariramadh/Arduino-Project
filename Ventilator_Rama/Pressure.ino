void read_pressure() {
  tp = millis();

  int analogValue = analogRead(A5) - 91; //Baca sensor tekanan1
  int analogValue2 = analogRead(A6) - 91; //Baca sensor tekanan2

  //=============Pressure 1==============
  pres = ((-0.0603 * analogValue)  + 28.461);

  pres = constrain(pres, 0, 100); //Membatasi nilai bacaan tekanan
  //  pres = EWMA(pres, 0, 0.1);  //Memfilter pres, (variabel proses, urutan, alpha untuk menentukan besar nilai filter)

  v1 = sqrt((2 * pres / rho) / (pow((a1 / a2), 2) - 1)); //kecepatan udara 1 //
  v2 = sqrt((2 * pres / rho) / (1 - (pow((a1 / a2), 2)))); //kecepatan udara 2 //
  Q1 = a1 * v1 * 1000000; //debit udara 1
  Q2 = a2 * v2 * 1000000; //debit udara 2
  //=====================================

  //=============Pressure 2==============
  pressure = (analogValue2 * 0.00512) - 2.8672; //Konversi sensor tekanan kPa
  pressure *= 10.1971;  //Konversi sensor tekanan
  pressure = constrain(pressure, 0, 100); //Membatasi nilai bacaan tekanan
  pressure = EWMA(pressure, 2, 0.3);  //Memfilter pressure
  //=====================================


  //=============Alarm tekanan==============
  if (pressure >= MAX_PRESSURE) { //Alarm tekanan tinggi
    start = 1;
    set_alarm(start);
    disp_alarm();
    Serial.print("  MAX PRESS!!!");
  }
  else if ((pressure  < MIN_PRESSURE) && (tp - tp_1 >= timer_pressure_limit) ) //Alarm tekanan rendah
  {
    tp_1 = tp;
    //    Serial.print("  LOW PRESS");
    start = 1;
    set_alarm(start);
    disp_alarm();
  }
  //========================================

  Serial.print("  ADC: "); Serial.print(analogValue); //Tampilkan tekanan
  //  Serial.println("TIMER PRESS: ");
  //  Serial.printlnln(tp-tp_1);

  Serial.print("  V1: "); Serial.print(v1, 8); //Tampilkan tekanan
  Serial.print("  PRESSURE: "); Serial.print(pres, 8); //Tampilkan tekanan
  Serial.print("  Flow 1: "); Serial.print(Q1, 4); //Tampilkan debit
  //  Serial.println("  Flow 2: "); Serial.println(Q2);
  //  Serial.println("t: "); Serial.printlnln(td - td_1);
}
