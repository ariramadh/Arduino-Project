float EWMA(float val, int iteration, float alpha) {

  //Average adalah hasil, val adalah nilai yang diproses, lastOutput adalah nilai t-1
  float average = alpha * val + (1 - alpha) * lastOutput[iteration]; //Persamaan EWMA dalam baris program
  
  lastOutput[iteration] = average;  //Masukan data suhu yang terfilter kedalam variabel lastHoutput

  return average;  //Mengembalikan nilai kedalam konstruktor
}
