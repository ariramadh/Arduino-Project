#include <Wire.h> //Memanggil library OneWire
#define limSwitch 3
#define encoder 2

int limState = 0, lastLimState;
float Q, Qprev, vol, pressure, tp;
float RC, fc, a;
int AVf, AVf_1;
String data1, data2;
char data;

String dataIn, dataArray[10], cmd, cmd2;
bool dataComplete = false, parsing = false;

bool oneCycle;
//variabel untuk flag
bool start;
int alarm;

int TV, RR, TV1, RR1, set, AC, tv;
float IE, IE1, desIE, Vin;
int zPressure[2];

float plateau, peep;

unsigned long time, tprev;
float Ts;

bool rise;
int count;

//#define encoder 2


void setup() {
  zPressure[0] = analogRead(A4) - 465;  //Mengkalibrasi ADC tekanan
  zPressure[1] = analogRead(A5) - 559 ;  //Mengkalibrasi ADC tekanan
  // put your setup code here, to run once:
  Serial.begin(115200);
  fc = 15.167;
  RC=1.0/(2*3.14159*fc);
  a=RC/0.02;
  pinMode(encoder, INPUT_PULLUP);
  pinMode(limSwitch, INPUT_PULLUP);
  //========Memasang Interupt======
  attachInterrupt(digitalPinToInterrupt(limSwitch), limSwich_Procedure, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder), motorEncoder, FALLING);
  //===============================

}

void loop() {
  t=millis();
  baca_potensio();  
  Ts = (t-tprev)/1000.0;
  read_pressure(); 
//  check_alarm();
//  setAlarm(alarm);
  vol += (Q * Ts/60.0)*1000.0;
  
  if (Serial.available() > 0) {
//    delay(10);
    char inChar = (char)Serial.read();
//    Serial.print(inChar);
    dataIn += inChar;
//      Serial.print(dataIn);
    //mulai parsing jika pada data terdapat tanda "/" dan "#" 
    if (dataIn.indexOf("/") >= 0 && inChar == '#') {
      parsing = true;
    }

    if (parsing) {
      parsingData();
      parsing = false;
      dataIn = "";
    }
  }
  tprev = t;
}

void parsingData() {
  Serial.print(" Data masuk: "); Serial.println(dataIn);
  int j = 0;
  dataArray[j]="";
  dataIn = dataIn.substring(dataIn.indexOf("/"));
//  Serial.print(" Data parsing: "); Serial.println(dataIn);
  for (int i=1; i<dataIn.length(); i++){
//    Serial.print("data parsing: ");Serial.println(dataIn[i]);
    if (dataIn[i] =='/')
    {
      j++;
      dataArray[j]="";
    }
    else{
      dataArray[j] = dataArray[j]+dataIn[i];
    }
  }
  
//  cmd = dataArray[0];
  cmd = dataArray[0];
//  Serial.print(" Perintah \t: ");Serial.println(cmd);
//  Serial.println("");

  if (cmd == "P"){
    Serial.print("/");Serial.print("p"); Serial.print(pressure);Serial.print("#");
  }else if (cmd == "F") {
    Serial.print("/");Serial.print("f"); Serial.print(Q);Serial.print("#");
  }else if (cmd == "E"){
    Serial.print("/");Serial.print("e"); Serial.print(count);Serial.print("#");
  }else if (cmd == "K"){
//    Serial.print("/");Serial.print("k"); Serial.print(TV);
//    Serial.print("*");Serial.print(RR);  Serial.print("*");Serial.print(IE);
//    Serial.print("*");Serial.print(AC);  Serial.print("#");
  }
}
