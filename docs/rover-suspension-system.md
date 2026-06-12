# 🔧 Rocker + Differential Suspension System — 4WD Rover

> **Type:** Rocker with differential bar (no bogie, 4 wheels)
> **Material:** 3D-printed PETG + bearings + steel shafts
> **Last updated:** May 2026

---

## 1. System overview

Our rover uses a simplified version of NASA's rocker-bogie system. Since it has only **4 wheels** (not 6), we drop the bogie and use just the rocker (swing arm) with a **differential bar** connecting both sides.

### System components

| # | Part | Qty | Function |
|---|---|---|---|
| 1 | Rocker arm | 2 | Connects the 2 wheels on each side to the chassis |
| 2 | Differential bar | 1 | Connects both rocker arms together to level the chassis |
| 3 | Side pivot (rocker shaft) | 2 | Shaft where the rocker arm rotates relative to the chassis |
| 4 | Central pivot (differential shaft) | 1 | Shaft where the differential bar rotates relative to the chassis |
| 5 | Connection links | 2 | Rods connecting the differential bar to each rocker arm |
| 6 | Motor mount | 4 | Fixes each motor to the end of the rocker arm |
| 7 | Bearings | 5 | 2 in side pivots, 1 in central pivot, 2 spare |
| 8 | Articulation stops | 4 | Limit the maximum rotation angle of the arms |

---

## 2. How it works — the differential principle

### Without a differential bar (❌ BAD)

If the rocker arms only pivot on the chassis without being connected to each other, the chassis behaves like a balance scale: it tilts toward the heavier side and never corrects itself. On rough terrain, one side rises and the chassis tilts uncontrollably.

### With a differential bar (✅ GOOD)

The differential bar is a horizontal lever mounted at the center of the chassis with its own pivot. Each end of the bar connects to the rocker arm on its side through a link (rod).

**Operation:**

1. The rover drives over a rock with its left side
2. The left arm rises → pushes the left link upward
3. The left link pushes its end of the differential bar upward
4. The differential bar rotates on its central pivot
5. The other end of the bar goes down → pulls the right link downward
6. The right link pushes the right arm downward
7. The right arm presses its wheels against the ground

**Result:** the chassis stays at the **average angle** of both arms. If the left side rises 20° and the right side drops 10°, the chassis tilts only 5°.

### Complete kinematic chain

```
Left wheel ─→ Left rocker arm ─→ Left link ─→ Differential bar ─→ Right link ─→ Right rocker arm ─→ Right wheel
                  │                                  │                                  │
            left side pivot                  central pivot                       right side pivot
                  │                                  │                                  │
              CHASSIS ◄──────────────────────── CHASSIS ────────────────────────►  CHASSIS
```

---

## 3. Mechanical design — dimensions

### 3.1 Rocker arm (×2, one per side)

The rocker arm is the main part. It is a rigid bar with a motor at each end and a pivot point in the middle (slightly off-center).

| Parameter | Value | Notes |
|---|---|---|
| Total length | 190mm | Distance between front and rear motor shaft centers |
| Width | 28mm | Enough to house the 608ZZ bearing (22mm OD) |
| Section thickness/height | 15mm | Rectangular profile for stiffness |
| Pivot position | 95mm from each end | Centered for even weight distribution |
| Pivot hole | Ø22mm outer / Ø8mm inner | To house a 608ZZ bearing |
| Motor holes | 2× M3, 17mm apart | JGA25-371 mounting pattern |
| Link connection hole | Ø5mm, 15mm above the pivot | Where the differential link connects |
| Material | PETG | 3mm walls, 50% infill |
| Estimated weight | ~30g per arm | Without motors |

**Arm geometry:**

```
                    Link hole (Ø5mm)
                         ○
                         |  15mm
    ┌────────────────────●────────────────────┐
    │  Front motor       │      Rear motor    │
    │  mount         608ZZ PIVOT    mount     │
    │  (M1/M2)      (Ø22/8mm)     (M3/M4)    │
    └────────────────────┴────────────────────┘
    ◄──── 95mm ────►     ◄──── 95mm ────►
    ◄──────────── 190mm total ────────────►
```

**Printing considerations:**

- Print in horizontal orientation (lying flat)
- The pivot zone is the most critical: reinforce with 4mm walls around the bearing hole
- The motor holes take M3 brass heat-set inserts
- The link hole takes a brass bushing or an M5 bolt with a self-locking nut

### 3.2 Differential bar (×1)

The differential bar is a horizontal lever mounted **on top of or inside the chassis**, perpendicular to the rover's longitudinal axis (left to right).

| Parameter | Value | Notes |
|---|---|---|
| Total length | 120mm | Enough to reach both links |
| Width | 20mm | Flat profile |
| Thickness | 10mm | Enough stiffness without excess weight |
| Central pivot | Ø22mm outer / Ø8mm inner | 608ZZ bearing, at the exact center |
| Link holes | 2× Ø5mm, one at each end | 50mm from the center, symmetric |
| Material | PETG | 3mm walls, 60% infill |
| Estimated weight | ~15g | |

**Bar geometry:**

```
    Left link joint              CENTRAL PIVOT             Right link joint
          ○──────────────────────────●──────────────────────────○
          │                         │                          │
        Ø5mm                    608ZZ                       Ø5mm
          │                     Ø8mm                         │
    ◄── 50mm ──►           ◄── 10mm ──►              ◄── 50mm ──►
    ◄──────────────────── 120mm total ──────────────────────►
```

**Chassis mounting:**

- The central pivot mounts on a raised support inside the chassis, centered
- The central pivot shaft runs perpendicular to the ground (the bar rotates in the horizontal plane)
- ⚠️ NO: the bar does NOT spin like helicopter blades. The central pivot shaft runs **parallel to the rover's longitudinal axis** (front to back), so each end of the bar can move up/down

### 3.3 Connection links (×2)

The links are short rods that connect each end of the differential bar to the link hole on each rocker arm. They convert each arm's vertical movement into angular movement of the differential bar.

| Parameter | Value | Notes |
|---|---|---|
| Length (center to center) | 40–50mm | Adjust to the final chassis geometry |
| Section | Cylindrical Ø8mm or rectangular 8×6mm | Cylindrical is easier to print |
| Holes at each end | 2× Ø5mm | For M5 bolts or pins |
| Joint type | Ball joint or free pivot | Both ends must rotate freely |
| Material | PETG | 80% infill since it transmits force |
| Estimated weight | ~5g each | |

**Link joints:**

Each link is joined with an **M5 × 20mm bolt** + washer + self-locking (nyloc) nut at each end. It must rotate freely but without excessive play. The washer prevents friction between the printed parts.

### 3.4 Motor mounts (×4)

Each mount fixes a JGA25-371 motor to the end of the rocker arm. The motor sits perpendicular to the arm, with the shaft pointing outward (toward the wheel).

| Parameter | Value | Notes |
|---|---|---|
| Shape | U-shaped or cylindrical clamp | Wraps the motor body (Ø25mm) |
| Inner diameter | 25mm | Fitted to the motor, 0.2mm tolerance |
| Attachment to the arm | 2× M3 screws | Standard JGA25 mounting pattern (17mm between centers) |
| Attachment to the motor | Press fit + M3 set screw | So it neither rotates nor slides |
| Orientation | Motor shaft perpendicular to the arm | Shaft points outward → hex coupler → wheel |
| Material | PETG | 3mm walls, 60% infill |

### 3.5 Articulation stops (×4)

They limit the maximum angle each rocker arm can rotate. Without them, an arm could rotate so far that the link locks up (dead point) or the motor hits the chassis.

| Parameter | Value | Notes |
|---|---|---|
| Type | Blocks/tabs printed into the chassis | Simple mechanical stops |
| Maximum allowed angle | ±25° from horizontal | Enough for obstacles ~40mm tall |
| Location | 2 per side, above and below the arm near the pivot | |
| Material | Integrated into the chassis (PETG) | Reinforce with 60% infill |

---

## 4. Articulation and capabilities

### 4.1 Range of motion

| Parameter | Value |
|---|---|
| Maximum angle per arm | ±25° |
| Maximum height difference between sides | 50° (one side +25°, the other -25°) |
| Maximum chassis tilt | ±12.5° (half the difference, thanks to the differential) |
| Maximum climbable obstacle height | ~40mm (roughly half the wheel diameter) |
| Nominal ground clearance | 42mm (= wheel radius, uses 100% of the travel) |
| Minimum ground clearance (while articulating) | ~22mm |
| Total rover height | ~72mm |

### 4.2 Key geometry for the 3D design

The following measurements determine where to place the pivots on the chassis:

| Parameter | Value | Calculation / Rationale |
|---|---|---|
| Wheel radius | 42.5mm | 85mm diameter / 2 |
| Motor shaft center height from the ground | 42.5mm | = wheel radius |
| Side pivot height from the ground | 42.5mm | Same level as the motor shaft (arm horizontal at rest) |
| Chassis ground clearance | 42mm | = wheel radius. The chassis clears any obstacle the wheels can climb |
| Vertical pivot position on the chassis | Chassis bottom (42mm) | The pivot sits at the level of the chassis's lower edge |
| Vertical travel per wheel | ±40mm | 95mm (arm) × sin(25°) = 40.1mm |
| Side pivot support height | 42.5mm from the chassis base at ground level | The support drops from the chassis down to the shaft line |

**Pivot-chassis-ground relationship:**

```
                    ┌─────── CHASSIS (30mm tall) ──────┐
                    │                                  │  ← Roof: 72mm from the ground
                    │         Electronics              │
                    │                                  │
     motor shaft →  ├──────── PIVOT (608ZZ) ──────────┤  ← 42.5mm from the ground
                    │                                  │
                    └──── chassis base (clearance) ────┘  ← 42mm from the ground
                    ·  ·  ·  ·  GROUND  ·  ·  ·  ·  ·  ← 0mm
```

**Note:** the difference between the chassis base (42mm) and the pivot center (42.5mm) is only 0.5mm. In practice, the side pivot support extends ~0.5mm below the chassis base, which is negligible. The pivot sits essentially flush with the lower edge.

### 4.2 Weight distribution

For the system to work correctly, the chassis's center of gravity must be as centered as possible, both longitudinally and laterally.

| Axis | Ideal | Tolerance |
|---|---|---|
| Longitudinal (front-back) | Exact center | ±15mm |
| Lateral (left-right) | Exact center | ±10mm |
| Vertical | As low as possible | Battery at the bottom of the chassis |

**Tip:** the battery is the heaviest component (~170g). Placing it centered and as low as possible in the chassis improves stability.

---

## 5. Hardware needed for the suspension

### 5.1 3D-printed parts

| Part | Qty | Material | Infill | Walls | Est. weight |
|---|---|---|---|---|---|
| Rocker arm | 2 | PETG | 50% | 3mm (4mm in the pivot zone) | ~30g each |
| Differential bar | 1 | PETG | 60% | 3mm | ~15g |
| Connection links | 2 | PETG | 80% | 3mm | ~5g each |
| Motor mounts | 4 | PETG | 60% | 3mm | ~8g each |
| Central pivot support (on chassis) | 1 | PETG | 60% | 3mm | ~10g |
| Articulation stops | 4 | Integrated into the chassis | 60% | — | — |
| Side pivot supports (on chassis) | 2 | PETG | 60% | 3mm | ~8g each |
| **Total printed parts weight** | | | | | **~167g** |

### 5.2 Bearings and shafts

| Component | Qty | Specification | Function |
|---|---|---|---|
| 608ZZ bearing | 3 (+2 spare) | 8mm bore, 22mm OD, 7mm wide | 2 in side pivots, 1 in the central pivot |
| Stainless steel shaft | 2 | Ø8mm × 50mm | Side pivots (rocker to chassis) |
| Stainless steel shaft | 1 | Ø8mm × 30mm | Central pivot (differential bar to chassis) |
| Shaft collars | 6 | Ø8mm | 2 per pivot shaft, lock it in position |

### 5.3 Suspension fasteners

| Component | Qty | Specification | Function |
|---|---|---|---|
| M5 × 20mm bolt | 4 | Stainless steel | Link pins (2 per link) |
| M5 self-locking nut (nyloc) | 4 | | Prevent loosening from vibration |
| M5 washer | 8 | | 2 per link joint (reduce friction) |
| M3 × 10mm screw | 8 | | Motor mounting to the arm (2 per motor) |
| M3 heat-set inserts | 8 | Brass, 5mm long | In the rocker arm motor holes |
| Blue Loctite (threadlocker) | — | Medium | Apply to all suspension screws |

### 5.4 Recommended optional parts

| Component | Qty | Function |
|---|---|---|
| Ø5mm nylon bushings | 4 | Alternative to M5 bolts for the links, less friction |
| Ø8mm silicone O-rings | 6 | Extra damping on the shaft collars |
| Ø8mm PTFE (teflon) washers | 6 | Between the bearing and the printed part, reduces friction |

---

## 6. Chassis mounting points

The chassis needs these specific mounting points for the suspension:

### 6.1 Side pivot supports (×2)

Located on the sides of the chassis, exactly at half its length (100mm from the front), at edge height. They are two vertical towers or walls with a Ø22mm through hole to house the 608ZZ bearing.

| Parameter | Value |
|---|---|
| Longitudinal position | 100mm from the front (chassis center) |
| Vertical position | Lower edge of the chassis. Hole center at 42.5mm from the ground (= motor shaft center) |
| Hole | Ø22mm through (for a 608ZZ bearing) |
| Wall thickness | 7mm minimum (608ZZ bearing width) |
| Support height | The support extends ~0.5mm below the chassis base |
| Reinforcement | Side ribs for stiffness |

### 6.2 Central differential pivot support (×1)

Located at the center of the chassis interior. It is a vertical pillar with a Ø22mm through hole at the top to house the differential bearing.

| Parameter | Value |
|---|---|
| Position | Exact center of the chassis (100mm × 80mm) |
| Height | ~20mm from the chassis base |
| Hole | Ø22mm through (for a 608ZZ bearing) |
| Shaft orientation | Parallel to the rover's longitudinal axis (front-back) |
| Reinforcement | Wide base with ribs |

---

## 7. Assembly — build order

### Step 1: Prepare the rocker arms

1. Print the 2 rocker arms in PETG
2. Install the M3 heat-set inserts (8 total, 4 per arm) with a soldering iron/heat gun
3. Press the 608ZZ bearings into the pivot holes of each arm
4. Check that they rotate freely

### Step 2: Mount the motors on the arms

1. Place each JGA25-371 motor in its mount
2. Screw the mounts to the rocker arm with M3 × 10mm screws
3. Check that the motor shaft points outward (perpendicular to the arm)
4. Fit the 4mm→12mm hex couplers on each motor shaft
5. Attach the wheels (do not wire the motors yet)

### Step 3: Prepare the differential bar

1. Print the differential bar
2. Press the 608ZZ bearing into the central hole
3. Check that it rotates freely

### Step 4: Prepare the links

1. Print the 2 links
2. Check that the Ø5mm holes are clean
3. Prepare the 4 sets of M5 bolt + washer + nyloc nut

### Step 5: Mount the side pivots on the chassis

1. Insert the Ø8mm × 50mm steel shafts into the chassis side supports
2. Pass each shaft through the bearing of each rocker arm
3. Lock with shaft collars on both sides of each support
4. Check that each arm rotates freely ±25° without touching the chassis

### Step 6: Mount the differential bar

1. Insert the Ø8mm × 30mm steel shaft into the chassis central support
2. Pass the shaft through the differential bar's bearing
3. Lock with shaft collars
4. Check that the bar rotates freely

### Step 7: Connect the links

1. Connect one end of each link to the rocker arm (link hole) with an M5 bolt + washers + nyloc nut
2. Connect the other end of each link to the corresponding end of the differential bar
3. **Do not overtighten:** the joints must rotate freely but without excessive play
4. Check that raising one arm lowers the other proportionally

### Step 8: Verify the system

1. With the rover resting on its 4 wheels on a flat surface, check that the chassis is level
2. Lift one side of the rover ~30mm: the other side should drop proportionally
3. The chassis should remain approximately horizontal
4. Check that no arm touches the chassis or binds at the ends of the travel
5. Apply blue Loctite to all shaft collars and M5 bolts

---

## 8. Common problems and solutions

| Problem | Likely cause | Solution |
|---|---|---|
| The chassis tilts to one side on flat ground | Off-center center of gravity | Redistribute internal components (move the battery) |
| An arm doesn't rotate smoothly | Poorly seated bearing or bent shaft | Check alignment, replace the bearing if damaged |
| The links bind | Joints too tight or misaligned | Loosen the nyloc nuts, check that the holes are aligned |
| An arm rotates too far and hits the chassis | Insufficient articulation stops | Add stops or reduce the clearance hole |
| Noises/creaks when articulating | Friction between printed parts | Add PTFE washers, lubricate with lithium grease |
| A motor comes loose from the arm | Vibration loosened the M3 screws | Apply blue Loctite, check the heat-set inserts |
| The differential doesn't level well | Links of unequal length | Measure and reprint links with identical length |
| Excessive lateral play | Loose shaft collars | Tighten the collars, add shim washers |

---

## 9. Suspension system weight summary

| Component | Weight |
|---|---|
| 2× rocker arms (printed) | ~60g |
| 1× differential bar (printed) | ~15g |
| 2× links (printed) | ~10g |
| 4× motor mounts (printed) | ~32g |
| Chassis supports (printed) | ~26g |
| 3× 608ZZ bearings | ~21g |
| 3× steel shafts | ~30g |
| 6× shaft collars | ~18g |
| Fasteners (M3 + M5 + inserts) | ~15g |
| **Total suspension system** | **~227g** |
| 4× JGA25-371 motors | ~344g |
| 4× 85×47mm wheels | ~120g (estimated) |
| **Total drivetrain + suspension** | **~691g** |

---

## 10. Important notes

- **Printing tolerances:** for the bearing holes (Ø22mm), print with 0.15mm press-fit tolerance. If loose, use cyanoacrylate glue. If too tight, sand with a round file.

- **Critical alignment:** the 3 pivot shafts must be perfectly parallel to each other. If a shaft is skewed, the system binds. Use a digital caliper to verify.

- **Do not grease the 608ZZ bearings:** they come lubricated from the factory (the ZZ metal shields seal the grease in). Only grease the link joints (M5 bolts) with white lithium grease.

- **Cable routing:** the motor and encoder cables run along the rocker arms and enter the chassis through the side pivots. Leave enough slack (~50mm extra) so the cables don't pull tight when the arm articulates ±25°. Ideally, add a routing channel in the rocker arm to guide the cables.

- **Stress test:** before installing the electronics, place the rover on the ground with dead weight (~500g sandbag on the chassis) and roll it over obstacles of different heights. Verify that the system articulates without binding and that all 4 wheels stay in contact.

- **Rocker arms — print orientation:** print lying flat (horizontal) so the layers resist bending forces. If printed vertically, the layers delaminate under load.

- **Sound as a diagnostic:** a well-assembled suspension system is silent. If you hear creaks or clicks, something isn't rotating freely — check the bearings and joints.
