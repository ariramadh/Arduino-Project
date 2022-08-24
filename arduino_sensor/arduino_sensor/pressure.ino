void read_pressure(){
  int analogValue = analogRead(A4) - zPressure[0]; //Baca sensor flow
  int analogValue2 = analogRead(A5) - zPressure[1];  //Baca sensor tekanan2
//  int analogValue = analogRead(A4) - 103;
//Serial.print("analog "); Serial.println(analogRead(A4));
//  Serial.print("analog "); Serial.println(analogRead(A5));
//  int analogValue2 = analogRead(A5) - 103;
  
  AVf = (analogValue+a*AVf_1)/(a+1);
  Q = (0.407*AVf - 180.65);
  if (Q < 9){
    Q = 0; //   l/m
  }
  pressure = (analogValue2*0.00512) - 2.8672;     // kPa
  pressure *= 10.1971;        
  pressure = constrain(pressure, 0, 100);
// 
//   Serial.print(analogValue2);
//  Serial.print("\t");
//  Serial.print("flow "); Serial.println(Q);// cmH2O

  AVf_1 = AVf;

}
