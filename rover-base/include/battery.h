#pragma once
#include <Arduino.h>
#include "config.h"

// ============================================================
// battery.h — LiPo 3S monitoring via 100K/27K divider on ADC1
// ============================================================

namespace Battery {

inline float voltage = 0;
inline uint8_t percent = 0;
inline bool low = false;
inline bool critical = false;

inline void begin() {
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_BATTERY, ADC_11db);  // range up to ~3.3V
}

// Call every TELEMETRY_MS. Averages several samples for stability.
inline void update() {
  uint32_t acc = 0;
  for (int i = 0; i < 8; i++) acc += analogRead(PIN_BATTERY);
  float adcV = (acc / 8.0f) * (3.3f / 4095.0f);
  // Exponential smoothing
  float v = adcV * BAT_DIVIDER;
  voltage = (voltage == 0) ? v : voltage * 0.85f + v * 0.15f;

  // Approximate LiPo 3S %: 9.9V = 0%, 12.6V = 100%
  percent = (uint8_t)constrain((voltage - 9.9f) / (12.6f - 9.9f) * 100.0f, 0.0f, 100.0f);
  low      = voltage < BAT_LOW_V;
  critical = voltage < BAT_CRIT_V;
}

} // namespace Battery
