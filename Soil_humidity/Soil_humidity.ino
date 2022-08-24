#include <SoilHygrometer.h>

SoilHygrometer hygrometer(A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  hygrometer.print("", 100, 0);
  Serial.println();

  delay(50);
}
