#pragma once
#include <Arduino.h>
#include "config.h"

// ============================================================
// motors.h — Control skid-steer con 2× BTS7960
// Canales LEDC: 0=L_RPWM 1=L_LPWM 2=R_RPWM 3=R_LPWM
// ============================================================

namespace Motors {

inline int8_t curLeft = 0;    // -100..100 (% velocidad con signo)
inline int8_t curRight = 0;

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

// Convierte % (0-100) a PWM aplicando deadband
inline uint8_t pctToPwm(uint8_t pct) {
  if (pct == 0) return 0;
  return map(pct, 1, 100, MOTOR_DEADBAND, 255);
}

// speed: -100 (reversa máx) .. 100 (avance máx)
inline void setSide(uint8_t chFwd, uint8_t chRev, int8_t speed) {
  if (speed > 0)      { ledcWrite(chFwd, pctToPwm(speed));  ledcWrite(chRev, 0); }
  else if (speed < 0) { ledcWrite(chFwd, 0); ledcWrite(chRev, pctToPwm(-speed)); }
  else                { ledcWrite(chFwd, 0); ledcWrite(chRev, 0); }
}

inline void drive(int8_t left, int8_t right) {
  curLeft  = constrain(left,  -100, 100);
  curRight = constrain(right, -100, 100);
  setSide(0, 1, curLeft);
  setSide(2, 3, curRight);
}

inline void stop() { drive(0, 0); }

// Mezcla joystick (x: -100..100 giro, y: -100..100 avance) a tank drive
inline void driveJoystick(int8_t x, int8_t y) {
  int16_t l = y + x;
  int16_t r = y - x;
  // Normalizar si supera 100
  int16_t maxMag = max(abs(l), abs(r));
  if (maxMag > 100) { l = l * 100 / maxMag; r = r * 100 / maxMag; }
  drive((int8_t)l, (int8_t)r);
}

inline bool isMoving()    { return curLeft != 0 || curRight != 0; }
inline bool isReversing() { return curLeft < 0 && curRight < 0; }
inline int  turning()     { // -1 izq, 0 recto, 1 der
  if (curLeft > curRight + 15) return 1;
  if (curRight > curLeft + 15) return -1;
  return 0;
}

} // namespace Motors
