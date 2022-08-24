#include <Fuzzy.h>
#include "DHT.h"
#include <Servo.h>

#define DHTPIN 2        // Digital pin connected to the DHT sensor
#define Pemanas 3
#define AC 4


#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); //Inisialisasi DHT

Servo myservo;  // create servo object to control a servo

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy(); //inisialisasi Fuzzy

int sp = 40;  //Setpoint

const int numReadings = 5;  //Jumlah sampel data

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int pos = 0;    // variable to store the servo position

void setup()
{
  // Set the Serial output
  Serial.begin(9600);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  dht.begin();  //Inisialisasi dht

	pinMode(pemanas, OUTPUT);
	pinMode(AC, OUTPUT);


  FuzzyInput *selisihSuhu = new FuzzyInput(1);  //Inisialisasi input fuzzy 1 selisihSuhu
  
  FuzzySet *kecil = new FuzzySet(0, 0, 2, 3); //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(kecil);  //Masukan objek fuzzy ke input selisihSuhu
  
  FuzzySet *sedang = new FuzzySet(1, 3, 3, 4);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(sedang); //Masukan objek fuzzy ke input selisihSuhu
  
  FuzzySet *besar = new FuzzySet(3, 10, 50, 50);  //Inisialisasi objek fuzzy
  selisihSuhu->addFuzzySet(besar);  //Masukan objek fuzzy ke input selisihSuhu

  fuzzy->addFuzzyInput(selisihSuhu); //masukan data input fuzzy ke sistem Fuzzy
  

  FuzzyOutput *pwm = new FuzzyOutput(1);  //Inisialisasi output fuzzy 1 pwm
  
  FuzzySet *lambat = new FuzzySet(0, 0, 0, 20); //Inisialisasi objek fuzzy
  pwm->addFuzzySet(lambat); //Masukan objek fuzzy ke output pwm
  
  FuzzySet *lumayan = new FuzzySet(10, 30, 30, 40); //Inisialisasi objek fuzzy
  pwm->addFuzzySet(lumayan);  //Masukan objek fuzzy ke output pwm
  
  FuzzySet *cepat = new FuzzySet(30, 90, 100, 100); //Inisialisasi objek fuzzy
  pwm->addFuzzySet(cepat);  //Masukan objek fuzzy ke output pwm
  
  fuzzy->addFuzzyOutput(pwm); //masukan data output fuzzy ke sistem fuzzy
  

  FuzzyRuleAntecedent *jikaSelisihKecil = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihKecil->joinSingle(kecil);  //selisih kecil masuk ke jikaSelisihKecil

  FuzzyRuleAntecedent *jikaSelisihSedang = new FuzzyRuleAntecedent(); //Instansiasi jikaSelisihkecil
  jikaSelisihSedang->joinSingle(sedang);  //selisih sedang masuk ke jikaSelisihSedang

  FuzzyRuleAntecedent *jikaSelisihBesar = new FuzzyRuleAntecedent();  //Instansiasi jikaSelisihkecil
  jikaSelisihBesar->joinSingle(besar);  //selisih besar masuk ke jikaSelisihBesar
  
  FuzzyRuleConsequent *makaLambat = new FuzzyRuleConsequent();  //Instansiasi makaLambat
  makaLambat->addOutput(lambat);  //pwm lambat masuk ke makaLambat

  FuzzyRuleConsequent *makaLumayan = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaLumayan->addOutput(lumayan);  //pwm lumayan masuk ke makaLumayan

  FuzzyRuleConsequent *makaCepat = new FuzzyRuleConsequent(); //Instansiasi makaLambat
  makaCepat->addOutput(cepat);  //pwm cepat masuk ke makaCepat

  // Membuat rules fuzzy
  // Jika selisih kecil > output besar
  // Jika selisih sedang > output lumayan
  // Jika selisih besar > output lambat
  
  FuzzyRule *aturan1 = new FuzzyRule(1, jikaSelisihKecil, makaCepat);
  fuzzy->addFuzzyRule(aturan1);
  
  FuzzyRule *aturan2 = new FuzzyRule(2, jikaSelisihSedang, makaLumayan);
  fuzzy->addFuzzyRule(aturan2);
  
  FuzzyRule *aturan3 = new FuzzyRule(3, jikaSelisihBesar, makaLambat);
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

  
  fuzzy->setInput(1, input);  //Masukan input Fuzzy 1
  fuzzy->fuzzify(); //proses fuzifikasi
  output = fuzzy->defuzzify(1); //proses defuzifikasi 1 & masukan ke output
  
  if (input < 1){
    output = 100;
		digitalWrite(pemanas,HIGH);
  }else{
		digitalWrite(pemanas,LOW);
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
