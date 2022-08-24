int zPressure;
float a;
float AVf, AVf_1;

void setup() {
  Serial.begin(38400);

  zPressure = analogRead(A4) - 450;  //Mengkalibrasi ADC flow
  float fc = 15.167;
  float RC = 1.0 / (2 * 3.14159 * fc);
  a = RC / 0.02;
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(20);
  bacaFlow();
}

void bacaFlow() {
  int analogValue = analogRead(A4) - zPressure; //Baca sensor flow
  float Q;

  Q = (5/5) * analogValue + (5/2);

  Serial.print(analogValue);// cmH2O
  Serial.print("\t");// cmH2O
  Serial.println(Q);// cmH2O

  if (Q < 9) {
    Q = 0; //   l/m
  }

  AVf_1 = AVf;
}
