#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C
#include <SimpleTimer.h>

//==========Timer================
unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas
SimpleTimer Hom(3000);
SimpleTimer lcdPrint(100);
//===============================

//==========Pin==================
#define led 13
#define Min1 11
#define Min2 10
#define Men 9
#define Mslp 8

#define limSwitch 2
#define encoder 3
//===============================

//============Variabel===========
const String indi[4] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "}; //String indikator
int indikator[4]; //Status indikator
volatile int count;
int countBef; //Menghitung pulsa

String teksAlarm; //Menampilkan teks alarm
int alarm;  //Mengkategorikan alarm
float pressure, pip, plateau, peep;
float pressureb, pipb, plateaub, peepb;
int Q1, tIn, tEx, volume, TV, RR, IE, AC, PR;
int Q1b, tInb, tExb, volumeb, TVb, RRb, IEb, ACb, PRb;
int setButton;
int duration[3];  //Variabel untuk menyimpan nilai waktu setiap siklus
String potensio, potensiob;
//===============================

//==============Flag=============
bool start = false; //Flag start
volatile bool limState = false; //Fungsi limit
bool rise = false;  //Menentukan apakah encoder count up atau count down
bool inOk = true, inState = false; //Menyimpan apakah Inhale berhasil
bool oneCycle = false;  //Menyimpan apakah program sudah dijalankan sebanyak 1 kali
bool ACMode = false;  //Kondisi pemilihan mode ventilator
bool toStart = false; //Ventilator akan berjalan
bool exState = false;  //saat kondisi exhale
//===============================


//==============LCD==============
LiquidCrystal_I2C lcd(0x27, 20, 4); //Inisialisasi LCD
//===============================

void setup() {
  lcd.backlight();  //Inisialisasi backlight
  lcd.init();       //Inisialisasi LCD
  welcomeTeks();
  delay(2000);
  lcd.clear();
  lcdTeks();        //Tampilkan LCD

  Serial.begin(57600);
  Serial.print("SJT Ventilator"); //Tampilan teks awal

  //Inisialisasi komunikasi serial

  Serial.print("."); //Tampilan teks awal

  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.print("."); //Tampilan teks awal

  digitalWrite(Men, HIGH);  //Motor enable
  digitalWrite(Mslp, HIGH); //
  digitalWrite(Min2, LOW);  //Motor Input2
  digitalWrite(Min1, LOW);  //Motor Input1

  //  Serial.print("."); //Tampilan teks awal

  //  Serial.print("."); //Tampilan teks awal

  Serial.print("."); //Tampilan teks awal
  //  Serial.print("."); //Tampilan teks awal
  //===============================

  start = false;  //Flag menyatakan program tidak akan berjalan bila belum ditekan Set
  Serial.print("."); //Tampilan teks awal

  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
}

void loop() {
  //  Serial.print("."); //Tampilan teks awal
  get_potensio(); //==================Baca nilai Potensio==================
  //  Serial.print("."); //Tampilan teks awal
  //==============Program Utama==================

  digitalWrite(led, start);

  if (start)  { //Jika kondisi start
    if (!toStart) {
      //      Serial.print(TV);
      //      Serial.print("/");
      //      Serial.print(RR);
      //      Serial.print("/");
      //      Serial.print(IE);
      //      Serial.print("/");
      //      Serial.print(AC);
      //      Serial.print("/");
      //      Serial.println(PR);

      //      Serial.print("\t TV: ");  Serial.print(TV);
      //      Serial.print("\t RR: ");  Serial.print(RR);
      //      Serial.print("\t IE: ");  Serial.print(IE);
      //      Serial.print("\t PR: ");  Serial.print(PR);
      //      Serial.print("\t AC: ");  Serial.print(IE);
      Serial.println("");
            TV = 400, RR = 10, IE = 1, AC = 1, PR = 20;

      hitung();
      homing();

      //            lcdTeks();
      toStart = true;
    }
    Serial.print("START ");
    inOk = true;
    limState = false;
    run();
    oneCycle = true; //Mengubah nilai apabila sudah dilakukan 1 putaran
  } else {
    toStart = false;
    //    Serial.print("STOP ");
    //    Serial.end();
//    if (lcdPrint.isReady()) {
      lcdDisp();
//    }

    //    Serial.begin(57600);
    //    Serial.println("...WAITING...");

//    if (potensiob != potensio) {
//      Serial.print("\t Nilai TV: ");  Serial.print(TV);
//      Serial.print("\t Nilai RR: ");  Serial.print(RR);
//      Serial.print("\t Nilai IE: ");  Serial.print(IE);
//      Serial.print("\t Nilai AC: ");  Serial.print(AC);
//      Serial.println();
//      potensiob = potensio;
//    }
    //        Serial.print("."); //Tampilan teks awal

    //    Serial.println();
  }
}

void run() {
  int posisi, pulsePerSecond, pwm;  //Menghitung PWM

  //  Serial.println("RUNNING");  //Tampilkan indikator program berjalan

  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = false; //reset limit

    tick = millis();  //Masukan nilai awal untuk setiap siklus pada millis
    int tBef;   //Mereset time before
    int volume = 0; //Mereset volume

    posisi = (3.5976 * TV) + 1761;  //Kalkulasi untuk menghitung posisi
    pulsePerSecond = posisi / (duration[i] / 1000.0);  //Kalkulasi untuk menghitung pulsa per detik

    pwm = (0.0581 * pulsePerSecond) + 27.065;     //Kalkulasi untuk menghitung nilai PWM
    pwm = constrain(pwm, 0, 255); //Membatasi output PWM dari 0 hingga 255

    for (int i = 0; i <= 2; i++) {
      Serial.print("t");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(duration[i]);
      Serial.print(" ");
    }
    Serial.println();

    switch (i) {  //Jalankan perintah sesuai siklus
      case 0:
        inOk = true;      //      Serial.println();

        Serial.print("INHALE MULAI ");
        Serial.print("/i# "); //==================Mengirim kondisi Inhale dimulai==================

        rise = true;

        while (count <=  posisi && inOk) { //Jalankan perintah sesuai durasi
          analogWrite(Min2, pwm);    //Jalankan motor
          digitalWrite(Min1, LOW);    //Jalankan motor//        Serial.print("\tAlarm\t");  Serial.print(alarm);
          //          Serial.print("INHALE "); Serial.print("\t Posisi:"); Serial.println(count);

          readSerial(); //==================Panggil fungsi serial==================

          //          if (pressure != pressureb) {
          //            Serial.print("Pressure:"); Serial.print(pressure);
          //            pressureb = pressure;
          //          }
          //
          //          if (Q1 != Q1b) {
          //            Serial.print("Flow:"); Serial.print(Q1);
          //            Q1b = Q1;
          //          }

        }

        //        Serial.print("\t Pressure:"); Serial.print(pressure);
        //        Serial.print("\t Flow:"); Serial.println(Q1);

        pip = pressure;

        //        Serial.println("INHALE SELESAI");
        Serial.println("/I#"); //==================Mengirim kondisi Inhale selesai==================

        indikator[1] = 1; //Indikator apabila hold selesai
        break;

      case 1:

        Serial.print("HOLD ");

        while (millis() - tick  <= 100 && inOk) { //Jalankan perintah sesuai durasi
          //          Serial.print(millis() - tick);
          //          Serial.print(" ");
          //          Serial.println(duration[1] * 1000);
          digitalWrite(Min2, LOW);    //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
          readSerial();//==================Panggil fungsi serial==================
        }

        plateau = pressure;

        indikator[2] = 1; //Indikator apabila hold selesai
        break;

      case 2:

        Serial.println("EXHALE MULAI");

        rise = false;

        while (millis() - tick  <= duration[2] && inOk) { //Jalankan perintah sesuai durasi

          //          Serial.print("EXHALE ");
          while (count >= 585 && !limState) {
            //  Serial.print("\t Posisi:"); Serial.println(count);
            digitalWrite(Min2, LOW);
            analogWrite(Min1, pwm); //Jalankan motor
            readSerial();//==================Panggil fungsi serial==================
          }

          digitalWrite(Min2, LOW);   //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
        }

        peep = pressure;

        //        Serial.println("EXHALE SELESAI");
        indikator[3] = 1; //Indikator apabila hold selesai

        break;
    }

    oneCycle = true;
  }
}

void hitung() {
  float tIn, tHold = 0.1, tEx, tPeriod;

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

void get_potensio() {
  int k = 0;
  String daPot[4];

  for (int i = 0; i < 4; i++) {
    daPot[i] = "";
  }

  readSerial(); //==================Panggil fungsi serial==================

  //    Serial.print("\t potensio: ");  Serial.println(potensio);

  for (int i = 0; i <= potensio.length(); i++) {
    //    Serial.print("\t Test: ");  Serial.println(potensio.length());
    //    Serial.print("\t Test: ");  Serial.println(potensio[i]);
    //    Serial.print("\t i: ");  Serial.println(i);
    if (potensio[i] == '*') {
      k++;
    } else {
      daPot[k] += potensio[i];
      //      Serial.print("\t Test: ");  Serial.print(k);
      //            Serial.print("\t Test: ");  Serial.println(daPot[k]);
    }
  }

  TV = daPot[0].toInt();
  RR = daPot[1].toInt();
  IE = daPot[2].toInt();
  if (ACMode) {
    AC = daPot[3].toInt();
  } else {
    PR = daPot[3].toInt();
  }
}
