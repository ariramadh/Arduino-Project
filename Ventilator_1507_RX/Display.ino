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

void lcdTeks() {
  if (TV != TVb) {
    Serial.println("aPDET");
    if (start) {
      lcd.setCursor(12, 0); lcd.print("P(cmH2O)");
      lcd.setCursor(10, 1); lcd.print("Peak:");
      lcd.setCursor(10, 2); lcd.print("Plat:");
      lcd.setCursor(10, 3); lcd.print("Peep:");
    } else {
      lcd.setCursor(0, 0); lcd.print("PR:");
      lcd.setCursor(0, 1); lcd.print("TV:");
      lcd.setCursor(0, 2); lcd.print("RR:");
      lcd.setCursor(0, 3); lcd.print("IE 1:");
      if (ACMode) {
        lcd.setCursor(10, 0); lcd.print("AC Mode ");
        lcd.setCursor(10, 1); lcd.print("AC:-");
      } else {
        lcd.setCursor(10, 0); lcd.print("Vol Mode");
      }
    }
  }
}

void lcd_disp(float plateau, float pressure, float peep) {
  //    lcd.clear();
  //  Serial.print("\tDISP OK\t");


  if (start) {
    if (pressure != pressureb) {
      lcd.setCursor(15, 1); lcd.print(pressure, 3);
      pressureb = pressure;
    }

    if (plateau != plateaub ) {
      lcd.setCursor(15, 2); lcd.print(plateau, 3);
      plateaub = plateau;
    }

    if (peep != peepb) {
      lcd.setCursor(15, 3); lcd.print(peep, 3);
      peepb = peep;
    }
  } else {
    if (pressure != pressureb) {
      lcd.setCursor(15, 1); lcd.print(pressure, 3);
      pressureb = pressure;
    }

    if (Q1 != Q1b) {
      lcd.setCursor(15, 2); lcd.print(Q1, 3);
      Q1b = Q1;
    }

    if (TV != TVb) {
      lcd.setCursor(3, 1); lcd.print(convInt(TV));
      TVb = TV;
    }

    if (RR != RRb) {
      lcd.setCursor(3, 2); lcd.print(RR);
      if (RR < 10) {
        lcd.setCursor(4, 2); lcd.print(" ");
      }
      RRb = RR;
    }

    if (IE != IEb) {
      lcd.setCursor(5, 3); lcd.print(IE, 0);
      IEb = IE;
    }
    
    if (AC != ACb) {
      if (ACMode) {
        lcd.setCursor(14, 1); lcd.print(AC);
        ACb = AC;
      }
    }

    if (alarm == 0) {
      lcd.setCursor (4, 0);
      Serial.print("");

      if (PR != PRb) {
        lcd.setCursor(3, 0); lcd.print(PR);
        PRb = PR;
      }

      //    lcd.setCursor(4, 0); lcd.print("  ");
    } else {
      lcd.setCursor(0, 0);
      //    lcd.print ("                    ");
      lcd.print (teksAlarm);
    }
  }

  td_1 = td;

}

void zeroingDisplay() {
  TVb = 0;
  RRb = 0;
  IEb = 0;
  PRb = 0;
  ACb = 0;
  pressureb = 0;
  plateaub = 0;
  peepb = 0;
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

//void lcd_alarm(String alarm) {
//  lcd.setCursor (4, 0);
//  Serial.print("");
//  lcd.setCursor(0, 0); lcd.print("AC:");
//  lcd.setCursor(3, 0); lcd.print(AC);
//  lcd.setCursor(4, 0); lcd.print("  ");
//  lcd.setCursor(12, 0); lcd.print("P(cmH2O)");
//}
