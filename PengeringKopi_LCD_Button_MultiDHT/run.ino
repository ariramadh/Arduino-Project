void run()
{
  delay(1500);

  totalT = 0;
  totalH = 0;
  
  // Baca kelembaban %
  valH[0] = dht0.readHumidity();
  // Baca temperatur celcius
  valT[0] = dht0.readTemperature();

  // Baca kelembaban %
  valH[1] = dht1.readHumidity();
  // Baca temperatur celcius
  valT[1] = dht1.readTemperature();

  // Baca kelembaban %
  valH[2] = dht2.readHumidity();
  // Baca temperatur celcius
  valT[2] = dht2.readTemperature();

  // Baca kelembaban %
  valH[3] = dht3.readHumidity();
  // Baca temperatur celcius
  valT[3] = dht3.readTemperature();

  for (int i = 0; i < jumlahDHT; i++) {
    if (isnan(valT[i]) || isnan(valH[i])) {
      Serial.print(F("Failed to read from DHT sensor number "));
      Serial.print(i);
      Serial.println("!");
      return;
    }

    totalT += valT[i];
    totalH += valH[i];

    if (i == jumlahDHT - 1) {
      avgT = totalT / jumlahDHT;
      avgH = totalH / jumlahDHT;
    }
  }

  float rawT = avgT;
  float rawH = avgH;

  float averageT = adcFilterT.filter(rawT);
  float averageH = adcFilterH.filter(rawH);
  //  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

  float inputH = spH - averageH;
  float inputT = spT - averageT;
  float output1;
  int output2;

  fuzzy->setInput(1, inputT);  //Masukan input Fuzzy 1
  fuzzy->setInput(2, inputH);  //Masukan input Fuzzy 2
  fuzzy->fuzzify(); //proses fuzifikasi
  output1 = fuzzy->defuzzify(1); //proses defuzifikasi 1 & masukan ke output
  output2 = fuzzy->defuzzify(2); //

  myservo.write(map(output1, 0, 100, 0, 255));   // sets the servo position according to the scaled value

  //  if (inputT < 1) {
  //    output1 = 100;
  //    digitalWrite(heater, HIGH);
  //  } else {
  //    digitalWrite(heater, LOW);
  //  }

  Serial.print("SetPoint:");
  Serial.print(spT);
  Serial.print("\tSelisih suhu: ");
  Serial.print(inputT);

  Serial.print("\tOutput: ");
  Serial.println(output1);
  Serial.println("");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("T:");
  lcd.write(avgT);
  lcd.write("C");
  lcd.setCursor(11, 0);
  lcd.write("S:");
  lcd.write(spT);
  lcd.write("C");

  lcd.setCursor(0, 1);
  lcd.write("H:");
  lcd.setCursor(4, 1);
  lcd.write(avgH);
  lcd.write("%");
  lcd.setCursor(11, 1);
  lcd.write("K:");
  lcd.write("xxx");
  lcd.write("%");
}
