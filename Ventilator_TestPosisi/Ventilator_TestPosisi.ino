#define Min1 10
#define Min2 11
#define Men 9
#define Mslp 8

#define limSwitch 2
#define encoder 3

int vol = 300;
float tIn = 3000;

bool rise, toStart;  //Menentukan apakah encoder count up atau count down
int pwm = 100;
int pos = 3800;
float pulsePerSecond;
int count;
int comp = 0;
bool limState;
int time, timeb;

void setup() {
  Serial.begin(38400);

  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);

  digitalWrite(Men, HIGH);  //Motor enable
  digitalWrite(Mslp, HIGH); //
  digitalWrite(Min2, LOW);  //Motor Input2
  digitalWrite(Min1, LOW);  //Motor Input1

  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.println(limState);
  if (!toStart) {
    homing();
    toStart = true;
  }
  limState = 0;

  run();
}

void run() {
  //  pos = (3.921 * vol) + 1406.3;
//  pulsePerSecond = pos / (tIn / 1000.0);
//  pwm = (0.0607 * pulsePerSecond) + 24.108;
//  pwm = constrain(pwm, 0, 255); //Membatasi output PWM dari 0 hingga 255
  comp = 0.5671 * pwm - 36.107;

  //  Serial.print("Kompensasi :");
  //  Serial.print(pos - comp);

  while (count <= pos - comp) {
    rise = true;
    analogWrite(Min2, pwm);    //Jalankan motor
    digitalWrite(Min1, LOW);    //Jalankan motor//        Serial.print("\tAlarm\t");  Serial.print(alarm);
    Serial.print(pos);
    Serial.print("\t");
    Serial.print(comp);
    Serial.print("\t");
    Serial.print(count);
    Serial.print("\t");
    Serial.print(pwm);
    Serial.println("\tIN");
  }

  digitalWrite(Min2, LOW);   //Motor berhenti
  digitalWrite(Min1, LOW);    //Motor berhenti
  Serial.print(count);
  Serial.println("\tHO");
  delay(100);

  while (count >= 0 + comp && limState == 0) {
    rise = false;
    digitalWrite(Min2, LOW);
    analogWrite(Min1, pwm); //Jalankan motor
    Serial.print(count);
    //    Serial.print("\t");
    Serial.println("\tEX\t");
  }
  digitalWrite(Min2, LOW);   //Motor berhenti
  digitalWrite(Min1, LOW);    //Motor berhenti
  delay(2000);
}
