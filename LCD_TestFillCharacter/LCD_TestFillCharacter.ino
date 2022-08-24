/*
   Displays text sent over the serial port (e.g. from the Serial Monitor) on
   an attached LCD.
   YWROBOT
  Compatible with the Arduino IDE 1.0
  Library version:1.1
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftWire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//SoftWire SWire(PB10, PB11, SOFT_FAST);

String text1 = ("Rivaldi"), text2 = ("Priansyah.S.T");

const int one = text1.length();
const int two = text2.length();

bool txt1 = false;

char buf1[16], buf2[16];

int dilay = 20;

void setup()
{
//  SWire.begin();
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
}

void loop()
{
  lcd.clear();

  text1.toCharArray(buf1, one + 1);
  text2.toCharArray(buf2, two + 1);

  delay(1000);

  int interval = 0;
  for (int i = 0; i <= one - 1; i++) {
    Serial.println(buf1[i]);
    for (int j = 16; j >= interval; j--) {
      lcd.clear();
      lcd.setCursor(j, 0);
      lcd.print(buf1[i]);

      if (i != 0) {
        for (int k = 0; k <= i; k++) {
          lcd.setCursor(k - 1, 0);
          lcd.print(buf1[k - 1]);
        }
      }
      delay(dilay);
    }
    interval++;
  }

  interval = 0;
  for (int i = 0; i <= two - 1; i++) {
    Serial.println(buf2[i]);
    for (int j = 16; j >= interval; j--) {
      lcd.clear();
      
      lcd.setCursor(0, 0);
      lcd.print(text1);
      
      lcd.setCursor(j, 1);
      lcd.print(buf2[i]);

      if (i != 0) {
        for (int k = 0; k <= i; k++) {
          lcd.setCursor(k - 1, 1);
          lcd.print(buf2[k - 1]);
        }
      }
      delay(dilay);
    }
    interval++;
  }
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Mantap lur");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Mantap lur");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Mantap lur");
  delay(500);
}
