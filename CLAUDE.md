# 4WD Rover IoT — Contexto del Proyecto

> Rover de exploración 4WD con ESP32, suspensión de doble articulación y arquitectura modular.
> Repo: https://github.com/manolosky/4WD-Rover-iot.git

## Arquitectura

- **rover-base/** — Firmware de la ESP32 principal (control de motores, WiFi AP, web UI, IMU, LEDs, buzzer, batería)
- **camera-fpv/** — Firmware de la Freenove ESP32-S3-WROOM CAM (stream de video, cliente del AP)
- **docs/** — Documentación de diseño (ver `rover-4wd-proyecto-contexto.md`)

## Fases

1. **Fase 1 (actual):** rover base + control WiFi desde móvil + cámara FPV
2. **Fase 2:** módulo de telemetría con ESP32 propia (UART vía conector GX16-10)
3. **Fase 3:** mando RC físico con NRF24L01 (receptor ya cableado en fase 1)

## Hardware clave

- ESP32 DevKit (principal) + Freenove ESP32-S3-WROOM CAM (FPV, independiente)
- 4× motores JGA25-370 12V 100RPM con encoder (11 PPR, ratio 1:43)
- 2× BTS7960: #1 = M1+M3 (izq paralelo), #2 = M2+M4 (der paralelo). Skid steer
- Ruedas 92mm hex 12mm. Velocidad máx ~0.48 m/s
- Batería LiPo 3S 2200mAh 30C XT60. LM2596 → 5V
- IMU MPU6050 (I2C, centro del chasis), buzzer pasivo, 8× WS2812B
- NRF24L01+PA+LNA cableado (inactivo hasta fase 3)

## Mapa de pines ESP32 principal

| Función | GPIO |
|---|---|
| BTS#1 RPWM / LPWM / EN (izq) | 25 / 33 / 32 |
| BTS#2 RPWM / LPWM / EN (der) | 14 / 12 / 13 |
| Encoder M1 / M2 / M3 / M4 | 4 / 0 / 36 / 39 |
| I2C SDA / SCL | 21 / 22 |
| LEDs WS2812B (8 px) | 2 |
| Buzzer pasivo (PWM) | 15 |
| Batería ADC (div 100K/27K) | 35 |
| UART1 TX / RX (GX16 módulos) | 17 / 16 |
| NRF24 MOSI/MISO/SCK/CSN/CE | 23/19/18/5/27 |

## Reglas críticas

- **ADC2 no funciona con WiFi activo** → solo ADC1 (GPIO 32-39)
- **GPIO 0** tiene pull-up de boot → válido como entrada de encoder tras arranque, nunca forzar LOW al encender
- Encoders: solo canal A (amarillo). Dirección se conoce por el comando al BTS7960
- 374 pulsos ≈ 1 vuelta de rueda (11 PPR × 34) → ~0.77mm/pulso con rueda 92mm
- Núcleo 0: WiFi + web. Núcleo 1: motores, encoders, IMU, LEDs (xTaskCreatePinnedToCore)
- WiFi AP: SSID `Rover-4WD`. La cámara se conecta como cliente con IP fija 192.168.4.2

## Convenciones de código

- PlatformIO + Arduino framework
- Módulos header-only en `include/` (config.h centraliza pines y constantes)
- Español en comentarios, inglés en identificadores
- No bloquear el loop: usar millis(), nunca delay() en lógica de control
