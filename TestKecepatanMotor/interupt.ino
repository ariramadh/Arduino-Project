
void blink1() {
//  Serial.print("  setSwitch Pressed!! ");
//
//  unsigned long interrupt_time = millis();
//  if (millis() - interrupt_time > 200) {
//    Serial.println (millis() - interrupt_time);
//    Serial.println ("Running!!");
//    start = !start;
//  }
}

void limSwich_Procedure () {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {

    Serial.println ("limSwitch Pressed!!");
    limState = true;
    count = 0;
  }
  last_interrupt_time = interrupt_time;
}

void motorEncoder(){
  if(rise){
    count += 1;
  }else{
    count -= 1;    
  }
}
