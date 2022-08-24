/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

// the setup routine runs once when you press reset:

int sensorOffset;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(38400);
  sensorOffset = analogRead(A5);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
//  float sensorValue = 0.1323 * (analogRead(A5)-sensorOffset) - 2.7531;
  float sensorValue = (analogRead(A5)-sensorOffset) / 10.0;

  // print out the value you read:
  Serial.print(analogRead(A5)-sensorOffset);
  Serial.print("\tADC\t");
  Serial.print(constrain(sensorValue,0,100));
  Serial.print("\tcmH2O");
  Serial.println();
  delay(20); // delay in between reads for stability
}
