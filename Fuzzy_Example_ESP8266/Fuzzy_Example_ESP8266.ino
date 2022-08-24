#include <Fuzzy.h>
#include "DHTesp.h"
#include <Servo.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;
Servo myservo;  // create servo object to control a servo

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

int sp = 40;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup()
{
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object

  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 17

  // Set the Serial output
  Serial.begin(9600);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

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
  FuzzySet *cepat = new FuzzySet(30, 80, 100, 100);
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
  delay(dht.getMinimumSamplingPeriod());

  float h = dht.getHumidity();
  float t = dht.getTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(dht.getStatusString());
  Serial.print(F("\tHumidity: "));
  Serial.print(h);
  Serial.print(F("%\tTemperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  Serial.print("\tSetPoint: ");
  Serial.print(sp);
  Serial.print("\tSelisih suhu: ");

  float input = sp - t;    
  
  Serial.print(input);

  fuzzy->setInput(1, input);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  float output = fuzzy->defuzzify(1);
  // Printing something

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

  
  Serial.print("\tOutput: ");
  myservo.write(map(average,0,100,0,255));                  // sets the servo position according to the scaled value
  Serial.println(output);
  Serial.println("\n");
}
