#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C
#include <PID_v1.h> //Memanggil library

//============Pressure===========
float pres, pressure, pressure1, plat2,
      rho = 1.2, //mass jenis udara 1.2 Kg/m3
      a1 = 188.691 * 1e-6,  //Luas venturi tube
      a2 = 103.868 * 1e-6, //Luas venturi tube
      v1, v2, Q1, Q2,   //Kecepatan udaraa
      interval_limit,
      volume;

const float MIN_PRESSURE = 1.0, MAX_PRESSURE = 20.0; //variabel batas atas dan bawah tekanan. satuan??
//===============================

//==========Timer================
float tHold = 0.1;  //Waktu penahanan nafas
int timer_pressure_limit = 5000;  //Batas waktu apabila tekanan dibawah nilai yg ditetapkan
unsigned long t, t_1, t_2, t_3, td, td_1, tp, tp_1;

unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas

float tCycleTimer;     // Absolute time (s) at start of each breathing cycle
float tHoldIn;         // Calculated time (s) since tCycleTimer for end of HOLD_IN_STATE
float tPeriod;         // Calculated time (s) since tCycleTimer for end of cycle
float tPeriodActual;   // Actual time (s) since tCycleTimer at end of cycle (for logging)
float tLoopTimer;      // Absolute time (s) at start of each control loop iteration
float tLoopBuffer;

unsigned long tBef;
unsigned long sampling;

float tIn;             // Calculated time (s) since tCycleTimer for end of IN_STATE
float tEx;             // Calculated time (s) since tCycleTimer for end of EX_STATE
//===============================

//==========Pin==================
const int potensioTV = A0;
const int potensioRR = A1;
const int potensioIE = A2;
const int potensioPEEP = A3;
const int potensioFIO2 = A4;

int ALARM_PIN = 13;
int SNOOZE_PIN = 4;

#define Min1 9
#define Min2 8
#define Men 10
#define Mslp 11

#define ledPin 12

const int  setPin = 2;
#define limSwitch 3
#define encoder 18
//===============================

//============Variabel===========
int duration[2];  //Variabel untuk menyimpan nilai waktu setiap siklus
int command[] = {0, 1, 2}; //Perintah siklus
//0 = tarik nafas
//1 = tahan nafas
//2 = buang nafas

int nilaiPotensioTV = 0;
int nilaiPotensioRR = 0;
int nilaiPotensioIE = 0;
int nilaiPotensioPEEP = 0;
int nilaiPotensioFIO2 = 0;

int TV, RR, TV1, RR1, set, AC, tv;
float IE, IE1, desIE, Vin;

char cTV[3];
int lastOutput[3]; //Untuk menyimpan nilai EWMA terakhir  //Tambah ukuran array jika ingin memperbanyak penggunaan ewma
int count, countBef, setpoint = 3000; //Menghitung pulsa dan setpoint pulsa

float plateau, peep;

String indi[4] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "};
int indikator[4];

int posisi;
int pulsePerSecond;
int pwm;

int alarm;
//===============================

//==============Flag=============
int startAlarm;
bool start;
volatile byte state = LOW;
int limState = 0; //Variabel menyimpan fungsi limit
bool rise = false;  //Menentukan apakah encoder count up atau count down
bool inOk = true, inState = false;
//===============================

//==============LCD==============
LiquidCrystal_I2C lcd(0x27, 20, 4);
//===============================

void setup() {
  Serial.begin(115200); //Inisialisasi komunikasi serial

  //==========Definisi pin=========
  pinMode(setPin, INPUT_PULLUP);
  pinMode(SNOOZE_PIN, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  pinMode(limSwitch, INPUT_PULLUP);
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

  Serial.println("SJT Ventilator  1");

  //========Inisialisasi LCD=======
  lcd.backlight();
  lcd.init();
  //===============================

  Serial.println("SJT Ventilator  2");

  //============Variabel===========
  interval_limit = 1000;

  start = false;  //Flag menyatakan program tidak akan berjalan bila belum ditekan Set

  startAlarm = 0;

  int posisi;
  int pulsePerSecond;
  int pwm;
  //===============================


  //========Memasang Interupt======
  attachInterrupt(digitalPinToInterrupt(setPin), blink1, FALLING);
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder),  motorEncoder, FALLING);
  //===============================
}

void loop() {

  //========Awal program==========
  int setPin_state = digitalRead(setPin); //Baca setpin
  if (setPin_state == LOW) {
    homing(); //Jalankan function Homing()
    start = true; //mengubah a menjadi true

    hitung(); //Panggil function hitung

    duration[0] = tIn * 1000; //Atur durasi tarik nafas milidetik
    duration[1] = tHold * 1000; //Atur durasi tahan nafas milidetik
    duration[2] = tEx * 1000;//Atur durasi buangnafas milidetik
  }

  //===============================

  //==============Program Utama==================
  if (start == true)  {
    startAlarm = 0;
    run();
  } else {
    check_CurrentAlarm();
    read_pressure();  //Baca tekanan
    lcd_disp(plateau, pressure1, peep);

    //*************TV*************
    nilaiPotensioTV = analogRead(potensioTV);
    TV = map(nilaiPotensioTV, 0, 1020, 0, 100);
    TV = TV * 10 ;
    //  Serial.println(nilaiPotensioTV);
    //****************************

    //*************RR*************
    nilaiPotensioRR = analogRead(potensioRR);
    RR = map(nilaiPotensioRR, 0, 1023, 10, 40);
    //****************************

    //*************IE*************
    nilaiPotensioIE = analogRead(potensioIE);
    IE = map(nilaiPotensioIE, 0, 1023, 1, 4);
    //****************************

    //*************PEEP***********
    nilaiPotensioPEEP = analogRead(potensioPEEP);
    AC = map(nilaiPotensioPEEP, 0, 1023, 2, 7);
    //****************************

    //*************AC*************
    nilaiPotensioFIO2 = analogRead(potensioFIO2);
    AC = map(nilaiPotensioPEEP, 0, 1023, 2, 7);
    //****************************
  }
  //=============================================

  //============Alarm==============
  int snz_state = digitalRead(SNOOZE_PIN);  //Inisialisasi pin Snooze

  if (snz_state == LOW) {
    startAlarm = 0;
    digitalWrite(ALARM_PIN, LOW);
    tp_1 = tp;
  }
  set_alarm(startAlarm);
  //===============================

  Serial.println();
}

void hitung() {
  tPeriod = 60.0 / RR;  //Menghitung perioda  //detik/nafas
  tPeriod = tPeriod - tHold;  //Menghitung perioda dikurangi waktu tahan nafas
//  Serial.print("tPeriod "); Serial.print(tPeriod);
  //  Serial.print("");
  tIn = tPeriod / (1 + IE); //Menghitung waktu tarik nafas  //detik
  tEx = tPeriod - tIn;  //Menghitung buang nafas  //detik
}

void run() {
  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = 0; //reset limit
    //    Serial.println(i);
    //    i = 0;

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
  read_pressure();
  //  lcd_disp(Q1, pressure);

  tick = millis();

  tBef = 0;
  volume = 0;

  Serial.println();
  Serial.print("  In Time   "); Serial.println(duration[0]);  //Waktu Inhale
  Serial.print("  Ho Time   "); Serial.println(duration[1]);  //Waktu Inhale
  Serial.print("  Ex Time   "); Serial.println(duration[2]);  //Waktu Inhale

  posisi = (3.96 * TV) + 558;
  pulsePerSecond = posisi / (durasi / 1000.0);

  pwm = (0.0581 * pulsePerSecond) + 27.065;
  pwm = constrain(pwm, 0, 255);

  switch (cycle) {  //Jalankan perintah sesuai siklus
    case 0:
      Serial.println();
//      Serial.print("  INHALE  ");
//      Serial.print("  pwm   "); Serial.println(pwm);  //Waktu Inhale

      while (count <= posisi && inOk) { //Jalankan perintah sesuai durasi
        
        check_CurrentAlarm();

        inState = true;

//        Serial.print("  Inhale OK:  "); Serial.print(inOk);    //Waktu berjalan inhale

//        Serial.print("  PosT:  "); Serial.print(posisi);    //Waktu berjalan inhale
//        Serial.print("  Pos:  "); Serial.print(count);    //Waktu berjalan inhale
//        Serial.print("  PosBef:  "); Serial.print(countBef);    //Waktu berjalan inhale

        read_pressure();

        lcd_disp(plateau, pressure1, peep);

        rise = true;

        sampling = millis() - tick;

        Serial.print("  Sampling  "); Serial.print(sampling - tBef);  //Waktu berjalan inhale

        volume += (Q1 * ((sampling - tBef) / 60000.0)) * 1000;

        tBef = sampling;

//        Serial.print("  Inhale  "); Serial.print(tIn * 1000);  //Waktu berjalan inhale
//        Serial.print("  tIn   "); Serial.print(sampling);    //Waktu Inhale
        Serial.print("  Volume   "); Serial.println(volume);    //Waktu Inhale
        analogWrite(Min2, pwm);    //Jalankan motor
        digitalWrite(Min1, LOW);    //Jalankan motor

//        delay(10);

        check_CurrentAlarm();

        if ( abs(count - countBef) <= 15) {
          inOk = false;

          Serial.println (" MOTOR STUCK!!");

          digitalWrite(Min2, LOW);    //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
          homing();
        }
        countBef = count;
      }
      

      inState = false;

      pressure1 = pressure;
      lcd_disp(plateau, pressure1, peep);

      indikator[1] = 1; //Indikator apabila inhale selesai

      break;

    case 1:
      Serial.println();
//      Serial.print("  HOLD  ");
      while (millis() - tick  <= tHold * 1000) { //Jalankan perintah sesuai durasi
//        Serial.print("  Pos:  "); Serial.print(count);    //Waktu berjalan inhale
        read_pressure();

//        Serial.print("  Hold  "); Serial.print(tHold * 1000);  //Waktu berjalan inhale
//        Serial.print("  tHold  "); Serial.println(millis() - tick);    //Waktu berjalan hold

        digitalWrite(Min2, LOW);    //Motor berhenti
        digitalWrite(Min1, LOW);    //Motor berhenti
      }

      plateau = pressure;
      lcd_disp(plateau, pressure1, peep);
      indikator[2] = 1; //Indikator apabila hold selesai

      break;

    case 2:   //Apabila timer exhale sudah fix, while bisa dihilangkan

      rise = false;

//      Serial.println();
//      Serial.println("  EXHALE  ");
//      Serial.print("  pwm   "); Serial.println(pwm);  //Waktu Inhale

      //========Limit Switch========

      if (inOk) {
        while (millis() - tick <= tEx * 1000) { //Kondisi motor akan bergerak hanya jika limitSwitch tidak ditekan
          //      while ((limState == 0 || count >= 200)) { //Kondisi motor akan bergerak hanya jika limitSwitch tidak ditekan
          while (count >= 558 && limState == 0) {
//            Serial.print("  Pos:  "); Serial.print(count);    //Waktu berjalan inhale
//            Serial.print("  Kondisi:  "); Serial.print(millis() - tick <= tEx * 1000 || count >= 200 || limState == 1);  //Waktu berjalan inhale
            read_pressure();
//            Serial.print("  Exhale  "); Serial.print(tEx * 1000);  //Waktu berjalan inhale
            digitalWrite(Min2, LOW);    //Jalankan motor
            analogWrite(Min1, constrain(pwm * 2, 0, 255)); //Jalankan motor
//            Serial.print("  tEx   "); Serial.println(millis() - tick);    //Waktu Inhale
          }

          lcd_disp(plateau, pressure1, peep);
          digitalWrite(Min2, LOW);    //Jalankan motor
          digitalWrite(Min1, LOW); //Jalankan motor
//          Serial.print("  Exhale  "); Serial.print(tEx * 1000);  //Waktu berjalan inhale
//          Serial.print("  tEx   "); Serial.println(millis() - tick);    //Waktu Inhale
        }
      }

      inOk = true;

      peep = pressure;
      lcd_disp(plateau, pressure1, peep);

      digitalWrite(Min2, LOW);    //Jalankan motor
      digitalWrite(Min1, LOW); //Jalankan motor

      indikator[3] = 1; //Indikator apabila exhale selesai
      //============================
      break;
  }

  digitalWrite(Min2, LOW);    //Motor berhenti
  digitalWrite(Min1, LOW);    //Motor berhenti

  Serial.print("  CYCLE END!!");
  Serial.println();
}
