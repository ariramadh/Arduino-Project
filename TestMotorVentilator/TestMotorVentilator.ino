#include <SimpleTimer.h>

#define Min1 11
#define Min2 10
#define Men 9
#define Mslp 8

#define encoder 2

int count; //Menghitung pulsa motor

SimpleTimer firstTimer(250);
SimpleTimer secondTimer(3000);

bool run = false;

int revolusi = 240 * 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);
  pinMode(encoder, INPUT_PULLUP);

  digitalWrite(Men, HIGH);  //Motor enable
  digitalWrite(Mslp, HIGH); //
  digitalWrite(Min2, LOW);  //Motor Input2
  digitalWrite(Min1, LOW);  //Motor Input1

  attachInterrupt(digitalPinToInterrupt(encoder),  motorEncoder, FALLING);

  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int pwm = 60;

  while (count <= revolusi) {
    digitalWrite(Min1, LOW);   //Jalankan motor
    analogWrite(Min2, pwm);    //Jalankan motor
    Serial.print("Running ");
    Serial.println(count);
  }

  if (count = revolusi) {
    digitalWrite(Min2, LOW);   //Jalankan motor
    analogWrite(Min1, pwm);    //Jalankan motor
    Serial.print("brake");
    run = true;
  }

  secondTimer.reset();

  while (!secondTimer.isReady()) {
    digitalWrite(Min1, LOW);    //Jalankan motor
    digitalWrite(Min2, LOW);    //Jalankan motor
    Serial.print("Stop ");
    Serial.println(count);
  }
  count = 0;
}

void motorEncoder() {
  count++;
}
