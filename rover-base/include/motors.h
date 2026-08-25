#pragma once
#include <Arduino.h>
#include "config.h"

// ============================================================
// motors.h — Skid-steer control with 2× BTS7960
// LEDC channels: 0=L_RPWM 1=L_LPWM 2=R_RPWM 3=R_LPWM
// ============================================================

namespace Motors {

inline int8_t curLeft = 0;    // -100..100 (signed speed %)
inline int8_t curRight = 0;
inline uint8_t maxSpeedPct = GEAR_NORMAL_PCT;   // current gear (set via /gear)

inline void begin() {
  ledcSetup(0, PWM_FREQ_HZ, PWM_RESOLUTION); ledcAttachPin(PIN_L_RPWM, 0);
  ledcSetup(1, PWM_FREQ_HZ, PWM_RESOLUTION); ledcAttachPin(PIN_L_LPWM, 1);
  ledcSetup(2, PWM_FREQ_HZ, PWM_RESOLUTION); ledcAttachPin(PIN_R_RPWM, 2);
  ledcSetup(3, PWM_FREQ_HZ, PWM_RESOLUTION); ledcAttachPin(PIN_R_LPWM, 3);
  pinMode(PIN_L_EN, OUTPUT);
  pinMode(PIN_R_EN, OUTPUT);
  digitalWrite(PIN_L_EN, HIGH);
  digitalWrite(PIN_R_EN, HIGH);
}

// Convert % (0-100) to PWM applying the deadband
inline uint8_t pctToPwm(uint8_t pct) {
  if (pct == 0) return 0;
  return map(pct, 1, 100, MOTOR_DEADBAND, 255);
}

// speed: -100 (full reverse) .. 100 (full forward)
inline void setSide(uint8_t chFwd, uint8_t chRev, int8_t speed) {
  if (speed > 0)      { ledcWrite(chFwd, pctToPwm(speed));  ledcWrite(chRev, 0); }
  else if (speed < 0) { ledcWrite(chFwd, 0); ledcWrite(chRev, pctToPwm(-speed)); }
  else                { ledcWrite(chFwd, 0); ledcWrite(chRev, 0); }
}

inline void drive(int8_t left, int8_t right) {
  curLeft  = constrain(left,  -100, 100);
  curRight = constrain(right, -100, 100);
  // The gear limit only applies to the physical output;
  // curLeft/curRight keep the full scale for LEDs and telemetry.
  setSide(0, 1, (int8_t)(curLeft  * maxSpeedPct / 100));
  setSide(2, 3, (int8_t)(curRight * maxSpeedPct / 100));
}

inline void stop() { drive(0, 0); }

// Mix joystick (x: -100..100 turn, y: -100..100 forward) into tank drive
inline void driveJoystick(int8_t x, int8_t y) {
  int16_t l = y + x;
  int16_t r = y - x;
  // Normalize if above 100
  int16_t maxMag = max(abs(l), abs(r));
  if (maxMag > 100) { l = l * 100 / maxMag; r = r * 100 / maxMag; }
  drive((int8_t)l, (int8_t)r);
}

inline bool isMoving()    { return curLeft != 0 || curRight != 0; }
inline bool isReversing() { return curLeft < 0 && curRight < 0; }
inline int  turning()     { // -1 left, 0 straight, 1 right
  if (curLeft > curRight + 15) return 1;
  if (curRight > curLeft + 15) return -1;
  return 0;
}

} // namespace Motors
