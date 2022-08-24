#define pin A0
#define led 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int dat = 0;
  if(digitalRead(led) == false){
    dat = 1000;
  }
//  Serial.print(dat);
//  Serial.print("\t");
  Serial.println(analogRead(pin));

  delay(50);
}
