#include <HCSR04.h>

#define TRIG_PIN 4
#define ECHO_PIN 5

#define s0 7  //S0 MultiPlexer
#define s1 8  //S1 MultiPlexer
#define s2 9  //S2 MultiPlexer
#define s3 9  //S2 MultiPlexer

#define jmlUltra 3

UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);  // Initialize sensor that uses digital pins 13 and 12.

int ch;

void setup(){
    Serial.begin(9600);
}

void loop() {
    for(int i = 0; i < jmlUltra; i++){
      Serial.print(("Sensor ke: "));
      Serial.print(i+1);  
      Serial.print("\t");  

      bcd(i);
    
      Serial.print(distanceSensor.measureDistanceCm());
      Serial.print("\t");
      delay(200);
    }
  Serial.println();
}

void bcd(int pin){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  if (bitRead(pin,0)){
    digitalWrite(s0, HIGH);
  }else{
    digitalWrite(s0, LOW);    
  }

  if (bitRead(pin,1)){
    digitalWrite(s1, HIGH);
  }else{
    digitalWrite(s1, LOW);    
  }
  
  if (bitRead(pin,2)){
    digitalWrite(s2, HIGH);
  }else{
    digitalWrite(s2, LOW);    
  }
  
  if (bitRead(pin,3)){
    digitalWrite(s3, HIGH);
  }else{
    digitalWrite(s3, LOW);    
  }
}
