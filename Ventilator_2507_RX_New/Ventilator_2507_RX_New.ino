#include <Wire.h> //Memanggil library OneWire
#include <LiquidCrystal_I2C.h>  //Memanggil library dengan I2C

//==========Timer================
unsigned long tick; //Variabel untuk menghitung durasi pada setiap siklus nafas
//===============================

//==========Pin==================
#define Min1 11
#define Min2 10
#define Men 12
#define Mslp 6

#define limSwitch 2
#define encoder 3
//===============================

//============Variabel===========
const String indi[4] = {"Homing: ", "  Inhale:  ", "  Hold:  ", "  Exhale:  "}; //String indikator
int indikator[4]; //Status indikator
int count, countBef; //Menghitung pulsa

String teksAlarm; //Menampilkan teks alarm
int alarm;  //Mengkategorikan alarm
float pressure, pip, plateau, peep;
float pressureb, pipb, plateaub, peepb;
int Q1, tIn, tEx, volume, TV, RR, IE, AC, PR;
int Q1b, tInb, tExb, volumeb, TVb, RRb, IEb, ACb, PRb;
int setButton;
int duration[3];  //Variabel untuk menyimpan nilai waktu setiap siklus
String potensio;
//===============================

//==============Flag=============
bool start; //Flag start
bool limState = 0; //Fungsi limit
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
  // put your setup code here, to run once:
  Serial.begin(57600); //Inisialisasi komunikasi serial

  pinMode(encoder, INPUT_PULLUP);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Men, OUTPUT);
  pinMode(Mslp, OUTPUT);

  digitalWrite(Men, HIGH);  //Motor enable
  digitalWrite(Mslp, HIGH); //
  digitalWrite(Min2, LOW);  //Motor Input2
  digitalWrite(Min1, LOW);  //Motor Input1

  Serial.println("SJT Ventilator"); //Tampilan teks awal

  //========Inisialisasi LCD=======
  lcd.backlight();  //Inisialisasi backlight
  lcd.init();       //Inisialisasi LCD
  lcd.clear();      //Bersihkan LCD
  lcdTeks();        //Tampilkan LCD
  //===============================

  start = false;  //Flag menyatakan program tidak akan berjalan bila belum ditekan Set

  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, RISING);
}

void loop() {
  if (setButton == 1) {
    ACMode = !ACMode;
  } else if (setButton == 2) {
    start != start;
  }

  //==============Program Utama==================
  if (start)  { //Jika kondisi start
    hitung();

    if (!toStart) {
      homing();
      lcdTeks();
      toStart = true;
    }
    inOk = true;
    run();
    oneCycle = true; //Mengubah nilai apabila sudah dilakukan 1 putaran
  } else {
    lcdTeks();
  }
}

void run() {
  int posisi, pulsePerSecond, pwm;  //Menghitung PWM

  Serial.println("RUNNING");  //Tampilkan indikator program berjalan

  for (int i = 0; i <= 2; i++) { //Iterasi siklus motor
    limState = 0; //reset limit

    tick = millis();  //Masukan nilai awal untuk setiap siklus pada millis
    int tBef;   //Mereset time before
    int volume = 0; //Mereset volume

    posisi = (3.5976 * TV) + 1761;  //Kalkulasi untuk menghitung posisi
    pulsePerSecond = posisi / (duration[i] / 1000.0);  //Kalkulasi untuk menghitung pulsa per detik

    pwm = (0.0581 * pulsePerSecond) + 27.065;     //Kalkulasi untuk menghitung nilai PWM
    pwm = constrain(pwm, 0, 255); //Membatasi output PWM dari 0 hingga 255

    switch (i) {  //Jalankan perintah sesuai siklus
      case 0:
        inOk = true;      //      Serial.println();

        Serial.println("INHALE MULAI");
        Serial.println("/i#");

        rise = true;

        while (count <=  posisi && inOk) { //Jalankan perintah sesuai durasi
          analogWrite(Min2, pwm);    //Jalankan motor
          digitalWrite(Min1, LOW);    //Jalankan motor//        Serial.print("\tAlarm\t");  Serial.print(alarm);
          readSerial();
        }

        pip = pressure;

        Serial.println("INHALE SELESAI");
        Serial.println("/I#");

        indikator[1] = 1; //Indikator apabila hold selesai
        break;

      case 1:

        Serial.println("HOLD");
        while (millis() - tick  <= duration[1] * 1000 && inOk) { //Jalankan perintah sesuai durasi
          digitalWrite(Min2, LOW);    //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
          readSerial();
        }

        plateau = pressure;

        indikator[2] = 1; //Indikator apabila hold selesai
        break;

      case 2:

        Serial.println("EXHALE MULAI");

        rise = false;

        while (millis() - tick  <= duration[2] && inOk) { //Jalankan perintah sesuai durasi

          while (count >= 585 && limState == 0) {
            digitalWrite(Min2, LOW);
            analogWrite(Min1, pwm); //Jalankan motor
            readSerial();
          }

          digitalWrite(Min2, LOW);   //Motor berhenti
          digitalWrite(Min1, LOW);    //Motor berhenti
        }

        peep = pressure;

        Serial.println("EXHALE SELESAI");
        indikator[3] = 1; //Indikator apabila hold selesai

        break;
    }

    oneCycle = true;
  }
}

void hitung() {
  float tIn, tHold, tEx;

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

  readSerial();

  //  Serial.print("\t potensio: ");  Serial.println(potensio);

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
  Serial.print("\t Nilai TV: ");  Serial.print(TV);
  Serial.print("\t Nilai RR: ");  Serial.print(RR);
  Serial.print("\t Nilai IE: ");  Serial.print(IE);
  Serial.print("\t Nilai AC: ");  Serial.print(AC);
}
