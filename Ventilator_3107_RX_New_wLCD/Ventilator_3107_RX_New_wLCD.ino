#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C

//==========Timer================
unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas
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
const String indi[] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "}; //String indikator
int indikator[4]; //Status indikator
volatile int count;
int countBef; //Menghitung pulsa

String teksAlarm[] = {"          ", "HI PRESS", " LO PRESS", "HI RESSIST", "NO TIDAL V", "LOW TIDAL P", "HI CURRENT"}; //Menampilkan teks alarm
int alarm;  //Mengkategorikan alarm
float pressure, pip, pipm, plateau, peep;
float pressureb, pipb, plateaub, peepb;
int Q1, tIn, tEx, volume, vol, TV, RR, IE, AC, PR;
int Q1b, tInb, tExb, volumeb, TVb, RRb, IEb, ACb, PRb;
int setButton;
int duration[3];  //Variabel untuk menyimpan nilai waktu setiap siklus
String potensio, potensiob;
//===============================

//==============Flag=============
bool start; //Flag start
volatile bool limState = 0; //Fungsi limit
bool rise;  //Menentukan apakah encoder count up atau count down
bool inOk = true, inState; //Menyimpan apakah Inhale berhasil
bool oneCycle;  //Menyimpan apakah program sudah dijalankan sebanyak 1 kali
bool ACMode;  //Kondisi pemilihan mode ventilator
bool toStart; //Ventilator akan berjalan
bool exState;  //saat kondisi exhale
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

  pinMode(rstSensor, OUTPUT);
  digitalWrite(rstSensor, HIGH);  //reset Arduino Sensor

  //Inisialisasi komunikasi serial

  Serial.begin(38400);

  pinMode(limSwitch, INPUT_PULLUP);
  pinMode(encoder, INPUT_PULLUP);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("SJT Ventilator"); //Tampilan teks awal

  Serial.print("."); //Tampilan teks awal


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

  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, FALLING);
}

void loop() {
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

      Serial.print("\t TV: ");  Serial.print(TV);
      Serial.print("\t RR: ");  Serial.print(RR);
      Serial.print("\t IE: ");  Serial.print(IE);
      Serial.print("\t PR: ");  Serial.print(PR);
      Serial.print("\t AC: ");  Serial.print(IE);
      Serial.println("");
      //      TV = 400, RR = 10, IE = 1, AC = 1, PR = 20;

      hitung();
      homing();

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
      lcdTeks();
    }

    Serial.println (limState);
    delay(100);
    //    Serial.print("STOP ");
    //    if (lcdPrint.isReady()) {
    toStart = false;
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
  zeroingDisplay();
  //  Serial.println("RUNNING");  //Tampilkan indikator program berjalan

  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = 0; //reset limit

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
        }

        pip = pressure;
        if (pip > pipm) {
          pipm = pip;
        }


        Serial.print(" pip "); Serial.println(pip);

        //        Serial.print("\t Pressure:"); Serial.print(pressure);
        //        Serial.print("\t Flow:"); Serial.println(Q1);

        lcdDisp();
        //        Serial.println("INHALE SELESAI");

        readSerial(); //==================Panggil fungsi serial==================

        Serial.print(" vol "); Serial.println(vol);

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

        Serial.println("/L#"); //==================Mengirim nilai Plateau==================

        plateau = pressure;
        lcdDisp();

        indikator[2] = 1; //Indikator apabila hold selesai
        break;

      case 2:

        Serial.println("EXHALE MULAI");

        rise = false;

        while (millis() - tick  <= duration[2] && inOk) { //Jalankan perintah sesuai durasi

          //          Serial.print("EXHALE ");
          while (count >= 585 && limState == 0) {
            //  Serial.print("\t Posisi:"); Serial.println(count);
            digitalWrite(Min2, LOW);
            analogWrite(Min1, pwm); //Jalankan motor
            readSerial();//==================Panggil fungsi serial==================
          }

          digitalWrite(Min2, LOW);   //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
        }

        peep = pressure;
        lcdDisp();

        //        Serial.println("EXHALE SELESAI");
        indikator[3] = 1; //Indikator apabila hold selesai

        break;
    }

    oneCycle = true;
  }

  pipm = 0;
}

void hitung() {
  float tHold = 0.1, tPeriod;

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
