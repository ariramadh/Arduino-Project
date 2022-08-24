void fuzzySetup() {
  FuzzyInput *SuhuInput = new FuzzyInput(1);  //Mendeklarasikan objek FuzzyInput dgn nama SuhuInput

  FuzzySet *T1 = new FuzzySet(25, 25, 38, 39);
  SuhuInput->addFuzzySet(T1);

  FuzzySet *T2 = new FuzzySet(38, 39, 39, 40);
  SuhuInput->addFuzzySet(T2);

  FuzzySet *T3 = new FuzzySet(39, 40, 40, 41);
  SuhuInput->addFuzzySet(T3);

  FuzzySet *T4 = new FuzzySet(40, 41, 41, 42);
  SuhuInput->addFuzzySet(T4);

  FuzzySet *T5 = new FuzzySet(41, 42, 50, 50);
  SuhuInput->addFuzzySet(T5);

  fuzzy->addFuzzyInput(SuhuInput);


  FuzzyOutput *KatupOuput = new FuzzyOutput(1); //Mendeklarasikan objek FuzzyOutput dgn nama KatupOutput

  FuzzySet *K1 = new FuzzySet(0, 0, 0, 10);
  KatupOuput->addFuzzySet(K1);

  FuzzySet *K2 = new FuzzySet(0, 10, 10, 20);
  KatupOuput->addFuzzySet(K2);

  FuzzySet *K3 = new FuzzySet(10, 20, 20, 50);
  KatupOuput->addFuzzySet(K3);

  FuzzySet *K4 = new FuzzySet(20, 50, 50, 60);
  KatupOuput->addFuzzySet(K4);

  FuzzySet *K5 = new FuzzySet(50, 60, 90, 90);
  KatupOuput->addFuzzySet(K5);

  fuzzy->addFuzzyOutput(KatupOuput);  //Memasukan Fuzzyset kedalam katupoutput


  FuzzyRuleAntecedent *I1 = new FuzzyRuleAntecedent();  //Memasukan objek sebab
  I1->joinSingle(T1); //Memasukan nilai fuzzyset suhuInput kedalam sebab
  FuzzyRuleConsequent *O1 = new FuzzyRuleConsequent(); //Memasukan objek akibat
  O1->addOutput(K1);  //Memasukan nilai fuzzyset katupOutput kedalam akibat
  FuzzyRule *R1 = new FuzzyRule(1, I1, O1); //Memasukan aturan IF-THEN
  fuzzy->addFuzzyRule(R1);  //Memasukan rule kedalam fuzzy

  FuzzyRuleAntecedent *I2 = new FuzzyRuleAntecedent();  //Memasukan objek sebab
  I2->joinSingle(T2); //Memasukan nilai fuzzyset suhuInput kedalam sebab
  FuzzyRuleConsequent *O2 = new FuzzyRuleConsequent(); //Memasukan objek akibat
  O2->addOutput(K2);  //Memasukan nilai fuzzyset katupOutput kedalam akibat
  FuzzyRule *R2 = new FuzzyRule(2, I2, O2); //Memasukan aturan IF-THEN
  fuzzy->addFuzzyRule(R2);  //Memasukan rule kedalam fuzzy
  
  FuzzyRuleAntecedent *I3 = new FuzzyRuleAntecedent();  //Memasukan objek sebab
  I3->joinSingle(T3); //Memasukan nilai fuzzyset suhuInput kedalam sebab
  FuzzyRuleConsequent *O3 = new FuzzyRuleConsequent(); //Memasukan objek akibat
  O3->addOutput(K3);  //Memasukan nilai fuzzyset katupOutput kedalam akibat
  FuzzyRule *R3 = new FuzzyRule(3, I3, O3); //Memasukan aturan IF-THEN
  fuzzy->addFuzzyRule(R3);  //Memasukan rule kedalam fuzzy
  
  FuzzyRuleAntecedent *I4 = new FuzzyRuleAntecedent();  //Memasukan objek sebab
  I4->joinSingle(T4); //Memasukan nilai fuzzyset suhuInput kedalam sebab
  FuzzyRuleConsequent *O4 = new FuzzyRuleConsequent(); //Memasukan objek akibat
  O4->addOutput(K4);  //Memasukan nilai fuzzyset katupOutput kedalam akibat
  FuzzyRule *R4 = new FuzzyRule(4, I4, O4); //Memasukan aturan IF-THEN
  fuzzy->addFuzzyRule(R4);  //Memasukan rule kedalam fuzzy

  FuzzyRuleAntecedent *I5 = new FuzzyRuleAntecedent();  //Memasukan objek sebab
  I5->joinSingle(T5); //Memasukan nilai fuzzyset suhuInput kedalam sebab
  FuzzyRuleConsequent *O5 = new FuzzyRuleConsequent(); //Memasukan objek akibat
  O5->addOutput(K5);  //Memasukan nilai fuzzyset katupOutput kedalam akibat
  FuzzyRule *R5 = new FuzzyRule(5, I5, O5); //Memasukan aturan IF-THEN
  fuzzy->addFuzzyRule(R5);  //Memasukan rule kedalam fuzzy
}
