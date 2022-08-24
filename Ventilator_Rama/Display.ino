void disp(){  
  td = millis();
  if (td - td_1 > interval_limit){
    lcd_disp();
    
    td_1 = td;
  }
  else {
    td = td;
//    lcd.clear();
  }    
  td_1 = 0;
}

void lcd_disp(){
//  Serial.printlnln("DISPLAY");
  
  lcd.setCursor(0, 0); lcd.println("AC=");
  lcd.setCursor(3, 0); lcd.println(AC);
  lcd.setCursor(0, 1); lcd.println("TV=");
  lcd.setCursor(3, 1); lcd.println(convInt(TV));
  lcd.setCursor(0, 2); lcd.println("RR=");
  lcd.setCursor(3, 2); lcd.println(RR);
  lcd.setCursor(0, 3); lcd.println("IE=1:");
  lcd.setCursor(5, 3); lcd.println(IE);
  lcd.setCursor(12, 0); lcd.println("P(cmH2O)");
  lcd.setCursor(10, 1); lcd.println("Peak=");
  lcd.setCursor(15, 1); lcd.println(pres);
  lcd.setCursor(10, 2); lcd.println("Plat=");
  lcd.setCursor(15, 2); lcd.println(plateau);
  lcd.setCursor(10, 3); lcd.println("Peep=");
  lcd.setCursor(15, 3); lcd.println(peep);
}

void disp_alarm(){  
  td = millis();
  if (td - td_1 > interval_limit){
//    lcd_alarm();
    
    td_1 = td;
  }
  else {
    td = td;
  }    
  td_1 = 0;
}

void lcd_alarm(){
  lcd.setCursor (4, 0);                 
  lcd.println ("HIGH PRESSURE"); 
}
