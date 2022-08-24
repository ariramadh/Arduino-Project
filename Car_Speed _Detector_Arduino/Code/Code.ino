#include<LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

int sensor1 = 2;
int sensor2 = 3;

int Time1;
int Time2;
int Time;
int flag = 0;

int distance = 27;

float Speed;

void setup() {
  attachInterrupt(0,fun1,RISING);
  attachInterrupt(1,fun2,FALLING);

  lcd.begin(16,2);
  lcd.clear();
  lcd.print("SPEED MEASURMENT");
}

void fun1()
{
  Time1 = millis(); 
  if (flag == 0) {flag = 1;}
  else {flag = 0;}
}

void fun2()
{
  Time2 = millis();
  if (flag == 0) {flag = 1;}
  else {flag = 0;}
}


void loop() {
  if (flag == 0){
  if      (Time1 > Time2) {Time = Time1 - Time2;  Speed = (distance*1000)/Time;}
  else if (Time2 > Time1) {Time = Time2 - Time1;  Speed = (distance*1000)/Time;}
  else    {Speed = 0;}
  }
  
  if (Speed == 0) { 
    lcd.setCursor(0, 1); 
    lcd.print(".......OK......."); 
    }
  else { 
    lcd.setCursor(2, 1); 
    lcd.print(Speed);
    lcd.print(" cm/sec");
    delay(10000);
    Time1 = 0;
    Time2 = 0;
    }
}
