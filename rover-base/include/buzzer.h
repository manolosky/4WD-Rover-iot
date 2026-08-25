#pragma once
#include <Arduino.h>
#include "config.h"
#include "imu.h"

// ============================================================
// buzzer.h — Passive PWM buzzer: distinct tones per situation
// ============================================================

namespace Buzzer {

inline uint32_t lastBeep = 0;
inline bool toneOn = false;
inline bool lowBattery = false;

inline void begin() {
  ledcSetup(BUZZER_CHANNEL, 2000, 10);
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0);
}

inline void tone(uint16_t freq) {
  ledcWriteTone(BUZZER_CHANNEL, freq);
  ledcWrite(BUZZER_CHANNEL, 512);   // 50% duty
}

inline void off() { ledcWrite(BUZZER_CHANNEL, 0); }

// "Find me" melody (short blocking call, on demand)
inline void playFind() {
  const uint16_t notes[] = {880, 1175, 1480, 1760, 1480, 1760};
  for (uint16_t n : notes) { tone(n); delay(110); }
  off();
}

// Short boot beep
inline void playBoot() {
  tone(880); delay(80); tone(1320); delay(80); off();
}

// Call every ~100ms: handles continuous alarms per state
inline void update() {
  uint32_t now = millis();

  switch (IMU::state) {
    case IMU::ROLLED_OVER:
      tone(220);                       // continuous low tone
      return;
    case IMU::LIFTED:
      // intermittent high tone
      if (now - lastBeep > 250) { lastBeep = now; toneOn = !toneOn; }
      if (toneOn) tone(2200); else off();
      return;
    default:
      break;
  }

  // Low battery: short beep every 10s
  if (lowBattery) {
    if (now - lastBeep > 10000) { lastBeep = now; tone(1000); }
    else if (now - lastBeep > 150) off();
    return;
  }

  off();
}

} // namespace Buzzer
