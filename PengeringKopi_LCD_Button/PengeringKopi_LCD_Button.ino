#include <Fuzzy.h>
#include "DHT.h"
#include <Servo.h>
#include "OneButton.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define S0 8  //S0 MultiPlexer
#define S1 7  //S1 MultiPlexer
#define S2 6  //S2 MultiPlexer
#define S3 5  //S3 MultiPlexer

#define DHTPIN 2  // Digital pin connected to the DHT sensor
#define heater 3

#define DHTTYPE DHT11   // DHT 11

#define jumlahDHT 6   //Jumlah dht yang dipakai

DHT dht(DHTPIN, DHTTYPE); //Inisialisasi DHT

OneButton btnP(A0, false);
OneButton btnM(A1, false);
OneButton btnT(A2, false);
OneButton btnSS(A3, false);

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

Servo myservo;  // create servo object to control a servo

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy(); //inisialisasi Fuzzy

int spT = 40;  //SetpointSuhu
int spH = 60;  //SetpointSuhu

int hum = 60;  //Kadar air awal

const int numReadings = 5;  //Jumlah sampel data

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int readingsT[numReadings];      // the readings from the analog input
int readIndexT = 0;              // the index of the current reading
int totalT = 0;                  // the running total
int averageT = 0;                // the average

int readingsH[numReadings];      // the readings from the analog input
int readIndexH = 0;              // the index of the current reading
int totalH = 0;                  // the running total
int averageH = 0;                // the average

bool running = false;

bool val;

void setup()
{
  // Set the Serial output
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  lcd.init();                      // initialize the lcd
  lcd.backlight();

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  dht.begin();  //Inisialisasi dht

  pinMode(heater, OUTPUT);

  btnP.attachClick(clickP);
  btnP.attachDuringLongPress(longPressP);

  btnM.attachClick(clickM);
  btnM.attachDuringLongPress(longPressM);

  btnT.attachClick(clickT);

  btnSS.attachLongPressStart(longPressSS);

  fuzzySetup();
}

void loop(){
  button1.tick();
  button2.tick();
  btnSS.tick();
  btnT.tick();
  
  Serial.println(running);
  
  if(running){
    run();
  }else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Kelembaban: ");
    lcd.write(hum);
    lcd.write("%");

    lcd.setCursor(1, 0);
    lcd.write("Berat Awal:");
    lcd.write("xxx");
    lcd.write("KG");
  }
}

void run()
{
  delay(1500);

  // Baca Suhu
  float h = dht.readHumidity();
  // Baca temperatur celcius
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));

    running = false;    
    return;
  }

  int errDHT = 0;

  totalH = totalH - readingsH[readIndexH];
  // read from the sensor:
  readingsH[readIndex] = h;
  
  if (readIndexH >= numReadings) {
    // add the reading to the total:    
    totalH = totalH + readingsH[readIndexH];
  }else{
    spH = readingsH[readIndexH];
  }
  // advance to the next position in the array:
  readIndexH = readIndexH + 1;

  // if we're at the end of the array...
  if (readIndexH >= numReadings) {
    // ...wrap around to the beginning:
    readIndexH = 0;
  }

  // calculate the average:
  averageH = totalH / numReadings;

  totalT = totalT - readingsT[readIndexT];
  // read from the sensor:
  readingsT[readIndexT] = t;
  // add the reading to the total:
  totalT = totalT + readingsT[readIndexT];
  // advance to the next position in the array:
  readIndexT = readIndexT + 1;

  // if we're at the end of the array...
  if (readIndexT >= numReadings) {
    // ...wrap around to the beginning:
    readIndexT = 0;
  }

  // calculate the average:
  averageT = totalT / numReadings;

  Serial.print(F("Humidity: "));
  Serial.print(averageH);
  Serial.print(F("%  Temperature: "));
  Serial.print(averageT);
  Serial.println(F("�C"));

  float inputH = spH - averageH;
  float inputT = spT - averageT;
  float output1;
  int output2;
  
  fuzzy->setInput(1, inputT);  //Masukan input Fuzzy 1
  fuzzy->setInput(2, inputH);  //Masukan input Fuzzy 2
  fuzzy->fuzzify(); //proses fuzifikasi
  output1 = fuzzy->defuzzify(1); //proses defuzifikasi 1 & masukan ke output
  output2 = fuzzy->defuzzify(2); //
  
  if (inputT < 1) {
    output1 = 100;
    digitalWrite(heater, HIGH);
  } else {
    digitalWrite(heater, LOW);
  }

  Serial.print("SetPoint:");
  Serial.print(spT);
  Serial.print("\tSelisih suhu: ");
  Serial.print(inputT);

  Serial.print("\tOutput: ");
  Serial.println(output1);
  Serial.println("");

  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = output1;
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

  output1 = map(average, 0, 100, 0, 255);

  myservo.write(output1);   // sets the servo position according to the scaled value

  Serial.write(output2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("T:");
  lcd.write(t);
  lcd.setCursor(7, 0);
  lcd.write("C");
  lcd.setCursor(11, 0);
  lcd.write("S:");
  lcd.write(spT);
  lcd.setCursor(16, 0);
  lcd.write("C");

  lcd.setCursor(0, 1);
  lcd.write("H:");
  lcd.setCursor(4, 1);
  lcd.write(h);
  lcd.setCursor(7, 1);
  lcd.write("%");
  lcd.setCursor(11, 1);
  lcd.write("K:");
  lcd.write("xxx");
  lcd.setCursor(16, 1);
  lcd.write("%");
}

void fuzzySetup(){
  FuzzyInput *selisihSuhu = new FuzzyInput(1);  //Inisialisasi input fuzzy 1 selisihSuhu

  FuzzySet *Tkecil = new FuzzySet(-3, 0, 0, 4); //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tkecil);  //Masukan objek fuzzy ke input selisihSuhu

  FuzzySet *Tsedang = new FuzzySet(3, 5, 5 7);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tsedang); //Masukan objek fuzzy ke input selisihSuhu

  FuzzySet *Tbesar = new FuzzySet(6, 8, 50, 50);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(Tbesar);  //Masukan objek fuzzy ke input selisihSuhu

  fuzzy->addFuzzyInput(selisihSuhu); //masukan data input fuzzy ke sistem Fuzzy


  FuzzyInput *selisihKelembaban = new FuzzyInput(2);  //Inisialisasi input fuzzy 2 selisihKelembaban

  FuzzySet *Hkecil = new FuzzySet(3, 3, 3, 6); //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hkecil);  //Masukan objek fuzzy ke input selisihKelembaban

  FuzzySet *Hsedang = new FuzzySet(4, 8, 8, 12);  //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hsedang); //Masukan objek fuzzy ke input selisihKelembaban

  FuzzySet *Hbesar = new FuzzySet(10, 20, 50, 50);  //Inisialisasi objek fuzzy
  selisihKelembaban->addFuzzySet(Hbesar);  //Masukan objek fuzzy ke input selisihKelembaban

  fuzzy->addFuzzyInput(selisihKelembaban); //masukan data input fuzzy ke sistem Fuzzy


  FuzzyOutput *katup = new FuzzyOutput(1);  //Inisialisasi output fuzzy 1 katup

  FuzzySet *k1 = new FuzzySet(0, 0, 0, 20); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k1); //Masukan objek fuzzy ke output katup

  FuzzySet *k2 = new FuzzySet(10, 30, 30, 40); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k2);  //Masukan objek fuzzy ke output katup

  FuzzySet *k3 = new FuzzySet(30, 90, 100, 100); //Inisialisasi objek fuzzy
  katup->addFuzzySet(k3);  //Masukan objek fuzzy ke output katup

  fuzzy->addFuzzyOutput(katup); //masukan data output fuzzy ke sistem fuzzy


  FuzzyOutput *pemanas = new FuzzyOutput(2);  //Inisialisasi output fuzzy 1 pemanas

  FuzzySet *p1 = new FuzzySet(0, 0, 0, 20); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p1); //Masukan objek fuzzy ke output pemanas

  FuzzySet *p2 = new FuzzySet(10, 30, 30, 40); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p2);  //Masukan objek fuzzy ke output pemanas

  FuzzySet *p3 = new FuzzySet(30, 90, 100, 100); //Inisialisasi objek fuzzy
  pemanas->addFuzzySet(p3);  //Masukan objek fuzzy ke output pemanas

  fuzzy->addFuzzyOutput(pemanas); //masukan data output fuzzy ke sistem fuzzy


  FuzzyRuleAntecedent *jikaSelisihTKecildanHBesar = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTKecildanHBesar->joinWithOR(Tkecil, Hbesar);  //selisih kecil masuk ke jikaSelisihKecil

  FuzzyRuleAntecedent *jikaSelisihTSedangdanHSedang = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTSedangdanHSedang->joinWithOR(Tsedang, Hsedang);  //selisih kecil masuk ke jikaSelisihKecil

  FuzzyRuleAntecedent *jikaSelisihTBesardanHKecil = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihTBesardanHKecil->joinWithOR(Tbesar, Hkecil);  //selisih kecil masuk ke jikaSelisihKecil


  FuzzyRuleConsequent *makaPWM1 = new FuzzyRuleConsequent();  //Instansiasi makaLambat
  makaPWM1->addOutput(k1);  //Katup buka kecil
  makaPWM1->addOutput(p3);  //Pemanas panas besar

  FuzzyRuleConsequent *makaPWM2 = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaPWM2->addOutput(k2);  //Katup buka lumayan
  makaPWM2->addOutput(p2);  //Pemanas panas lumayan

  FuzzyRuleConsequent *makaPWM3 = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaPWM3->addOutput(k3);  //Katup buka besar
  makaPWM3->addOutput(p1);  //Pemanas panas kecil

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

void clickM(){
  if (hum > 0) {
    hum -= 1;  
  }
}

void clickP(){
  if (hum < 100) {
    hum += 1;
  }
}

void longPressM(){
  if (hum > 0) {
    hum -= 1;
  }else{
    hum = 0;
  }
  delay(50);
}

void longPressP(){
  if (hum < 100) {
    hum += 1;
  }else{
    hum = 100;
  }
  delay(50);
}


void clickT(){
  digitalWrite(LED_BUILTIN, val);
  val = !val;

//  btnTare();
}

void longPressSS(){
  running = !running;
}
