#pragma once
// ============================================================
// config.h — Rover 4WD · Phase 1
// Pin map and constants. Single source of truth.
// ============================================================

// ---------- WiFi ----------
// Credentials: defined in secrets.ini (see secrets.ini.example)
#ifndef SECRET_AP_SSID
  #define SECRET_AP_SSID "Rover-4WD"      // fallback if secrets.ini is missing
#endif
#ifndef SECRET_AP_PASS
  #define SECRET_AP_PASS "cambiame123"    // fallback (change it in secrets.ini)
#endif
#define WIFI_AP_SSID      SECRET_AP_SSID
#define WIFI_AP_PASS      SECRET_AP_PASS  // min 8 chars
#define WIFI_AP_CHANNEL   6
#define CAMERA_ENABLED    0   // 0 = camera on standby (the UI never tries to connect)
#define CAMERA_STREAM_URL "http://192.168.4.2:81/stream"

// ---------- Motors (BTS7960) ----------
// Final assignment calibrated on the bench (forward and turning verified):
// - Wiring "Driver B" (pins 32/33) drives the LEFT side
// - Wiring "Driver A" (pins 25/26) drives the RIGHT side
// - RPWM/LPWM swapped vs. the physical labels so that + = forward
// Left side: M1 (front) + M3 (rear) in parallel
#define PIN_L_RPWM  32
#define PIN_L_LPWM  33
#define PIN_L_EN    27   // shared: all 4 EN pins of both drivers on GPIO27
// Right side: M2 (front) + M4 (rear) in parallel
#define PIN_R_RPWM  26
#define PIN_R_LPWM  25
#define PIN_R_EN    27   // same pin as PIN_L_EN (single shared enable line)

#define PWM_FREQ_HZ     20000   // 20kHz: above the audible range
#define PWM_RESOLUTION  8       // 0-255
#define MOTOR_DEADBAND  25      // minimum PWM to overcome static friction
// Gears: speed limit selectable from the web UI (% of the physical maximum)
#define GEAR_LOW_PCT     35
#define GEAR_NORMAL_PCT  60     // default gear at boot
#define GEAR_HIGH_PCT    85

// ---------- Encoders (channel A, interrupt driven) ----------
#define PIN_ENC_M1  4    // front left
#define PIN_ENC_M2  0    // front right (boot pull-up: OK after startup)
#define PIN_ENC_M3  36   // rear left (input only)
#define PIN_ENC_M4  39   // rear right (input only)

#define ENC_PULSES_PER_REV  374.0f   // 11 PPR x 1:34 gear ratio
#define WHEEL_DIAMETER_MM   92.0f
#define WHEEL_CIRCUM_MM     (WHEEL_DIAMETER_MM * 3.14159f)  // ~289mm
#define MM_PER_PULSE        (WHEEL_CIRCUM_MM / ENC_PULSES_PER_REV)

// ---------- I2C (MPU6050 + future GX16) ----------
#define PIN_I2C_SDA  21
#define PIN_I2C_SCL  22

// ---------- IMU MPU6050 ----------
#define IMU_ADDR             0x68
#define TILT_ROLLOVER_DEG    120.0f  // |roll| or |pitch| above this = rolled over
#define TILT_WARN_DEG        35.0f   // dangerous tilt warning
#define LIFT_ACCEL_THRESH    0.45f   // Z accel (g) below this = lifted / free fall

// ---------- WS2812B LEDs ----------
#define PIN_LEDS    2
#define NUM_LEDS    8
// Indices along the strip
#define LED_FRONT_LEFT    0
#define LED_FRONT_C1      1
#define LED_FRONT_C2      2
#define LED_FRONT_RIGHT   3
#define LED_REAR_LEFT     4
#define LED_REAR_C1       5
#define LED_REAR_C2       6
#define LED_REAR_RIGHT    7

// ---------- Passive buzzer ----------
#define PIN_BUZZER       15
#define BUZZER_CHANNEL   7     // dedicated LEDC channel

// ---------- Battery (100K/27K divider on ADC1) ----------
#define PIN_BATTERY      35
#define BAT_DIVIDER      ((100.0f + 27.0f) / 27.0f)   // ~4.70
#define BAT_LOW_V        10.2f    // 3.4V/cell: warning
#define BAT_CRIT_V       9.9f     // 3.3V/cell: critical

// ---------- UART1 — GX16 connector (modules, phase 2) ----------
#define PIN_GX16_TX  17
#define PIN_GX16_RX  16

// ---------- NRF24L01 (inactive until phase 3) ----------
// NOTE: GPIO27, originally planned for CE, is now the motor EN line.
// Phase 3 will need a different pin for the NRF24 CE.
#define PIN_NRF_CSN  5
// Standard SPI: MOSI 23, MISO 19, SCK 18

// ---------- Task timing ----------
#define CONTROL_LOOP_MS    20    // 50Hz: motors + failsafe
#define IMU_LOOP_MS        100
#define TELEMETRY_MS       250
#define ENCODER_CALC_MS    100   // RPM calculation window
#define CMD_TIMEOUT_MS     600   // no command within this time -> stop (failsafe)
