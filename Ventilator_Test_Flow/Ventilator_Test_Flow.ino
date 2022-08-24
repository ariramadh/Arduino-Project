int zPressure;
float a;
float AVf_1;
#define pin A4

void setup() {
  Serial.begin(38400);

  zPressure = analogRead(pin) - 480;  //Mengkalibrasi ADC flow
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
  int analogValue = analogRead(pin) - zPressure; //Baca sensor flow
  //  int analogValue = analogRead(A4); //Baca sensor flow

  float AVf = (analogValue + a * AVf_1) / (a + 1);
  //  float Q = (0.5885 * AVf - 265.63);
  float Q;
  //  if (analogValue < 463) {
  //    Q = (1.2389 * AVf - 554.73);
  //  }
  //
  //  if (analogValue > 503) {
  //    Q = (-0.0008 * pow(AVf, 2)) + (1.0813 * AVf) - 308.96;
  //  }
  //
  //  if (analogValue <= 463 && analogValue >= 503){
  //    Q = (0.4157 * AVf) - 174.97;
  //  }
//  Serial.print(AVf); // cmH2O
//  Serial.print("\t");// cmH2O

//  AVf = sqrt(AVf);

  //y = -34,347x2 + 1291,1x - 12104
  //  Q = -34.347 * pow(AVf, 2) + 1291.1 * AVf - 12104;
  //  Q = (35.264 * AVf) - 623.76;
  //  Q = -8.2053 * pow(AVf, 2) + 329.37 * AVf - 3261.3;

//  Q = -34.347 * pow(AVf, 2) + 1291.1 * AVf - 12104;
  Q = (-0.0683 * AVf) + 32.821;
  Q = sqrt(Q);
  
  if (Q < 4) {
    Q = 0; //   l/m
  }

  //  if (AVf < 18.03) {
  //    Q = 0; //   l/m
  //  }

  Serial.print(AVf, 2); // cmH2O
//  Serial.print("\t");// cmH2O
//  Serial.print(Q);// cmH2O
  Serial.println();

  AVf_1 = AVf;
}
