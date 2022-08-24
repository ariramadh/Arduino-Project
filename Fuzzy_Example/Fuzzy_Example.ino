#include <Fuzzy.h>
#include "DHT.h"
#include <Servo.h>

#define DHTPIN 2        // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;  // create servo object to control a servo

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

int sp = 40;

const int numReadings = 5;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int pos = 0;    // variable to store the servo position

void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  dht.begin();

  // Set the Serial output
  Serial.begin(9600);

  // Instantiating a FuzzyInput object
  FuzzyInput *selisihSuhu = new FuzzyInput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *kecil = new FuzzySet(0, 0, 2, 3);
  // Including the FuzzySet into FuzzyInput
  selisihSuhu->addFuzzySet(kecil);
  
  // Instantiating a FuzzySet object
  FuzzySet *sedang = new FuzzySet(1, 3, 3, 4);
  // Including the FuzzySet into FuzzyInput
  selisihSuhu->addFuzzySet(sedang);
  
  // Instantiating a FuzzySet object
  FuzzySet *besar = new FuzzySet(3, 10, 50, 50);
  
  // Including the FuzzySet into FuzzyInput
  selisihSuhu->addFuzzySet(besar);

  // Including the FuzzyInput into Fuzzy
  fuzzy->addFuzzyInput(selisihSuhu);


  // Instantiating a FuzzyOutput objects
  FuzzyOutput *pwm = new FuzzyOutput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *lambat = new FuzzySet(0, 0, 0, 20);
  // Including the FuzzySet into FuzzyOutput
  pwm->addFuzzySet(lambat);
  
  // Instantiating a FuzzySet object
  FuzzySet *lumayan = new FuzzySet(10, 30, 30, 40);
  // Including the FuzzySet into FuzzyOutput
  pwm->addFuzzySet(lumayan);
  
  // Instantiating a FuzzySet object
  FuzzySet *cepat = new FuzzySet(30, 90, 100, 100);
  // Including the FuzzySet into FuzzyOutput
  pwm->addFuzzySet(cepat);
  // Including the FuzzyOutput into Fuzzy
  
  fuzzy->addFuzzyOutput(pwm);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihKecil = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihKecil->joinSingle(kecil);

  // Building FuzzyRule "IF distance = safe THEN speed = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihSedang = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihSedang->joinSingle(sedang);

  // Building FuzzyRule "IF distance = big THEN speed = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihBesar = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihBesar->joinSingle(besar);
  
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaLambat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaLambat->addOutput(lambat);

  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaLumayan = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaLumayan->addOutput(lumayan);

  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaCepat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaCepat->addOutput(cepat);

  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan1 = new FuzzyRule(1, jikaSelisihKecil, makaCepat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan1);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan2 = new FuzzyRule(2, jikaSelisihSedang, makaLumayan);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan2);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihBesar, makaLambat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan3);
}

void loop()
{
  delay(1500);

  // Baca Suhu
  float h = dht.readHumidity();
  // Baca temperatur celcius
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  float input = sp - t;  
  float output;
  
  fuzzy->setInput(1, input);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  output = fuzzy->defuzzify(1);
  // Printing something  

  if (input < 1){
    output = 100;
  }

  Serial.print("SetPoint: ");
  Serial.print(sp);
  Serial.print("\tSelisih suhu: ");
  Serial.print(input);
  
  Serial.print("\tOutput: ");
  Serial.println(output);
  Serial.println("");
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = output;
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

  output = map(average,0,100,0,255);

  myservo.write(output);                  // sets the servo position according to the scaled value
}
