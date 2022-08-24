#define potensioTV A3
#define potensioRR A2
#define potensioIE A1
#define potensioPRAC A0

int nilaiPotensioTV = 0;
int nilaiPotensioRR = 0;
int nilaiPotensioIE = 0;
int nilaiPotensioPEEP = 0;

int TV, RR, IE, AC, PR;
int TVb, RRb, IEb, ACb, PRb;
int TVc, RRc, IEc;

bool pwmStats;

int sendTV() {
  return TV;
}

void hitung() {
  int tHold = 100;

  float tPeriod = 60000.0 / RRc;  //Menghitung perioda  //milidetik/nafas
  tPeriod = tPeriod - tHold;  //Menghitung perioda dikurangi waktu tahan nafas
  //  Serial.print("tPeriod "); Serial.print(tPeriod);
  //  Serial.print("");
  float tIn = tPeriod / (1 + IEc); //Menghitung waktu tarik nafas  //milidetik

  int posisi = (3.5976 * TVc) + 1761;  //Kalkulasi untuk menghitung posisi
  int pulsePerSecond = posisi / (tIn / 1000.0);  //Kalkulasi untuk menghitung pulsa per detik

  int pwm = (0.0581 * pulsePerSecond) + 27.065;     //Kalkulasi untuk menghitung nilai PWM
//  pwm = constrain(pwm, 0, 255); //Membatasi output PWM dari 0 hingga 255
  pwmStats = pwm<=255;

//  Serial.println(pwm);
}

void bacaPotensio() {
  hitung();

  int nilaiPotensioTV = analogRead(potensioTV);
  TVc = map(nilaiPotensioTV, 0, 1023, 12, 6);
  TVc = TVc * 50;
  int nilaiPotensioRR = analogRead(potensioRR);
  RRc = map(nilaiPotensioRR, 0, 1023, 30, 10);
  int nilaiPotensioIE = analogRead(potensioIE);
  IEc = map(nilaiPotensioIE, 100, 800, 3, 1);

  if (pwmStats) {
    TV = TVc;
    RR = RRc;
    IE = IEc;
  }

  int nilaiPotensioPRAC = analogRead(potensioPRAC);

  //  Serial.println(nilaiPotensioPRAC);

  if (mode) {
    AC = map(nilaiPotensioPRAC, 1023, 0, 1, 5);
  } else {
    PR = map(nilaiPotensioPRAC, 1023, 0, 10, 40);
    maxPress = PR;
  }

  sendPotensio();
}

void sendPotensio() {
  int pat;

  if (mode) {
    pat = AC;
  } else {
    pat = PR;
  }

  if (TVb != TV || RRb != RR || IEb != IE || ACb != AC || PRb != PR) {
    Serial.print("/k");
    Serial.print(TV);
    Serial.print("*");
    Serial.print(RR);
    Serial.print("*");
    Serial.print(IE);
    Serial.print("*");
    Serial.print(pat);
    Serial.println("#");
    //        Serial.println(TV);
    //        Serial.println(RR);
    //        Serial.println(IE);
    //        Serial.println(AC);

    TVb = TV;
    RRb = RR;
    IEb = IE;
    ACb = AC;
    PRb = PR;
  }
}
