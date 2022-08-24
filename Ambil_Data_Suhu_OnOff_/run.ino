void run()
{
  readDHT();

  float rawT = avgT;
  float rawH = avgH;

  float averageT = adcFilterT.filter(rawT);
  float averageH = adcFilterH.filter(rawH);
  //  Serial.printf("RawT=%d, Filter1=%.3f, RawH=%d, Filter2=%.3f", rawT, filteredT, rawH, filteredH);

  float inputH = spH - averageH;
  float inputT = spT - averageT;

  if (inputT <= 0) {
    relay = HIGH;
  } else if (inputT >= 2) {
    relay = LOW;
  }

  digitalWrite(heater, relay);

  Serial.print("SetPoint:");
  Serial.print(spT);
  Serial.print("\tSuhu: ");
  Serial.print(averageT);
  Serial.print("\tKelembaban: ");
  Serial.print(averageH);
  Serial.print("\tRelay: ");
  Serial.println(relay);

  if (rec) {
    unsigned long currentMillis2 = millis();

    if (currentMillis2 - previousMillis2 >= interval2) {
      previousMillis2 = currentMillis2;

      dataRecord.print("SetPoint\t");
      dataRecord.print(spT);
      dataRecord.print("\tSuhu\t");
      dataRecord.print(averageT);
      dataRecord.print("\tKelembaban\t");
      dataRecord.print(averageH);
      dataRecord.print("\tRelay\t");
      dataRecord.println(relay);
    }

    saveCtrl = true;
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (!rec && saveCtrl) {
    dataRecord.close();
    saveCtrl = false;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("T :");
  lcd.write(averageT);
  lcd.write("C");
  lcd.setCursor(11, 0);
  lcd.write("H:");
  lcd.write(averageH);
  lcd.write("%");

  lcd.setCursor(0, 1);
  lcd.write("SP:");
  lcd.setCursor(4, 1);
  lcd.write(spT);
  lcd.write("C");
  lcd.setCursor(11, 1);
  lcd.write("R:");
  if (relay) {
    lcd.write("Off");
  } else {
    lcd.write("On");
  }
}

void readDHT() {
  delay(500);

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
      Serial.print(("Failed to read from DHT sensor "));
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
}
