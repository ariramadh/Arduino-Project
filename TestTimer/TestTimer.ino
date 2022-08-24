unsigned long time, tick;

int a[] = {3000, 1000, 4000}, b[] = {0, 1, 2};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 3; i++) {
    tick = millis();
    Serial.println(i);
    perintah(a[i], b[i]);
  }
}

void perintah(int durasi, int perintah) {
  while (millis() - tick <= durasi) {
      switch (perintah) {
        case 0:
          Serial.print("Inhale  "); Serial.println(millis() - tick);
          break;
        case 1:
          Serial.print("Hold  "); Serial.println(millis() - tick);
          break;
        case 2:
          Serial.print("Exhale  "); Serial.println(millis() - tick);
          break;
    }
  }
}
