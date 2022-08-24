#include "OneButton.h"

OneButton button1(A0, false);
OneButton button2(A1, false);
OneButton button3(A3, false);

int value = 60;

bool running = false;
//bool last = false;

void setup(){
  Serial.begin(9600);

  button1.attachClick(click1);
  button1.attachDuringLongPress(longPress1);

  button2.attachClick(click2);
  button2.attachDuringLongPress(longPress2);

  button3.attachLongPressStart(longPressStart3);
}

void loop(){
  button1.tick();
  button2.tick();
  button3.tick();

  Serial.println(running);    

  delay(50);
}

void click1(){
  if (value > 0) {
    value -= 1;  
  }
}

void click2(){
  if (value < 100) {
    value += 1;
  }
}

void longPress1(){
  if (value > 0) {
    value -= 1;
  }else{
    value = 0;
  }
}

void longPress2(){
  if (value < 100) {
    value += 1;
  }else{
    value = 100;
  }
}

void longPressStart3(){
  running = !running;

}
