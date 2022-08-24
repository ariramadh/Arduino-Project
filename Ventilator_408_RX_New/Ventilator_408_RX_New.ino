#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C

//==========Timer================
unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas
int tPeriod;
//===============================

//==========Pin==================
#define Min1 10
#define Min2 11
#define Men 9
#define Mslp 8

#define limSwitch 2
#define encoder 3

#define rstSensor 5
//===============================

//============Variabel===========
const String indi[4] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "}; //String indikator
int indikator[4]; //Status indikator
volatile int count;
int countBef; //Menghitung pulsa

String teksAlarm[] = {"RUNNING    ", "HI PRESS", " LO PRESS", "HI RESSIST", "NO TIDAL V", "LOW TIDAL P", "HI CURRENT"}; //Menampilkan teks alarm
int alarm, alarmb;  //Mengkategorikan alarm
float pressure, pip, pipm, plateau, peep;
float pressureb, pipb, plateaub, peepb;
int Q1, tIn, tEx, volume, vol, TV, RR, IE, AC, PR;
int Q1b, tInb, tExb, volumeb, TVb, RRb, IEb, ACb, PRb;
int setButton;
int duration[3];  //Variabel untuk menyimpan nilai waktu setiap siklus
String potensio, potensiob;
int comp;
//===============================

//==============Flag=============
bool start, startb; //Flag start
volatile bool limState = 0; //Fungsi limit
bool rise;  //Menentukan apakah encoder count up atau count down
bool inOk = true, inState; //Menyimpan apakah Inhale berhasil
bool oneCycle;  //Menyimpan apakah program sudah dijalankan sebanyak 1 kali
bool ACMode, ACModeb;  //Kondisi pemilihan mode ventilator
bool toStart; //Ventilator akan berjalan
bool exState;  //saat kondisi exhale
bool resetSens;
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

  Serial.begin(38400);
  Serial.print("SJT Ventilator"); //Tampilan teks awal

  //Inisialisasi komunikasi serial

  Serial.print("."); //Tampilan teks awal

  //  digitalWrite(rstSensor, LOW);

  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(rstSensor, OUTPUT);

  Serial.print("."); //Tampilan teks awal

  digitalWrite(Men, HIGH);  //Motor enable
  digitalWrite(Mslp, HIGH); //
  digitalWrite(Min2, LOW);  //Motor Input2
  digitalWrite(Min1, LOW);  //Motor Input1

  Serial.print("."); //Tampilan teks awal
  //===============================

  start = false;  //Flag menyatakan program tidak akan berjalan bila belum ditekan Set
  Serial.print("."); //Tampilan teks awal

  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, FALLING);
}

void loop() {

  if (!resetSens) {
    digitalWrite(rstSensor, LOW);
    delay(1000);
    resetSens = true;
  }
  digitalWrite(rstSensor, HIGH);

  //  Serial.print("."); //Tampilan teks awal
  get_potensio(); //==================Baca nilai Potensio==================
  //  Serial.print("."); //Tampilan teks awal
  //==============Program Utama==================

  digitalWrite(LED_BUILTIN, start);

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
      //      Serial.println("");
      //      TV = 400, RR = 10, IE = 1, AC = 1, PR = 20;

      hitung();
      homing();

      lcd.clear();
      lcdTeks();
      toStart = true;
    }
    Serial.print("START ");
    inOk = true;
    limState = 0;
    run();
    oneCycle = true; //Mengubah nilai apabila sudah dilakukan 1 putaran
  } else {
    if (toStart) {
      lcd.clear();
      lcdTeks();
      zeroingDisplay();
      resetSens = false;
    }
    toStart = false;
    //    Serial.print("STOP ");
    //        if (lcdPrint.isReady()) {

    if (ACMode != ACModeb || start != startb) {
      lcd.clear();
      lcdTeks();
      zeroingDisplay();

      Serial.print("ACModeb ");
      Serial.println(ACModeb);

      ACModeb = ACMode;
      startb = start;
    }

    lcdDisp();
    //    }

    //    Serial.begin(57600);
    //    Serial.println("...WAITING...");

    //        Serial.print("."); //Tampilan teks awal

    //    Serial.println();
  }
}

void run() {
  int posisi, pulsePerSecond, pwm;  //Menghitung PWM
  zeroDisplay();
  //  Serial.println("RUNNING");  //Tampilkan indikator program berjalan

  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = 0; //reset limit

    tick = millis();  //Masukan nilai awal untuk setiap siklus pada millis
    int tBef;   //Mereset time before
    int volume = 0; //Mereset volume

    if (TV == 300) {
      posisi = 2761;
    } else if (TV == 350) {
      posisi = 2878;
    } else if (TV == 400) {
      posisi = 3002;
    } else if (TV == 450) {
      posisi = 3227;
    } else if (TV == 500) {
      posisi = 3498;
    } else if (TV == 550) {
      posisi = 3568;
    } else if (TV == 600) {
      posisi = 3949;
    }

    //    posisi = (3.921 * TV) + 1406.3;  //Kalkulasi untuk menghitung posisi
    pulsePerSecond = posisi / (duration[i] / 1000.0);  //Kalkulasi untuk menghitung pulsa per detik
    //    pwm = (0.044 * pulsePerSecond) + 33.707;     //Kalkulasi untuk menghitung nilai PWM
    pwm = (0.0607 * pulsePerSecond) + 24.108;     //Kalkulasi untuk menghitung nilai PWM
    pwm = constrain(pwm, 0, 255); //Membatasi output PWM dari 0 hingga 255
    comp = (-0.2284 * duration[i]) + 678.05;

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

        while (millis() - tick  <= duration[0] && count < posisi - comp && inOk) { //Jalankan perintah sesuai durasi
          analogWrite(Min2, pwm);    //Jalankan motor
          digitalWrite(Min1, LOW);    //Jalankan motor//        Serial.print("\tAlarm\t");  Serial.print(alarm);
          //          Serial.print("INHALE "); Serial.print("\t Posisi:"); Serial.println(count);

          //          if (pressure != pressureb) {
          //            Serial.print("Pressure:"); Serial.print(pressure);
          //            pressureb = pressure;
          //          }
          //
          //          if (Q1 != Q1b) {
          //            Serial.print("Flow:"); Serial.print(Q1);
          //            Q1b = Q1;
          //          }
          readSerial(); //==================Panggil fungsi serial==================

          Serial.print("Posisi : ");
          Serial.print(count);
          Serial.print(" Kompensasi : ");
          Serial.print(comp);
          Serial.print(" Target : ");
          Serial.print(posisi);
          Serial.print(" Tar-Komp : ");
          Serial.println(posisi - comp);
          //          delay(10);

          //          if (abs(count - countBef) <= 3) {
          //            inOk = false;
          //
          //            Serial.println ("MOTOR STUCK/ENCODER ERROR!!\t");
          //
          //            digitalWrite(Min2, LOW);    //Motor berhenti
          //            digitalWrite(Min1, LOW);    //Motor berhenti
          //            countBef = 0;
          //            homing();
          //            start = false;
          //          }

          countBef = count;
        }

        //        zeroingDisplay();

        pip = pressure;

        //        Serial.print("\t Flow:"); Serial.println(Q1);

        lcdDisp();
        //        Serial.println("INHALE SELESAI");

        readSerial(); //==================Panggil fungsi serial==================

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
        Serial.print(" ");
        Serial.print(count);

        Serial.println("/L#"); //==================Mengirim nilai Plateau==================

        plateau = pressure;
        lcdDisp();

        indikator[2] = 1; //Indikator apabila hold selesai
        break;

      case 2:

        Serial.println("EXHALE MULAI");

        rise = false;

        if (!ACMode) {
          while (millis() - tick  <= duration[2] && inOk) { //Jalankan perintah sesuai durasi
            //          Serial.print("EXHALE ");
            while (count + comp  >= 0 && limState == 0) {
              //  Serial.print("\t Posisi:"); Serial.println(count);
              digitalWrite(Min2, LOW);
              analogWrite(Min1, constrain(pwm * 2, 0, 200)); //Jalankan motor
            }

            digitalWrite(Min2, LOW);   //Motor berhenti
            digitalWrite(Min1, LOW);    //Motor berhenti

            readSerial();//==================Pangg5 fungsi serial==================
            peep = pressure;

            lcdDisp();
          }
        } else {
          while (millis() - tick  <= tPeriod && pressure < peep - AC && inOk) { //Jalankan perintah sesuai durasi
            while (count + comp >= 0 && limState == 0) {
              //  Serial.print("\t Posisi:"); Serial.println(count);
              digitalWrite(Min2, LOW);
              analogWrite(Min1, constrain(pwm * 2, 0, 200)); //Jalankan motor
            }

            digitalWrite(Min2, LOW);   //Motor berhenti
            digitalWrite(Min1, LOW);    //Motor berhenti


            readSerial();//==================Pangg5 fungsi serial==================
            peep = pressure;

            lcdDisp();
            //            Serial.print("\t Pressure:"); Serial.println(pressure);


            //            Serial.print("Pressure Check ");
            //            Serial.print(pressure);
            //            Serial.print("<");
            //            Serial.println(peep - AC);
            //

          }
        }

        //        Serial.println("EXHALE SELESAI");
        indikator[3] = 1; //Indikator apabila hold selesai

        break;
    }

    oneCycle = true;
  }
}

void hitung() {
  float tHold = 100;

  tPeriod = 60000.0 / RR;  //Menghitung perioda  //milidetik/nafas
  tPeriod = tPeriod - tHold;  //Menghitung perioda dikurangi waktu tahan nafas
  //  Serial.print("tPeriod "); Serial.print(tPeriod);
  //  Serial.print("");
  tIn = tPeriod / (1 + IE); //Menghitung waktu tarik nafas  //milidetik
  tEx = tPeriod - tIn;  //Menghitung buang nafas  //milidetik

  duration[0] = tIn; //Atur durasi tarik nafas milidetik
  duration[1] = tHold; //Atur durasi tahan nafas milidetik
  duration[2] = tEx;//Atur durasi buangnafas milidetik
}

void get_potensio() {
  int k = 0;
  String daPot[4];

  for (int i = 0; i < 4; i++) {
    daPot[i] = "";
  }

  readSerial(); //==================Panggil fungsi serial==================

  //  Serial.print("\t potensio: ");  Serial.println(potensio);

  for (int i = 0; i <= potensio.length(); i++) {
    //    Serial.print("\t Test: ");  Serial.println(potensio.length());
    //    Serial.print("\t Test: ");  Serial.println(potensio[i]);
    //    Serial.print("\t i: ");  Serial.println(i);
    if (potensio[i] == '*') {
      k++;
    } else {
      daPot[k] += potensio[i];
      //            Serial.print("\t Test: ");  Serial.print(k);
      //      Serial.print("Test: "); Serial.print(k); Serial.print(" "); Serial.println(daPot[k]);
    }
  }

  TV = daPot[0].toInt();
  RR = daPot[1].toInt();
  IE = daPot[2].toInt();

  if (ACMode) {
    AC = daPot[3].toInt();
    AC = AC - (2 * AC);
  } else {
    PR = daPot[3].toInt();
  }

  //  if (potensiob != potensio) {
  //    Serial.print("\t Nilai TV: ");  Serial.print(TV);
  //    Serial.print("\t Nilai RR: ");  Serial.print(RR);
  //    Serial.print("\t Nilai IE: ");  Serial.print(IE);
  //    Serial.print("\t Nilai AC: ");  Serial.print(AC);
  //    Serial.println();
  //    potensiob = potensio;
  //  }
}
