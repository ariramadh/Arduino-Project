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

int sendTV() {
  return TV;
}

void bacaPotensio() {
  int nilaiPotensioTV = analogRead(potensioTV);

  TV = map(nilaiPotensioTV, 0, 1023, 16, 4);
  TV = TV * 50;

  int nilaiPotensioRR = analogRead(potensioRR);
  RR = map(nilaiPotensioRR, 0, 1023, 40, 10);

  int nilaiPotensioIE = analogRead(potensioIE);
  IE = map(nilaiPotensioIE, 100, 800, 3, 1);

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
