// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int potPin = A0;    // select the input pin for the potentiometer
int inputPin = A1;    // select the input pin for the potentiometer
int ledPin = 3;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int potValue = 0;  // variable to store the value coming from the sensor

void setup() {
  pinMode(ledPin, OUTPUT);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  Serial.begin(9600);

  fuzzyInit();
}

void loop() {

  bacaData();

  potValue = map(analogRead(potPin),0,1023,0,10);

  int value = potValue * average;

  Serial.print("Input : ");
  Serial.print(value);
  Serial.print("\t");
  
  // Set the value as an input
  fuzzy->setInput(1, value);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  float output = fuzzy->defuzzify(1);
  // Printing something

  Serial.print("Output : ");
  Serial.print(output);
  Serial.println("\t");

  analogWrite(ledPin, map(output,0,100,0,255));

  delay(10);
}

void fuzzyInit(){
  randomSeed(analogRead(0));

  // Instantiating a FuzzyInput object
  FuzzyInput *inputF = new FuzzyInput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *gelap = new FuzzySet(0, 0, 0, 30);
  FuzzySet *sedang = new FuzzySet(20, 50, 50, 70);
  FuzzySet *terang = new FuzzySet(60, 80, 100, 100);

  // Including the FuzzySet into FuzzyInput
  inputF->addFuzzySet(gelap);
  inputF->addFuzzySet(sedang);
  inputF->addFuzzySet(terang);
  
  fuzzy->addFuzzyInput(inputF);


  // Instantiating a FuzzyOutput objects
  FuzzyOutput *pwm = new FuzzyOutput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *lambat = new FuzzySet(0, 0, 0, 30);
  FuzzySet *lumayan = new FuzzySet(30, 50, 50, 70);
  FuzzySet *cepat = new FuzzySet(60, 80, 100, 100);

  // Including the FuzzySet into FuzzyOutput
  pwm->addFuzzySet(lambat);
  pwm->addFuzzySet(lumayan);
  pwm->addFuzzySet(cepat);
  
  fuzzy->addFuzzyOutput(pwm);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihKecil = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihKecil->joinSingle(gelap);
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaLambat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaLambat->addOutput(lambat);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan1 = new FuzzyRule(1, jikaSelisihKecil, makaLambat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan1);

  
  // Building FuzzyRule "IF distance = safe THEN speed = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihSedang = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihSedang->joinSingle(sedang);
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaLumayan = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaLumayan->addOutput(lumayan);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan2 = new FuzzyRule(2, jikaSelisihSedang, makaLumayan);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan2);

  
  // Building FuzzyRule "IF distance = big THEN speed = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihBesar = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihBesar->joinSingle(terang);
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaCepat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaCepat->addOutput(cepat);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihBesar, makaCepat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan3);
}

void bacaData(){
    // read the value from the sensor:
  potValue = analogRead(potPin);
  // turn the ledPin on
  analogWrite(ledPin, map(potValue,0,1023,0,255));

  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  average = map(average, 0, 1023, 0, 10);

}
