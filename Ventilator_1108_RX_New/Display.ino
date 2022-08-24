void welcomeTeks() {
  lcd.setCursor(0, 0); lcd.print("WELCOME");
  lcd.setCursor(0, 1); lcd.print("VENTILATOR");
  //  Serial.println("Welcome Text");
}

void lcdTeks() {
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
      lcd.setCursor(10, 2); lcd.print("TS:");
    } else {
      lcd.setCursor(12, 1); lcd.print("Vol Mode");
    }
  }
}

void lcdDisp() {

  //  delay(1000);
  if (start) {
    //    Serial.println("LCDStart");
    if (pip != pipb) {
      lcd.setCursor(5, 0); lcd.print(pip);
      pipb = pip;
    }

    if (plateau != plateaub ) {
      lcd.setCursor(5, 1); lcd.print(plateau);
      plateaub = plateau;
    }

    if (peep != peepb) {
      lcd.setCursor(5, 2); lcd.print(peep);
      peepb = peep;
    }

    if (vol != volumeb) {
      float flow = vol / (tIn / 1000.0);
      //      Serial.print(" flow "); Serial.println(flow);
      lcd.setCursor(16, 2); lcd.print(vol);
      lcd.setCursor(5, 3); lcd.print(flow,0);
//      lcd.setCursor(8, 3); lcd.print("   ");
      //      Q1b = volume / tIn;
      volumeb = vol;
    }

    if (alarm != alarmb) {
      Serial.print(" Alarm:");    Serial.println(alarm);
      delay(100);
      lcd.setCursor(9, 3);    lcd.print ("    ");
      lcd.setCursor(9, 3);    lcd.print (teksAlarm[alarm]);
      alarmb = alarm;
    }

    if (tIn != tInb) {
      lcd.setCursor(16, 0); lcd.print(tIn);
      tInb = tIn;
    }

    if (tEx != tExb) {
      lcd.setCursor(16, 1); lcd.print(tEx);
      tExb = tEx;
    }


  } else {
    //    Serial.println("LCDStop");
    if (TV != TVb) {
      lcd.setCursor(3, 1); lcd.print(TV);
      TVb = TV;
    }

    if (RR != RRb) {
      lcd.setCursor(3, 2); lcd.print(RR);
      RRb = RR;
    }

    if (IE != IEb) {
      lcd.setCursor(5, 3); lcd.print(IE);
      IEb = IE;
    }

    if (AC != ACb) {
      if (ACMode) {
        lcd.setCursor(13, 2); lcd.print(AC); lcd.print(" ");
        ACb = AC;
      }
    }

    if (PR != PRb) {
      lcd.setCursor(3, 0); lcd.print(PR);
      PRb = PR;
    }
  }
}

void zeroingDisplay() {
  TVb = -1;
  RRb = -1;
  IEb = -1;
  PRb = -1;
  ACb = -1;
  pipb = -1;
  plateaub = -1;
  peepb = -1;
  tInb = -1;
  tExb = -1;
  volumeb = -1;
  alarmb = -1;

  lcd.setCursor(5, 0); lcd.print("     ");
  lcd.setCursor(5, 1); lcd.print("     ");
  lcd.setCursor(5, 2); lcd.print("     ");
  lcd.setCursor(16, 2); lcd.print("    ");
  lcd.setCursor(5, 3); lcd.print("    ");
  lcd.setCursor(9, 3);    lcd.print ("        ");

  Q1b = -1;
  volumeb = -1;
}

void zeroDisplay() {
  //  if (pip == 0) {
  //    lcd.setCursor(5, 0); lcd.print("0 ");
  //  }
  //  if (plateau == 0)
  //    lcd.setCursor(5, 1); lcd.print("0 ");
  //  if (peep == 0)
  //    lcd.setCursor(5, 2); lcd.print("0 ");
  //  if (volume == 0) {
  //    lcd.setCursor(5, 3); lcd.print("0 ");
  //    lcd.setCursor(16, 2); lcd.print("0 ");
  //  }
}
