#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int delayPrint = LOW;             // ledState used to set the LED
unsigned long previousMillisPrint = 0;        // will store last time LED was updated
const long intervalPrint = 1000;         // interval at which to blink (milliseconds)

void lcdsetup() {
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
}

void lcdloop() {
    lcd.setCursor(0, 0);
    lcd.print("Sensor ke:");
    lcd.print(interval+1);
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(t[interval]);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(h[interval],0);
    lcd.print("%");
}
