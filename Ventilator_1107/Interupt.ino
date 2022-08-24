void blink1() {
  TV1 = TV;
  RR1 = RR;
  IE1 = desIE;
  //Serial.printlnln("jalan");
}

void limSwich_Procedure () {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    Serial.println (" limSwitch Pressed!!");
    limState = 1;
    count = 0;
  }
  last_interrupt_time = interrupt_time;
}

void motorEncoder() {
  if (rise) {
    count += 1;
  } else {
    count -= 1;
  }
}
