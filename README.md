# 🤖 4WD Rover IoT

Rover de exploración 4WD basado en ESP32 con suspensión de doble articulación, control FPV vía WiFi y arquitectura modular.

## Estructura

```
4WD-Rover-iot/
├── CLAUDE.md            # Contexto del proyecto para Claude Code
├── rover-base/          # Firmware ESP32 principal (PlatformIO)
│   ├── include/         # Módulos header-only
│   │   ├── config.h     # ⭐ Pines y constantes (fuente única de verdad)
│   │   ├── motors.h     # Skid-steer con 2× BTS7960
│   │   ├── encoders.h   # 4 encoders por interrupción + RPM
│   │   ├── imu.h        # MPU6050: vuelco/alzado/inclinación
│   │   ├── leds.h       # 8× WS2812B: luces, intermitentes, alertas
│   │   ├── buzzer.h     # Tonos por situación
│   │   ├── battery.h    # Monitoreo LiPo 3S
│   │   └── webui.h      # Interfaz web embebida
│   └── src/main.cpp     # Dual-core: web (núcleo 0) + control (núcleo 1)
├── camera-fpv/          # Firmware Freenove ESP32-S3-WROOM CAM
│   └── src/main.cpp     # Stream MJPEG en 192.168.4.2:81/stream
└── docs/                # Documentación de diseño
```

## Fase 1 — Qué hace este firmware

- **Control desde el móvil:** la ESP32 crea el WiFi `Rover-4WD`. Conéctate y abre `http://192.168.4.1` — joystick virtual + video FPV + telemetría en una sola página
- **Video FPV:** la ESP32-S3 CAM se conecta sola al rover y transmite VGA ~20fps
- **Seguridad:** failsafe (sin señal 600ms → stop), corte de motores al detectar vuelco o alzado (IMU), aviso de batería baja
- **Luces automotrices:** delanteras (toggle), traseras según movimiento, intermitentes al girar, reversa en blanco, alerta roja parpadeante si vuelca
- **Buzzer:** tono grave = volcado, agudo intermitente = alzado, melodía = botón "encontrar", beep = batería baja
- **Telemetría:** voltaje/% batería, velocidad m/s, RPM promedio, distancia recorrida, pitch/roll

## Setup

### Requisitos
- [PlatformIO](https://platformio.org/) (CLI o extensión de VS Code)

### Configurar credenciales (primera vez)
```bash
# En rover-base/ Y en camera-fpv/, copia la plantilla:
cp secrets.ini.example secrets.ini
# Edita secrets.ini con tu SSID/password (mismo valor en ambos)
# secrets.ini está en .gitignore: nunca se sube al repo
```

### Flashear el rover
```bash
cd rover-base
pio run -t upload          # ESP32 conectada por USB
pio device monitor         # ver logs
```

### Flashear la cámara
```bash
cd camera-fpv
pio run -t upload          # Freenove ESP32-S3 por USB nativo
```

> El SSID/password se define una sola vez por proyecto en `secrets.ini`. Usa los mismos valores en rover-base y camera-fpv.

### Probar
1. Enciende el rover (y la cámara)
2. En el móvil: WiFi → `Rover-4WD` (el password de tu secrets.ini)
3. Navegador → `http://192.168.4.1`

## Endpoints HTTP del rover

| Ruta | Función |
|---|---|
| `/` | Interfaz web completa |
| `/cmd?x=&y=` | Comando de joystick (-100..100) |
| `/lights` | Toggle de luces delanteras |
| `/find` | Buzzer + flash "encontrar" |
| `/status` | JSON de telemetría |

## Roadmap

- [x] **Fase 1:** rover base + WiFi + FPV *(este código)*
- [ ] **Fase 2:** módulo de telemetría (ESP32 propia, UART vía GX16-10)
- [ ] **Fase 3:** mando RC físico (NRF24L01, ~1km)

## Hardware

Ver `CLAUDE.md` para el mapa de pines completo y `docs/` para el diseño mecánico.
