#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <SimpleTimer.h>

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node00 = 00;

const float CONST_16G = 2048;
const float CONST_2000 = 16.4;
const float CONST_G = 9.81;
const float RADIANS_TO_DEGREES = 180 / 3.14159;
const float ALPHA = 0.96;
const float KMPH = 3.6;

MPU6050 accelgyro;

unsigned long last_read_time;
int16_t ax, ay, az, gx, gy, gz;
int16_t gyro_angle_x_l, gyro_angle_y_l;
int16_t angle_x_l, angle_y_l;
int16_t ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
int16_t temperature;

int count = 0;
float calories;
float distance;
float spd[] = {0, 0};
unsigned tick[2];

bool done = false;
bool line = false;
bool start = false;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

SimpleTimer firstTimer(5000);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);

  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(0x03);
  accelgyro.setFullScaleGyroRange(0x03);
  calibrate_sensors();
  set_last_time(millis());

}

void loop() {
  jsonCompile();
  unsigned long pedometer = count;  // Read the potentiometer value
}

void jsonCompile() {
  if (start = true) {
    readSerial();
    line = false;

    JsonArray& dataK = root.createNestedArray("kecepatan");
    JsonArray& dataP = root.createNestedArray("percepatan");

    while (line = false) {
      if (firstTimer.isReady()) {
        readSerial();
        dataK.add(bacaKecepatan());
        spd[0] = bacaKecepatan();
        tick[0] = millis();
        float percepatan = (spd[0] - spd[1]) / (tick[0] - tick[1]);
        dataP.add(percepatan);
      }

      RF24NetworkHeader header3(node00);     // (Address where the data is going)
      network.update();
      bacaKecepatan();
      tick[1] = tick[0];
      spd[1] = spd[0];
    }


    root["id"] = "02";
    root["kecepatanR"] = 1351824120;
  }
}

void readSerial() {
  done = false;

  if (Serial.available() > 0 && !done) {
    //    Serial.print("SERIAL AVAILABLE  ");
    //    Serial.println(Serial.available());
    String buffer = Serial.readStringUntil("\n");

    if (buffer  == "02") {
      root.printTo(Serial);
      jsonBuffer.clear();
      start = true;
      line = true;
    }
  }
}

void calibrate_sensors() {
  int                   num_readings = 100;
  float                 x_accel = 0;
  float                 y_accel = 0;
  float                 z_accel = 0;
  float                 x_gyro = 0;
  float                 y_gyro = 0;
  float                 z_gyro = 0;

  // Discard the first set of values read from the IMU
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Read and average the raw values from the IMU
  for (int i = 0; i < num_readings; i++) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    x_accel += ax;
    y_accel += ay;
    z_accel += az;
    x_gyro += gx;
    y_gyro += gy;
    z_gyro += gz;
    delay(10);
  }
  x_accel /= num_readings;
  y_accel /= num_readings;
  z_accel /= num_readings;
  x_gyro /= num_readings;
  y_gyro /= num_readings;
  z_gyro /= num_readings;

  // Store the raw calibration values globally
  ax_offset = x_accel;
  ay_offset = y_accel;
  az_offset = z_accel;
  gx_offset = x_gyro;
  gy_offset = y_gyro;
  gz_offset = z_gyro;
}

float bacaKecepatan() {
  unsigned long t_now = millis();
  float dt = get_delta_time(t_now);
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float ax_p = (ax - ax_offset) / CONST_16G;
  float ay_p = (ay - ay_offset) / CONST_16G;
  float az_p = (az / CONST_16G);

  float accel_angle_y = atan(-1 * ax_p / sqrt(pow(ay_p, 2) + pow(az_p, 2))) * RADIANS_TO_DEGREES;
  float accel_angle_x = atan(ay_p / sqrt(pow(ax_p, 2) + pow(az_p, 2))) * RADIANS_TO_DEGREES;

  float gx_p = (gx - gx_offset) / CONST_2000;
  float gy_p = (gy - gy_offset) / CONST_2000;
  float gz_p = (gz - gz_offset) / CONST_2000;

  float gyro_angle_x = gx_p * dt + get_last_angle_x();
  float gyro_angle_y = gy_p * dt + get_last_angle_y();

  float angle_x = ALPHA * gyro_angle_x + (1.0 - ALPHA) * accel_angle_x;
  float angle_y = ALPHA * gyro_angle_y + (1.0 - ALPHA) * accel_angle_y;

  float vel_x = (ax_p * dt * CONST_G);
  float vel_y = (ay_p * dt * CONST_G);
  float vel = sqrt(pow(vel_x, 2) + pow(vel_y, 2)) * KMPH;

  temperature = (accelgyro.getTemperature() + 12412) / 340;

  set_last_time(t_now);

  set_last_gyro_angle_x(gyro_angle_x);
  set_last_gyro_angle_y(gyro_angle_y);

  set_last_angle_x(angle_x);
  set_last_angle_y(angle_y);

  return vel;
}

inline unsigned long get_last_time() {
  return last_read_time;
}

inline void set_last_time(unsigned long _time) {
  last_read_time = _time;
}

inline float get_delta_time(unsigned long t_now) {
  return (t_now - get_last_time()) / 1000.0;
}

inline int16_t get_last_gyro_angle_x() {
  return gyro_angle_x_l;
}

inline void set_last_gyro_angle_x(int16_t _gyro_angle_x) {
  gyro_angle_x_l = _gyro_angle_x;
}

inline int16_t get_last_gyro_angle_y() {
  return gyro_angle_y_l;
}

inline void set_last_gyro_angle_y(int16_t _gyro_angle_y) {
  gyro_angle_y_l = _gyro_angle_y;
}

inline int16_t get_last_angle_x() {
  return angle_x_l;
}

inline void set_last_angle_x(int16_t _ang_x) {
  angle_x_l = _ang_x;
}

inline int16_t get_last_angle_y() {
  return angle_y_l;
}

inline void set_last_angle_y(int16_t _ang_y) {
  angle_y_l = _ang_y;
}

inline float get_accel_xy(float ax_p, float ay_p) {
  return sqrt(pow(ax_p, 2) + pow(ay_p, 2));
}
