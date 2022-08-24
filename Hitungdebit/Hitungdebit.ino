float pres, rho = 1.2, //mass jenis udara 1.2 Kg/m3
            v1, v2, Q1, Q2,
            interval_limit;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.backlight();
  lcd.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  read_pressure();
  lcd_disp();
  delay(1);
}
