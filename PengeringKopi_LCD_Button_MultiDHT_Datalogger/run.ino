void run()
{
  time();
  delay(1500);

  total[0] = 0;
  total[1] = 0;
  
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

    total[0] += valT[i];
    total[1] += valH[i];

    if (i == jumlahDHT - 1) {
      avg[0] = total[0] / jumlahDHT;
      avg[1] = total[1] / jumlahDHT;
    }
  }

  float raw[] = {avg[0],avg[1]};

//  Dengan Filter
//  average[0] = adcFilterT.filter(raw[0]);
//  average[1] = adcFilterH.filter(raw[1]);
  //  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

//  Tanpa Filter
  average[0] = raw[0];
  average[1] = raw[1];

  float input[] = {sp[0]-average[0], sp[1]-average[1]};

  fuzzy->setInput(1, input[0]);  //Masukan input Fuzzy 1
  fuzzy->setInput(2, input[1]);  //Masukan input Fuzzy 2
  fuzzy->fuzzify(); //proses fuzifikasi
  output1 = fuzzy->defuzzify(1); //proses defuzifikasi 1 & masukan ke output
  output2 = fuzzy->defuzzify(2); //

  //  if (inputT < 1) {
  //    output1 = 100;
  //    digitalWrite(heater, HIGH);
  //  } else {
  //    digitalWrite(heater, LOW);
  //  }

  Serial.print("SetPoint:");
  Serial.print(sp[0]);
  Serial.print("\tSelisih suhu: ");
  Serial.print(input[0]);

  Serial.print("\tOutput: ");
  Serial.println(output1);
  Serial.println("");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(avg[0]);
  lcd.print("C");
  lcd.setCursor(11, 0);
  lcd.print("S:");
  lcd.print(sp[0]);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.setCursor(4, 1);
  lcd.print(avg[1]);
  lcd.print("%");
  lcd.setCursor(11, 1);
  lcd.print("K:");
  lcd.print("xxx");
  lcd.print("%");
}
