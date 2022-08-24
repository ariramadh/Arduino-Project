void setup() 
{
     pinMode(LED_BUILTIN, OUTPUT); 
     Serial.begin(9600);
}
 
void loop() 
{
     Serial.println("*b1*j2*t3*i4*");
     digitalWrite(LED_BUILTIN,HIGH);
     delay(100);
     digitalWrite(LED_BUILTIN,LOW);
     delay(900);
}
