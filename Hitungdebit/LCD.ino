void lcd_disp(){
  lcd.clear();
  
  lcd.setCursor(0, 0); lcd.print("Flow1=");
  lcd.setCursor(7, 0); lcd.print(Q1);
  lcd.setCursor(0, 1); lcd.print("Flow2=");
  lcd.setCursor(7, 1); lcd.print(Q1);
}
