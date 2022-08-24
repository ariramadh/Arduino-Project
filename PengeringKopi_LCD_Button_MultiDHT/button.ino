void record(){
  
}

void clickM(){
  if (hum > 0) {
    hum -= 1;  
  }
}

void clickP(){
  if (hum < 100) {
    hum += 1;
  }
}

void longPressM(){
  if (hum > 0) {
    hum -= 1;
  }else{
    hum = 0;
  }
  delay(50);
}

void longPressP(){
  if (hum < 100) {
    hum += 1;
  }else{
    hum = 100;
  }
  delay(50);
}

void clickT(){
  digitalWrite(LED_BUILTIN, val);
  val = !val;

//  btnTare();
}

void longPressSS(){
  running = !running;
}
