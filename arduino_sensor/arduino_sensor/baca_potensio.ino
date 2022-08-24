const int potensioTV = A0;
const int potensioRR = A1;
const int potensioIE = A2;
const int potensioPEEP = A3;
const int potensioFIO2 = A4;

int nilaiPotensioTV = 0;
int nilaiPotensioRR = 0;
int nilaiPotensioIE = 0;
int nilaiPotensioPEEP = 0;

void baca_potensio(){    
    nilaiPotensioTV = analogRead(potensioTV);
    TV = map(nilaiPotensioTV, 0, 1023, 16, 4);
    TV = TV * 50;

    nilaiPotensioRR = analogRead(potensioRR);
    RR = map(nilaiPotensioRR, 0, 1023, 40, 10);

    nilaiPotensioIE = analogRead(potensioIE);
    IE = map(nilaiPotensioIE, 0, 1023, 3, 1);
    
    nilaiPotensioPEEP = analogRead(potensioPEEP);
    AC = map(nilaiPotensioPEEP, 0, 1023, -5, -1);
    
    Serial.print("/");Serial.print("k"); Serial.print(TV);
    Serial.print("*");Serial.print(RR);  Serial.print("*");Serial.print(IE);
    Serial.print("*");Serial.print(AC);  Serial.print("#");
}
