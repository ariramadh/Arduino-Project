// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Fuzzy.h>
#include <Servo.h>

Fuzzy *fuzzy = new Fuzzy();

#define DHTPIN1 2     // Pin Digital Sensor
#define DHTPIN2 3     // Pin Digital Sensor
#define DHTPIN3 4     // Pin Digital Sensor
#define DHTPIN4 5     // Pin Digital Sensor
#define DHTPIN5 6     // Pin Digital Sensor
#define KIPAS 8   // Pin Digital Relay

#define DHTTYPE DHT11   

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);

Servo myServo;

float t = 0;
float h = 0;

#define targetT 40
#define targetH 60

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();


  fuzzyInit();

  pinMode(KIPAS, OUTPUT);

  myServo.attach(9);
}

void loop() {

  bacadht();

  int inputT = t - targetT;

  Serial.print("% Selisih suhu: ");
  Serial.print(inputT);

  // Set the value as an input
  fuzzy->setInput(1, inputT);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  float output = fuzzy->defuzzify(1);
  // Printing something

  Serial.print("% Pemanas : ");
  Serial.println(output);

  digitalWrite(KIPAS, map(output,0,100,0,255));

  float inputH = h - targetH;

  if(inputH >10){
    myServo.write(180);
  }else if(inputH <=0){
    myServo.write(0);
  }else{
    myServo.write(map(inputH,0,10,0,180));
  }

  delay(2000);
}

void fuzzyInit(){
  randomSeed(analogRead(0));

  // Instantiating a FuzzyInput object
  FuzzyInput *selisihSuhu = new FuzzyInput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *kecil = new FuzzySet(0, 0, 3, 5);
  FuzzySet *sedang = new FuzzySet(4, 6, 6, 8);
  FuzzySet *besar = new FuzzySet(7, 9, 50, 50);

  // Including the FuzzySet into FuzzyInput
  selisihSuhu->addFuzzySet(kecil);
  selisihSuhu->addFuzzySet(sedang);
  selisihSuhu->addFuzzySet(besar);
  
  fuzzy->addFuzzyInput(selisihSuhu);


  // Instantiating a FuzzyOutput objects
  FuzzyOutput *pwm = new FuzzyOutput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *lambat = new FuzzySet(0, 0, 0, 10);
  FuzzySet *lumayan = new FuzzySet(10, 30, 40, 60);
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
  jikaSelisihKecil->joinSingle(kecil);
  
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
  jikaSelisihBesar->joinSingle(besar);
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaCepat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaCepat->addOutput(cepat);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihBesar, makaCepat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan3);
}

void bacadht(){
  String D  = "dht";
  String Rh = ".readHumidity()";
  String Rt = ".readTemperature()";

  for(int i=1; i <= 5; i++){

    float dht1 = D+i+Rh;
    float dht2 = D+i+Rt;
    
    float h1 = 0;
    float t1 = 0;
    
    h1 += dht1;
    t1 += dht2;
  
    if (isnan(dht1) || isnan(dht2)) {
      Serial.println(F("Failed to read from DHT sensor")+i+("!"));
      return;
    }    
  }

  h = h1/5;
  t = t1/5;

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}
