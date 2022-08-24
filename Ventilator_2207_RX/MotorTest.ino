unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 3000;

void testMotor () {

  if (limState == 0) {
    digitalWrite(Min2, LOW);
    analogWrite(Min1, 100);
  }
  if (limState == 1) {
    analogWrite(Min2, 100);
    digitalWrite(Min1, LOW);
  }

  //  Serial.printlnln(limState);
  currentMillis = millis();
  if ( currentMillis -  previousMillis >= interval) {
    previousMillis = currentMillis;
    limState = 0;
  }
}

void get_encoder(){
  Serial.print("/E/#");
  readSerial();
}

void motor_maju() {
  digitalWrite(Min1, LOW);
  analogWrite(Min2, 70);
  delay(3000);
  // put your main code here, to run repeatedly:

}


void motor_mundur() {
  digitalWrite(Min2, LOW);
  analogWrite(Min1, 70);
  delay(3000);
  // put your main code here, to run repeatedly:

}
