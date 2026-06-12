# 🤖 4WD Rover — Project Context Document

> **Status:** 3D design and component acquisition phase
> **Last updated:** June 2026
> **Scale:** 1/16
> **Suspension:** Double articulation with differential
> **Control:** WiFi AP (web interface from phone)
> **Architecture:** Modular — rover base + interchangeable roof modules

---

## 1. Overview

4WD exploration rover with double-articulation suspension (rocker + lower arm + shock absorber + differential bar). The hexagonal chassis is controlled over WiFi from a phone, with an onboard FPV camera for driving without line of sight.

**Modular architecture with distributed processing:** each module (telemetry, robotic arm, etc.) carries its own ESP32 that processes its sensors locally. Modules attach to the rover through a standard GX16-10 connector and communicate with the main ESP32 using a UART message protocol. The rover's ESP32 only drives — it never runs module logic.

### Project phases

**Phase 1 — Base rover (mechanical build + RC + FPV camera)**
3D-printed hexagonal chassis, double-articulation suspension, 4 JGA25-370 motors with encoder, 2× BTS7960, main ESP32, ESP32-CAM (independent FPV camera), LM2596 regulator, LiPo 3S battery, WS2812B lighting system (8 LEDs), passive buzzer, MPU6050 IMU, battery monitoring, NRF24L01+PA+LNA receiver module (installed, active in phase 3), GX16-10 connector on the roof and T-slot 2020 rails for modules. Remote control from a phone over WiFi (web interface with virtual joystick + live video).

**Phase 2 — Telemetry module (its own ESP32)**
Smart module with its own ESP32 that reads and processes: VL53L0X distance sensors, GPS, compass, weather station, air quality. Stores data locally on MicroSD. Connects to the rover via GX16-10 and sends processed data over UART. The rover's ESP32 never touches the module's sensors — it only receives ready-to-use data.

**Phase 3 — Physical RC transmitter (its own ESP32)**
Hand-built RC transmitter using the third ESP32, NRF24L01+PA+LNA transmitter module, 2× analog joysticks, OLED display, function buttons, LiPo 1S battery with USB charging (TP4056), 3D-printed enclosure. Range ~1km. Replaces the phone WiFi control with a dedicated transmitter with real-time feedback.

---

## 2. Available components (already acquired)

| Component | Qty | Notes |
|---|---|---|
| ESP32 DevKit | 2 | Main rover + RC (phase 3). One more needed for the telemetry module (phase 2) |
| ESP32-CAM | 0 | To buy: Freenove ESP32-S3-WROOM CAM (independent FPV camera) |
| OLED/LCD displays | 3 | For the rover, module and/or RC transmitter |
| VL53L0X sensors (laser ToF distance) | Several | For the telemetry module (phase 2) |
| 92mm wheels, 12mm hex | 4 | Final size |
| 3D printer | 1 | For chassis, arms, mounts and module enclosures |

---

## 3. Mechanical design

### 3.1 Hexagonal chassis

Hexagonal shape with arrow-tipped front and rear to maximize the approach angle on rough terrain. 3D printed (PETG).

**Main dimensions:**

| Measurement | Value |
|---|---|
| Chassis length | 200mm |
| Chassis width | 160mm |
| Chassis height | 30mm |
| Total width with wheels | ~260mm |
| Ground clearance | 46mm (= wheel radius, uses 100% of the travel) |
| Wheelbase | 170mm |
| Approach angle | ~20° |
| Total rover height | ~76mm |
| Wall thickness | 2.5–3mm |

**Rover roof:**

- 2× aluminum T-slot 2020 profiles (160mm each) bolted to the roof
- 1× female GX16-10 connector mounted on the roof (accessible from above)
- Modules slide onto the rails and plug into the GX16

**Internal layout (top view, front to back):**

- Front zone: LiPo 3S battery (~105×35×25mm)
- Center zone: ESP32, MPU6050 IMU (exact center), LM2596 buck regulator
- Middle zone: 2× BTS7960 (motor drivers)
- Rear zone: buzzer, switch, XT60 battery connector
- Roof: T-slot rails + GX16-10 connector

**3D printing:**

| Parameter | Value |
|---|---|
| Material | PETG (chassis, arms, mounts) |
| Secondary material | PLA (lids, non-structural enclosures) |
| Infill | 40–60% (structural parts) |
| Threaded inserts | M3 × 5mm brass heat-set (20 pcs) |
| Threaded inserts | M2 × 4mm brass heat-set (10 pcs for modules) |

### 3.2 Double-articulation suspension with differential

> **📄 Detailed diagrams:** see `rover-suspension-system.md`

Passive suspension system with two absorption levels:

1. **Level 1 — Shock absorber (fast impacts):** each wheel has a lower arm that pivots at the end of the upper arm, with an oil-filled shock that absorbs vibration and hits without moving the rocker.

2. **Level 2 — Rocker + differential (large obstacles):** when the shock fully compresses, the force transfers to the upper arm (rocker) and the differential bar levels the chassis.

**System components:**

| Component | Qty | Specification |
|---|---|---|
| Upper arms (rocker) | 2 | PETG printed, 190mm total (95mm per side) |
| Lower arms | 4 | PETG printed, ~80mm, one per wheel |
| Differential bar | 1 | PETG printed, 120mm, connects both rockers |
| Connection links | 2 | PETG printed, 40-50mm |
| 70mm oil-filled shocks | 4 | One per wheel, 20-30wt silicone oil, soft spring |
| KFL08 (flanged bearing block) | 6 (+2 spare) | Doubled: 2× left side pivot + 2× right side pivot + 2× central diff pivot |
| Loose 608ZZ bearings | 4 (+2 spare) | At the upper arm ends (connect to the lower arm) |
| Stainless steel shafts 8mm × 50-65mm | 2 | Side pivots (length depends on wall thickness + 2× KFL08) |
| Stainless steel shaft 8mm × 30mm | 1 | Central differential pivot |
| Stainless steel shafts 8mm × 25mm | 4 | Lower arm pivots |
| 8mm E-clips / retaining rings | 20 pack | Shaft retention in end 608ZZ bearings |
| Motor mounts | 4 | PETG printed, U-shaped clamp Ø25mm |
| M5 × 20mm bolts + nyloc nuts | 4+4 | Link pins |
| M5 washers | 8 | Reduce friction in the links |
| Articulation stops | 4 | Integrated into the chassis, limit to ±25° |

**Operating principle:**

Fast impact (vibration, small rock):
→ Lower arm rotates, shock compresses → the chassis never notices

Large obstacle (rock, slope):
→ Shock fully compresses → force transfers to the upper arm
→ Rocker articulates → differential bar transfers to the other side → chassis levels out

**Capabilities:**

| Parameter | Value |
|---|---|
| Max angle per rocker arm | ±25° |
| Rocker travel | ±40mm vertical |
| Shock travel | ~10mm compression |
| Combined travel per wheel | ~50mm total |
| Max step obstacle | ~46mm |
| Max rounded rock obstacle | ~60mm |
| Max slope (compact dirt) | ~25-30° |
| Max chassis tilt | ±12.5° |

**Heights from the ground:**

| Reference | Height |
|---|---|
| Ground | 0mm |
| Chassis bottom (clearance) | 46mm |
| Motor shaft / wheel center | 46mm |
| End bearing (upper arm) | 66mm |
| Main KFL08 pivot | 66mm |
| Chassis roof | 76mm |

### 3.3 Wheels and traction (final)

| Specification | Value |
|---|---|
| Wheel diameter | 92mm |
| Coupling hex | 12mm |
| Rubber type | Soft, aggressive lugs (crawler-style) |
| Circumference | ~289mm (distance per revolution) |
| Steering | Skid steer (tank-style) |

### 3.4 T-slot 2020 rails for modules

2 aluminum T-slot 2020 profiles, 160mm each, bolted to the chassis roof. Modules mount by sliding T-nuts into the slots and tightening.

---

## 4. Motors

### JGA25-370 DC 12V 100RPM with integrated encoder

| Specification | Value |
|---|---|
| Model | JGA25-370 with encoder |
| Voltage | 12V DC |
| No-load RPM | 100 RPM |
| Output shaft | 4mm D-shaped |
| Motor diameter | 25mm |
| Total length | ~56mm (includes encoder) |
| Weight | ~100g per unit |
| Gearbox | Fully metal |
| Reduction ratio | ~1:43 |
| Estimated torque | ~5.3 kg·cm |
| Encoder | Incremental AB type, 11 PPR, Hall |
| Quantity | 4 units |

**Motor wiring (6 wires):**

| Wire | Color | Function |
|---|---|---|
| Motor + | Red | Motor power (to the BTS7960) |
| Motor − | Black | Motor power (to the BTS7960) |
| Encoder GND | Green | Encoder ground |
| Encoder VCC | Blue | 5V for the encoder (from the LM2596) |
| Channel A | Yellow | Pulse signal A (to the ESP32, interrupt-driven) |
| Channel B | White | Not connected (direction is known from the BTS7960) |

**Coupling:** 4mm → 12mm hex coupler with set screw.

**Electrical connection (skid steer):**

- BTS7960 #1 → M1 (front left) + M3 (rear left) in parallel
- BTS7960 #2 → M2 (front right) + M4 (rear right) in parallel
- Encoders: read individually (4 GPIO pins, one per motor)

**Performance with 92mm wheels:**

| Parameter | Value |
|---|---|
| Force per motor | 1.15 kg |
| Total force (4 motors) | 4.6 kg |
| Max speed (no load) | 0.48 m/s = 1.73 km/h |
| Speed on rough terrain | ~0.31 m/s = 1.13 km/h |
| Distance per encoder pulse | ~0.67mm |

### Estimated current draw (phase 1)

| Scenario | Motors (×4) | Base electronics | Total |
|---|---|---|---|
| Smooth floor | ~0.4A | ~0.4A | ~0.8A |
| Dirt/gravel | ~1.2A | ~0.4A | ~1.6A |
| Rocks/slope | ~2.0A | ~0.4A | ~2.4A |

---

## 5. Power and electrical protection

### 5.1 Main battery

| Specification | Value |
|---|---|
| Type | LiPo 3S |
| Nominal voltage | 11.1V (12.6V charged) |
| Capacity | 2200mAh |
| Discharge rate | 30C |
| Connector | XT60 |
| Dimensions | ~102×34×23mm |
| Weight | ~170g |
| Quantity | 2 (pack, one spare) |

**Estimated runtime (phase 1, no modules):**

| Terrain | Runtime |
|---|---|
| Smooth floor (~0.8A) | ~2h 45min |
| Dirt/gravel (~1.6A) | ~1h 22min |
| Rocks/slope (~2.4A) | ~55min |

### 5.2 Voltage regulation

**Power flow:**

```
Battery 11.1V → 10A fuse → Switch → 1N5822 Schottky diode
    │
    ├──→ LM2596 → 5V → ESP32 (VIN) → 3.3V (sensors)
    │                  → Encoders (5V)
    │                  → WS2812B LEDs (5V)
    │                  → GX16 pins 2,3,4 (5V + GND)
    │
    ├──→ BTS7960 → Motors (12V direct)
    │
    └──→ GX16 pin 1 (12V direct for modules)
```

### 5.3 Battery monitoring

Voltage divider: R1=100KΩ + R2=27KΩ → GPIO 35 (ADC1). Max at the ADC: ~2.68V.

### 5.4 Protections

| Component | Function | Qty |
|---|---|---|
| 10A fuse + fuse holder | Short circuit | 1 |
| 1N5822 Schottky diode | Reverse polarity | 1 |
| 1000µF 25V capacitor | LM2596 stabilization | 2 |
| 100nF ceramic capacitor | Driver/motor decoupling | 6 |
| LiPo 2S-3S alarm | Low battery warning | 1 |
| LiPo fireproof bag | Charging/storage safety | 1 |

---

## 6. Electronics and control (Phase 1 — base rover)

### 6.1 ESP32 — resource usage

| Resource | Used | Available | Status |
|---|---|---|---|
| Total GPIO | ~27 | ~0 free | ⚠️ At the limit |
| I2C buses | 1 | 2 | ✅ |
| SPI buses | 1 (NRF24L01) | 2 | ✅ |
| UART ports | 1 (GX16) | 3 | ✅ |
| ADC1 channels | 2 | 6 | ✅ |
| PWM channels | 5 | 16 | ✅ |
| Interrupts | 4 (encoders) | — | ✅ |

**Task distribution per core:**

```
Core 0 (PRO_CPU):
  ├── WiFi AP management (phase 1: control from phone)
  ├── Web server (virtual joystick, telemetry)
  ├── NRF24L01 reception (phase 3: control from physical RC)
  └── Communication with phone / RC

Core 1 (APP_CPU):
  ├── Encoder reading (interrupts, RPM calculation)
  ├── Per-side speed PID control
  ├── Motor control (PWM to BTS7960)
  ├── Rollover/lift detection (IMU)
  ├── WS2812B LED control
  ├── Battery monitoring
  └── OLED display updates

Interrupts (ISR):
  ├── Encoder M1 → GPIO 4
  ├── Encoder M2 → GPIO 0
  ├── Encoder M3 → GPIO 36 (VP)
  └── Encoder M4 → GPIO 39 (VN)
```

### 6.2 Drivers — 2× BTS7960 (43A)

- BTS #1: M1 (front left) + M3 (rear left) in parallel
- BTS #2: M2 (front right) + M4 (rear right) in parallel

### 6.3 Remote control

**Phase 1 — WiFi AP (control from phone):**
The ESP32 creates its own WiFi network. The phone connects, opens a browser, web interface with virtual joystick. No apps. Range ~50-100m.

**Phase 3 — NRF24L01+PA+LNA (control from physical RC):**
The receiver module is installed on the rover since phase 1. When the RC is built in phase 3, only the reception code needs to be enabled. Range ~1km with the PA+LNA antenna. WiFi can stay active simultaneously for web monitoring.

### 6.4 NRF24L01+PA+LNA (receiver on the rover)

| Specification | Value |
|---|---|
| Module | NRF24L01+PA+LNA (with external antenna) |
| Interface | SPI |
| Range | ~1km open field (with PA+LNA) |
| Voltage | 3.3V (power from the ESP32's 3.3V, NOT from 5V) |
| Current | ~115mA while transmitting |
| Phase 1 status | Installed, wired, inactive in software |
| Phase 3 status | Active, receives commands from the RC transmitter |

**⚠️ Important:** the NRF24L01+PA+LNA is sensitive to electrical noise. Solder a 10µF capacitor between VCC and GND directly on the module pins to avoid resets.

### 6.5 FPV camera — Freenove ESP32-S3-WROOM CAM

Independent board with an ESP32-S3 (more powerful than the original ESP32), OV2640 camera, 8MB PSRAM and native USB for programming. It uses no GPIO pins on the main ESP32 — it only needs 5V and GND from the LM2596.

| Specification | Value |
|---|---|
| Board | Freenove ESP32-S3-WROOM CAM |
| Processor | ESP32-S3 dual-core 240MHz |
| PSRAM | 8MB (double the original ESP32-CAM) |
| Sensor | OV2640 (2MP, the most stable and well documented) |
| FPV resolution | VGA 640×480 at 20-25 fps |
| Latency | ~100ms (half of the original ESP32-CAM) |
| Connection | Joins the rover's WiFi AP as a client |
| Power | 5V + GND from the LM2596 (~200-300mA) |
| Programming | Native USB (no FTDI adapter needed) |
| MicroSD | Built-in slot (can record photos/video locally) |
| Main ESP32 pins used | 0 (fully independent) |
| Mounting | Front of the chassis, bracket tilted ~15° downward |
| Price | ~$12-15 |

**Why the ESP32-S3 and not the basic ESP32-CAM?** Double the PSRAM (8MB vs 4MB) and the S3 processor allow 20-25 fps in VGA with ~100ms latency, vs 12-15 fps with ~200ms on the basic one. For FPV driving, that difference separates a smooth experience from a frustrating one.

**Why separate from the main ESP32?** Video streaming consumes resources unpredictably. If the camera freezes, the rover keeps driving normally. If both ran on the same ESP32, a video spike could leave the motors unresponsive for 200ms — enough for a crash.

**Network operation:** the main ESP32 creates the WiFi AP ("Rover-4WD"). The ESP32-S3 CAM connects to that network as a client. The phone connects to the same network and a single web page shows the joystick (served by the main ESP32) and the video (served by the ESP32-S3 CAM).

### 6.6 IMU — MPU6050

Exact center of the chassis. Detects rollover (inverted Z axis), lift (Z gravity changes), slope vs wall (pitch + distance).

### 6.7 Passive buzzer (PWM)

| Situation | Tone |
|---|---|
| Rollover | Continuous low tone |
| Lifted | Intermittent high tone |
| Find button (WiFi) | Fast beep melody |
| Low battery | Short tone every 10s |

### 6.8 WS2812B LEDs (×8) — GPIO 2

| LED # | Position | Function |
|---|---|---|
| 0 | Front left | Light + left turn signal |
| 1, 2 | Front center | Headlights (white) |
| 3 | Front right | Light + right turn signal |
| 4 | Rear left | Light + left turn signal |
| 5, 6 | Rear center | Lights/brake (red) |
| 7 | Rear right | Light + right turn signal |

Auto on/off with an LTR390 sensor (phase 2 module) or manual from the web interface.

---

## 7. Standard module connector — GX16-10

Aviation connector mounted on the chassis roof (female). Modules carry the male side. Threaded locking ring.

### Pinout

| Pin | Signal | Type | Function |
|---|---|---|---|
| 1 | 12V | Power | Direct from battery |
| 2 | 5V | Power | Regulated from the LM2596 |
| 3 | GND | Power | Common ground |
| 4 | GND | Power | Doubled ground (more current) |
| 5 | SDA | I2C data | GPIO 21 |
| 6 | SCL | I2C clock | GPIO 22 |
| 7 | TX | UART | GPIO 17 → module |
| 8 | RX | UART | GPIO 16 ← module |
| 9 | GPIO A | Digital | Free |
| 10 | GPIO B | Digital | Free |

### Rover ↔ module communication (UART protocol)

Each module carries its own ESP32. The rover does not process module logic — it only receives processed data and sends commands.

**Message protocol (UART, pins 7-8, 115200 baud):**

Module → Rover (processed data):
- Obstacle distances (6 VL53L0X already processed, in cm)
- GPS position (lat, lon, alt, speed)
- Compass heading (degrees)
- Weather data (temp, humidity, pressure, UV, rain, PM2.5)
- Module status (OK, error, low battery)

Rover → Module (commands):
- Request specific data
- Change sampling rate
- Start/stop SD logging
- Low power mode

**Suggested format:** JSON per line or a lightweight binary protocol (to be defined during implementation).

**Advantages of this architecture:**

- The rover's ESP32 is never overloaded with sensors
- You can unplug a module and the rover keeps working perfectly
- Each module can be updated/reflashed independently
- Modules can be developed and tested on the bench without the rover
- Future modules (arm, 360 camera, LIDAR) only need to follow the protocol

---

## 8. Telemetry module (Phase 2) — dedicated ESP32

3D-printed enclosure mounted on the roof T-slot rails. Plugs into the GX16-10. Carries its own ESP32 that processes all sensors locally and sends processed data to the rover over UART. The rover never touches the sensors directly.

**Module processor:** ESP32 DevKit (the project's third one)

**Module responsibilities (autonomous):**
- Read the 6 VL53L0X every 50ms via TCA9548A
- Read GPS, compass, BME280, LTR390 every 1-5 seconds
- Read air quality (PMS5003) every 5 seconds
- Log everything to MicroSD with timestamps
- Send processed data to the rover over UART (GX16 pins 7-8)
- Receive commands from the rover (change rate, start/stop logging)

**What the module does NOT do:** control motors, LEDs, buzzer, or any base rover function.

### 8.1 Distance sensors — VL53L0X (×6)

| Sensor | Position | Angle |
|---|---|---|
| V1 | Front center | 0° |
| V2 | Front left | ~45° |
| V3 | Front right | ~45° |
| V4 | Rear center | 180° |
| V5 | Left side | 90° |
| V6 | Right side | 270° |

Mounting: ~65mm from the ground, +3° upward tilt. They ignore obstacles <60mm.
Connected via TCA9548A (channels 0-5, 2 left free).

### 8.2 Navigation

| Sensor | Friendly name | Interface |
|---|---|---|
| GPS NEO-6M | Satellite navigation | UART |
| QMC5883L | Digital compass | I2C |

### 8.3 Weather station

| Sensor | Friendly name | Measures | Interface |
|---|---|---|---|
| BME280 | Thermometer + humidity + pressure | Temp, hum, pressure, altitude | I2C |
| LTR390 | UV + light sensor | UV, luminosity | I2C |
| Rain drop | Rain detector | Rain yes/no | Analog |
| PMS5003 | Air quality | PM2.5 particles | UART |

### 8.4 Storage

MicroSD module (SPI) + 8-16GB FAT32 card.

### 8.5 Sensor exposure

| Sensor | Exposure | Mounting |
|---|---|---|
| VL53L0X × 6 | 🔴 Exposed | Module edges, ~65mm from the ground |
| BME280 | 🔴 Exposed | Top, in airflow |
| LTR390 | 🔴 Exposed | Top, facing the sky |
| Rain drop | 🔴 Exposed | Top, horizontal |
| GPS antenna | 🔴 Exposed | Highest point of the module |
| PMS5003 | 🔴 Exposed | Side, 2 ventilation openings |
| QMC5883L | 🟢 Inside | Away from motors |
| TCA9548A | 🟢 Inside | I2C wires only |
| MicroSD | 🟢 Inside | Accessible to remove the card |

---

## 9. ESP32 pin map (Phase 1)

### I2C (shared + GX16)

| GPIO | Devices |
|---|---|
| GPIO 21 (SDA) | MPU6050, OLED → GX16 pin 5 |
| GPIO 22 (SCL) | Shared → GX16 pin 6 |

### Motors (BTS7960)

| Signal | GPIO |
|---|---|
| BTS #1 RPWM | GPIO 25 |
| BTS #1 LPWM | GPIO 33 |
| BTS #1 EN | GPIO 32 |
| BTS #2 RPWM | GPIO 14 |
| BTS #2 LPWM | GPIO 12 |
| BTS #2 EN | GPIO 13 |

### Encoders (channel A, interrupt-driven)

| Motor | GPIO |
|---|---|
| M1 front left | GPIO 4 |
| M2 front right | GPIO 0 |
| M3 rear left | GPIO 36 (VP) |
| M4 rear right | GPIO 39 (VN) |

### Individual pins

| Signal | GPIO | Type |
|---|---|---|
| WS2812B LEDs | GPIO 2 | PWM (8 LEDs chained) |
| Passive buzzer | GPIO 15 | PWM |
| Battery voltage | GPIO 35 | ADC1 (100K/27K divider) |
| Rain sensor (if on base) | GPIO 34 | ADC1 |

### UART for GX16

| TX | RX | Use |
|---|---|---|
| GPIO 17 | GPIO 16 | GX16 pins 7-8 (modules) |

### SPI — NRF24L01+PA+LNA (RC receiver)

| Signal | GPIO | Note |
|---|---|---|
| MOSI | GPIO 23 | Standard SPI |
| MISO | GPIO 19 | Standard SPI |
| SCK | GPIO 18 | Standard SPI |
| CSN | GPIO 5 | Chip select |
| CE | GPIO 27 | Chip enable |

**Note:** the NRF24 is installed and wired in phase 1 but the reception code is enabled in phase 3. The SPI pins do not interfere with WiFi (they use separate hardware).

**⚠️ ADC2 does NOT work with WiFi. Use ADC1 only (GPIO 32-39).**
**⚠️ GPIO 0: do not hold LOW during boot.**
**⚠️ NRF24L01: power at 3.3V (NOT 5V). Solder a 10µF capacitor between the module's VCC and GND.**

---

## 10. Software

### Phase 1 — Base rover

| Library | Function |
|---|---|
| WiFi.h | AP mode |
| WebServer.h | Web control interface |
| Wire.h | I2C |
| SPI.h | SPI (NRF24L01) |
| RF24 (nRF24/RF24) | Communication with the RC transmitter (active in phase 3) |
| Adafruit_NeoPixel | WS2812B LEDs |
| MPU6050 (jrowberg) | IMU |
| ESP32Encoder | Encoders |
| PID_v1 | Speed PID control |

### Phase 2 — Telemetry module

| Library | Function |
|---|---|
| Adafruit_VL53L0X | Distance |
| Adafruit_BME280 | Weather |
| Adafruit_LTR390 | UV + light |
| TinyGPS++ | GPS |
| QMC5883LCompass | Compass |
| Adafruit_TCA9548A | I2C multiplexer |
| SD.h + SPI.h | MicroSD logging |

### Phase 3 — RC transmitter

| Library | Function |
|---|---|
| RF24 (nRF24/RF24) | Communication with the rover |
| SPI.h | SPI (NRF24L01) |
| Adafruit_SSD1306 | Transmitter OLED display |
| Wire.h | I2C (display) |

---

## 11. Shopping list

### Phase 1 — Base rover

**Structure and suspension:**

| Component | Qty | Notes |
|---|---|---|
| PETG filament 1kg | 1 | Chassis, arms, mounts |
| PLA filament 1kg | 1 | Lids, enclosures (optional) |
| M3 × 5mm brass heat-set inserts | 20 | Structural joints |
| M2 × 4mm brass heat-set inserts | 10 | Electronics modules |
| KFL08 (8mm flanged bearing block) | 8 | 6 pivots + 2 spare |
| Loose 608ZZ bearings | 6 | 4 ends + 2 spare |
| Stainless steel shafts 8mm × 50-65mm | 2 | Side pivots |
| Stainless steel shaft 8mm × 30mm | 1 | Differential pivot |
| Stainless steel shafts 8mm × 25mm | 4 | Lower arm pivots |
| 8mm retaining E-clips | 20 pack | Shaft retention |
| 70mm oil-filled shocks (set of 4) | 1 set | INJORA or similar |
| 20-30wt silicone oil | 1 | For the shocks |
| M5 × 20mm stainless bolts | 4 | Link pins |
| M5 nyloc nuts | 4 | Anti-vibration |
| M5 washers | 8 | Link friction |
| Assorted M3 screw kit | 1 | ~200 pcs |
| Assorted M2 screw kit | 1 | Modules |
| M3 hex standoffs | 1 pack | PCBs |
| T-slot 2020 profile (300mm) | 2 | Cut to 160mm |
| M3 T-nuts | 10 | Module mounting |
| M5 T-nuts | 10 | Heavy modules |
| Blue Loctite | 1 | All suspension hardware |

**Motors:**

| Component | Qty | Notes |
|---|---|---|
| JGA25-370 12V 100RPM with encoder | 4 | 4mm D shaft, 11 PPR encoder |
| 4mm → 12mm hex coupler | 4 | With set screw |

**Power:**

| Component | Qty | Notes |
|---|---|---|
| LiPo 3S 2200mAh 30C XT60 battery | 2 | Pack |
| LiPo charger/balancer | 1 | IMAX B6 or similar |
| LiPo voltage alarm | 1 | Standalone |
| LiPo fireproof bag | 1 | Safety |
| Switch ≥10A | 1 | Main cutoff |
| XT60 connectors (pairs) | 2 | Battery/charging |
| LM2596 buck regulator | 1 | 12V → 5V |
| 16-18 AWG silicone wire (red+black) | 1m each | Power |
| 22-24 AWG multicolor wire pack | 1 | 6 colors × 2m |
| 1000µF 25V capacitor | 2 | LM2596 |
| 100nF ceramic capacitor | 6 | Decoupling |
| 1N5822 Schottky diode | 1 | Polarity |
| 10A fuse + holder | 1 | Short circuit |

**Electronics:**

| Component | Qty | Notes |
|---|---|---|
| BTS7960 43A | 2 | Motor drivers |
| MPU6050 | 1 | IMU, chassis center |
| Passive buzzer 5V | 1 | PWM |
| WS2812B strip (30 LEDs/m) | 1 | Cut 8 LEDs |
| 1000µF 6.3V capacitor | 1 | LED power |
| 330Ω resistor | 1 | LED data |
| 100KΩ resistor | 1 | Battery divider |
| 27KΩ resistor | 1 | Battery divider |
| 4.7KΩ resistor | 4 | Spare I2C pull-ups |
| SSD1306 I2C OLED display | 1 | Data display |
| NRF24L01+PA+LNA (with antenna) | 1 | RC receiver, installed in phase 1, active in phase 3 |
| 10µF electrolytic capacitor | 1 | Solder on the NRF24's VCC/GND pins (stability) |
| Freenove ESP32-S3-WROOM CAM (OV2640) | 1 | Independent FPV camera, native USB, 8MB PSRAM |
| IPEX WiFi antenna for ESP32-S3 CAM | 1 | Improves video signal (optional but recommended) |

**Module connector:**

| Component | Qty | Notes |
|---|---|---|
| GX16-10 female (panel mount) | 1 | Chassis roof |
| GX16-10 male (cable) | 2 | One per module |

**Wiring:**

| Component | Qty | Notes |
|---|---|---|
| Assorted Dupont wires | 1 pack | Prototyping |
| JST-XH 2.54mm connectors | 1 pack | Detachable wiring |
| JST-PH 2.0mm connectors | 1 pack | Sensors |
| Pin headers | 1 pack | For soldering |
| 7×5cm perfboard | 2 | Circuits |
| Heat-shrink tubing | 1 pack | Various diameters |
| Zip ties | 1 pack | Cables |
| 3M foam tape | 1 | Battery mounting |

### Phase 2 — Telemetry module

| Component | Qty | Notes |
|---|---|---|
| TCA9548A | 1 | I2C multiplexer |
| GPS NEO-6M with antenna | 1 | Navigation |
| QMC5883L | 1 | Compass, away from motors |
| BME280 | 1 | Weather |
| LTR390 | 1 | UV + light |
| Rain sensor | 1 | Analog |
| PMS5003 | 1 | Air quality (optional) |
| MicroSD SPI module | 1 | Logging |
| MicroSD 8-16GB FAT32 | 1 | SanDisk/Kingston |
| ESP32 DevKit | 1 | The module's dedicated processor (the project's third one) |

### Phase 3 — Physical RC transmitter

| Component | Qty | Notes |
|---|---|---|
| NRF24L01+PA+LNA (with antenna) | 1 | Transmitter for the controller |
| 10µF electrolytic capacitor | 1 | NRF24 stability on the controller |
| PS2-style analog joysticks | 2 | Steering and speed control |
| Buttons/push buttons | 4-6 | Functions: lights, find buzzer, autonomous mode, etc. |
| 0.96" SSD1306 I2C OLED display | 1 | Feedback: rover battery, signal, speed, telemetry |
| LiPo 1S 3.7V battery (~1000mAh) | 1 | Controller power |
| TP4056 module (USB charging) | 1 | USB-C or micro-USB charging for the controller battery |
| Mini switch | 1 | Controller power switch |
| 5×7cm perfboard | 1 | Controller circuit |
| PETG filament | (same spool) | 3D-printed enclosure |

**Note:** the second ESP32 (already acquired) is used for the RC transmitter. The controller sends commands over NRF24 and receives rover telemetry (battery, speed, signal, IMU status) on the OLED display.

### Future upgrades

| Component | Qty | Notes |
|---|---|---|
| PCF8574 I2C GPIO expander | 1 | 8 extra GPIOs if needed |
| LoRa SX1278 module | 2 | Telemetry beyond 5km (complements NRF24) |
| Robotic arm (module with its own ESP32) | 1 | Plugs into the GX16-10 |

### Tools

| Tool | Notes |
|---|---|
| Adjustable 60W soldering iron | Pinecil, TS101 |
| 0.8mm solder with flux | Lead-free |
| Digital multimeter | — |
| Metric Allen keys | 1.5–6mm |
| Wire stripper 16-26 AWG | — |
| Diagonal cutters | Flush cutters |
| Precision screwdrivers | Phillips + flat |
| Helping hands with magnifier | — |
| Heat gun | Inserts + heat-shrink |
| Digital caliper | Measure shafts/parts |
| Anti-static tweezers | — |
| Silicone soldering mat | — |
| E-clip pliers | Optional |

### Consumables

| Consumable | Notes |
|---|---|
| Flux paste | Soldering |
| Desoldering braid | Corrections |
| Isopropyl alcohol 90%+ | Cleaning |
| Cyanoacrylate glue | Joint reinforcement |
| Blue Loctite | Screws under vibration |

---

## 12. Design decisions

| Decision | Rationale |
|---|---|
| Distributed processing (one ESP32 per module) | Isolates each module's logic, the rover is never overloaded, modules are developed independently |
| UART communication (not direct I2C) | The module sends processed data, not raw readings. The rover doesn't need to know the sensors |
| Separate ESP32-S3 CAM (not integrated into the main ESP32) | Video consumes CPU unpredictably. If the camera freezes, the rover keeps driving. $5 extra for total reliability |
| Freenove ESP32-S3 instead of the basic ESP32-CAM | Double PSRAM (8MB), 20-25fps vs 12-15fps in VGA, ~100ms latency vs ~200ms. Native USB, no adapter |
| Modular architecture (base + GX16 modules) | Simple phase 1, interchangeable modules per mission |
| GX16-10 instead of USB-C | 10 pins, rugged, anti-vibration threaded ring, 5A/pin |
| Double articulation instead of simple rocker | Shock absorbs vibration, rocker handles large obstacles |
| Doubled KFL08 instead of bearings in plastic | Metal carries the load, self-aligning |
| E-clips instead of shaft collars | They don't loosen, minimal space |
| 92mm wheels (final) | Balance between climbing (~60mm) and torque (4.6 kg) |
| 100RPM motor instead of 150/200RPM | More torque with larger wheels. 0.48 m/s is enough |
| Brushed instead of brushless | 10× cheaper, simple control, native reverse |
| WiFi AP instead of Bluetooth | No apps, web interface, longer range |
| VL53L0X on the module (not the chassis) | Interchangeable, the base rover works without them |
| T-slot 2020 for modules | Maker standard, cheap, compatible ecosystem |
| Ground clearance = wheel radius (46mm) | Uses 100% of the suspension travel |
| Sensors at 65mm with +3° | They ignore climbable obstacles <60mm |
| Encoder channel A only | Saves 4 pins, direction from the BTS7960 |

---

## 13. Important notes

**Mechanics:**
- The 3 main pivot shafts must be parallel. A skewed shaft = jammed system
- Lower arm shafts (25mm): secure with E-clips
- Blue Loctite on KFL08 set screws and link bolts
- Shocks: 20-30wt oil. If the rocker doesn't articulate, try without the spring
- Motor/encoder cables: +50mm slack for the ±25° articulation

**Electronics:**
- Test each motor individually before assembly
- GPIO 0 (encoder M2): never LOW during boot
- WS2812B LEDs: 330Ω resistor on data, power at 5V
- QMC5883L (phase 2): as far from the motors as possible
- ADC2 does not work with WiFi. ADC1 only

**3D printing:**
- Chassis: upside down
- Rocker arms: horizontal (layers resist bending)
- Pivot zones: 4mm walls, 60% infill
- Ø22mm tolerance: +0.15mm press fit
- Lower arms: vertical, bearing at the top
