# Cinema Hall Scene - Coordinate System and Layout

## Coordinate System

The 3D cinema hall uses a **right-handed coordinate system**:

```
        +Y (Up)
         ?
         ?
         ?
         ??????? +X (Right)
        /
       /
     +Z (Back, toward door)
```

### Axis Definitions
- **X axis:** Left (-X) / Right (+X)
- **Y axis:** Down (-Y) / Up (+Y)  
- **Z axis:** Front toward screen (-Z) / Back toward door (+Z)

### Key Reference Points
- **Origin (0, 0, 0):** Center of the hall at floor level
- **Screen location:** Front wall at **-Z** (negative Z)
- **Door location:** Back wall at **+Z** (positive Z)
- **Camera spawn:** **(0, 1.7, 8)** — Near back of hall, eye height, facing screen

---

## Hall Dimensions

### Overall Size
- **Width (X):** 18 meters (from -9 to +9)
- **Height (Y):** 4.5 meters (from 0.5 to 5.0)
- **Depth (Z):** 18 meters (from -9 to +9)

### Component Details

#### Floor
- **Position:** Y = 0.5 (slightly below ground reference)
- **Size:** 18m × 0.1m × 18m (wide, thin, deep)
- **Color:** Medium gray-blue (0.25, 0.25, 0.3)

#### Ceiling
- **Position:** Y = 5.0 (top of hall)
- **Size:** 18m × 0.1m × 18m
- **Color:** Light gray (0.3, 0.3, 0.32)

#### Walls
- **Thickness:** 0.2 meters
- **Height:** 4.5 meters (from floor to ceiling)
- **Color:** Dark gray (0.2, 0.2, 0.22)

**Wall Positions:**
- **Front Wall (screen wall):** Z = -9.0
- **Back Wall (door wall):** Z = +9.0
- **Left Wall:** X = -9.0
- **Right Wall:** X = +9.0

#### Screen
- **Position:** (0, 3.0, -8.79) — Centered on front wall, slightly in front
- **Size:** 8m wide × 3m tall × 0.05m thick
- **Color:** **Pure white (1.0, 1.0, 1.0)** ? CRITICAL
- **Note:** Positioned slightly in front of wall (Z = -8.79 instead of -9.0) to avoid z-fighting

#### Door Placeholder
- **Position:** (0, 1.75, 8.79) — Centered on back wall, slightly in front
- **Size:** 2m wide × 2.5m tall × 0.05m thick
- **Color:** Brown-ish (0.4, 0.25, 0.15)
- **Height:** Bottom at floor level, extends 2.5m upward

---

## Camera Bounds (AABB Collision)

The camera is constrained to stay inside the hall using an AABB (Axis-Aligned Bounding Box):

```cpp
AABB hallBounds(
    glm::vec3(-9.0f, 0.5f, -9.0f),   // Min corner
    glm::vec3(9.0f, 5.0f, 9.0f)      // Max corner
);
```

**With padding:** 0.3 meters from each wall

**Effective camera bounds:**
- X: -8.7 to +8.7
- Y: 0.8 to 4.7
- Z: -8.7 to +8.7

This matches the visible hall geometry, preventing the camera from walking through walls.

---

## Scene Objects (Rendering Order)

1. **Floor** — Rendered first (bottom layer)
2. **Ceiling** — Top layer
3. **Front Wall** — Screen wall (at -Z)
4. **Back Wall** — Door wall (at +Z)
5. **Left Wall** — Side wall (at -X)
6. **Right Wall** — Side wall (at +X)
7. **Screen** — White rectangle on front wall
8. **Door** — Brown rectangle on back wall

All objects use the same cube mesh (DebugCube) scaled to appropriate dimensions.

---

## Visual Layout (Top-Down View)

```
                        +Z (Back, Door)
                             ?
    ???????????????????????????????????????????????????
    ?                        ?                        ?
    ?                      DOOR                       ? ? Back Wall
    ?                   (Brown 2×2.5)                 ?
    ?                                                 ?
    ?                                                 ?
-X  ?                                                 ?  +X
    ?                   (0, 1.7, 8)                   ?
    ?                  CAMERA SPAWN                   ?
    ?                        ?                        ?
    ?                   (looking -Z)                  ?
    ?                                                 ?
    ?                                                 ?
    ?                   FLOOR (Gray)                  ?
    ?                                                 ?
    ?                                                 ?
    ?                     SCREEN                      ? ? Front Wall
    ?                 (White 8×3)                     ?
    ???????????????????????????????????????????????????
                             ?
                        -Z (Front, Screen)
```

---

## Coordinate Convention Summary

| Direction | Axis | Value Range |
|-----------|------|-------------|
| **Left** | -X | -9.0 to 0 |
| **Right** | +X | 0 to +9.0 |
| **Down** | -Y | Below 0.5 (outside bounds) |
| **Up** | +Y | 0.5 to 5.0 |
| **Forward (to screen)** | -Z | 0 to -9.0 |
| **Backward (to door)** | +Z | 0 to +9.0 |

---

## Implementation Notes

- All geometry uses **scaled cubes** from the existing DebugCube mesh
- No custom mesh generation required
- Reuses existing **basic.vert/basic.frag** shaders with MVP uniforms
- Solid colors only (no textures in Phase 3)
- Scene is **static** — no animation yet
- **Screen MUST be white** (requirement for later film projection in Phase 9)

---

**Created:** Phase 3 Implementation  
**Last Updated:** Auto-generated  
**Coordinate System:** Right-handed (X right, Y up, Z back)
