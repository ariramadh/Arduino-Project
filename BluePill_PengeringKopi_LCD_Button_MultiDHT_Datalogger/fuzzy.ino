void fuzzySetup(){
  FuzzyInput *selisihSuhu = new FuzzyInput(1);  //Inisialisasi input fuzzy 1 selisihSuhu

  FuzzySet *Tkecil = new FuzzySet(-3, 0, 0, 4); //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tkecil);  //Masukan objek fuzzy ke input selisihSuhu

  FuzzySet *Tsedang = new FuzzySet(3, 5, 5, 7);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tsedang); //Masukan objek fuzzy ke input selisihSuhu

  FuzzySet *Tbesar = new FuzzySet(6, 8, 50, 50);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tbesar);  //Masukan objek fuzzy ke input selisihSuhu

  fuzzy->addFuzzyInput(selisihSuhu); //masukan data input fuzzy ke sistem Fuzzy
//=====================================================================================================
  FuzzyInput *selisihKelembaban = new FuzzyInput(2);  //Inisialisasi input fuzzy 2 selisihKelembaban

  FuzzySet *Hkecil = new FuzzySet(3, 3, 3, 6); //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hkecil);  //Masukan objek fuzzy ke input selisihKelembaban

  FuzzySet *Hsedang = new FuzzySet(4, 8, 8, 12);  //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hsedang); //Masukan objek fuzzy ke input selisihKelembaban

  FuzzySet *Hbesar = new FuzzySet(10, 20, 50, 50);  //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hbesar);  //Masukan objek fuzzy ke input selisihKelembaban

  fuzzy->addFuzzyInput(selisihKelembaban); //masukan data input fuzzy ke sistem Fuzzy
//=====================================================================================================
  FuzzyOutput *katup = new FuzzyOutput(1);  //Inisialisasi output fuzzy 1 katup

  FuzzySet *k1 = new FuzzySet(0, 0, 0, 20); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k1); //Masukan objek fuzzy ke output katup

  FuzzySet *k2 = new FuzzySet(10, 30, 30, 40); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k2);  //Masukan objek fuzzy ke output katup

  FuzzySet *k3 = new FuzzySet(30, 90, 100, 100); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k3);  //Masukan objek fuzzy ke output katup

  fuzzy->addFuzzyOutput(katup); //masukan data output fuzzy ke sistem fuzzy
//=====================================================================================================
  FuzzyOutput *pemanas = new FuzzyOutput(2);  //Inisialisasi output fuzzy 1 pemanas

  FuzzySet *p1 = new FuzzySet(0, 0, 0, 20); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p1); //Masukan objek fuzzy ke output pemanas

  FuzzySet *p2 = new FuzzySet(10, 30, 30, 40); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p2);  //Masukan objek fuzzy ke output pemanas

  FuzzySet *p3 = new FuzzySet(30, 90, 100, 100); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p3);  //Masukan objek fuzzy ke output pemanas

  fuzzy->addFuzzyOutput(pemanas); //masukan data output fuzzy ke sistem fuzzy
//=====================================================================================================
//=====================================================================================================
  FuzzyRuleAntecedent *jikaSelisihTKecildanHBesar = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTKecildanHBesar->joinWithOR(Tkecil, Hbesar);  //selisih kecil masuk ke jikaSelisihKecil

  FuzzyRuleAntecedent *jikaSelisihTSedangdanHSedang = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTSedangdanHSedang->joinWithOR(Tsedang, Hsedang);  //selisih kecil masuk ke jikaSelisihKecil

  FuzzyRuleAntecedent *jikaSelisihTBesardanHKecil = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTBesardanHKecil->joinWithOR(Tbesar, Hkecil);  //selisih kecil masuk ke jikaSelisihKecil
//=====================================================================================================
//=====================================================================================================

  FuzzyRuleConsequent *makaPWM1 = new FuzzyRuleConsequent();  //Instansiasi makaLambat
  makaPWM1->addOutput(k1);  //Katup buka kecil
  makaPWM1->addOutput(p3);  //Pemanas panas besar

  FuzzyRuleConsequent *makaPWM2 = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaPWM2->addOutput(k2);  //Katup buka lumayan
  makaPWM2->addOutput(p2);  //Pemanas panas lumayan

  FuzzyRuleConsequent *makaPWM3 = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaPWM3->addOutput(k3);  //Katup buka besar
  makaPWM3->addOutput(p1);  //Pemanas panas kecil

//=====================================================================================================
//=====================================================================================================

  // Membuat rules fuzzy
  // Jika selisih kecil > output besar
  // Jika selisih sedang > output lumayan
  // Jika selisih besar > output lambat

  FuzzyRule *aturan1 = new FuzzyRule(1, jikaSelisihTKecildanHBesar, makaPWM3);
  fuzzy->addFuzzyRule(aturan1);

  FuzzyRule *aturan2 = new FuzzyRule(2, jikaSelisihTSedangdanHSedang, makaPWM2);
  fuzzy->addFuzzyRule(aturan2);

  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihTBesardanHKecil, makaPWM1);
  fuzzy->addFuzzyRule(aturan3);  
}
