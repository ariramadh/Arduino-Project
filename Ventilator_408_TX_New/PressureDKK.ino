int AVf, AVf_1;
float vol;

String bacaPressure() {
  int analogValue = analogRead(A5) - zPressure[1];  //Baca sensor tekanan2

//  float pressure = (analogValue * 0.00512) - 2.8672;   // kPa
//  pressure *= 10.1971;
  float pressure = analogValue / 10.0;   // kPa
  
  pressure = abs(pressure);
  
  //  Serial.print("\t");
//  Serial.print("Pressure "); Serial.println(analogRead(A5));// cmH2O

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
  int analogValue = analogRead(A4) - zPressure[0]; //Baca sensor flow

  zPressure[0];

  AVf = (analogValue + a * AVf_1) / (a + 1);
  float Q = (0.5895 * AVf - 265.63);

  //  Serial.print(analogValue);// cmH2O
  //  Serial.print("\t");// cmH2O
  //  Serial.print(Q);// cmH2O

  //  Q -= 9;
  if (Q < 9) {
    Q = 0; //   l/m
  }

  AVf_1 = AVf;


  //  Serial.print("Flow "); Serial.println(Q);// cmH2O

  return (String)Q;
}

String bacaVolume() {
  unsigned long time = millis();

  //  Serial.print("=============");
  //  Serial.print(getVol);

  if (getVol) {
    float Ts = (time - tprev) / 1000.0;
    vol += (bacaFlow().toFloat() * Ts / 60.0) * 1000;
    tprev = time;
  } else {
    vol = -1;
  }
  return (String)vol;
}
