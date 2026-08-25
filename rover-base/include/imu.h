#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// ============================================================
// imu.h — MPU6050: pitch/roll + rollover and lift detection
// Direct register access (no heavy library).
// ============================================================

namespace IMU {

enum State { OK_LEVEL, TILT_WARN, ROLLED_OVER, LIFTED };

inline float pitch = 0, roll = 0, accZ = 1.0f;
inline State state = OK_LEVEL;
inline bool present = false;

inline void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(reg); Wire.write(val);
  Wire.endTransmission();
}

inline void begin() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  Wire.beginTransmission(IMU_ADDR);
  present = (Wire.endTransmission() == 0);
  if (!present) return;
  writeReg(0x6B, 0x00);  // PWR_MGMT_1: wake up
  writeReg(0x1C, 0x00);  // ACCEL_CONFIG: ±2g
  writeReg(0x1A, 0x04);  // DLPF ~21Hz: filters motor vibration
}

// Call every IMU_LOOP_MS
inline void update() {
  if (!present) return;
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x3B);                       // ACCEL_XOUT_H
  if (Wire.endTransmission(false) != 0) return;
  Wire.requestFrom((uint8_t)IMU_ADDR, (uint8_t)6);
  if (Wire.available() < 6) return;

  int16_t rawX = (Wire.read() << 8) | Wire.read();
  int16_t rawY = (Wire.read() << 8) | Wire.read();
  int16_t rawZ = (Wire.read() << 8) | Wire.read();

  float ax = rawX / 16384.0f;
  float ay = rawY / 16384.0f;
  float az = rawZ / 16384.0f;
  accZ = az;

  // Angles from the gravity vector (enough for rollover; DLPF filters vibration)
  pitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * 180.0f / PI;
  roll  = atan2f(ay, az) * 180.0f / PI;

  float aMag = sqrtf(ax * ax + ay * ay + az * az);

  if (aMag < LIFT_ACCEL_THRESH)                      state = LIFTED;       // free fall / sudden lift
  else if (fabsf(roll) > TILT_ROLLOVER_DEG ||
           fabsf(pitch) > TILT_ROLLOVER_DEG)         state = ROLLED_OVER;
  else if (fabsf(roll) > TILT_WARN_DEG)              state = TILT_WARN;    // dangerous side slope
  else                                               state = OK_LEVEL;
}

// Slope vs wall: sustained rising pitch means a slope
inline bool isClimbing() { return pitch > 8.0f && pitch < TILT_WARN_DEG; }

} // namespace IMU
