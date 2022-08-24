/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
}

void loop() {
  // read the analog in value:

  const int normal = 2467;

  sensorValue = analogRead(analogInPin);
  int mVolt = 3150 * (sensorValue /1024.0);
  int mAmps = (normal - mVolt) /  0.185; //Ieu nu 5Amps
//  int mAmps = (normal - mVolt ) / 0.066; //Ieu nu 30Amps
  
  Serial.print("ADC = ");
  Serial.print(sensorValue);  //mV
  Serial.print("\tnormal-mVolt = ");
  Serial.print(normal - mVolt);  //mV
  Serial.print("\tTegangan = ");
  Serial.print(mVolt);  //mV
  Serial.print("\tArus = ");
  Serial.println(mAmps);  //mV  
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(1000);
}
