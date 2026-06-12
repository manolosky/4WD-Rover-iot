# 🤖 4WD Rover IoT

4WD exploration rover based on ESP32 with double-articulation suspension, FPV control over WiFi and a modular architecture.

## Structure

```
4WD-Rover-iot/
├── CLAUDE.md            # Project context for Claude Code
├── rover-base/          # Main ESP32 firmware (PlatformIO)
│   ├── include/         # Header-only modules
│   │   ├── config.h     # ⭐ Pins and constants (single source of truth)
│   │   ├── motors.h     # Skid-steer with 2× BTS7960
│   │   ├── encoders.h   # 4 interrupt-driven encoders + RPM
│   │   ├── imu.h        # MPU6050: rollover/lift/tilt detection
│   │   ├── leds.h       # 8× WS2812B: lights, turn signals, alerts
│   │   ├── buzzer.h     # Situation tones
│   │   ├── battery.h    # LiPo 3S monitoring
│   │   └── webui.h      # Embedded web interface
│   └── src/main.cpp     # Dual-core: web (core 0) + control (core 1)
├── camera-fpv/          # Freenove ESP32-S3-WROOM CAM firmware
│   └── src/main.cpp     # MJPEG stream at 192.168.4.2:81/stream
└── docs/                # Design documentation
```

## Phase 1 — What this firmware does

- **Control from your phone:** the ESP32 creates the `Rover-4WD` WiFi network. Connect and open `http://192.168.4.1` — virtual joystick + FPV video + telemetry on a single page
- **FPV video:** the ESP32-S3 CAM connects to the rover by itself and streams VGA at ~20fps
- **Safety:** failsafe (no signal for 600ms → stop), motor cutoff on rollover or lift detection (IMU), low battery warning
- **Automotive lighting:** headlights (toggle), tail lights based on movement, turn signals when steering, white reverse lights, blinking red alert on rollover
- **Buzzer:** low tone = rollover, intermittent high tone = lifted, melody = "find me" button, beep = low battery
- **Telemetry:** battery voltage/%, speed in m/s, average RPM, distance traveled, pitch/roll

## Setup

### Requirements
- [PlatformIO](https://platformio.org/) (CLI or VS Code extension)

### Configure credentials (first time)
```bash
# In rover-base/ AND in camera-fpv/, copy the template:
cp secrets.ini.example secrets.ini
# Edit secrets.ini with your SSID/password (same values in both)
# secrets.ini is in .gitignore: it is never pushed to the repo
```

### Flash the rover
```bash
cd rover-base
pio run -t upload          # ESP32 connected over USB
pio device monitor         # watch logs
```

### Flash the camera
```bash
cd camera-fpv
pio run -t upload          # Freenove ESP32-S3 over native USB
```

> The SSID/password is defined once per project in `secrets.ini`. Use the same values in rover-base and camera-fpv.

### Test it
1. Power on the rover (and the camera)
2. On your phone: WiFi → `Rover-4WD` (the password from your secrets.ini)
3. Browser → `http://192.168.4.1`

## Rover HTTP endpoints

| Route | Function |
|---|---|
| `/` | Full web interface |
| `/cmd?x=&y=` | Joystick command (-100..100) |
| `/lights` | Headlights toggle |
| `/find` | "Find me" buzzer + flash |
| `/status` | Telemetry JSON |

## Roadmap

- [x] **Phase 1:** base rover + WiFi + FPV *(this code)*
- [ ] **Phase 2:** telemetry module (dedicated ESP32, UART over GX16-10)
- [ ] **Phase 3:** physical RC transmitter (NRF24L01, ~1km)

## Hardware

See `CLAUDE.md` for the full pin map and `docs/` for the mechanical design.
