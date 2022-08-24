#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C
#include "OneButton.h"

//============Pressure===========
float pressure = 0,           //Sensor tekanan
      pip = 0, plateau = 0, peep = 0, //Variabel untuk penyimpan peak, plateau, dan peep
      Q1 = 0, Q1b = 0,                //Variabel kecepatan udaraa
      volume = 0;             //Variabel menyimpan volume
String tekanan;
const float MIN_PRESSURE = 1.0; //variabel batas atas dan bawah tekanan. cmH20
float MAX_PRESSURE = 20.0; //variabel batas atas dan bawah tekanan. cmH20
const int pResist_Max = 10;
//===============================

//==========Timer================
float tHold = 0.1;  //Waktu penahanan nafas
int timer_pressure_limit = 5000;  //Batas waktu apabila tekanan dibawah nilai yg ditetapkan

unsigned long td, td_1, tp, tp_1, interval_limit;

unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas

float tHoldIn;         // Calculated time (s) since tCycleTimer for end of HOLD_IN_STATE
float tPeriod;         // Calculated time (s) since tCycleTimer for end of cycle

unsigned long tBef;
unsigned long sampling;

float tIn;             // Calculated time (s) since tCycleTimer for end of IN_STATE
float tEx;             // Calculated time (s) since tCycleTimer for end of EX_STATE
//===============================

//==========Pin==================
#define potensioTV A0
#define potensioRR A1
#define potensioIE A2
#define potensioPEEP A3
#define potensioFIO2 A4

#define ALARM_PIN 9
#define SNOOZE_PIN 8

#define Min1 11
#define Min2 10
#define Men 12
#define Mslp 6

#define ledPin 12

#define setPin 2
//#define limSwitch 3
#define encoder 3
//===============================

//============Variabel===========
int duration[3];  //Variabel untuk menyimpan nilai waktu setiap siklus
int command[] = {0, 1, 2}; //Perintah siklus
//0 = tarik nafas
//1 = tahan nafas
//2 = buang nafas

int nilaiPotensioTV = 0;    //Nilai Potensio TV
int nilaiPotensioRR = 0;    //Nilai Potensio RR
int nilaiPotensioIE = 0;    //Nilai Potensio IE
int nilaiPotensioPEEP = 0;  //Nilai Potensio PEEP

int TV, RR, AC, PR, tv, TVset, RRset, set;
float IE, IEset, desIE;

int TVb, RRb, IEb, ACb, PRb, pressureb, plateaub, peepb; //Variabel untuk kontrol display

int count, countBef; //Menghitung pulsa dan setpoint pulsa

String indi[4] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "};
int indikator[4];

int posisi;
int pulsePerSecond;
int pwm;

String teksAlarm;
int alarm;

int zPressure[2];  //Variabel untuk mengkalibrasi tekanan

String potensio;
//===============================

//==============Flag=============
bool start;
volatile byte state = LOW;
int limState = 0; //Variabel menyimpan fungsi limit
bool rise = false;  //Menentukan apakah encoder count up atau count down
bool inOk = true, inState = false;
bool oneCycle = false;
bool ACMode = false;
bool toStart = false;
//===============================

//==============LCD==============
LiquidCrystal_I2C lcd(0x27, 20, 4);
//===============================

//==============BUTTON==============
// Setup a new OneButton on pin A1.
OneButton btnSet(setPin, true);
unsigned long btnTick;
//==================================

void setup() {
  Serial.begin(115200); //Inisialisasi komunikasi serial
  //  Serial3.begin(115200); //Inisialisasi komunikasi serial

  //  for (int i=0; i<=10; i++){
  //    Serial3.print("/P/#");
  //    delay(500);
  //  }
  //==========Definisi pin=========
  pinMode(setPin, INPUT_PULLUP);
  pinMode(SNOOZE_PIN, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  //  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);

  digitalWrite(Men, HIGH);
  digitalWrite(Mslp, HIGH);
  digitalWrite(Min2, LOW);
  digitalWrite(Min1, LOW);
  //===============================

  zPressure[0] = analogRead(A6) - 464;  //Mengkalibrasi ADC tekanan
  zPressure[1] = analogRead(A5) - 468;  //Mengkalibrasi ADC tekanan

  Serial.println("SJT Ventilator");

  //========Inisialisasi LCD=======
//  lcd.backlight();
//  lcd.init();
//  lcd.clear();
//  lcdTeks();
  //===============================

  //============Variabel===========
  interval_limit = 1000;

  start = false;  //Flag menyatakan program tidak akan berjalan bila belum ditekan Set

  int posisi;
  int pulsePerSecond;
  int pwm;
  //===============================

  //========Memasang Interupt======
  attachInterrupt(digitalPinToInterrupt(setPin), setBtn, HIGH);
  //  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder),  motorEncoder, FALLING);
  //===============================

  btnSet.attachClick(modeSet);
  btnSet.attachLongPressStart(setStart);

  //  btnSet.attachDuringLongPress(setCount);
}


void loop() {
  //==============Program Utama==================
  if (start)  {
    if (toStart) {
      homing(); //Jalankan function Homing();
    }
    toStart = false;
    //    zeroingDisplay();
    inOk = true;
    oneCycle = false;
    run();
    check_CurrentAlarm();
    read_pressure();  //Baca tekanan
  } else {
    toStart = true;
    //    zeroingDisplay();

    get_pressure();
    get_flow();
    get_encoder();
    get_potensio();

    //    Serial.print("\t Potensio : "); Serial.print(potensio);
    Serial.print("\t Pressure : "); Serial.print(pressure);
    Serial.print("\t Flow : "); Serial.print(Q1);
    Serial.print("\t Count : "); Serial.print(count);
    lcdTeks();
    lcd_disp(plateau, pip, peep);

    //*************TV*************
    //    nilaiPotensioTV = analogRead(potensioTV);
    //    TV = map(nilaiPotensioTV, 1020, 0, 2.5, 80);
    //    TV = TV * 10 ;
    //  Serial.println(nilaiPotensioTV);
    //****************************

    //*************RR*************
    //    nilaiPotensioRR = analogRead(potensioRR);
    //    RR = map(nilaiPotensioRR, 0, 1023, 10, 30);
    //****************************

    //*************IE*************
    //    nilaiPotensioIE = analogRead(potensioIE);
    //    IE = map(nilaiPotensioIE, 1023, 0, 1, 4);
    //****************************

    //*************PEEP***********
    //    nilaiPotensioPEEP = analogRead(potensioPEEP);

    if (ACMode) {
      AC = map(nilaiPotensioPEEP, 1023, 0, 1, 5);
    } else {
      PR = map(nilaiPotensioPEEP, 1023, 0, 15, 40);
    }

    //****************************

    //*************AC*************
    //    nilaiPotensioFIO2 = analogRead(potensioFIO2);
    //    AC = map(nilaiPotensioPEEP, 0, 1023, 2, 7);
    //****************************
  }

  //  Serial.print("\tStart: "); Serial.print(start);
  //  Serial.print("\tAC Mode: "); Serial.print(ACMode);
  //  Serial.print("\tAC: "); Serial.print(AC);
  //  Serial.print("\tPeak: "); Serial.print(pip);
  //  Serial.print("\tPlateau: "); Serial.print(plateau);
  //  Serial.print("\tPEEP: "); Serial.print(peep);
  //  Serial.print("\n=======================================================================================================================================");
  //
  //  Serial.println();
  //=============================================

  //============Alarm============================
  int snz_state = digitalRead(SNOOZE_PIN);  //Inisialisasi pin Snooze

  //  Serial.print("  SNOOZE  "); Serial.println(snz_state);

  if (snz_state == LOW) {
    tp_1 = tp;
    alarm = 0;
    setAlarm(alarm);
  }
  //=============================================
  Serial.println();
}

void hitung() {
  tPeriod = 60.0 / RR;  //Menghitung perioda  //detik/nafas
  tPeriod = tPeriod - tHold;  //Menghitung perioda dikurangi waktu tahan nafas
  //  Serial.print("tPeriod "); Serial.print(tPeriod);
  //  Serial.print("");
  tIn = tPeriod / (1 + IE); //Menghitung waktu tarik nafas  //detik
  tEx = tPeriod - tIn;  //Menghitung buang nafas  //detik

  duration[0] = tIn * 1000; //Atur durasi tarik nafas milidetik
  duration[1] = tHold * 1000; //Atur durasi tahan nafas milidetik
  duration[2] = tEx * 1000;//Atur durasi buangnafas milidetik

}

void run() {
  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = 0; //reset limit
    Serial.print("Running");
    //    i = 0;

    Serial.print("\tIn Time\t"); Serial.print(duration[0]);   //Waktu Inhale
    Serial.print("\tHo Time\t"); Serial.print(duration[1]);   //Waktu Inhale
    Serial.print("\tEx Time\t"); Serial.println(duration[2]); //Waktu Inhale
    perintah(duration[i], command[i]);  //Panggil fungsi durasi dan siklus
  }

  //==================Indikator==================
  for (int i = 0; i <= 3; i++) {
    Serial.print(indi[i]);
    if (indikator[i]) {
      Serial.print("OK!!!");
    } else {
      Serial.print("FAIL!!!");
    }
  }

  //  delay(5000);
  //=============================================
}

void perintah(int durasi, int cycle) {
  //    read_pressure();
  //  lcd_disp(Q1, pressure);

  //  Serial.print("\tDurasi: "); Serial.print(durasi);    //Waktu berjalan inhale


  tick = millis();

  tBef = 0;   //Mereset time before
  volume = 0; //Mereset volume

  //3.5976x + 1760.9
  posisi = (3.5976 * TV) + 1761;                //
  pulsePerSecond = posisi / (durasi / 1000.0);  //

  pwm = (0.0581 * pulsePerSecond) + 27.065;
  pwm = constrain(pwm, 0, 255);

  lcdTeks();
  lcd_disp(plateau, pip, peep);

  switch (cycle) {  //Jalankan perintah sesuai siklus
    case 0:
      //      Serial.println();
      //      Serial.print("  pwm   "); Serial.println(pwm);  //Waktu Inhale

      while (count <= posisi && inOk) { //Jalankan perintah sesuai durasi
        //        Serial.print("\tINHALE\t");
        //        Serial.print("\tAlarm\t");  Serial.print(alarm);

        get_pressure();
        setAlarm(alarm);
        //        Serial.print(" pressure \t: ");Serial.println(pressure);
        inState = true;
        //        Serial.print("  Inhale OK:  "); Serial.print(inOk);    //Waktu berjalan inhale
        //        Serial.print("  PosT:  "); Serial.print(posisi);    //Waktu berjalan inhale
        //        Serial.print("\tPos:\t"); Serial.print(count);    //Waktu berjalan inhale
        //        Serial.print("\tPosBef:\t"); Serial.print(countBef);    //Waktu berjalan inhale

        rise = true;

        sampling = millis() - tick;

        //        Serial.print("\tSampling\t"); Serial.print(sampling - tBef);  //Waktu berjalan inhale

        get_flow();
        volume += (Q1 * ((sampling - tBef) / 60000.0)) * 1000;

        tBef = sampling;

        //        Serial.print("  Inhale  "); Serial.print(tIn * 1000);  //Waktu berjalan inhale
        //        Serial.print("  tIn   "); Serial.print(sampling);    //Waktu Inhale
        //        Serial.print("\tVolume\t"); Serial.print(volume);    //Waktu Inhale
        analogWrite(Min2, pwm);    //Jalankan motor
        digitalWrite(Min1, LOW);    //Jalankan motor

        delay(10);

        check_CurrentAlarm();

        //        Serial.print("\tC\t"); Serial.print(count);    //Waktu Inhale
        //        Serial.print("\tCb\t"); Serial.print(countBef);    //Waktu Inhale

        if (abs(count - countBef) <= 10) {
          inOk = false;

          Serial.println ("\tMOTOR STUCK!!\t");

          digitalWrite(Min2, LOW);    //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
          countBef = 0;
          //          homing();
          start = false;
        }

        //        Serial.print("  In OK?   "); Serial.print(inOk);    //Waktu Inhale
        //        Serial.println();
        countBef = count;
      }

      inState = false;

      pip = pressure;
      lcd_disp(plateau, pip, peep);

      indikator[1] = 1; //Indikator apabila inhale selesai
      if (!inOk) {
        indikator[1] = 0; //Indikator apabila inhale gagal
        indikator[2] = 0; //Indikator apabila inhale gagal
        indikator[3] = 0; //Indikator apabila inhale gagal
      }
      break;

    case 1:
      //      Serial.println();
      while (millis() - tick  <= tHold * 1000 && inOk) { //Jalankan perintah sesuai durasi
        //        Serial.print("\tHOLD\t");
        //        Serial.print("\tAlarm\t");  Serial.print(alarm);
        //        Serial.print("  Pos:  "); Serial.print(count);    //Waktu berjalan inhale
        //        read_pressure();
        get_pressure();
        check_CurrentAlarm();

        //        Serial.print("  Hold  "); Serial.print(tHold * 1000);  //Waktu berjalan inhale
        //        Serial.print("  tHold  "); Serial.println(millis() - tick);    //Waktu berjalan hold

        digitalWrite(Min2, LOW);    //Motor berhenti
        digitalWrite(Min1, LOW);    //Motor berhenti
        setAlarm(alarm);
        //        Serial.println();
      }

      plateau = pressure;
      lcd_disp(plateau, pip, peep);
      indikator[2] = 1; //Indikator apabila hold selesai

      break;

    case 2:   //Apabila timer exhale sudah fix, while bisa dihilangkan

      rise = false;

      //      Serial.println();
      //      Serial.print("  pwm   "); Serial.println(pwm);  //Waktu Inhale

      //========Limit Switch========

      while (millis() - tick <= tEx * 1000 && inOk) { //Kondisi motor akan bergerak hanya jika limitSwitch tidak ditekan
        //      while ((limState == 0 || count >= 200)) { //Kondisi motor akan bergerak hanya jika limitSwitch tidak ditekan
        while (count >= 558 && limState == 0) {
          Serial.print("\tEXHALE\t");
          //                    Serial.print("\tAlarm\t");  Serial.print(alarm);
          //                      Serial.print("  Pos:  "); Serial.print(count);    //Waktu berjalan inhale
          //                      Serial.print("  Kondisi:  "); Serial.print(millis() - tick <= tEx * 1000 || count >= 200 || limState == 1);  //Waktu berjalan inhale
          //          read_pressure();
          get_pressure();
          check_CurrentAlarm();
          //            Serial.print("  Exhale  "); Serial.print(tEx * 1000);  //Waktu berjalan inhale
          digitalWrite(Min2, LOW);    //Jalankan motor
          analogWrite(Min1, constrain(pwm * 2, 0, 255)); //Jalankan motor
          //            Serial.print("  tEx   "); Serial.println(millis() - tick);    //Waktu Inhale
          //          Serial.print("\tPWMEx: "); Serial.print(pwm);
          setAlarm(alarm);
          Serial.println();
        }

        peep = pressure;
        lcd_disp(plateau, pip, peep);
        digitalWrite(Min2, LOW);    //Jalankan motor
        digitalWrite(Min1, LOW); //Jalankan motor
        //          Serial.print("  Exhale  "); Serial.print(tEx * 1000);  //Waktu berjalan inhale
        //          Serial.print("  tEx   "); Serial.println(millis() - tick);    //Waktu Inhale
      }

      //      peep = pressure;
      //      lcd_disp(plateau, pressure1, peep);

      digitalWrite(Min2, LOW);    //Jalankan motor
      digitalWrite(Min1, LOW); //Jalankan motor

      indikator[3] = 1; //Indikator apabila exhale selesai
      //============================
      break;
  }

  if (pip - plateau >= pResist_Max) {
    alarm = 4;
  }

  digitalWrite(Min2, LOW);    //Motor berhenti
  digitalWrite(Min1, LOW);    //Motor berhenti

  oneCycle = true;
  //  Serial.print("  CYCLE END!!");
  Serial.println();
}

void get_potensio() {
  int k = 0;
  String daPot[4];
  Serial.print("/K/#");
  readSerial();

  //  pot[] =
  //  daPot = strtok(potensio.toCharArray(), "*");

  //  Serial.print("\t Test: ");  Serial.println(potensio);

  for (int i = 0; i <= potensio.length(); i++) {
    //    Serial.print("\t Test: ");  Serial.println(potensio.length());
    //    Serial.print("\t Test: ");  Serial.println(potensio[i]);
    //    Serial.print("\t i: ");  Serial.println(i);
    if (potensio[i] == '*') {
      k++;
    } else {
      daPot[k] += potensio[i];
      //      Serial.print("\t Test: ");  Serial.print(k);
      //      Serial.print("\t Test: ");  Serial.println(daPot[k]);
    }
  }

  //  for (int i = 0; i < 4; i ++) {
  //    Serial.print("\t Data pot: ");  Serial.print(daPot[i]);    Serial.print(" i: ");  Serial.println(i);
  //  }

  TV = daPot[0].toInt();
  RR = daPot[1].toInt();
  IE = daPot[2].toInt();
  if (ACMode) {
    AC = daPot[3].toInt();
  } else {
    PR = daPot[3].toInt();
  }
  //  Serial.print("\t Nilai TV: ");  Serial.println(TV);
  //  Serial.print("\t Nilai RR: ");  Serial.println(RR);
  //  Serial.print("\t Nilai IE: ");  Serial.println(IE);
  //  Serial.print("\t Nilai AC: ");  Serial.println(AC);
}
