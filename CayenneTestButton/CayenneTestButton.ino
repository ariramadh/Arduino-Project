/*
Cayenne Generic Digital Input Example

This sketch shows how to send Generic Digital Input Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a digital input device (e.g. a button) to a digital pin on your Arduino.
2. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
3. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of an Generic Digital Input Sensor widget you have added) in the Dashboard.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the Generic Digital Input Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Rvs";
char wifiPassword[] = "jashujan";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3e0c3e40-62d4-11ea-b73d-1be39589c6b2";
char password[] = "1c5c4bbe04dc8daabce0a357cfe4d039114a7def";
char clientID[] = "79b616d0-b43e-11ea-93bf-d33a96695544";

#define SENSOR_PIN 2 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
#define VIRTUAL_CHANNEL 1

void setup()
{
	Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	Cayenne.virtualWrite(VIRTUAL_CHANNEL, millis());
}
