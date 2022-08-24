int adcMax = 432;
int adcZero = 355;
int adcMin = 289;

float lastOutput[3];

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0:
  // print out the value you read:
  int sen[] = {analogRead(A3), analogRead(A4), analogRead(A5)};
  Serial.print(sen[0]); //Z ADC
  Serial.print(" ");
  Serial.print(sen[1]); //X ADC
  Serial.print(" ");
  Serial.print(sen[2]); //Y ADC
  Serial.print("\t");

  float g[] = {map(sen[0],adcMax,adcMin,100,-100)/100.0,map(sen[1],adcMax,adcMin,100,-100)/100.0,map(sen[2],adcMax,adcMin,100,-100)/100.0};

  Serial.print(g[0]); //Z ADC
  Serial.print(" ");
  Serial.print(g[1]); //X ADC
  Serial.print(" ");
  Serial.print(g[2]); //Y ADC
  Serial.print("\t");
  Serial.print("\t");

  Serial.print(filter(g[0],0)); //Z ADC
  Serial.print(" ");
  Serial.print(filter(g[1],1)); //X ADC
  Serial.print(" ");
  Serial.print(filter(g[2],2)); //Y ADC
  Serial.println("");

  delay(1);        // delay in between reads for stability
}

float filter(float raw, int iteration) {
  float alpha = 0.8;
  float average = alpha * raw + (1 - alpha) * lastOutput[iteration];

  lastOutput[iteration] = average;

  return average;
}
