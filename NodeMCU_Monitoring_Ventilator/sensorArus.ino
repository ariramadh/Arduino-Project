int normalState;
float mAmpere, maxCurrentState;
bool alarm;

float readCurrent() {
  analogValue = analogRead(A0);
  mVolt = 3150 * (analogValue / 1024.0); //mV
  normalState = 2448;                      //mV  nilai apabila ampere 0

  mAmpere = (normalState - mVolt) / 0.185;  //mA ieu nu 5amps
//  mAmpere = (normalState - mVolt) / 0.100;  //mA ieu nu 20amps
//  mAmpere = (normalState - mVolt) / 0.066;  //mA ieu nu 30amps

  //  if (readSuhu() >= 30.00) {  //Sementaun pake ini dulu, sensor arusnya belum di kalibrasi
  if (mAmpere >= 2000) {
    overCurrent = true;
  } else if (!tombol) {
    overCurrent = false;
  }

  return constrain(mAmpere,0,5000);
}
