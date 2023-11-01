#include "BluetoothSerial.h"
#include <Wire.h>
#include <MPU9250.h>
BluetoothSerial SerialBT;

float rawValues[9];
MPU9250 mpu;

int incoming;
int id = -1;
int val_byte1 = -1;
int val_byte2 = -1;
int val_byte3 = -1;
int val_byte4 = -1;
int val_byte5 = -1;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("VibREAD");  // Set your device name here
  delay(1000);
  Wire.begin();
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed");
    }
  }
  rawValues[0] = 0.00;  //accel data
  rawValues[1] = 0.00;
  rawValues[2] = 0.00;
  rawValues[3] = 0.00;  //gyro data
  rawValues[4] = 0.00;
  rawValues[5] = 0.00;
  rawValues[6] = 0.00;  //mag data
  rawValues[7] = 0.00;
  rawValues[8] = 0.00;
}

void loop() {
  if (SerialBT.connected()) {
    if (mpu.update()) {
      get_calibrated_accel(rawValues);
      get_calibrated_gyro(rawValues);
      get_calibrated_mag(rawValues);

      uint8_t byteArray[sizeof(float) * 9];
      memcpy(byteArray, rawValues, sizeof(float));
      send_BT(1, rawValues[0]);  // accelx
      send_BT(2, rawValues[1]);  // accely
      send_BT(3, rawValues[2]);  // accelz
      send_BT(4, rawValues[3]);  // gyrox
      send_BT(5, rawValues[4]);  // gyrox
      send_BT(6, rawValues[5]);  // gyrox
      send_BT(7, rawValues[6]);  // magx
      send_BT(8, rawValues[7]);  // magy
      send_BT(9, rawValues[8]);  // magz
      Serial.println(String(rawValues[0]) + ", " + String(rawValues[1]) + ", " + String(rawValues[2]) + "  |  " + String(rawValues[3]) + ", " + String(rawValues[4]) + ", " + String(rawValues[5]));
      delay(1000);
    }

  } else {
    if (mpu.update()) {
      get_calibrated_accel(rawValues);
      get_calibrated_gyro(rawValues);
      get_calibrated_mag(rawValues);
      Serial.println(String(rawValues[0]) + ", " + String(rawValues[1]) + ", " + String(rawValues[2]) + "  ||  " + String(rawValues[3]) + ", " + String(rawValues[4]) + ", " + String(rawValues[5]));
    }
  }
  delay(10);
}
void reset_rx_BT() {  // function to erase all bytes (set to -1)
  id = -1;
  val_byte1 = -1;
  val_byte2 = -1;
  val_byte3 = -1;
  val_byte4 = -1;
  val_byte5 = -1;
}
void send_BT(int idx, float value) {  // function to write id and value to the bluetooth interface
  id = idx;
  int valInt = (value + 100) * 100;

  val_byte1 = floor((((valInt % 128) % 128) % 128) / 128);
  val_byte2 = floor(((valInt % 128) % 128) / 128);
  val_byte3 = floor((valInt % 128) / 128);
  val_byte4 = floor(valInt / 128);
  val_byte5 = valInt % 128;

  SerialBT.write(128 + id);
  SerialBT.write(val_byte1);  // MSB
  SerialBT.write(val_byte2);
  SerialBT.write(val_byte3);
  SerialBT.write(val_byte4);
  SerialBT.write(val_byte5);  //LSB
  reset_rx_BT();
}
void get_calibrated_accel(float* floatValues) {
  floatValues[0] = mpu.getAccX() - 0.04;
  floatValues[1] = mpu.getAccY() - 0.04;
  floatValues[2] = mpu.getAccZ() - 0.91;
}
void get_calibrated_gyro(float* floatValues) {
  floatValues[3] = mpu.getGyroX() + 0.00;
  floatValues[4] = mpu.getGyroY() + 0.00;
  floatValues[5] = mpu.getGyroZ() + 0.00;
}
void get_calibrated_mag(float* floatValues) {
  floatValues[6] = (mpu.getMagX() - 0.00) * 1.00;
  floatValues[7] = (mpu.getMagY() - 0.00) * 1.00;
  floatValues[8] = (mpu.getMagZ() - 0.00) * 1.00;
}