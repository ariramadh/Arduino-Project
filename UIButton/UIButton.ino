#include "OneButton.h"  //Download Library OneButton by Matthias Herthel

//Inisialisasi Button di pin Analog
OneButton btnP(A0, false);
OneButton btnN(A1, false);
OneButton btnSS(A2, false);

//Variabel nilai suhu batas atas dan bawah
float valUp = 40;
float valDown = 36;

//Selektor setting suhu batas atas atau bawah
bool UpDown = false;

//Kondisi Running
bool running = false;

void setup() {
  Serial.begin(9600);
  
  btnP.attachClick(clickP);
  btnP.attachDuringLongPress(longPressP);

  btnN.attachClick(clickN);
  btnN.attachDuringLongPress(longPressN);

  btnSS.attachClick(clickSS);
  btnSS.attachLongPressStart(longPressSS);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  btnP.tick();
  btnN.tick();
  btnSS.tick();

  //Kondisi supaya nilai valDown tidak melebih valUp
  if(valDown > valUp){
    valDown = valUp;
  }

  //Kondisi supaya valDown tidak kurang dari 30
  if(valDown < 30){
    valDown = 30;
  }

  //Kondisi supaya valUp tidak lebih dari 50
  if(valUp > 50){
    valUp = 50;
  }
  
  if(running){ //Jika kondisi running true, maka jalankan program yang ada di dalam void run()
    run();  
  }else{
    //Disini isi dengan kondisi awal sebelum dijalankan
    Serial.print(valDown);
    Serial.print("\t");
    Serial.println(valUp);
  }
}

void run(){
  //Disini isi saat pemanas berjalan
  Serial.println("Running");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

void clickP(){
  if(UpDown){
    valDown += 0.1;
  }else{
    valUp += 0.1;    
  }
}

void clickN(){
  if(UpDown){
    valDown -= 0.1;
  }else{
    valUp -= 0.1;    
  }
}

void longPressP(){
  if(UpDown){
    valDown += 0.1;
  }else{
    valUp += 0.1;    
  }
}

void longPressN(){
  if(UpDown){
    valDown -= 0.1;
  }else{
    valUp -= 0.1;    
  }
}

void clickSS(){
  UpDown = !UpDown; //Button ke 3, jika ditekan berfungsi sebagai selektor
}

void longPressSS(){
  running = !running; //Button ke 3, jika ditekan selama 1 detik berfungsi sebagai start stop
}
