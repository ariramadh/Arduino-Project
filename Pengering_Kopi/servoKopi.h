
Servo katup;  // create servo object to control a servo

const int numReadings = 5;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int pos = 0;    // variable to store the servo position

void servoSetup(int servoPin){
  katup.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void servoKontrol(int output){
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = output;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  average = total / numReadings;

  katup.write(output);  
}
