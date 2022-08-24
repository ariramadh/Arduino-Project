void setBtn() {
  //  Serial.println (" setBtn Pressed!!");
  btnSet.tick();
  //  start = true;
}

void limSwich_Procedure () {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200 && exhale) {
    Serial.println (" limSwitch Pressed!!");
    resetEncoder();
    limState = 1;
    count = 0;
  }
  last_interrupt_time = interrupt_time;
}

void motorEncoder() {
  if (rise) {
    count += encd;
  } else {
    count -= encd;
  }
}
