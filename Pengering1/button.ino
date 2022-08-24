void record() {
  rec = !rec;

  if (rec) {
    Serial.println("Recording!!");
  } else {
    Serial.println("Recording Stopped!!");
  }
}

void longPressSS() {
  running = !running;

  if (running) {
    Serial.println("System Running!!");
  } else {
    Serial.println("System Stopped!!");
  }
}

void clickP() {
  if (spT < 99) {
    spT += 1;
  } else {
    spT = 99;
  }
}

void longPressP() {
  if (spT < 99) {
    spT += 1;
  } else {
    spT = 99;
  }
  delay(50);
}

void clickM() {
  if (spT > 0) {
    spT -= 1;
  } else {
    spT = 0;
  }
}

void longPressM() {
  if (spT > 0) {
    spT -= 1;
  } else {
    spT = 0;
  }
  delay(50);
}
