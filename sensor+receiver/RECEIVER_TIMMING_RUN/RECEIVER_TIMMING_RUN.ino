#include <SoftwareSerial.h>
#include <SimpleTimer.h>

byte mode = 1;
boolean flag1 = 0;
boolean flag2 = 0;

int address;
int i;

SimpleTimer firstTimer(200);

SoftwareSerial HC12(10, 11);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HC12.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (firstTimer.isReady()) {

    HC12.print("*");
    HC12.print(mode);
    HC12.print(",");
    HC12.print(i + 1);
    HC12.println("#");

    i++;
    if (i == 5) {
      i = 0;
      delay(5000);
    }
    firstTimer.reset();
  }

  delay(10);

  if (HC12.available() > 0) {
//    Serial.println(HC12.available());
        Serial.write(HC12.read());
  }
}

void cek1() {
  if (firstTimer.isReady()) {

    for (int i = 0; i < 5; i++) {

      HC12.print("*");
      HC12.print(mode);
      HC12.print(",");
      HC12.print(i + 1);
      HC12.println("#");

      while (HC12.available() > 1) {
        flag2 = 1;
      }

      while (flag2) {
        if (flag1 == 0) {
          Serial.print("Receiving data from sensor ");
          Serial.println(i + 1);
          flag1 = 1;
        }
        Serial.println("while");
      }
      flag1 = 0;
      flag2 = 0;

    }
    firstTimer.reset();
  }

}
