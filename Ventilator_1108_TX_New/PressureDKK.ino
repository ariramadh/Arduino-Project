String bacaPressure() {
  int analogValue = analogRead(A7) - zPressure[1];  //Baca sensor tekanan2

  //  float pressure = 0.1323 * (analogValue) - 2.7531;
  //  pressure *= 10.1971;
  float pressure = analogValue / 10.0;
  pressure = constrain(pressure, 0, 100);
  //
  //  Serial.print("\t");
  //    Serial.print("Pressure "); Serial.println(analogRead(A5));// cmH2O

  if (getVol) {
    if (pressurem < pressure) {
      pressurem = pressure;
    }
  } else {
    pressurem = pressure;
  }

  return (String)pressurem;
}

String bacaFlow() {
  int16_t results;

  results = ads.readADC_Differential_0_1();

  //  int analogValue = analogRead(A4) - zPressure[0]; //Baca sensor flow
  //  Serial.println(analogValue);// cmH2O
  //  float Q;

  //  float AVf = (constrain(analogValue, 0, 1024) + a * AVf_1) / (a + 1);

  //  if (analogValue < 458){
  //     Q = 0;
  //  }

  //  Serial.print("\t ADC "); Serial.print(analogValue);// cmH2O
  //  Serial.print("\t AVf "); Serial.print(AVf);// cmH2O
  //  Serial.print("\t AVf_1 "); Serial.print(AVf_1);// cmH2O

  //  AVf_1 = AVf;

  //  AVf = sqrt(AVf);

  //  Serial.print("\t sqrt AVf "); Serial.print(AVf);// cmH2O

  //  Q = -34.347 * pow(AVf, 2) + 1291.1 * AVf - 12104;
  //  Q = 2.5825 * pow(AVf, 3) - 153.79 * pow(AVf, 2) + 3063.5 * AVf - 20368;

  //  if (Q < 6) {
  //    Q = 0; //   l/m
  //  }

  //  Serial.print("Flow "); Serial.print(Q);// cmH2O

  //  Serial.print(analogValue);// cmH2O
  //  Serial.print("\t");// cmH2O
  //  Serial.print(Q);// cmH2O

  //  return (String)Q;
}

String bacaVolume() {
  unsigned long time = millis();

  //  Serial.print("=============");
  //  Serial.print(getVol);

  Serial.print("\t Flow "); Serial.print(bacaFlow().toFloat());// cmH2O

  if (getVol) {
    float Ts = (time - tprev);
    //    Serial.print(" Ts :");
    //    Serial.print(Ts);
    //    Serial.println(" ");
    vol += bacaFlow().toFloat() / Ts;
    tprev = time;
    Serial.print("\t Volume "); Serial.print(vol);// cmH2O
  } else {
    vol = -1;
  }
  return (String)vol;
}
