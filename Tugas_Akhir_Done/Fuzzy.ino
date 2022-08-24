void fuzzySetup() {
  FuzzyInput *SuhuInput = new FuzzyInput(1);

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


  FuzzyOutput *KatupOuput = new FuzzyOutput(1);

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

  fuzzy->addFuzzyOutput(KatupOuput);


  FuzzyRuleAntecedent *I1 = new FuzzyRuleAntecedent();
  I1->joinSingle(T1);
  FuzzyRuleConsequent *O1 = new FuzzyRuleConsequent();
  O1->addOutput(K1);
  FuzzyRule *R1 = new FuzzyRule(1, I1, O1);
  fuzzy->addFuzzyRule(R1);

  FuzzyRuleAntecedent *I2 = new FuzzyRuleAntecedent();
  I2->joinSingle(T2);
  FuzzyRuleConsequent *O2 = new FuzzyRuleConsequent();
  O2->addOutput(K2);
  FuzzyRule *R2 = new FuzzyRule(2, I2, O2);
  fuzzy->addFuzzyRule(R2);
  
  FuzzyRuleAntecedent *I3 = new FuzzyRuleAntecedent();
  I3->joinSingle(T3);
  FuzzyRuleConsequent *O3 = new FuzzyRuleConsequent();
  O3->addOutput(K3);
  FuzzyRule *R3 = new FuzzyRule(3, I3, O3);
  fuzzy->addFuzzyRule(R3);
  
  FuzzyRuleAntecedent *I4 = new FuzzyRuleAntecedent();
  I4->joinSingle(T4);
  FuzzyRuleConsequent *O4 = new FuzzyRuleConsequent();
  O4->addOutput(K4);
  FuzzyRule *R4 = new FuzzyRule(4, I4, O4);
  fuzzy->addFuzzyRule(R4);

  FuzzyRuleAntecedent *I5 = new FuzzyRuleAntecedent();
  I5->joinSingle(T5);
  FuzzyRuleConsequent *O5 = new FuzzyRuleConsequent();
  O5->addOutput(K5);
  FuzzyRule *R5 = new FuzzyRule(5, I5, O5);
  fuzzy->addFuzzyRule(R5);
}
