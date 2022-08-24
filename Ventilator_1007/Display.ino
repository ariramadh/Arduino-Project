//void disp(){
//  td = millis();
//  if (td - td_1 > interval_limit){
//    lcd_disp();
//
//    td_1 = td;
//  }
//  else {
//    td = td;
//    lcd.clear();
//  }
//  td_1 = 0;
//}

void lcd_disp(float plateau, float pressure, float peep) {
//  Serial.print("Start:  ");
//  Serial.println(startAlarm);
  //  lcd.clear();
//  Serial.print("\tDISP OK\t");

  lcd.setCursor(0, 0); lcd.print("AC:");
  lcd.setCursor(3, 0); lcd.print(AC);
  lcd.setCursor(12, 0); lcd.print("P(cmH2O)");
  lcd.setCursor(0, 1); lcd.print("TV:");
  lcd.setCursor(3, 1); lcd.print(convInt(TV));
  lcd.setCursor(0, 2); lcd.print("RR:");
  lcd.setCursor(3, 2); lcd.print(RR);
  lcd.setCursor(0, 3); lcd.print("IE 1:");
  lcd.setCursor(5, 3); lcd.print(IE, 0);
  lcd.setCursor(10, 1); lcd.print("Peak:");
  lcd.setCursor(15, 1); lcd.print(pressure, 3);
  lcd.setCursor(10, 2); lcd.print("Plat:");
  lcd.setCursor(15, 2); lcd.print(plateau, 3);
  lcd.setCursor(10, 3); lcd.print("Peep:");
  lcd.setCursor(15, 3); lcd.print(peep, 3);
  td_1 = td;
}

void disp_alarm() {
  td = millis();
  if (td - td_1 > interval_limit) {
    //    lcd_alarm();

    td_1 = td;
  }
  else {
    td = td;
  }
  td_1 = 0;
}

void lcd_alarm(int alarm) {
  lcd.setCursor (4, 0);
  Serial.print("  Alarm: ");
  Serial.print(alarm);
  Serial.print("");
  if (start == 1) {
    if (alarm == 1) {
      lcd.print ("HIGH PRESSURE");
    }
    else if (alarm == 2) {
      lcd.print("LOW PRESSURE");
    }
    else if (alarm == 3) {
      lcd.print("OVER CURRENT");
    }
    else if (alarm == 0) {
      lcd.setCursor(0, 0); lcd.print("AC:");
      lcd.setCursor(3, 0); lcd.print(AC);
      lcd.setCursor(4, 0); lcd.print("  ");
      lcd.setCursor(12, 0); lcd.print("P(cmH2O)");
    }
  }
}
