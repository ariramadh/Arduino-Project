
#include <SoftwareSerial.h>
#include "RDM6300.h"

SoftwareSerial rdm_serial(12, 11);
RDM6300<SoftwareSerial> rdm(&rdm_serial);

static enum {jalur1, jalur2, jalur3, berhenti}jalur;
int motor1a = 7, motor2a = 5, motor1b = 4, motor2b = 2;
int pwm1 = 6, pwm2 = 3;
int sen1,sen2,sen3,sen4,sen5, state;

void setup() {
  // put your setup code here, to run once:


  
Serial.begin(9600);
  pinMode(15, INPUT); pinMode(16, INPUT);
  pinMode(17, INPUT);  pinMode(18, INPUT);
  pinMode(19, INPUT);

  pinMode(motor1a, OUTPUT); pinMode(motor2a, OUTPUT);
  pinMode(motor1b, OUTPUT); pinMode(motor2b,OUTPUT);
  jalur = berhenti;
  state=0;
}

void loop() {    
switch(jalur){
  case jalur1 :
    sen1 = digitalRead(19);sen2 = digitalRead(18);sen3 = digitalRead(17);
    sen4 = digitalRead(16);sen5 = digitalRead(15);
             
              digitalWrite(motor1a,HIGH);digitalWrite(motor2a,LOW);
              digitalWrite(motor1b,HIGH);digitalWrite(motor2b,LOW);

     if(sen1 == LOW && sen2 == LOW && sen3 == HIGH && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1, 65); analogWrite(pwm2,40);};//lurus
     if(sen1 == LOW && sen2 == HIGH && sen3 == LOW  && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1, 50); analogWrite(pwm2, 0);}//belok kiri
     if(sen1 == HIGH && sen2 == LOW && sen3 == LOW  && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1, 65); analogWrite(pwm2, 0);}//belok kiri full
     if(sen1 == LOW && sen2 == LOW && sen3 == LOW && sen4 == HIGH && sen5 == LOW ){analogWrite(pwm1, 0); analogWrite(pwm2, 75);}//belok kanan
     if(sen1 == LOW && sen2 == LOW && sen3 == LOW && sen4 == LOW && sen5 == HIGH ){analogWrite(pwm1, 0); analogWrite(pwm2, 80);}//belok kanan
     if(sen1 == LOW && sen2 == LOW && sen3 == LOW && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2, 65);}//belok kanan penuh
     if(sen1 == LOW && sen2 == LOW && sen3 == HIGH && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2,60);}//belok ke A 
     if(sen1 == HIGH && sen2 == HIGH && sen3 == HIGH && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2, 0);} //berhenti
  break;
  
  case jalur2 :
    sen1 = digitalRead(19);sen2 = digitalRead(18);sen3 = digitalRead(17);
    sen4 = digitalRead(16);sen5 = digitalRead(15);
    
              digitalWrite(motor1a,HIGH);digitalWrite(motor2a,LOW);
              digitalWrite(motor1b,HIGH);digitalWrite(motor2b,LOW);
              
       if(sen1 == LOW && sen2 == LOW && sen3 == HIGH && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1, 68); analogWrite(pwm2, 50);}//lurus
  else if(sen1 == LOW && sen2== LOW && sen3 == LOW && sen4 == HIGH && sen5 == LOW){analogWrite(pwm1,0);analogWrite(pwm2,45);}//belok kanan
  else if(sen1 == LOW && sen2== HIGH && sen3 == LOW && sen4 == LOW && sen5 == LOW){analogWrite(pwm1,45);analogWrite(pwm2,0);}//belok kiri
   if(sen1 == HIGH && sen2 == LOW && sen3 == LOW && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1,80); analogWrite(pwm2, 0);}//belok kanan
  else if(sen1== HIGH && sen2== HIGH && sen3 == LOW && sen4 == LOW && sen5 == LOW){analogWrite(pwm1,80);analogWrite(pwm2,0);}//belok kiri penuh
  else if(sen1== HIGH && sen2== HIGH && sen3 == HIGH && sen4 == LOW && sen5 == LOW){analogWrite(pwm1,80);analogWrite(pwm2,0);}//belok ke B
  if(sen1 == HIGH && sen2 == HIGH && sen3 == HIGH && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2, 0);}//berhemti
  break;

  
 case jalur3 :
   sen1 = digitalRead(19);sen2 = digitalRead(18);sen3 = digitalRead(17);
   sen4 = digitalRead(16);sen5 = digitalRead(15);
    
              digitalWrite(motor1a,HIGH);digitalWrite(motor2a,LOW);
              digitalWrite(motor1b,HIGH);digitalWrite(motor2b,LOW);
              
       if(sen1 == LOW && sen2 == LOW && sen3 == HIGH && sen4 == LOW && sen5 == LOW ){analogWrite(pwm1, 68); analogWrite(pwm2, 50);}//lurus
  else if(sen1 == LOW && sen2== LOW && sen3 == LOW && sen4 == HIGH && sen5 == LOW){analogWrite(pwm1,0);analogWrite(pwm2,40);}//belok kanan
  else if(sen1 == LOW && sen2== HIGH && sen3 == LOW && sen4 == LOW && sen5 == LOW){analogWrite(pwm1,45);analogWrite(pwm2,0);}//belok kiri
  if(sen1 == LOW && sen2 == LOW && sen3 == LOW && sen4 == LOW && sen5 == HIGH ){analogWrite(pwm1, 0); analogWrite(pwm2, 60);}//belok kanan
  if(sen1 == LOW && sen2 == LOW && sen3 == LOW && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2, 60);}
  if(sen1 == LOW && sen2 == LOW && sen3 == HIGH && sen4 == HIGH && sen5 == HIGH){
    state++; 
  if(state > 0){analogWrite(pwm1,0); analogWrite(pwm2,60);}
  else {analogWrite(pwm1, 68); analogWrite(pwm2, 50);}}
  if(sen1 == HIGH && sen2 == HIGH && sen3 == HIGH && sen4 == HIGH && sen5 == HIGH){analogWrite(pwm1,0); analogWrite(pwm2, 0);}
  break;
 //Serial.println("jalur 3");
 
 case berhenti :
 Serial.println("berhenti");
 static const unsigned long long A = 0x85008279F7, B = 0x5500098B75, C = 0x8500828056;
  static unsigned long long last_id = 0;

  last_id = rdm.read(); rdm.print_int64(last_id);

       if (last_id == A) {Serial.println("kartu A");jalur = jalur1;}
  else if (last_id == B) {Serial.println("kartu B");jalur = jalur2;}
  else if (last_id == C) {Serial.println("kartu C");jalur = jalur3;}

 analogWrite(pwm1,0);analogWrite(pwm2,0);
 break;
}
  
}
