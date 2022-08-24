float read_pressure() {
  //  Serial.println("MEASURE");
  tp = millis();
  int analogValue = analogRead(A5) - 93;
  int analogValue2 = analogRead(A6);

  pres = ((-0.0603 * analogValue)  + 28.461);

  float alpha = 0.8;

  float average = alpha * pres + (1 - alpha) * lastOutput; //Persamaan EWMA dalam baris program

  lastOutput = average;  //Masukan data suhu yang terfilter kedalam variabel lastHoutput

  //  pres = round(average * 100) / (float)100;  //Untuk membulatkan nilai AverageH ke 2 angka dibelakang koma

  //  pres *= 10.1971;  //
  //  Serial.print("  PRESSUREb: ");Serial.print(pres, 5);
  pres = constrain(pres, 0, 100); //kPa
//  Serial.print("  PRESSUREa: "); Serial.print(pres, 5);
  v1 = sqrt((2 * pres / rho) / (pow((a1 / a2), 2) - 1)); // m/s
  v2 = sqrt((2 * pres / rho) / (1 - (pow((a2 / a1), 2)))); // m/s
  Q1 = a1 * v1;                                   // m3/s
  Q2 = a2 * v2;                                   // m3/s
  Q1 = Q1 * 1000000;                              // ml/s
  Q2 = Q2 * 1000000;                              // ml/s

//  Q1av = vol / 

  pressure = (analogValue2 * 0.00512) - 2.8772;   // kPa
  pressure *= 10.1971;                            // cmH2O
  pressure = constrain(pressure, 0, 100);
  //  Serial.print("TIMER PRESS: ");
  //  Serial.println(tp-tp_1);
  //  if (pressure >= MAX_PRESSURE){
  //    start = 1;
  //    set_alarm(start);
  //    disp_alarm();
  //    Serial.println("max pres");
  //
  //  }
  //  else if ((pressure < MIN_PRESSURE) && (tp - tp_1 >= timer_pressure_limit) )
  //  {
  //    tp_1 = tp;
  //    Serial.println("low pres");
  //    start = 1;
  //    set_alarm(start);
  //    disp_alarm();
  //  }

//  Serial.print("  ");  Serial.print(analogValue);
//  Serial.print("  ti: "); Serial.print(ti);
  Serial.print("  Volume(t-1): "); Serial.print(vol);
//  Serial.print("  V1: "); Serial.print(v1, 3);
//  Serial.print("  V2: "); Serial.print(v2);
//  Serial.print("  Flow 1: "); Serial.print(Q1, 3);
//  Serial.print("  Flow 2: "); Serial.print(Q2);
//  Serial.print("  Q1av: "); Serial.print(Q1av, 3);
  //  Serial.print("t: "); Serial.println(td - td_1);
  //  tp_1 = tp;
  return Q1;
  return pressure;
}
