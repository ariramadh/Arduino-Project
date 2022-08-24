#define val 6

#define S1 7  //S0 MultiPlexer
#define S2 8  //S1 MultiPlexer
#define S3 9  //S2 MultiPlexer
#define S4 10  //S3 MultiPlexer

#define jumlah 4   //Jumlah komponen yang dipakai

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(val, OUTPUT);

  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < jumlah; i++) {
    bcd(i);

    digitalWrite(val, HIGH);
    delay(1);
  }
}

void bcd(int pin) {
  if (bitRead(pin, 0)) {
    digitalWrite(S1, HIGH);
  } else {
    digitalWrite(S1, LOW);
  }

  if (bitRead(pin, 1)) {
    digitalWrite(S2, HIGH);
  } else {
    digitalWrite(S2, LOW);
  }

  if (bitRead(pin, 2)) {
    digitalWrite(S3, HIGH);
  } else {
    digitalWrite(S3, LOW);
  }

  if (bitRead(pin, 3)) {
    digitalWrite(S4, HIGH);
  } else {
    digitalWrite(S4, LOW);
  }
}
