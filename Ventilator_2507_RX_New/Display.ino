void lcdTeks() {
  lcd.clear();

  if (start) {
    lcd.setCursor(0, 0); lcd.print("PIP ="); lcd.setCursor(11, 0); lcd.print("Tin");
    lcd.setCursor(0, 1); lcd.print("Plat="); lcd.setCursor(11, 1); lcd.print("Tout");
    lcd.setCursor(0, 2); lcd.print("Peep="); lcd.setCursor(11, 2); lcd.print("Vol");
    lcd.setCursor(0, 3); lcd.print("Flow=");
  } else {
    lcd.setCursor(0, 0); lcd.print("PR="); lcd.setCursor(6, 0); lcd.print("cmH2O");
    lcd.setCursor(0, 1); lcd.print("TV="); lcd.setCursor(7, 1); lcd.print("ml");
    lcd.setCursor(0, 2); lcd.print("RR="); lcd.setCursor(6, 2); lcd.print("bpm");
    lcd.setCursor(0, 3); lcd.print("IE=1:");
    if (ACMode) {
      lcd.setCursor(12, 1); lcd.print("AC Mode ");
      lcd.setCursor(10, 2); lcd.print("AC:-");
    } else {
      lcd.setCursor(12, 1); lcd.print("Vol Mode");
    }
  }
}

void lcdDisp() {
  if (start) {
    if (pip != pipb) {
      lcd.setCursor(5, 0); lcd.print(pip, 3);
      pipb = pip;
    }

    if (plateau != plateaub ) {
      lcd.setCursor(5, 1); lcd.print(plateau, 3);
      plateaub = plateau;
    }

    if (peep != peepb) {
      lcd.setCursor(5, 2); lcd.print(peep, 3);
      peepb = peep;
    }

    if (Q1 != Q1b) {
      lcd.setCursor(5, 3); lcd.print(Q1, 3);
      Q1b = Q1;
    }

    if (tIn != tInb) {
      lcd.setCursor(16, 0); lcd.print(tIn, 3);
      tInb = tIn;
    }

    if (tEx != tExb) {
      lcd.setCursor(16, 1); lcd.print(tEx, 3);
      tExb = tEx;
    }

    if (volume != volumeb) {
      lcd.setCursor(16, 2); lcd.print(volume, 3);
      volumeb = volume;
    }
  } else {
    if (TV != TVb) {
      lcd.setCursor(3, 1); lcd.print(TV);
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

    if (PR != PRb) {
      lcd.setCursor(3, 0); lcd.print(PR);
      PRb = PR;
    }

    //    lcd.setCursor(4, 0); lcd.print("  ");
    lcd.setCursor(9, 3);    lcd.print (teksAlarm);
    //    lcd.print ("                    ");
  }
}

void zeroingDisplay() {
  TVb = 0;
  RRb = 0;
  IEb = 0;
  PRb = 0;
  ACb = 0;
  pipb = 0;
  plateaub = 0;
  peepb = 0;
  tInb = 0;
  tExb = 0;
  Q1b = 0;
  volumeb = 0;
}
