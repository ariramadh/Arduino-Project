/*
  This example sketch shows how a value can be sent from the Arduino to the Cayenne Dashboard at automatic intervals.

  The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

  Steps:
  1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
  2. Compile and upload the sketch.
  3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial     // Comment this out to disable prints and save space
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Rvs";
char wifiPassword[] = "jashujan";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3e0c3e40-62d4-11ea-b73d-1be39589c6b2";
char password[] = "1c5c4bbe04dc8daabce0a357cfe4d039114a7def";
char clientID[] = "a6c3b100-bea7-11ea-93bf-d33a96695544";

// Use Virtual Channel 5 for uptime display.
#define tempChannel 2
#define humChannel 1
#define harChannel 5

#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;
float humidity, temperature;
bool hareudang = false;

void setup()
{
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  pinMode(2, OUTPUT);

  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 5 / D2
}

void loop()
{
Cayenne.loop();

  digitalWrite(2, LOW);
  delay(dht.getMinimumSamplingPeriod());

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t");
  Serial.print(temperature, 1);

  if (dht.getStatusString() == "TIMEOUT") {
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  }

  if (temperature >= 29.0) {
    hareudang = true;
  } else {
    hareudang = false;
  }

  Serial.print("\t");
  Serial.println(hareudang, 1);

  Cayenne.virtualWrite(tempChannel, temperature);
}

CAYENNE_OUT(tempChannel) {
  CAYENNE_LOG("Send data for Virtual Channel %d", tempChannel);

  Cayenne.celsiusWrite(tempChannel, temperature);
}

CAYENNE_OUT(humChannel) {
  CAYENNE_LOG("Send data for Virtual Channel %d", humChannel);
  Cayenne.virtualWrite(humChannel, humidity);
}

CAYENNE_OUT(harChannel) {
  CAYENNE_LOG("Send data for Virtual Channel %d", harChannel);
  Cayenne.virtualWrite(harChannel, hareudang, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);

}

CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
