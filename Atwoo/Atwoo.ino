int in1 = 3, in2 = 4, in3 = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int start=0;
  int ch1=0;
  int ch2=0;

  if(digitalRead(in1) == 1){
    start = 0;
  }else{
    start = 1;    
  }

  if(digitalRead(in2) == 1){
    ch1 = 0;
  }else{
    ch1 = 1;    
  }

  if(digitalRead(in3) == 1){
    ch2 = 0;
  }else{
    ch2 = 1;    
  }
  
  Serial.print(start);
  Serial.print(ch1);
  Serial.println(ch2);

  delay(1);
}
