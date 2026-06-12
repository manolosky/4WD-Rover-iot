#pragma once
#include <Arduino.h>
#include "config.h"

// ============================================================
// encoders.h — Lectura de 4 encoders (canal A) por interrupción
// RPM y distancia por rueda. Dirección la da Motors.
// ============================================================

namespace Encoders {

inline volatile uint32_t pulses[4] = {0, 0, 0, 0};
inline uint32_t lastPulses[4] = {0, 0, 0, 0};
inline float rpm[4] = {0, 0, 0, 0};
inline float totalDistanceMM = 0;   // promedio de las 4 ruedas
inline uint32_t lastCalcMs = 0;

void IRAM_ATTR isrM1() { pulses[0]++; }
void IRAM_ATTR isrM2() { pulses[1]++; }
void IRAM_ATTR isrM3() { pulses[2]++; }
void IRAM_ATTR isrM4() { pulses[3]++; }

inline void begin() {
  pinMode(PIN_ENC_M1, INPUT_PULLUP);
  pinMode(PIN_ENC_M2, INPUT_PULLUP);
  pinMode(PIN_ENC_M3, INPUT);   // 36 y 39 no tienen pull-up interno:
  pinMode(PIN_ENC_M4, INPUT);   // el encoder Hall ya entrega señal limpia
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M1), isrM1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M2), isrM2, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M3), isrM3, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M4), isrM4, RISING);
  lastCalcMs = millis();
}

// Llamar cada ENCODER_CALC_MS desde la tarea de control
inline void update() {
  uint32_t now = millis();
  uint32_t dt = now - lastCalcMs;
  if (dt < ENCODER_CALC_MS) return;
  lastCalcMs = now;

  float sumDeltaMM = 0;
  for (int i = 0; i < 4; i++) {
    uint32_t p = pulses[i];               // copia atómica (32-bit OK en ESP32)
    uint32_t delta = p - lastPulses[i];
    lastPulses[i] = p;
    // RPM = (pulsos / PPR) / (dt en minutos)
    rpm[i] = (delta / ENC_PULSES_PER_REV) * (60000.0f / dt);
    sumDeltaMM += delta * MM_PER_PULSE;
  }
  totalDistanceMM += sumDeltaMM / 4.0f;
}

// Detecta rueda trabada: hay comando de movimiento pero no llegan pulsos
inline bool wheelStalled(uint8_t idx, bool commanded) {
  return commanded && rpm[idx] < 2.0f;
}

inline float avgRPM() { return (rpm[0] + rpm[1] + rpm[2] + rpm[3]) / 4.0f; }
inline float speedMS() { return avgRPM() * WHEEL_CIRCUM_MM / 60000.0f; }

} // namespace Encoders
