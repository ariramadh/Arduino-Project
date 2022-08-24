// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <Fuzzy.h>
#include <RBDdimmer.h>//
#include <SimpleDHT.h>

#define outputPin  3 
#define zerocross  2 // for boards with CHANGEBLE input pins

int pinDHT11 = 8;
SimpleDHT11 dht11(pinDHT11);  //Inisialisasi DHT

Fuzzy *fuzzy = new Fuzzy(); //Inisialisasi Fuzzy

dimmerLamp dimmer(outputPin); //initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero

int spSuhu = 26;  //SetPoint Suhu

int lastMillis;   //Waktu terakhir yang disimpan
int currentMillis;  //Waktu saat ini

int intervalBacaSensor = 10000;

void setup() {
  fuzzyInit();
  Serial.begin(9600); 
  dimmer.begin(NORMAL_MODE, OFF); //dimmer initialisation: name.begin(MODE, STATE) 
}

void loop(){

  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  // read without samples.

  curretMillis = millis();  //Masukan nilai milis terkini

  if(lastMillis - currentMillis >= waktuBacaSensor){
    delay(2000);  //Waktu tunggu pembacaan sensor
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
      return;
    }
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, "); 
    Serial.print((int)humidity); Serial.println(" H");

    lastMillis = curretMillis ;
  }
  
  dimmer.setState(ON);
  
  float input = temperature - spSuhu - 2;

  Serial.print(input);

  if(input >= 0){
    fuzzy->setInput(1, input);  //Masukan input fuzzy
    
    fuzzy->fuzzify(); // Running the Fuzzification
    
    float output = fuzzy->defuzzify(1); // Running the Defuzzification
    // Printing something  
    
    Serial.print("\t");
    Serial.println(output);
    dimmer.setPower(output); // setPower(0-100%);   

  }
    dimmer.setState(OFF);
}

void fuzzyInit(){
  // Instantiating a FuzzyInput object
  FuzzyInput *inputSuhu = new FuzzyInput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *dekat = new FuzzySet(0, 0, 0, 2);
  FuzzySet *sedang = new FuzzySet(2, 4, 4, 6);
  FuzzySet *jauh = new FuzzySet(6, 8, 100, 100);

  // Including the FuzzySet into FuzzyInput
  inputSuhu->addFuzzySet(dekat);
  inputSuhu->addFuzzySet(sedang);
  inputSuhu->addFuzzySet(jauh);
  
  fuzzy->addFuzzyInput(inputSuhu);


  // Instantiating a FuzzyOutput objects
  FuzzyOutput *pwm = new FuzzyOutput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *lambat = new FuzzySet(50, 60, 60, 70);
  FuzzySet *lumayan = new FuzzySet(70, 80, 80, 90);
  FuzzySet *cepat = new FuzzySet(80, 90, 90, 99);

  // Including the FuzzySet into FuzzyOutput
  pwm->addFuzzySet(lambat);
  pwm->addFuzzySet(lumayan);
  pwm->addFuzzySet(cepat);
  
  fuzzy->addFuzzyOutput(pwm);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *jikaSelisihKecil = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  jikaSelisihKecil->joinSingle(dekat);
  
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
  jikaSelisihBesar->joinSingle(jauh);
  
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *makaCepat = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  makaCepat->addOutput(cepat);
  
  // Instantiating a FuzzyRule objects
  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihBesar, makaCepat);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(aturan3);
}
