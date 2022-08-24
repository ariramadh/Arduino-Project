#include "OneButton.h"

OneButton button1(A0, true);  //button1 di A0
OneButton button2(A1, true);  //button2 di A1
OneButton button3(A2, true);  //button3 di A2
OneButton button4(A3, true);  //button4 di A3

#define out1 12 //pin output
#define out2 13 //pin output

#define S0 7  //S0 MultiPlexer
#define S1 8  //S1 MultiPlexer
#define S2 9  //S2 MultiPlexer
#define S3 10  //S3 MultiPlexer

#define jumlah 2   //Jumlah komponen yang dipakai

int val1[jumlah] = {false, true};
int val2[jumlah] = {true, true};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  button1.attachClick(sel1);
  button2.attachClick(sel2);
  button3.attachClick(sel3);
  button4.attachClick(sel4);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();

  for (int i = 0; i < jumlah; i++) {
    bcd(i);

    digitalWrite(out1, val1[i]);
    digitalWrite(out2, val2[i]);
//    digitalWrite(out1, HIGH);
//    digitalWrite(out2, HIGH);

    delay(1);
  }
}

void bcd(int pin) {
  if (bitRead(pin, 0)) {
    digitalWrite(S0, HIGH);
  } else {
    digitalWrite(S0, LOW);
  }

  if (bitRead(pin, 1)) {
    digitalWrite(S1, HIGH);
  } else {
    digitalWrite(S1, LOW);
  }

  if (bitRead(pin, 2)) {
    digitalWrite(S2, HIGH);
  } else {
    digitalWrite(S2, LOW);
  }

  if (bitRead(pin, 3)) {
    digitalWrite(S3, HIGH);
  } else {
    digitalWrite(S3, LOW);
  }
}

void sel1() {
//  val1[0] = HIGH;
  Serial.println("MAshook pak eko");
}

void sel2() {
  val1[1] = HIGH;
}

void sel3() {
  val2[0] = HIGH;
}

void sel4() {
  val2[1] = HIGH;
}
