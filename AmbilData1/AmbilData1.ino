const int jmlDHT = 4;
int interval = 0;

float h[jmlDHT];
float t[jmlDHT];

void setup() {
  Serial.begin(115200);

  runsetup();
  lcdsetup();
}

void loop() {
  runloop();
  lcdloop();
}
