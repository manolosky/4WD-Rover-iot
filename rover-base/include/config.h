#pragma once
// ============================================================
// config.h — Rover 4WD · Fase 1
// Mapa de pines y constantes. Fuente única de verdad.
// ============================================================

// ---------- WiFi ----------
// Credenciales: definidas en secrets.ini (ver secrets.ini.example)
#ifndef SECRET_AP_SSID
  #define SECRET_AP_SSID "Rover-4WD"      // fallback si no hay secrets.ini
#endif
#ifndef SECRET_AP_PASS
  #define SECRET_AP_PASS "cambiame123"    // fallback (cámbialo en secrets.ini)
#endif
#define WIFI_AP_SSID      SECRET_AP_SSID
#define WIFI_AP_PASS      SECRET_AP_PASS  // min 8 chars
#define WIFI_AP_CHANNEL   6
#define CAMERA_STREAM_URL "http://192.168.4.2:81/stream"

// ---------- Motores (BTS7960) ----------
// Lado izquierdo: M1 (frontal) + M3 (trasero) en paralelo
#define PIN_L_RPWM  25
#define PIN_L_LPWM  33
#define PIN_L_EN    32
// Lado derecho: M2 (frontal) + M4 (trasero) en paralelo
#define PIN_R_RPWM  14
#define PIN_R_LPWM  12
#define PIN_R_EN    13

#define PWM_FREQ_HZ     20000   // 20kHz: fuera del rango audible
#define PWM_RESOLUTION  8       // 0-255
#define MOTOR_DEADBAND  25      // PWM mínimo para vencer fricción estática

// ---------- Encoders (canal A, interrupción) ----------
#define PIN_ENC_M1  4    // frontal izq
#define PIN_ENC_M2  0    // frontal der (pull-up de boot: OK tras arranque)
#define PIN_ENC_M3  36   // trasero izq (input only)
#define PIN_ENC_M4  39   // trasero der (input only)

#define ENC_PULSES_PER_REV  374.0f   // 11 PPR x ratio 1:34
#define WHEEL_DIAMETER_MM   92.0f
#define WHEEL_CIRCUM_MM     (WHEEL_DIAMETER_MM * 3.14159f)  // ~289mm
#define MM_PER_PULSE        (WHEEL_CIRCUM_MM / ENC_PULSES_PER_REV)

// ---------- I2C (MPU6050 + futuro GX16) ----------
#define PIN_I2C_SDA  21
#define PIN_I2C_SCL  22

// ---------- IMU MPU6050 ----------
#define IMU_ADDR             0x68
#define TILT_ROLLOVER_DEG    120.0f  // |roll| o |pitch| > esto = volcado
#define TILT_WARN_DEG        35.0f   // alerta de inclinación peligrosa
#define LIFT_ACCEL_THRESH    0.45f   // g en Z por debajo = alzado/caída libre

// ---------- LEDs WS2812B ----------
#define PIN_LEDS    2
#define NUM_LEDS    8
// Índices en la cadena
#define LED_FRONT_LEFT    0
#define LED_FRONT_C1      1
#define LED_FRONT_C2      2
#define LED_FRONT_RIGHT   3
#define LED_REAR_LEFT     4
#define LED_REAR_C1       5
#define LED_REAR_C2       6
#define LED_REAR_RIGHT    7

// ---------- Buzzer pasivo ----------
#define PIN_BUZZER       15
#define BUZZER_CHANNEL   7     // canal LEDC dedicado

// ---------- Batería (divisor 100K/27K en ADC1) ----------
#define PIN_BATTERY      35
#define BAT_DIVIDER      ((100.0f + 27.0f) / 27.0f)   // ~4.70
#define BAT_LOW_V        10.2f    // 3.4V/celda: aviso
#define BAT_CRIT_V       9.9f     // 3.3V/celda: crítico

// ---------- UART1 — conector GX16 (módulos, fase 2) ----------
#define PIN_GX16_TX  17
#define PIN_GX16_RX  16

// ---------- NRF24L01 (cableado, inactivo hasta fase 3) ----------
#define PIN_NRF_CSN  5
#define PIN_NRF_CE   27
// SPI estándar: MOSI 23, MISO 19, SCK 18

// ---------- Timing de tareas ----------
#define CONTROL_LOOP_MS    20    // 50Hz: motores + failsafe
#define IMU_LOOP_MS        100
#define TELEMETRY_MS       250
#define ENCODER_CALC_MS    100   // ventana para cálculo de RPM
#define CMD_TIMEOUT_MS     600   // sin comando en este tiempo -> stop (failsafe)
