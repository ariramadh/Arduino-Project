//Pin Control Motor////////////////////////////////////////////////////////////
#define Min1 9
#define Min2 8
#define Men 10
#define Mslp 11
#define limSwitch 3

#define encoder 18

const byte setPin = 2;

int nilaiPotensioTV = 0;

const int potensioTV = A0;

int TV = 500;
int tIn = 1500;

bool start = 0;
bool limState = false;

int count;
bool rise = false;

float pres, rho = 1.2754, //mass jenis udara 1.2 Kg/m3
            a1 = 188.691 * 1e-6,  //m^2
            a2 = 78.539 * 1e-6,  //m^2
            v1, v2, Q1, Q2,
            interval_limit, pressure, volume, tp, MAX_PRESSURE;

int tBef;

float lastOutput;
float vol;
unsigned long ti;

int posisi;
int pulsePerSecond;
int pwm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(setPin, INPUT_PULLUP);
  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(setPin), blink1, FALLING);
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder),  motorEncoder, FALLING);

  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);
  digitalWrite(Men, HIGH);
  digitalWrite(Mslp, HIGH);
  digitalWrite(Min2, LOW);
  digitalWrite(Min1, LOW);

  Serial.println("Wait");
  delay(5000);

  digitalWrite(Min1, LOW);
  analogWrite(Min2, 70);
  Serial.println("Close");
  delay(2000);


  while (limState == 0) {
    digitalWrite(Min2, LOW);
    Serial.println("Open");
    analogWrite(Min1, 70);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int time[] = {2000, 2000, 0};

  limState = false;

  Serial.println("Running ");
  for (int i = 0; i < 2; i++) {

    nilaiPotensioTV = analogRead(potensioTV);
    nilaiPotensioTV = map(nilaiPotensioTV, 0, 1020, 40, 255);

    //    time[0] = map(nilaiPotensioTV, 100, 70, 3000, 5000);

    perintah(i , time[i]);
    //    Serial.print("Waktu :");

    //    Serial.print("Perintah :");
    //    Serial.print(i);
    //    Serial.print("  ");
  }

  limState = false;

  while (limState == 0) {
    //  while (count[0] >= 1627) {
    read_pressure();

    nilaiPotensioTV = analogRead(potensioTV);
    nilaiPotensioTV = map(nilaiPotensioTV, 0, 1020, 40, 255);

    //    Serial.print("PWM : ");
    //    Serial.print(nilaiPotensioTV);
    //    Serial.print("  ");

    analogWrite(Min1, 70);
    digitalWrite(Min2, LOW);

    rise = false;

    Serial.print("  Opening");
    Serial.print("  ");
    Serial.println(count);
  }

  delay(10);
}

void perintah(int c, int t) {
  unsigned long tick = millis();

  tBef = 0;
  volume = 0;

  posisi = (1.9204 * TV) + 1917;

  pulsePerSecond = posisi / (tIn / 1000.0);

  pwm = (0.0607 * pulsePerSecond) + 24.108;

  while (millis() - tick <= 5000) {
    read_pressure();
    t = millis() - tick; // ms
    //    Serial.print("  t: "); Serial.print(t);

    volume += Q1 * ((t - tBef) / 1000.0);  // ml

    //    Serial.print("  DeltaT: "); Serial.print(t - tBef);
    //    Serial.print("  Vol= "); Serial.print(volume);

    tBef = t;

    nilaiPotensioTV = analogRead(potensioTV);
    pwm = map(nilaiPotensioTV, 0, 1020, 40, 255);

    Serial.print("PosisiT : ");
    Serial.print(posisi);
    Serial.print("  ");

    Serial.print("PosisiA : ");
    Serial.print(count);
    Serial.print("  ");

    Serial.print("Pulse : ");
    Serial.print(pulsePerSecond);
    Serial.print("  ");

    Serial.print("PWM : ");
    Serial.print(pwm);
    Serial.print("  ");

    switch (c) {
      case 0:
        if (count <= 3000) {
          digitalWrite(Min1, LOW);
          analogWrite(Min2, pwm);
          Serial.print("  time: ");
          Serial.print(millis() - tick);
          Serial.print("    ");
        } else {
          digitalWrite(Min1, LOW);
          digitalWrite(Min2, LOW);
        }

        Serial.print("  Closing");
        rise = true;
        Serial.print("  ");
        break;
      case 1:
        delay(100);
        digitalWrite(Min1, LOW);
        digitalWrite(Min2, LOW);
        Serial.print("  Holding");
        Serial.print("  ");
        Serial.print(count);
        break;
        //      case 2:
        //        if (limState == 0) {
        //          analogWrite(Min1, nilaiPotensioTV);
        //          digitalWrite(Min2, LOW);
        //          Serial.print("  Opening");
        //        } else {
        //          digitalWrite(Min2, LOW);
        //          digitalWrite(Min1, LOW);
        //        }
        //        break;
    }
    Serial.println();
  }

  digitalWrite(Min1, LOW);
  digitalWrite(Min2, LOW);

  if (volume != 0) {
    vol = map(volume , 0 , 93, 0, 1000);
    //      vol = volume;
  }

  if (t != 0) {
    ti = t;
  }
}
