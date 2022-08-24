void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.write("t");
  Serial.write("*t*");
  delay(1000);
//  Serial.write("f");
  Serial.write("*f*");
  delay(1000);
}
