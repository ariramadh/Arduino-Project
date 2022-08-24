void limSwich_Procedure() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore

  if (interrupt_time - last_interrupt_time > 100 && !rise) {
    Serial.println ("========limSwitch Pressed!!");
    limState = 1;
    count = 0;
  }
  last_interrupt_time = interrupt_time;
}

void motorEncoder() {
  if (rise) {
    count++;
  } else {
    count--;
  }
}
