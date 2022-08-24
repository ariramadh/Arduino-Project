int value = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,value);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    char inByte = Serial.read();

    Serial.print(inByte);
    if(inByte == 't'){
      digitalWrite(LED_BUILTIN,HIGH); 
    }else if(inByte == 'f'){
      digitalWrite(LED_BUILTIN,LOW);      
    }
  }
}
