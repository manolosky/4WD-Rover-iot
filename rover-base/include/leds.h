#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "motors.h"
#include "imu.h"

// ============================================================
// leds.h — 8× WS2812B: luces, intermitentes, freno, alertas
// ============================================================

namespace Leds {

inline Adafruit_NeoPixel strip(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);
inline bool headlightsOn = false;   // controlado desde la web (manual fase 1, auto con LTR390 en fase 2)
inline uint32_t lastBlink = 0;
inline bool blinkPhase = false;

inline void begin() {
  strip.begin();
  strip.setBrightness(120);
  strip.clear();
  strip.show();
}

inline uint32_t C(uint8_t r, uint8_t g, uint8_t b) { return strip.Color(r, g, b); }

// Llamar cada ~50ms desde la tarea de control
inline void update() {
  uint32_t now = millis();
  if (now - lastBlink > 350) { blinkPhase = !blinkPhase; lastBlink = now; }

  strip.clear();

  // --- Alertas críticas dominan todo ---
  if (IMU::state == IMU::ROLLED_OVER) {
    uint32_t red = blinkPhase ? C(255, 0, 0) : 0;
    for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, red);
    strip.show();
    return;
  }

  // --- Luces delanteras ---
  if (headlightsOn) {
    strip.setPixelColor(LED_FRONT_C1, C(255, 255, 255));
    strip.setPixelColor(LED_FRONT_C2, C(255, 255, 255));
    strip.setPixelColor(LED_FRONT_LEFT,  C(180, 180, 180));
    strip.setPixelColor(LED_FRONT_RIGHT, C(180, 180, 180));
  }

  // --- Luces traseras ---
  uint32_t rear = Motors::isReversing() ? C(255, 255, 255)       // reversa: blanco
                : Motors::isMoving()    ? C(120, 0, 0)            // rodando: rojo tenue
                : C(40, 0, 0);                                    // parado: rojo mínimo
  strip.setPixelColor(LED_REAR_C1, rear);
  strip.setPixelColor(LED_REAR_C2, rear);

  // --- Intermitentes (naranja parpadeante al girar) ---
  int t = Motors::turning();
  if (t != 0 && blinkPhase) {
    uint32_t amber = C(255, 90, 0);
    if (t < 0) { strip.setPixelColor(LED_FRONT_LEFT, amber);  strip.setPixelColor(LED_REAR_LEFT, amber); }
    else       { strip.setPixelColor(LED_FRONT_RIGHT, amber); strip.setPixelColor(LED_REAR_RIGHT, amber); }
  }

  strip.show();
}

// Animación de "encontrar": destello azul/blanco
inline void flashFind() {
  for (int k = 0; k < 6; k++) {
    uint32_t c = (k % 2) ? C(0, 80, 255) : C(255, 255, 255);
    for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, c);
    strip.show();
    delay(120);
  }
  strip.clear();
  strip.show();
}

} // namespace Leds
