# 🤖 Rover 4WD — Documento de Contexto del Proyecto

> **Estado:** Fase de diseño 3D y adquisición de componentes
> **Última actualización:** Junio 2026
> **Escala:** 1/16
> **Suspensión:** Doble articulación con diferencial
> **Control:** WiFi AP (interfaz web desde móvil)
> **Arquitectura:** Modular — base rover + módulos intercambiables en el techo

---

## 1. Visión general

Rover de exploración 4WD con suspensión de doble articulación (rocker + brazo inferior + amortiguador + barra diferencial). El chasis hexagonal se controla vía WiFi desde el móvil con cámara FPV a bordo para conducción sin línea de vista.

**Arquitectura modular con procesamiento distribuido:** cada módulo (telemetría, brazo robótico, etc.) lleva su propia ESP32 que procesa sus sensores localmente. Los módulos se conectan al rover mediante un conector estándar GX16-10 y se comunican con la ESP32 principal usando un protocolo de mensajes por UART. La ESP32 del rover solo se encarga de conducir — nunca procesa lógica de módulos.

### Fases del proyecto

**Fase 1 — Rover base (construcción mecánica + RC + cámara FPV)**
Chasis hexagonal impreso en 3D, suspensión de doble articulación, 4 motores JGA25-370 con encoder, 2× BTS7960, ESP32 principal, ESP32-CAM (cámara FPV independiente), regulador LM2596, batería LiPo 3S, sistema de iluminación WS2812B (8 LEDs), buzzer pasivo, IMU MPU6050, monitoreo de batería, módulo receptor NRF24L01+PA+LNA (instalado, activo en fase 3), conector GX16-10 en el techo y rieles T-slot 2020 para módulos. Control remoto desde el móvil vía WiFi (interfaz web con joystick virtual + video en vivo).

**Fase 2 — Módulo de telemetría (ESP32 propia)**
Módulo inteligente con su propia ESP32 que lee y procesa: sensores de distancia VL53L0X, GPS, brújula, estación meteorológica, calidad del aire. Almacena datos en MicroSD localmente. Se conecta al rover por GX16-10 y envía datos procesados por UART. La ESP32 del rover no toca ningún sensor del módulo — solo recibe datos ya listos.

**Fase 3 — Mando RC físico (ESP32 propia)**
Mando RC fabricado a mano con la tercera ESP32, módulo transmisor NRF24L01+PA+LNA, 2× joysticks analógicos, pantalla OLED, botones de función, batería LiPo 1S con carga USB (TP4056), carcasa impresa en 3D. Alcance ~1km. Reemplaza el control WiFi del móvil por un mando dedicado con feedback en tiempo real.

---

## 2. Componentes disponibles (ya adquiridos)

| Componente | Cantidad | Notas |
|---|---|---|
| ESP32 DevKit | 2 | Rover principal + RC (fase 3). Necesitas comprar 1 más para módulo telemetría (fase 2) |
| ESP32-CAM | 0 | Comprar: Freenove ESP32-S3-WROOM CAM (cámara FPV independiente) |
| Pantallas OLED/LCD | 3 | Para el rover, módulo y/o mando RC |
| Sensores VL53L0X (distancia láser ToF) | Varios | Para el módulo de telemetría (fase 2) |
| Ruedas 92mm hex 12mm | 4 | Tamaño definitivo |
| Impresora 3D | 1 | Para chasis, brazos, soportes y carcasas de módulos |

---

## 3. Diseño mecánico

### 3.1 Chasis hexagonal

Forma hexagonal con punta de flecha frontal y trasera para maximizar el ángulo de ataque en terreno irregular. Impreso en 3D (PETG).

**Cotas principales:**

| Medida | Valor |
|---|---|
| Largo del chasis | 200mm |
| Ancho del chasis | 160mm |
| Alto del chasis | 30mm |
| Ancho total con ruedas | ~260mm |
| Ground clearance | 46mm (= radio de rueda, aprovecha 100% del recorrido) |
| Distancia entre ejes | 170mm |
| Ángulo de ataque | ~20° |
| Altura total del rover | ~76mm |
| Grosor de paredes | 2.5–3mm |

**Techo del rover:**

- 2× perfiles T-slot aluminio 2020 (160mm cada uno) atornillados al techo
- 1× conector hembra GX16-10 montado en el techo (accesible desde arriba)
- Los módulos se deslizan sobre los rieles y se conectan al GX16

**Distribución interna (vista superior, de frente a atrás):**

- Zona frontal: batería LiPo 3S (~105×35×25mm)
- Zona central: ESP32, IMU MPU6050 (centro exacto), regulador buck LM2596
- Zona media: 2× BTS7960 (drivers de motor)
- Zona trasera: buzzer, interruptor, conector XT60 batería
- Techo: rieles T-slot + conector GX16-10

**Impresión 3D:**

| Parámetro | Valor |
|---|---|
| Material | PETG (chasis, brazos, soportes) |
| Material secundario | PLA (tapas, carcasas no estructurales) |
| Infill | 40–60% (piezas estructurales) |
| Insertos roscados | M3 × 5mm latón heat-set (20 uds) |
| Insertos roscados | M2 × 4mm latón heat-set (10 uds para módulos) |

### 3.2 Suspensión de doble articulación con diferencial

> **📄 Diagramas detallados:** ver `rover-suspension-doble-articulacion.html`

Sistema de suspensión pasiva con dos niveles de absorción:

1. **Nivel 1 — Amortiguador (impactos rápidos):** cada rueda tiene un brazo inferior que pivota en el extremo del brazo superior, con un amortiguador de aceite que absorbe vibraciones y golpes sin mover el rocker.

2. **Nivel 2 — Rocker + diferencial (obstáculos grandes):** cuando el shock se comprime al máximo, la fuerza pasa al brazo superior (rocker) y la barra diferencial nivela el chasis.

**Componentes del sistema:**

| Componente | Cantidad | Especificación |
|---|---|---|
| Brazos superiores (rocker) | 2 | Impresos en PETG, 190mm total (95mm por lado) |
| Brazos inferiores | 4 | Impresos en PETG, ~80mm, uno por rueda |
| Barra diferencial | 1 | Impresa en PETG, 120mm, conecta ambos rockers |
| Bielas de conexión (links) | 2 | Impresas en PETG, 40-50mm |
| Amortiguadores 70mm oil-filled | 4 | Uno por rueda, aceite silicona 20-30wt, resorte suave |
| KFL08 (chumacera con rodamiento) | 6 (+2 repuesto) | Dobles: 2× pivote lat izq + 2× pivote lat der + 2× pivote central dif |
| Rodamientos 608ZZ sueltos | 4 (+2 repuesto) | En extremos de los brazos superiores (conectan al brazo inferior) |
| Ejes acero inox 8mm × 50-65mm | 2 | Pivotes laterales (largo según grosor pared + 2× KFL08) |
| Eje acero inox 8mm × 30mm | 1 | Pivote central diferencial |
| Ejes acero inox 8mm × 25mm | 4 | Pivotes brazos inferiores |
| E-clips / anillos retención 8mm | 20 pack | Fijación de ejes en 608ZZ de extremos |
| Soportes de motor | 4 | Impresos en PETG, abrazadera tipo U Ø25mm |
| Tornillos M5 × 20mm + tuercas nyloc | 4+4 | Pasadores de bielas |
| Arandelas M5 | 8 | Reducen fricción en bielas |
| Topes de articulación | 4 | Integrados al chasis, limitan ±25° |

**Principio de funcionamiento:**

Impacto rápido (vibración, piedra pequeña):
→ Brazo inferior gira, amortiguador comprime → chasis no se entera

Obstáculo grande (roca, pendiente):
→ Shock se comprime al máximo → fuerza pasa al brazo superior
→ Rocker articula → barra diferencial transfiere al otro lado → chasis se nivela

**Capacidades:**

| Parámetro | Valor |
|---|---|
| Ángulo máximo por brazo rocker | ±25° |
| Recorrido rocker | ±40mm vertical |
| Recorrido amortiguador | ~10mm compresión |
| Recorrido combinado por rueda | ~50mm total |
| Obstáculo máx escalón | ~46mm |
| Obstáculo máx roca redondeada | ~60mm |
| Pendiente máx (tierra compacta) | ~25-30° |
| Inclinación máx del chasis | ±12.5° |

**Alturas desde el suelo:**

| Referencia | Altura |
|---|---|
| Suelo | 0mm |
| Base del chasis (clearance) | 46mm |
| Centro eje motor / rueda | 46mm |
| Rodamiento extremo (brazo superior) | 66mm |
| Pivote principal KFL08 | 66mm |
| Techo del chasis | 76mm |

### 3.3 Ruedas y tracción (definitivo)

| Especificación | Valor |
|---|---|
| Diámetro de rueda | 92mm |
| Hex de acople | 12mm |
| Tipo de goma | Blanda, lugs agresivos (tipo crawler) |
| Circunferencia | ~289mm (distancia por vuelta) |
| Dirección | Skid steer (tipo tanque) |

### 3.4 Rieles T-slot 2020 para módulos

2 perfiles de aluminio T-slot 2020 de 160mm atornillados al techo del chasis. Los módulos se montan deslizando T-nuts por las ranuras y apretando.

---

## 4. Motores

### JGA25-370 DC 12V 100RPM con encoder integrado

| Especificación | Valor |
|---|---|
| Modelo | JGA25-370 con encoder |
| Voltaje | 12V DC |
| RPM sin carga | 100 RPM |
| Eje de salida | 4mm en forma D |
| Diámetro del motor | 25mm |
| Largo total | ~56mm (incluye encoder) |
| Peso | ~100g por unidad |
| Caja de engranajes | Totalmente metálica |
| Ratio de reducción | ~1:43 |
| Torque estimado | ~5.3 kg·cm |
| Encoder | Incremental tipo AB, 11 PPR, Hall |
| Cantidad | 4 unidades |

**Cableado del motor (6 cables):**

| Cable | Color | Función |
|---|---|---|
| Motor + | Rojo | Alimentación motor (al BTS7960) |
| Motor − | Negro | Alimentación motor (al BTS7960) |
| Encoder GND | Verde | Tierra del encoder |
| Encoder VCC | Azul | 5V para el encoder (desde LM2596) |
| Canal A | Amarillo | Señal de pulsos A (al ESP32, por interrupción) |
| Canal B | Blanco | No conectado (dirección se conoce por BTS7960) |

**Acoplamiento:** Acoplador hex 4mm → 12mm con set screw.

**Conexión eléctrica (skid steer):**

- BTS7960 #1 → M1 (frontal izq) + M3 (trasero izq) en paralelo
- BTS7960 #2 → M2 (frontal der) + M4 (trasero der) en paralelo
- Encoders: lectura individual (4 pines GPIO, uno por motor)

**Rendimiento con ruedas de 92mm:**

| Parámetro | Valor |
|---|---|
| Fuerza por motor | 1.15 kg |
| Fuerza total (4 motores) | 4.6 kg |
| Velocidad máx (sin carga) | 0.48 m/s = 1.73 km/h |
| Velocidad en terreno irregular | ~0.31 m/s = 1.13 km/h |
| Distancia por pulso encoder | ~0.67mm |

### Consumo estimado (fase 1)

| Escenario | Motores (×4) | Electrónica base | Total |
|---|---|---|---|
| Piso liso | ~0.4A | ~0.4A | ~0.8A |
| Tierra/grava | ~1.2A | ~0.4A | ~1.6A |
| Rocas/pendiente | ~2.0A | ~0.4A | ~2.4A |

---

## 5. Alimentación y protección eléctrica

### 5.1 Batería principal

| Especificación | Valor |
|---|---|
| Tipo | LiPo 3S |
| Voltaje nominal | 11.1V (12.6V cargada) |
| Capacidad | 2200mAh |
| Tasa de descarga | 30C |
| Conector | XT60 |
| Dimensiones | ~102×34×23mm |
| Peso | ~170g |
| Cantidad | 2 (pack, una de repuesto) |

**Autonomía estimada (fase 1, sin módulos):**

| Terreno | Autonomía |
|---|---|
| Piso liso (~0.8A) | ~2h 45min |
| Tierra/grava (~1.6A) | ~1h 22min |
| Rocas/pendiente (~2.4A) | ~55min |

### 5.2 Regulación de voltaje

**Flujo de energía:**

```
Batería 11.1V → Fusible 10A → Interruptor → Diodo Schottky 1N5822
    │
    ├──→ LM2596 → 5V → ESP32 (VIN) → 3.3V (sensores)
    │                  → Encoders (5V)
    │                  → LEDs WS2812B (5V)
    │                  → GX16 pines 2,3,4 (5V + GND)
    │
    ├──→ BTS7960 → Motores (12V directo)
    │
    └──→ GX16 pin 1 (12V directo para módulos)
```

### 5.3 Monitoreo de batería

Divisor de voltaje: R1=100KΩ + R2=27KΩ → GPIO 35 (ADC1). Máx al ADC: ~2.68V.

### 5.4 Protecciones

| Componente | Función | Cant. |
|---|---|---|
| Fusible 10A + portafusible | Cortocircuito | 1 |
| Diodo Schottky 1N5822 | Polaridad inversa | 1 |
| Capacitor 1000µF 25V | Estabilización LM2596 | 2 |
| Capacitor 100nF cerámico | Desacoplo drivers/motores | 6 |
| Alarma LiPo 2S-3S | Aviso batería baja | 1 |
| Bolsa ignífuga LiPo | Seguridad carga/almacenamiento | 1 |

---

## 6. Electrónica y control (Fase 1 — rover base)

### 6.1 ESP32 — uso de recursos

| Recurso | Usados | Disponibles | Estado |
|---|---|---|---|
| GPIO totales | ~27 | ~0 libres | ⚠️ Al límite |
| I2C buses | 1 | 2 | ✅ |
| SPI buses | 1 (NRF24L01) | 2 | ✅ |
| UART puertos | 1 (GX16) | 3 | ✅ |
| ADC1 canales | 2 | 6 | ✅ |
| PWM canales | 5 | 16 | ✅ |
| Interrupciones | 4 (encoders) | — | ✅ |

**Distribución de tareas por núcleo:**

```
Núcleo 0 (PRO_CPU):
  ├── WiFi AP management (fase 1: control desde móvil)
  ├── Servidor web (joystick virtual, telemetría)
  ├── NRF24L01 recepción (fase 3: control desde RC físico)
  └── Comunicación con móvil / RC

Núcleo 1 (APP_CPU):
  ├── Lectura de encoders (interrupciones, cálculo RPM)
  ├── Control PID de velocidad por lado
  ├── Control de motores (PWM a BTS7960)
  ├── Detección de vuelco/alzado (IMU)
  ├── Control de LEDs WS2812B
  ├── Monitoreo de batería
  └── Actualización de pantalla OLED

Interrupciones (ISR):
  ├── Encoder M1 → GPIO 4
  ├── Encoder M2 → GPIO 0
  ├── Encoder M3 → GPIO 36 (VP)
  └── Encoder M4 → GPIO 39 (VN)
```

### 6.2 Drivers — 2× BTS7960 (43A)

- BTS #1: M1 (frontal izq) + M3 (trasero izq) en paralelo
- BTS #2: M2 (frontal der) + M4 (trasero der) en paralelo

### 6.3 Control remoto

**Fase 1 — WiFi AP (control desde móvil):**
ESP32 crea red WiFi propia. Móvil se conecta, abre navegador, interfaz web con joystick virtual. Sin apps. Alcance ~50-100m.

**Fase 3 — NRF24L01+PA+LNA (control desde RC físico):**
El módulo receptor ya está instalado en el rover desde la fase 1. Cuando se construya el RC en fase 3, solo se activa el código de recepción. Alcance ~1km con antena PA+LNA. WiFi puede seguir activo simultáneamente para monitoreo web.

### 6.4 NRF24L01+PA+LNA (receptor en rover)

| Especificación | Valor |
|---|---|
| Módulo | NRF24L01+PA+LNA (con antena externa) |
| Interfaz | SPI |
| Alcance | ~1km en abierto (con PA+LNA) |
| Voltaje | 3.3V (alimentar desde 3.3V del ESP32, NO de 5V) |
| Corriente | ~115mA transmitiendo |
| Estado en fase 1 | Instalado, cableado, inactivo en software |
| Estado en fase 3 | Activo, recibe comandos del mando RC |

**⚠️ Importante:** el NRF24L01+PA+LNA es sensible al ruido eléctrico. Soldar un capacitor de 10µF entre VCC y GND directamente en los pines del módulo para evitar resets.

### 6.5 Cámara FPV — Freenove ESP32-S3-WROOM CAM

Placa independiente con ESP32-S3 (más potente que la ESP32 original), cámara OV2640, 8MB PSRAM y USB nativo para programar. No usa pines GPIO de la ESP32 principal — solo necesita 5V y GND del LM2596.

| Especificación | Valor |
|---|---|
| Placa | Freenove ESP32-S3-WROOM CAM |
| Procesador | ESP32-S3 dual-core 240MHz |
| PSRAM | 8MB (doble que la ESP32-CAM original) |
| Sensor | OV2640 (2MP, el más estable y documentado) |
| Resolución para FPV | VGA 640×480 a 20-25 fps |
| Latencia | ~100ms (la mitad que la ESP32-CAM original) |
| Conexión | Se une al WiFi AP del rover como cliente |
| Alimentación | 5V + GND del LM2596 (~200-300mA) |
| Programación | USB nativo (no necesita adaptador FTDI) |
| MicroSD | Slot integrado (puede grabar fotos/video localmente) |
| Pines de la ESP32 principal | 0 (totalmente independiente) |
| Montaje | Parte frontal del chasis, soporte con inclinación ~15° hacia abajo |
| Precio | ~$12-15 |

**¿Por qué la ESP32-S3 y no la ESP32-CAM básica?** El doble de PSRAM (8MB vs 4MB) y el procesador S3 permiten 20-25 fps en VGA con ~100ms de latencia, vs 12-15 fps con ~200ms en la básica. Para conducción FPV esa diferencia es la que separa una experiencia fluida de una frustrante.

**¿Por qué separada de la ESP32 principal?** El stream de video consume muchos recursos de forma impredecible. Si se congela la cámara, el rover sigue conduciendo normalmente. Si estuvieran en la misma ESP32, un pico del video podría dejar los motores sin respuesta por 200ms — suficiente para un choque.

**Funcionamiento en red:** la ESP32 principal crea el WiFi AP ("Rover-4WD"). La ESP32-S3 CAM se conecta a esa red como cliente. El teléfono se conecta a la misma red y en una sola página web ve el joystick (servido por ESP32 principal) y el video (servido por ESP32-S3 CAM).

### 6.6 IMU — MPU6050

Centro exacto del chasis. Detecta vuelco (eje Z invertido), alzado (gravedad Z cambia), pendiente vs pared (pitch + distancia).

### 6.7 Buzzer pasivo (PWM)

| Situación | Tono |
|---|---|
| Vuelco | Grave continuo |
| Alzado | Agudo intermitente |
| Botón encontrar (WiFi) | Melodía beeps rápidos |
| Batería baja | Tono corto cada 10s |

### 6.8 LEDs WS2812B (×8) — GPIO 2

| LED # | Posición | Función |
|---|---|---|
| 0 | Frontal izq | Luz + giro izq |
| 1, 2 | Frontal centro | Luces delanteras (blanco) |
| 3 | Frontal der | Luz + giro der |
| 4 | Trasero izq | Luz + giro izq |
| 5, 6 | Trasero centro | Luces/freno (rojo) |
| 7 | Trasero der | Luz + giro der |

Auto-encendido con sensor LTR390 (módulo fase 2) o manual desde interfaz web.

---

## 7. Conector estándar de módulos — GX16-10

Conector de aviación montado en el techo del chasis (hembra). Módulos llevan macho. Rosca de seguridad.

### Pinout

| Pin | Señal | Tipo | Función |
|---|---|---|---|
| 1 | 12V | Potencia | Directo de batería |
| 2 | 5V | Potencia | Regulada del LM2596 |
| 3 | GND | Potencia | Tierra común |
| 4 | GND | Potencia | Tierra doble (más corriente) |
| 5 | SDA | I2C datos | GPIO 21 |
| 6 | SCL | I2C reloj | GPIO 22 |
| 7 | TX | UART | GPIO 17 → módulo |
| 8 | RX | UART | GPIO 16 ← módulo |
| 9 | GPIO A | Digital | Libre |
| 10 | GPIO B | Digital | Libre |

### Comunicación rover ↔ módulo (protocolo UART)

Cada módulo lleva su propia ESP32. El rover no procesa lógica de módulos — solo recibe datos procesados y envía comandos.

**Protocolo de mensajes (UART, pines 7-8, 115200 baud):**

Módulo → Rover (datos procesados):
- Distancias de obstáculos (6 VL53L0X ya procesados en cm)
- Posición GPS (lat, lon, alt, velocidad)
- Heading de brújula (grados)
- Datos meteorológicos (temp, humedad, presión, UV, lluvia, PM2.5)
- Estado del módulo (OK, error, batería baja)

Rover → Módulo (comandos):
- Solicitar datos específicos
- Cambiar frecuencia de muestreo
- Iniciar/detener logging SD
- Modo de bajo consumo

**Formato sugerido:** JSON por línea o protocolo binario ligero (definir en la implementación).

**Ventajas de esta arquitectura:**

- La ESP32 del rover nunca se sobrecarga con sensores
- Puedes desconectar un módulo y el rover sigue funcionando perfecto
- Cada módulo se puede actualizar/reprogramar independientemente
- Puedes desarrollar y probar módulos en la mesa sin el rover
- Futuros módulos (brazo, cámara 360, LIDAR) solo necesitan cumplir el protocolo

---

## 8. Módulo de telemetría (Fase 2) — ESP32 dedicada

Carcasa impresa en 3D montada en rieles T-slot del techo. Se conecta al GX16-10. Lleva su propia ESP32 que procesa todos los sensores localmente y envía datos procesados al rover por UART. El rover nunca toca los sensores directamente.

**Procesador del módulo:** ESP32 DevKit (la tercera del proyecto)

**Responsabilidades del módulo (autónomas):**
- Leer los 6 VL53L0X cada 50ms vía TCA9548A
- Leer GPS, brújula, BME280, LTR390 cada 1-5 segundos
- Leer calidad del aire (PMS5003) cada 5 segundos
- Registrar todo en MicroSD con timestamp
- Enviar datos procesados al rover por UART (GX16 pines 7-8)
- Recibir comandos del rover (cambiar frecuencia, iniciar/detener log)

**Lo que NO hace el módulo:** controlar motores, LEDs, buzzer, o cualquier función del rover base.

### 8.1 Sensores de distancia — VL53L0X (×6)

| Sensor | Posición | Ángulo |
|---|---|---|
| V1 | Frontal centro | 0° |
| V2 | Frontal izquierdo | ~45° |
| V3 | Frontal derecho | ~45° |
| V4 | Trasero centro | 180° |
| V5 | Lateral izquierdo | 90° |
| V6 | Lateral derecho | 270° |

Montaje: ~65mm del suelo, +3° inclinación arriba. Ignoran obstáculos <60mm.
Conectados vía TCA9548A (canales 0-5, quedan 2 libres).

### 8.2 Navegación

| Sensor | Nombre amigable | Interfaz |
|---|---|---|
| GPS NEO-6M | Navegación por satélite | UART |
| QMC5883L | Brújula digital | I2C |

### 8.3 Estación meteorológica

| Sensor | Nombre amigable | Mide | Interfaz |
|---|---|---|---|
| BME280 | Termómetro + humedad + presión | Temp, hum, pres, altitud | I2C |
| LTR390 | Sensor UV + luz | UV, luminosidad | I2C |
| Rain drop | Detector de lluvia | Lluvia sí/no | Analógico |
| PMS5003 | Calidad del aire | PM2.5 partículas | UART |

### 8.4 Almacenamiento

Módulo MicroSD (SPI) + tarjeta 8-16GB FAT32.

### 8.5 Exposición de sensores

| Sensor | Exposición | Montaje |
|---|---|---|
| VL53L0X × 6 | 🔴 Expuesto | Bordes del módulo, ~65mm del suelo |
| BME280 | 🔴 Expuesto | Parte superior, flujo de aire |
| LTR390 | 🔴 Expuesto | Parte superior, mirando al cielo |
| Rain drop | 🔴 Expuesto | Parte superior, horizontal |
| GPS antena | 🔴 Expuesto | Parte más alta del módulo |
| PMS5003 | 🔴 Expuesto | Lateral, 2 aberturas ventilación |
| QMC5883L | 🟢 Dentro | Lejos de motores |
| TCA9548A | 🟢 Dentro | Solo cables I2C |
| MicroSD | 🟢 Dentro | Accesible para sacar tarjeta |

---

## 9. Mapa de pines ESP32 (Fase 1)

### I2C (compartido + GX16)

| GPIO | Dispositivos |
|---|---|
| GPIO 21 (SDA) | MPU6050, OLED → GX16 pin 5 |
| GPIO 22 (SCL) | Compartido → GX16 pin 6 |

### Motores (BTS7960)

| Señal | GPIO |
|---|---|
| BTS #1 RPWM | GPIO 25 |
| BTS #1 LPWM | GPIO 33 |
| BTS #1 EN | GPIO 32 |
| BTS #2 RPWM | GPIO 14 |
| BTS #2 LPWM | GPIO 12 |
| BTS #2 EN | GPIO 13 |

### Encoders (canal A, interrupción)

| Motor | GPIO |
|---|---|
| M1 frontal izq | GPIO 4 |
| M2 frontal der | GPIO 0 |
| M3 trasero izq | GPIO 36 (VP) |
| M4 trasero der | GPIO 39 (VN) |

### Pines individuales

| Señal | GPIO | Tipo |
|---|---|---|
| LEDs WS2812B | GPIO 2 | PWM (8 LEDs en cadena) |
| Buzzer pasivo | GPIO 15 | PWM |
| Voltaje batería | GPIO 35 | ADC1 (divisor 100K/27K) |
| Sensor lluvia (si en base) | GPIO 34 | ADC1 |

### UART para GX16

| TX | RX | Uso |
|---|---|---|
| GPIO 17 | GPIO 16 | GX16 pines 7-8 (módulos) |

### SPI — NRF24L01+PA+LNA (receptor RC)

| Señal | GPIO | Nota |
|---|---|---|
| MOSI | GPIO 23 | SPI estándar |
| MISO | GPIO 19 | SPI estándar |
| SCK | GPIO 18 | SPI estándar |
| CSN | GPIO 5 | Chip select |
| CE | GPIO 27 | Chip enable |

**Nota:** el NRF24 se instala y cablea en fase 1 pero el código de recepción se activa en fase 3. Los pines SPI no interfieren con el WiFi (usan hardware distinto).

**⚠️ ADC2 NO funciona con WiFi. Usar solo ADC1 (GPIO 32-39).**
**⚠️ GPIO 0: no mantener LOW durante boot.**
**⚠️ NRF24L01: alimentar a 3.3V (NO 5V). Soldar capacitor 10µF entre VCC y GND del módulo.**

---

## 10. Software

### Fase 1 — Rover base

| Librería | Función |
|---|---|
| WiFi.h | Modo AP |
| WebServer.h | Interfaz web control |
| Wire.h | I2C |
| SPI.h | SPI (NRF24L01) |
| RF24 (nRF24/RF24) | Comunicación con mando RC (activa en fase 3) |
| Adafruit_NeoPixel | LEDs WS2812B |
| MPU6050 (jrowberg) | IMU |
| ESP32Encoder | Encoders |
| PID_v1 | Control PID velocidad |

### Fase 2 — Módulo telemetría

| Librería | Función |
|---|---|
| Adafruit_VL53L0X | Distancia |
| Adafruit_BME280 | Meteorología |
| Adafruit_LTR390 | UV + luz |
| TinyGPS++ | GPS |
| QMC5883LCompass | Brújula |
| Adafruit_TCA9548A | Multiplexor I2C |
| SD.h + SPI.h | Logging MicroSD |

### Fase 3 — Mando RC

| Librería | Función |
|---|---|
| RF24 (nRF24/RF24) | Comunicación con rover |
| SPI.h | SPI (NRF24L01) |
| Adafruit_SSD1306 | Pantalla OLED del mando |
| Wire.h | I2C (pantalla) |

---

## 11. Lista de compras

### Fase 1 — Rover base

**Estructura y suspensión:**

| Componente | Cant. | Notas |
|---|---|---|
| Filamento PETG 1kg | 1 | Chasis, brazos, soportes |
| Filamento PLA 1kg | 1 | Tapas, carcasas (opcional) |
| Insertos M3 × 5mm latón heat-set | 20 | Uniones estructurales |
| Insertos M2 × 4mm latón heat-set | 10 | Módulos electrónicos |
| KFL08 (chumacera flange 8mm) | 8 | 6 pivotes + 2 repuesto |
| Rodamientos 608ZZ sueltos | 6 | 4 extremos + 2 repuesto |
| Ejes acero inox 8mm × 50-65mm | 2 | Pivotes laterales |
| Eje acero inox 8mm × 30mm | 1 | Pivote diferencial |
| Ejes acero inox 8mm × 25mm | 4 | Pivotes brazos inferiores |
| E-clips retención 8mm | 20 pack | Fijación ejes |
| Amortiguadores 70mm oil-filled (set 4) | 1 set | INJORA o similar |
| Aceite silicona 20-30wt | 1 | Para amortiguadores |
| Tornillos M5 × 20mm inox | 4 | Pasadores bielas |
| Tuercas nyloc M5 | 4 | Anti-vibración |
| Arandelas M5 | 8 | Fricción bielas |
| Kit tornillos M3 surtido | 1 | ~200 uds |
| Kit tornillos M2 surtido | 1 | Módulos |
| Separadores hexagonales M3 | 1 pack | PCBs |
| Perfil T-slot 2020 (300mm) | 2 | Cortar a 160mm |
| T-nuts M3 | 10 | Montaje módulos |
| T-nuts M5 | 10 | Módulos pesados |
| Loctite azul | 1 | Toda la suspensión |

**Motores:**

| Componente | Cant. | Notas |
|---|---|---|
| JGA25-370 12V 100RPM con encoder | 4 | Eje 4mm D, encoder 11 PPR |
| Acoplador hex 4mm → 12mm | 4 | Con set screw |

**Alimentación:**

| Componente | Cant. | Notas |
|---|---|---|
| Batería LiPo 3S 2200mAh 30C XT60 | 2 | Pack |
| Cargador/balanceador LiPo | 1 | IMAX B6 o similar |
| Alarma voltaje LiPo | 1 | Independiente |
| Bolsa ignífuga LiPo | 1 | Seguridad |
| Interruptor ≥10A | 1 | Corte general |
| Conectores XT60 (pares) | 2 | Batería/carga |
| Regulador buck LM2596 | 1 | 12V → 5V |
| Cables silicona 16-18 AWG (rojo+negro) | 1m c/u | Potencia |
| Pack cables 22-24 AWG multicolor | 1 | 6 colores × 2m |
| Capacitor 1000µF 25V | 2 | LM2596 |
| Capacitor 100nF cerámico | 6 | Desacoplo |
| Diodo Schottky 1N5822 | 1 | Polaridad |
| Fusible 10A + portafusible | 1 | Cortocircuito |

**Electrónica:**

| Componente | Cant. | Notas |
|---|---|---|
| BTS7960 43A | 2 | Drivers motor |
| MPU6050 | 1 | IMU, centro chasis |
| Buzzer pasivo 5V | 1 | PWM |
| Tira WS2812B (30 LEDs/m) | 1 | Cortar 8 LEDs |
| Capacitor 1000µF 6.3V | 1 | Alimentación LEDs |
| Resistencia 330Ω | 1 | Datos LEDs |
| Resistencia 100KΩ | 1 | Divisor batería |
| Resistencia 27KΩ | 1 | Divisor batería |
| Resistencia 4.7KΩ | 4 | Pull-up I2C repuesto |
| Pantalla OLED SSD1306 I2C | 1 | Display datos |
| NRF24L01+PA+LNA (con antena) | 1 | Receptor RC, instalado en fase 1, activo en fase 3 |
| Capacitor 10µF electrolítico | 1 | Soldar en pines VCC/GND del NRF24 (estabilidad) |
| Freenove ESP32-S3-WROOM CAM (OV2640) | 1 | Cámara FPV independiente, USB nativo, 8MB PSRAM |
| Antena WiFi IPEX para ESP32-S3 CAM | 1 | Mejora señal de video (opcional pero recomendada) |

**Conector módulos:**

| Componente | Cant. | Notas |
|---|---|---|
| GX16-10 hembra (panel mount) | 1 | Techo chasis |
| GX16-10 macho (cable) | 2 | Uno por módulo |

**Cableado:**

| Componente | Cant. | Notas |
|---|---|---|
| Cables Dupont surtidos | 1 pack | Prototipado |
| Conectores JST-XH 2.54mm | 1 pack | Cableado desconectable |
| Conectores JST-PH 2.0mm | 1 pack | Sensores |
| Pin headers | 1 pack | Para soldar |
| PCB perforada 7×5cm | 2 | Circuitos |
| Termo-retráctil | 1 pack | Varios diámetros |
| Bridas / zip ties | 1 pack | Cables |
| Cinta espuma 3M | 1 | Fijar batería |

### Fase 2 — Módulo de telemetría

| Componente | Cant. | Notas |
|---|---|---|
| TCA9548A | 1 | Multiplexor I2C |
| GPS NEO-6M con antena | 1 | Navegación |
| QMC5883L | 1 | Brújula, lejos de motores |
| BME280 | 1 | Meteorología |
| LTR390 | 1 | UV + luz |
| Sensor lluvia | 1 | Analógico |
| PMS5003 | 1 | Calidad aire (opcional) |
| Módulo MicroSD SPI | 1 | Logging |
| MicroSD 8-16GB FAT32 | 1 | SanDisk/Kingston |
| ESP32 DevKit | 1 | Procesador dedicado del módulo (la tercera del proyecto) |

### Fase 3 — Mando RC físico

| Componente | Cant. | Notas |
|---|---|---|
| NRF24L01+PA+LNA (con antena) | 1 | Transmisor para el mando |
| Capacitor 10µF electrolítico | 1 | Estabilidad NRF24 del mando |
| Joysticks analógicos tipo PS2 | 2 | Control de dirección y velocidad |
| Botones/pulsadores | 4-6 | Funciones: luces, buzzer encontrar, modo autónomo, etc. |
| Pantalla OLED 0.96" SSD1306 I2C | 1 | Feedback: batería rover, señal, velocidad, telemetría |
| Batería LiPo 1S 3.7V (~1000mAh) | 1 | Alimentación del mando |
| Módulo TP4056 (carga USB) | 1 | Carga USB-C o micro-USB para la batería del mando |
| Interruptor mini | 1 | Encendido del mando |
| PCB perforada 5×7cm | 1 | Circuito del mando |
| Filamento PETG | (del mismo rollo) | Carcasa impresa en 3D |

**Nota:** la segunda ESP32 (ya adquirida) se usa para el mando RC. El mando envía comandos por NRF24 y recibe telemetría del rover (batería, velocidad, señal, estado IMU) en la pantalla OLED.

### Upgrade futuro

| Componente | Cant. | Notas |
|---|---|---|
| PCF8574 expansor GPIO I2C | 1 | 8 GPIOs extra si necesario |
| Módulo LoRa SX1278 | 2 | Telemetría a +5km (complementa NRF24) |
| Brazo robótico (módulo con ESP32 propia) | 1 | Se conecta al GX16-10 |

### Herramientas

| Herramienta | Notas |
|---|---|
| Soldador 60W regulable | Pinecil, TS101 |
| Estaño con flux 0.8mm | Sin plomo |
| Multímetro digital | — |
| Llaves Allen métricas | 1.5–6mm |
| Pelacables 16-26 AWG | — |
| Alicates corte diagonal | Flush cutters |
| Destornilladores precisión | Phillips + plano |
| Tercera mano con lupa | — |
| Pistola de calor | Insertos + termoretráctil |
| Calibre digital | Medir ejes/piezas |
| Pinzas antiestáticas | — |
| Tapete silicona soldar | — |
| Alicates E-clips | Opcional |

### Consumibles

| Consumible | Notas |
|---|---|
| Flux en pasta | Soldaduras |
| Malla desoldadora | Correcciones |
| Alcohol isopropílico 90%+ | Limpieza |
| Pegamento cianoacrilato | Refuerzo uniones |
| Loctite azul | Tornillos vibración |

---

## 12. Decisiones de diseño

| Decisión | Justificación |
|---|---|
| Procesamiento distribuido (ESP32 por módulo) | Aísla la lógica de cada módulo, el rover nunca se sobrecarga, módulos se desarrollan independientemente |
| Comunicación por UART (no I2C directo) | El módulo envía datos procesados, no lecturas crudas. Rover no necesita conocer los sensores |
| ESP32-S3 CAM separada (no integrada en ESP32 principal) | Video consume CPU impredeciblemente. Si la cámara se congela, el rover sigue conduciendo. $5 extra por fiabilidad total |
| Freenove ESP32-S3 en vez de ESP32-CAM básica | Doble PSRAM (8MB), 20-25fps vs 12-15fps en VGA, ~100ms latencia vs ~200ms. USB nativo sin adaptador |
| Arquitectura modular (base + módulos GX16) | Fase 1 simple, módulos intercambiables según misión |
| GX16-10 en vez de USB-C | 10 pines, robusto, rosca anti-vibración, 5A/pin |
| Doble articulación en vez de rocker simple | Shock absorbe vibraciones, rocker maneja obstáculos grandes |
| KFL08 dobles en vez de rodamiento en plástico | Metal soporta carga, autoalineantes |
| E-clips en vez de collarines | No se aflojan, mínimo espacio |
| Ruedas 92mm (definitivo) | Balance escalada (~60mm) y torque (4.6 kg) |
| Motor 100RPM en vez de 150/200RPM | Mayor torque con ruedas más grandes. 0.48 m/s suficiente |
| Brushed en vez de brushless | 10× más barato, control simple, reversa nativa |
| WiFi AP en vez de Bluetooth | Sin apps, interfaz web, mayor alcance |
| VL53L0X en módulo (no en chasis) | Intercambiables, rover base funciona sin ellos |
| T-slot 2020 para módulos | Estándar maker, barato, ecosistema compatible |
| Ground clearance = radio rueda (46mm) | Aprovecha 100% del recorrido de suspensión |
| Sensores a 65mm con +3° | Ignoran obstáculos <60mm superables |
| Solo canal A encoder | Ahorra 4 pines, dirección por BTS7960 |

---

## 13. Notas importantes

**Mecánica:**
- Los 3 ejes de pivote principales deben ser paralelos. Eje torcido = sistema trabado
- Ejes brazos inferiores (25mm): fijar con E-clips
- Loctite azul en tornillos prisioneros KFL08 y bielas
- Amortiguadores: aceite 20-30wt. Si rocker no articula, probar sin resorte
- Cables motores/encoders: +50mm holgura para articulación ±25°

**Electrónica:**
- Probar cada motor individual antes de montar
- GPIO 0 (encoder M2): no LOW durante boot
- LEDs WS2812B: resistencia 330Ω en datos, alimentar a 5V
- QMC5883L (fase 2): lo más lejos posible de motores
- ADC2 no funciona con WiFi. Solo ADC1

**Impresión 3D:**
- Chasis: boca abajo
- Brazos rocker: horizontal (capas resisten flexión)
- Zona pivotes: paredes 4mm, infill 60%
- Tolerancia Ø22mm: +0.15mm ajuste a presión
- Brazos inferiores: vertical, rodamiento arriba
