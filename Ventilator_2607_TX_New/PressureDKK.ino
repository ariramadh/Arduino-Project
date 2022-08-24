int AVf, AVf_1;
float vol;

String bacaPressure() {
  int analogValue2 = analogRead(A5) - zPressure[1];  //Baca sensor tekanan2

  float pressure = (analogValue2 * 0.00512) - 2.8672;   // kPa
  pressure *= 10.1971;
  pressure = constrain(pressure, 0, 100);
  //
  //   Serial.print(analogValue2);
  //  Serial.print("\t");
  //  Serial.print("flow "); Serial.println(Q);// cmH2O
  return (String)pressure;
}

String bacaFlow() {
  int analogValue = analogRead(A4) - zPressure[0]; //Baca sensor flow

  AVf = (analogValue + a * AVf_1) / (a + 1);
  float Q = (0.407 * AVf - 180.65);
  if (Q < 9) {
    Q = 0; //   l/m
  }
  AVf_1 = AVf;

  return (String)Q;
}

String bacaVolume() {
  unsigned long time = millis();


  if (getVol) {
    float Ts = (time - tprev) / 1000.0;
    vol += (bacaFlow().toFloat() * Ts / 60.0) * 1000;
    tprev = time;
    return (String)vol;
  } else {
    vol = 0;
  }
}
