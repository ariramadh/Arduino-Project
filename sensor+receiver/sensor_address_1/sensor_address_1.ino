#include "MPU6050.h"
#include "Wire.h"
#include <SimpleTimer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SimpleTimer firstTimer(5000);
SoftwareSerial mySerial(10, 11); // RX, TX

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

const int PULSE_INPUT = A0;
const int PULSE_BLINK = 13;    // Pin 13 is the on-board LED
const int PULSE_FADE = 5;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

const float CONST_16G = 2048;
const float CONST_2000 = 16.4;
const float CONST_G = 9.81;
const float RADIANS_TO_DEGREES = 180 / 3.14159;
const float ALPHA = 0.96;
const float KMPH = 3.6;
const float angle = 3.6;
const int jmlFilter = 2;

float lastOutput[jmlFilter];

int heartBPM = 0;
int countBPM = 0;

int mod, dat;

MPU6050 accelgyro;

PulseSensorPlayground pulseSensor;

unsigned long last_read_time;
int16_t ax, ay, az, gx, gy, gz;
int16_t gyro_angle_x_l, gyro_angle_y_l;
int16_t angle_x_l, angle_y_l;
int16_t ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
int16_t temperature;
int16_t pedo;

bool y_up = false;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  mySerial.begin(9600);

  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  accelgyro.initialize();

  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  accelgyro.setFullScaleAccelRange(0x03);
  accelgyro.setFullScaleGyroRange(0x03);

  calibrate_sensors();
  set_last_time(millis());

  if (!pulseSensor.begin()) {
    for (;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
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
  //  float vel = sqrt(pow(vel_x, 2) + pow(vel_y, 2)) * KMPH;
  float vel = constrain(vel_x, 0, 1000) * KMPH; //Hanya ambil kecepatan maju

  set_last_time(millis());

  if (ax_p * 100 < -80) {
    y_up = true;
  }

  if (ax_p * 100 > 100 && y_up) {
    pedo++;
    y_up = false;
  }

  temperature = (accelgyro.getTemperature() + 12412) / 340;

  if (pulseSensor.sawStartOfBeat()) {
    heartBPM = constrain(pulseSensor.getBeatsPerMinute(), 0, 250);
    countBPM = 0;
  } else {
    countBPM++;
  }

  if (countBPM == 300) {
    heartBPM = 0;
  }

  //  Serial.print("Step: ");
  //  Serial.print(pedo);
  //  Serial.print("\t");
  //  Serial.print("Heartbeat: ");
  //  Serial.print(heartBPM);
  //  Serial.print("\t");
  //  Serial.println();

  //    Serial.print("accel: ");
  //    Serial.print(ax_p);
  //    Serial.print(",");
  //    Serial.print(ay_p);
  //    Serial.print(",");
  //    Serial.print(az_p);
  //
  //    Serial.print("\tgyro: ");
  //    Serial.print(gx_p);
  //    Serial.print(",");
  //    Serial.print(gy_p);
  //    Serial.print(",");
  //    Serial.print(gz_p);
  //    Serial.print("\ttvel: ");
  //    Serial.print(vel, 4);
  //    Serial.print(", X:");
  //    Serial.print(vel_x, 4);
  //    Serial.print(", Y:");
  //    Serial.print(vel_y, 4);
  //    Serial.print("\tpitch: ");
  //    Serial.print(angle_x);
  //    Serial.print("   roll: ");
  //    Serial.print(angle_y);
  //    Serial.print("   temp: ");
  //    Serial.print(temperature);
  //    Serial.print("\t");
  //    Serial.print("Step: ");
  //    Serial.print(pedo);
  //    Serial.print("\t");
  //    Serial.print("Heartbeat: ");
  //    Serial.print(heartBPM);
  //    Serial.print("\t");
  //    Serial.println();

  //  while (Serial.available() > 0) {
  //    if (Serial.read() == 'a') {

  /*

    if (firstTimer.isReady()) {
      root["ID"] = "002";
      root["step"] = pedo;
      root["heartbeat"] = heartBPM / 2;

      root.printTo(Serial);
      root.printTo(mySerial);
      mySerial.print("\n");
      Serial.println();
      firstTimer.reset();
    }

  */

  ///////////////////// kirim berdasarkan perintah
  if (Serial.available()) {
    if ((char)Serial.read() == '*') {
      mod = Serial.parseInt();
      dat = Serial.parseInt();
      //Serial.println("HC12 is Receiving fist data");
    }
    if ((char)Serial.read() == '#') {
      //Serial.println("HC12 is Receiving last data");
      if (mod == 1) {
        if (dat == 1) {

          root["ID"] = "001";
          root["step"] = pedo;
          root["heartbeat"] = heartBPM;

          root.printTo(Serial);
          root.printTo(mySerial);
          mySerial.print("\n");
          Serial.println();
          mod = 0;
          dat = 0;
        }
      }
    }
  }

  //    }
  //  }
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
    /*   Serial.print(i);
       Serial.print("-CALIBRATION: ");
       Serial.print((ax / CONST_16G));
       Serial.print(",");
       Serial.print((ay / CONST_16G));
       Serial.print(",");
       Serial.print((az / CONST_16G));
       Serial.print(",");
       Serial.print(gx / CONST_2000);
       Serial.print(",");
       Serial.print(gy / CONST_2000);
       Serial.print(",");
       Serial.println(gz / CONST_2000);
    */
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

  Serial.print("Offsets: ");
  Serial.print(ax_offset);
  Serial.print(", ");
  Serial.print(ay_offset);
  Serial.print(", ");
  Serial.print(az_offset);
  Serial.print(", ");
  Serial.print(gx_offset);
  Serial.print(", ");
  Serial.print(gy_offset);
  Serial.print(", ");
  Serial.println(gz_offset);
  Serial.println("Finishing Calibration");
}

float ewma(float raw, int iteration) {
  float alpha = 0.5;
  float average = alpha * raw + (1 - alpha) * lastOutput[iteration];

  lastOutput[iteration] = average;

  return average;

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
