#define out 3 //Pin output

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(out, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int freq;
  int in;

  while (Serial.available()) {
    in = Serial.parseInt();

    if (in != 0)  freq = in;

    if (in != freq) {
      tone(out, freq);
      Serial.println(freq);
    }
  }

}
