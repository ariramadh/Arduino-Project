
Fuzzy *fuzzy = new Fuzzy();

void fuzzySetup(){
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

float fuzzyProses(float input){
  float
  fuzzy->setInput(1, input);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  output = fuzzy->defuzzify(1);
  // Printing something  
  return(output);
}
