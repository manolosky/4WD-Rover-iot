# 🔧 Sistema de Suspensión Rocker + Diferencial — Rover 4WD

> **Tipo:** Rocker con barra diferencial (sin bogie, 4 ruedas)
> **Material:** PETG impreso en 3D + rodamientos + ejes de acero
> **Última actualización:** Mayo 2026

---

## 1. Visión general del sistema

Nuestro rover usa una versión simplificada del sistema rocker-bogie de NASA. Al tener solo **4 ruedas** (no 6), eliminamos el bogie y usamos solo el rocker (brazo basculante) con una **barra diferencial** que conecta ambos lados.

### Componentes del sistema

| # | Pieza | Cantidad | Función |
|---|---|---|---|
| 1 | Brazo rocker | 2 | Conecta las 2 ruedas de cada lado al chasis |
| 2 | Barra diferencial | 1 | Conecta ambos brazos rocker entre sí para nivelar el chasis |
| 3 | Pivote lateral (eje de rocker) | 2 | Eje donde el brazo rocker gira respecto al chasis |
| 4 | Pivote central (eje del diferencial) | 1 | Eje donde la barra diferencial gira respecto al chasis |
| 5 | Bielas de conexión (links) | 2 | Varillas que conectan la barra diferencial con cada brazo rocker |
| 6 | Soporte de motor | 4 | Fija cada motor al extremo del brazo rocker |
| 7 | Rodamientos | 5 | 2 en pivotes laterales, 1 en pivote central, 2 de repuesto |
| 8 | Topes de articulación | 4 | Limitan el ángulo máximo de giro de los brazos |

---

## 2. Cómo funciona — el principio del diferencial

### Sin barra diferencial (❌ MAL)

Si los brazos rocker solo pivotan en el chasis sin estar conectados entre sí, el chasis se comporta como una balanza: se inclina hacia el lado más pesado y no se corrige. En terreno irregular, un lado sube y el chasis se inclina sin control.

### Con barra diferencial (✅ BIEN)

La barra diferencial es una palanca horizontal montada en el centro del chasis con un pivote propio. Cada extremo de la barra se conecta al brazo rocker de su lado mediante una biela (link/varilla).

**Funcionamiento:**

1. El rover pasa sobre una roca con el lado izquierdo
2. El brazo izquierdo sube → empuja la biela izquierda hacia arriba
3. La biela izquierda empuja su extremo de la barra diferencial hacia arriba
4. La barra diferencial gira sobre su pivote central
5. El otro extremo de la barra baja → tira de la biela derecha hacia abajo
6. La biela derecha empuja el brazo derecho hacia abajo
7. El brazo derecho presiona sus ruedas contra el suelo

**Resultado:** El chasis se mantiene en el **ángulo promedio** de ambos brazos. Si el lado izquierdo sube 20° y el derecho baja 10°, el chasis se inclina solo 5°.

### Cadena cinemática completa

```
Rueda izq ─→ Brazo rocker izq ─→ Biela izq ─→ Barra diferencial ─→ Biela der ─→ Brazo rocker der ─→ Rueda der
                  │                                    │                                    │
              pivote lateral izq              pivote central                         pivote lateral der
                  │                                    │                                    │
               CHASIS ◄────────────────────────── CHASIS ──────────────────────────►  CHASIS
```

---

## 3. Diseño mecánico — dimensiones

### 3.1 Brazo rocker (×2, uno por lado)

El brazo rocker es la pieza principal. Es una barra rígida con un motor en cada extremo y un punto de pivote en el medio (ligeramente descentrado).

| Parámetro | Valor | Notas |
|---|---|---|
| Largo total | 190mm | Distancia entre centros de eje de motor delantero y trasero |
| Ancho | 28mm | Suficiente para alojar el rodamiento 608ZZ (22mm Ø ext) |
| Grosor/altura de la sección | 15mm | Perfil rectangular para rigidez |
| Posición del pivote | 95mm desde cada extremo | Centrado para distribución de peso uniforme |
| Orificio de pivote | Ø22mm exterior / Ø8mm interior | Para alojar rodamiento 608ZZ |
| Orificios de motor | 2× M3 con separación de 17mm | Patrón de montaje del JGA25-371 |
| Orificio de conexión biela | Ø5mm, a 15mm del pivote hacia arriba | Donde se conecta la biela del diferencial |
| Material | PETG | Paredes 3mm, infill 50% |
| Peso estimado | ~30g por brazo | Sin motores |

**Geometría del brazo:**

```
                    Orificio biela (Ø5mm)
                         ○
                         |  15mm
    ┌────────────────────●────────────────────┐
    │  Soporte motor     │     Soporte motor  │
    │  delantero     PIVOTE 608ZZ   trasero   │
    │  (M1/M2)      (Ø22/8mm)     (M3/M4)    │
    └────────────────────┴────────────────────┘
    ◄──── 95mm ────►     ◄──── 95mm ────►
    ◄──────────── 190mm total ────────────►
```

**Consideraciones de impresión:**

- Imprimir en orientación horizontal (acostado)
- La zona del pivote es la más crítica: reforzar con paredes de 4mm alrededor del orificio del rodamiento
- Los orificios de motor llevan insertos M3 de latón heat-set
- El orificio de la biela lleva un buje de latón o un tornillo M5 con tuerca autoblocante

### 3.2 Barra diferencial (×1)

La barra diferencial es una palanca horizontal que se monta **encima o dentro del chasis**, perpendicular al eje longitudinal del rover (de izquierda a derecha).

| Parámetro | Valor | Notas |
|---|---|---|
| Largo total | 120mm | Suficiente para alcanzar ambas bielas |
| Ancho | 20mm | Perfil plano |
| Grosor | 10mm | Suficiente rigidez sin peso excesivo |
| Pivote central | Ø22mm ext / Ø8mm int | Rodamiento 608ZZ, en el centro exacto |
| Orificios de biela | 2× Ø5mm, uno a cada extremo | A 50mm del centro, simétricos |
| Material | PETG | Paredes 3mm, infill 60% |
| Peso estimado | ~15g | |

**Geometría de la barra:**

```
    Conexión biela izq          PIVOTE CENTRAL          Conexión biela der
          ○──────────────────────────●──────────────────────────○
          │                         │                          │
        Ø5mm                    608ZZ                       Ø5mm
          │                     Ø8mm                         │
    ◄── 50mm ──►           ◄── 10mm ──►              ◄── 50mm ──►
    ◄──────────────────── 120mm total ──────────────────────►
```

**Montaje en el chasis:**

- El pivote central se monta en un soporte elevado en el interior del chasis, centrado
- El eje del pivote central va perpendicular al suelo (la barra gira en el plano horizontal)
- ⚠️ NO: la barra NO gira como las aspas de un helicóptero. El eje del pivote central va **paralelo al eje longitudinal del rover** (de adelante hacia atrás), así la barra puede subir/bajar en cada extremo

### 3.3 Bielas de conexión (links) (×2)

Las bielas son varillas cortas que conectan cada extremo de la barra diferencial con el orificio de biela de cada brazo rocker. Transforman el movimiento vertical de cada brazo en movimiento angular de la barra diferencial.

| Parámetro | Valor | Notas |
|---|---|---|
| Largo (centro a centro) | 40–50mm | Ajustar según geometría final del chasis |
| Sección | Cilíndrica Ø8mm o rectangular 8×6mm | La cilíndrica es más fácil de imprimir |
| Orificios en cada extremo | 2× Ø5mm | Para tornillos M5 o pasadores |
| Tipo de unión | Rótula o pivote libre | Ambos extremos deben girar libremente |
| Material | PETG | Infill 80% por ser pieza de transmisión de fuerza |
| Peso estimado | ~5g cada una | |

**Unión de las bielas:**

Cada biela se une con un **tornillo M5 × 20mm** + arandela + tuerca autoblocante (nyloc) en cada extremo. Debe girar libremente pero sin holgura excesiva. La arandela evita fricción entre las piezas impresas.

### 3.4 Soportes de motor (×4)

Cada soporte fija un motor JGA25-371 al extremo del brazo rocker. El motor queda perpendicular al brazo, con el eje apuntando hacia fuera (hacia la rueda).

| Parámetro | Valor | Notas |
|---|---|---|
| Forma | Abrazadera tipo U o cilíndrica | Rodea el cuerpo del motor (Ø25mm) |
| Diámetro interno | 25mm | Ajustado al motor, con tolerancia de 0.2mm |
| Fijación al brazo | 2× tornillos M3 | Patrón de montaje estándar JGA25 (17mm entre centros) |
| Fijación al motor | Por presión + tornillo prisionero M3 | Para que no gire ni se deslice |
| Orientación | Eje del motor perpendicular al brazo | Eje apunta hacia fuera → acoplador hex → rueda |
| Material | PETG | Paredes 3mm, infill 60% |

### 3.5 Topes de articulación (×4)

Limitan el ángulo máximo que cada brazo rocker puede girar. Sin ellos, un brazo podría girar tanto que la biela se traba (punto muerto) o el motor golpea el chasis.

| Parámetro | Valor | Notas |
|---|---|---|
| Tipo | Bloques/pestañas impresas en el chasis | Simples topes mecánicos |
| Ángulo máximo permitido | ±25° desde la horizontal | Suficiente para obstáculos de ~40mm de alto |
| Ubicación | 2 por lado, arriba y abajo del brazo en la zona del pivote | |
| Material | Integrado al chasis (PETG) | Reforzar con infill 60% |

---

## 4. Articulación y capacidades

### 4.1 Rango de movimiento

| Parámetro | Valor |
|---|---|
| Ángulo máximo por brazo | ±25° |
| Desnivel máximo entre lados | 50° (un lado +25°, otro -25°) |
| Inclinación máxima del chasis | ±12.5° (la mitad del desnivel, gracias al diferencial) |
| Altura máxima de obstáculo superable | ~40mm (aprox. la mitad del diámetro de rueda) |
| Ground clearance nominal | 42mm (= radio de rueda, aprovecha 100% del recorrido) |
| Ground clearance mínimo (al articular) | ~22mm |
| Altura total del rover | ~72mm |

### 4.2 Geometría clave para el diseño 3D

Las siguientes medidas son las que determinan dónde colocar los pivotes en el chasis:

| Parámetro | Valor | Cálculo / Justificación |
|---|---|---|
| Radio de rueda | 42.5mm | Diámetro 85mm / 2 |
| Altura centro de eje del motor al suelo | 42.5mm | = radio de la rueda |
| Altura del pivote lateral al suelo | 42.5mm | Mismo nivel que el eje del motor (brazo horizontal en reposo) |
| Ground clearance del chasis | 42mm | = radio de rueda. El chasis pasa por encima de cualquier obstáculo que las ruedas puedan trepar |
| Posición vertical del pivote en el chasis | Base del chasis (42mm) | El pivote está al nivel del borde inferior del chasis |
| Recorrido vertical por rueda | ±40mm | 95mm (brazo) × sin(25°) = 40.1mm |
| Altura del soporte lateral del pivote | 42.5mm desde la base del chasis a nivel del suelo | El soporte baja desde el chasis hasta la línea del eje |

**Relación pivote-chasis-suelo:**

```
                    ┌─────── CHASIS (30mm alto) ──────┐
                    │                                  │  ← Techo: 72mm del suelo
                    │         Electrónica              │
                    │                                  │
    eje del motor → ├──────── PIVOTE (608ZZ) ──────────┤  ← 42.5mm del suelo
                    │                                  │
                    └──── base chasis (clearance) ─────┘  ← 42mm del suelo
                    ·  ·  ·  ·  SUELO  ·  ·  ·  ·  ·  ·  ← 0mm
```

**Nota:** La diferencia entre la base del chasis (42mm) y el centro del pivote (42.5mm) es solo 0.5mm. En la práctica, el soporte lateral del pivote se extiende ~0.5mm por debajo de la base del chasis, lo cual es despreciable. El pivote queda esencialmente al ras del borde inferior.

### 4.2 Distribución de peso

Para que el sistema funcione correctamente, el centro de gravedad del chasis debe estar lo más centrado posible, tanto longitudinal como lateralmente.

| Eje | Ideal | Tolerancia |
|---|---|---|
| Longitudinal (adelante-atrás) | Centro exacto | ±15mm |
| Lateral (izquierda-derecha) | Centro exacto | ±10mm |
| Vertical | Lo más bajo posible | Batería en el fondo del chasis |

**Tip:** La batería es el componente más pesado (~170g). Colocarla centrada y en la parte más baja del chasis mejora la estabilidad.

---

## 5. Hardware necesario para la suspensión

### 5.1 Piezas impresas en 3D

| Pieza | Cant. | Material | Infill | Paredes | Peso est. |
|---|---|---|---|---|---|
| Brazo rocker | 2 | PETG | 50% | 3mm (4mm en zona pivote) | ~30g c/u |
| Barra diferencial | 1 | PETG | 60% | 3mm | ~15g |
| Bielas de conexión | 2 | PETG | 80% | 3mm | ~5g c/u |
| Soportes de motor | 4 | PETG | 60% | 3mm | ~8g c/u |
| Soporte pivote central (en chasis) | 1 | PETG | 60% | 3mm | ~10g |
| Topes de articulación | 4 | Integrado al chasis | 60% | — | — |
| Soportes laterales de pivote (en chasis) | 2 | PETG | 60% | 3mm | ~8g c/u |
| **Total peso piezas impresas** | | | | | **~167g** |

### 5.2 Rodamientos y ejes

| Componente | Cant. | Especificación | Función |
|---|---|---|---|
| Rodamiento 608ZZ | 3 (+2 repuesto) | 8mm bore, 22mm ext, 7mm ancho | 2 en pivotes laterales, 1 en pivote central |
| Eje de acero inox | 2 | Ø8mm × 50mm | Pivotes laterales (rocker al chasis) |
| Eje de acero inox | 1 | Ø8mm × 30mm | Pivote central (barra diferencial al chasis) |
| Collarines de eje (shaft collars) | 6 | Ø8mm | 2 por cada eje de pivote, fijan en posición |

### 5.3 Tornillería de la suspensión

| Componente | Cant. | Especificación | Función |
|---|---|---|---|
| Tornillo M5 × 20mm | 4 | Acero inox | Pasadores de las bielas (2 por biela) |
| Tuerca autoblocante M5 (nyloc) | 4 | | Evitan que se aflojen con vibración |
| Arandela M5 | 8 | | 2 por cada unión de biela (reducen fricción) |
| Tornillo M3 × 10mm | 8 | | Montaje de motores al brazo (2 por motor) |
| Insertos M3 heat-set | 8 | Latón, 5mm largo | En los orificios de motor del brazo rocker |
| Loctite azul (frenaroscas) | — | Medio | Aplicar en todos los tornillos de la suspensión |

### 5.4 Piezas opcionales recomendadas

| Componente | Cant. | Función |
|---|---|---|
| Bujes de nylon Ø5mm | 4 | Alternativa a tornillo M5 para las bielas, menor fricción |
| O-rings de silicona Ø8mm | 6 | Amortiguación extra en los collarines de eje |
| Arandelas de teflón (PTFE) Ø8mm | 6 | Entre el rodamiento y la pieza impresa, reduce fricción |

---

## 6. Puntos de montaje en el chasis

El chasis necesita incorporar estos puntos de montaje específicos para la suspensión:

### 6.1 Soportes laterales de pivote (×2)

Ubicados en los laterales del chasis, exactamente a la mitad del largo (100mm desde el frente), a la altura del borde. Son dos torretas o paredes verticales con un orificio pasante de Ø22mm para alojar el rodamiento 608ZZ.

| Parámetro | Valor |
|---|---|
| Posición longitudinal | 100mm desde el frente (centro del chasis) |
| Posición vertical | Borde inferior del chasis. Centro del orificio a 42.5mm del suelo (= centro del eje del motor) |
| Orificio | Ø22mm pasante (para rodamiento 608ZZ) |
| Grosor de la pared | 7mm mínimo (ancho del rodamiento 608ZZ) |
| Altura del soporte | El soporte se extiende ~0.5mm por debajo de la base del chasis |
| Refuerzo | Nervaduras laterales para rigidez |

### 6.2 Soporte del pivote central del diferencial (×1)

Ubicado en el centro del interior del chasis. Es un pilar vertical con un orificio pasante de Ø22mm en la parte superior para alojar el rodamiento del diferencial.

| Parámetro | Valor |
|---|---|
| Posición | Centro exacto del chasis (100mm × 80mm) |
| Altura | ~20mm desde la base del chasis |
| Orificio | Ø22mm pasante (para rodamiento 608ZZ) |
| Orientación del eje | Paralelo al eje longitudinal del rover (adelante-atrás) |
| Refuerzo | Base ancha con nervaduras |

---

## 7. Ensamblaje — orden de montaje

### Paso 1: Preparar los brazos rocker

1. Imprimir los 2 brazos rocker en PETG
2. Insertar los insertos M3 heat-set (8 total, 4 por brazo) con soldador/pistola de calor
3. Presionar los rodamientos 608ZZ en los orificios de pivote de cada brazo
4. Verificar que giren libremente

### Paso 2: Montar los motores en los brazos

1. Colocar cada motor JGA25-371 en su soporte
2. Atornillar los soportes al brazo rocker con tornillos M3 × 10mm
3. Verificar que el eje del motor apunte hacia fuera (perpendicular al brazo)
4. Colocar los acopladores hex 4mm→12mm en cada eje de motor
5. Conectar las ruedas (aún no cablear los motores)

### Paso 3: Preparar la barra diferencial

1. Imprimir la barra diferencial
2. Presionar el rodamiento 608ZZ en el orificio central
3. Verificar que gira libremente

### Paso 4: Preparar las bielas

1. Imprimir las 2 bielas
2. Verificar que los orificios Ø5mm estén limpios
3. Preparar los 4 conjuntos de tornillo M5 + arandela + tuerca nyloc

### Paso 5: Montar los pivotes laterales en el chasis

1. Insertar los ejes de acero Ø8mm × 50mm en los soportes laterales del chasis
2. Pasar cada eje a través del rodamiento de cada brazo rocker
3. Fijar con collarines de eje a ambos lados de cada soporte
4. Verificar que cada brazo gira libremente ±25° sin tocar el chasis

### Paso 6: Montar la barra diferencial

1. Insertar el eje de acero Ø8mm × 30mm en el soporte central del chasis
2. Pasar el eje a través del rodamiento de la barra diferencial
3. Fijar con collarines de eje
4. Verificar que la barra gira libremente

### Paso 7: Conectar las bielas

1. Conectar un extremo de cada biela al brazo rocker (orificio de biela) con tornillo M5 + arandelas + tuerca nyloc
2. Conectar el otro extremo de cada biela al extremo correspondiente de la barra diferencial
3. **No apretar demasiado**: las uniones deben girar libremente pero sin juego excesivo
4. Verificar que al subir un brazo, el otro baja proporcionalmente

### Paso 8: Verificar el sistema

1. Con el rover apoyado en las 4 ruedas sobre superficie plana, verificar que el chasis esté nivelado
2. Levantar un lado del rover ~30mm: el otro lado debe bajar proporcionalmente
3. El chasis debe permanecer aproximadamente horizontal
4. Verificar que ningún brazo toque el chasis ni se trabe en los extremos del recorrido
5. Aplicar Loctite azul en todos los collarines de eje y tornillos M5

---

## 8. Problemas comunes y soluciones

| Problema | Causa probable | Solución |
|---|---|---|
| El chasis se inclina hacia un lado estando en plano | Centro de gravedad descentrado | Redistribuir componentes internos (mover batería) |
| Un brazo no gira suave | Rodamiento mal asentado o eje torcido | Revisar alineación, reemplazar rodamiento si dañado |
| Las bielas se traban | Uniones demasiado apretadas o desalineadas | Aflojar tuercas nyloc, verificar que los orificios estén alineados |
| El brazo gira demasiado y golpea el chasis | Topes de articulación insuficientes | Agregar topes o reducir el orificio de paso |
| Ruidos/crujidos al articular | Fricción entre piezas impresas | Agregar arandelas de teflón, lubricar con grasa de litio |
| Un motor se suelta del brazo | Vibración aflojó los tornillos M3 | Aplicar Loctite azul, verificar insertos heat-set |
| El diferencial no nivela bien | Bielas de largo desigual | Medir y reimprimir bielas con largo idéntico |
| Excesiva holgura lateral | Collarines de eje mal fijados | Apretar collarines, agregar arandelas de ajuste |

---

## 9. Resumen de pesos del sistema de suspensión

| Componente | Peso |
|---|---|
| 2× brazos rocker (impresos) | ~60g |
| 1× barra diferencial (impresa) | ~15g |
| 2× bielas (impresas) | ~10g |
| 4× soportes de motor (impresos) | ~32g |
| Soportes en chasis (impresos) | ~26g |
| 3× rodamientos 608ZZ | ~21g |
| 3× ejes de acero | ~30g |
| 6× collarines de eje | ~18g |
| Tornillería (M3 + M5 + insertos) | ~15g |
| **Total sistema de suspensión** | **~227g** |
| 4× motores JGA25-371 | ~344g |
| 4× ruedas 85×47mm | ~120g (estimado) |
| **Total tren motriz + suspensión** | **~691g** |

---

## 10. Notas importantes

- **Tolerancias de impresión:** Para los orificios de los rodamientos (Ø22mm), imprimir con 0.15mm de tolerancia de ajuste a presión. Si queda flojo, usar pegamento cianoacrilato. Si queda muy apretado, lijar con lima redonda.

- **Alineación crítica:** Los 3 ejes de pivote deben estar perfectamente paralelos entre sí. Si un eje está torcido, el sistema se traba. Usar calibre digital para verificar.

- **No engrasar los rodamientos 608ZZ:** Ya vienen lubricados de fábrica (las tapas metálicas ZZ sellan la grasa). Solo engrasar las uniones de las bielas (tornillos M5) con grasa de litio blanca.

- **Cable routing:** Los cables de los motores y encoders pasan por los brazos rocker y entran al chasis a través de los pivotes laterales. Dejar suficiente holgura (~50mm extra) para que los cables no se tensen cuando el brazo articula ±25°. Idealmente, hacer un canal de paso en el brazo rocker para guiar los cables.

- **Prueba de estrés:** Antes de montar la electrónica, poner el rover en el suelo con peso muerto (bolsa de arena ~500g en el chasis) y rodarlo sobre obstáculos de diferentes alturas. Verificar que el sistema articula sin trabar y que las 4 ruedas mantienen contacto.

- **Brazos rocker — orientación de impresión:** Imprimir acostados (horizontal) para que las capas resistan las fuerzas de flexión. Si se imprimen verticales, las capas se delaminan bajo carga.

- **Sonido como diagnóstico:** Un sistema de suspensión bien montado es silencioso. Si se escuchan crujidos o clicks, algo no está girando libre — revisar rodamientos y uniones.
