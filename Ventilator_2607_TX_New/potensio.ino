#define potensioTV A0
#define potensioRR A1
#define potensioIE A2
#define potensioPRAC A3

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
  IE = map(nilaiPotensioIE, 0, 1023, 3, 1);

  int nilaiPotensioPRAC = analogRead(potensioPRAC);

  if (clickSet()) {
    AC = map(nilaiPotensioPRAC, 0, 1023, 1, 5);
  } else {
    PR = map(nilaiPotensioPRAC, 0, 1023, 10, 40);
  }

  sendPotensio();
}

void sendPotensio() {
  if (TVb != TV || RRb != RR || IEb != IE || ACb != AC || PRb != PR) {
    String sendPot = (String)TV + '*' + (String)RR + '*' + (String)IE + '*' + (String)AC + '*';

    sendHere('k', sendPot);
    //    Serial.print("/"); Serial.print("k"); Serial.print(TV);
    //    Serial.print("*"); Serial.print(RR);  Serial.print("*"); Serial.print(IE);
    //    Serial.print("*"); Serial.print(AC);  Serial.print("#");

    TVb = TV;
    RRb = RR;
    IEb = IE;
    ACb = AC;
    PRb = PR;
  }
}
