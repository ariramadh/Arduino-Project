float       a1 = 188.691 * 1e-6,
            a2 = 63.617 * 1e-6;

void read_pressure() {
  //  Serial.println("MEASURE");
  //  tp = millis();
  int analogValue = analogRead(A5);
  pres = (analogValue * 0.00512) - 2.8672; //kPa
  pres *= 10.1971;
  pres = constrain(pres, 0, 100);
  v1 = sqrt((2 * pres / rho) / (pow((a1 / a2), 2) - 1));
  v2 = sqrt((2 * pres / rho) / (1 - (pow((a1 / a2), 2))));

  Q1 = a1 * v1 * 1000;
  Q2 = a2 * v2;
  //  Serial.print("TIMER PRESS: ");
  //  Serial.println(tp-tp_1);

  //  if (pres >= MAX_PRESSURE){
  //    start = 1;
  //    set_alarm(start);
  //    disp_alarm();
  //    Serial.println("max pres");
  //
  //  }
  //  else if ((pres < MIN_PRESSURE) && (tp - tp_1 >= timer_pressure_limit) )
  //  {
  //    tp_1 = tp;
  //    Serial.println("low pres");
  //    start = 1;
  //    set_alarm(start);
  //    disp_alarm();
  //  }

  Serial.print(analogValue);
  Serial.print("  PRESSURE: "); Serial.print(pres);
  Serial.print("  SPEED: "); Serial.print(v1);
  Serial.print("  Area: "); Serial.print(a1);
  Serial.print("  Flow 1: "); Serial.println(Q1);
  //  Serial.print("Flow 2: "); Serial.println(Q2);
  //  Serial.print("t: "); Serial.println(td - td_1);
  //  tp_1 = tp;
  return pres;
}
