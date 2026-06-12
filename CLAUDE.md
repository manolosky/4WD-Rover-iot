# 4WD Rover IoT — Project Context

> 4WD exploration rover with ESP32, double-articulation suspension and modular architecture.
> Repo: https://github.com/manolosky/4WD-Rover-iot.git

## Architecture

- **rover-base/** — Main ESP32 firmware (motor control, WiFi AP, web UI, IMU, LEDs, buzzer, battery)
- **camera-fpv/** — Freenove ESP32-S3-WROOM CAM firmware (video stream, AP client)
- **docs/** — Design documentation (see `rover-4wd-project-context.md`)

## Phases

1. **Phase 1 (current):** base rover + WiFi control from phone + FPV camera
2. **Phase 2:** telemetry module with its own ESP32 (UART via GX16-10 connector)
3. **Phase 3:** physical RC transmitter with NRF24L01 (receiver already wired in phase 1)

## Key hardware

- ESP32 DevKit (main) + Freenove ESP32-S3-WROOM CAM (FPV, independent)
- 4× JGA25-370 12V 100RPM motors with encoder (11 PPR, 1:43 ratio)
- 2× BTS7960: #1 = M1+M3 (left, paralleled), #2 = M2+M4 (right, paralleled). Skid steer
- 92mm wheels, 12mm hex. Max speed ~0.48 m/s
- LiPo 3S 2200mAh 30C battery, XT60 connector. LM2596 → 5V
- MPU6050 IMU (I2C, chassis center), passive buzzer, 8× WS2812B
- NRF24L01+PA+LNA wired (inactive until phase 3)

## Main ESP32 pin map

| Function | GPIO |
|---|---|
| BTS#1 RPWM / LPWM / EN (left) | 25 / 33 / 32 |
| BTS#2 RPWM / LPWM / EN (right) | 14 / 12 / 13 |
| Encoder M1 / M2 / M3 / M4 | 4 / 0 / 36 / 39 |
| I2C SDA / SCL | 21 / 22 |
| WS2812B LEDs (8 px) | 2 |
| Passive buzzer (PWM) | 15 |
| Battery ADC (100K/27K divider) | 35 |
| UART1 TX / RX (GX16 modules) | 17 / 16 |
| NRF24 MOSI/MISO/SCK/CSN/CE | 23/19/18/5/27 |

## Critical rules

- **ADC2 does not work while WiFi is active** → ADC1 only (GPIO 32-39)
- **GPIO 0** has a boot pull-up → valid as encoder input after boot, never force it LOW at power-up
- Encoders: channel A only (yellow wire). Direction is known from the command sent to the BTS7960
- 374 pulses ≈ 1 wheel revolution (11 PPR × 34) → ~0.77mm/pulse with a 92mm wheel
- Core 0: WiFi + web. Core 1: motors, encoders, IMU, LEDs (xTaskCreatePinnedToCore)
- WiFi AP: SSID `Rover-4WD`. The camera connects as a client with static IP 192.168.4.2

## Code conventions

- PlatformIO + Arduino framework
- Header-only modules in `include/` (config.h centralizes pins and constants)
- Spanish in comments, English in identifiers
- Never block the loop: use millis(), never delay() in control logic
