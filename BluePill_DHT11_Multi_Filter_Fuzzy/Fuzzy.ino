void fuzzyInit(){
  // Instantiating a FuzzyInput object
  FuzzyInput *inputF = new FuzzyInput(1);
  
  // Instantiating a FuzzySet object
  FuzzySet *gelap = new FuzzySet(0, 1, 1, 2);
  FuzzySet *sedang = new FuzzySet(1, 2, 2, 3);
  FuzzySet *terang = new FuzzySet(2, 4, 50, 50);

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
