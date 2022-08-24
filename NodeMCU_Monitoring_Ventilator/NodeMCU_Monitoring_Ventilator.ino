/*
  This example shows how to connect to Cayenne using a WiFi shield and send/receive sample data.

  The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

  Steps:
  1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
  2. Set the network name and password.
  3. Compile and upload the sketch.
  4. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTESP8266.h>
#include <SimpleTimer.h>

// WiFi network info.
char ssid[] = "GERIN_APRIL";
char wifiPassword[] = "04032112";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3e0c3e40-62d4-11ea-b73d-1be39589c6b2";
char password[] = "1c5c4bbe04dc8daabce0a357cfe4d039114a7def";
char clientID[] = "6a9aa5f0-bf45-11ea-b767-3f1a8f1211ba";

#define pinRelay 5

SimpleTimer sendData(3000);
SimpleTimer temp(10000);

bool tombol;
bool overCurrent;
bool statusRelay;

int analogValue;
float mVolt;
float valSuhu;

void setup() {
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  pinMode(pinRelay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  suhuSetup();
  valSuhu = readSuhu();
}

void loop() {
  Cayenne.loop();

  if (temp.isReady()) {
    valSuhu = readSuhu();
    temp.reset();
  }

  statusRelay = tombol && overCurrent;

  Serial.print("Waktu menyala(s) : ");
  Serial.print(millis() / 1000);
  Serial.print("s\t\tSuhu : ");
  Serial.print(valSuhu);
  Serial.print("C\tADC Arus: ");
  Serial.print(analogValue);
  Serial.print("\tTegangan arus : ");
  Serial.print(mVolt);
  Serial.print("\tArus : ");
  Serial.print(readCurrent());
  Serial.print("mA\t\tTombol : ");
  Serial.print(tombol);
  Serial.print("\tStatus arus : ");
  Serial.print(overCurrent);
  Serial.print("\t\tRelay: ");
  Serial.print(statusRelay);
  Serial.println();

  //  if (statusRelay) {
  //    digitalWrite(LED_BUILTIN, !statusRelay);
  //  } else
  digitalWrite(LED_BUILTIN, !tombol);

  digitalWrite(pinRelay, !statusRelay);

  if (sendData.isReady()) {
    cayenneOut();
    sendData.reset();
  }
}

void cayenneOut() {
  CAYENNE_LOG("Send data");
  Cayenne.virtualWrite(0, millis() / 1000);
  Cayenne.virtualWrite(1, valSuhu);
  Cayenne.virtualWrite(2, readCurrent());
  Cayenne.virtualWrite(4, statusRelay);
}

CAYENNE_IN(3)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", 3, 2, value);
  // Write the value received to the digital pin.
  tombol = value;
}
