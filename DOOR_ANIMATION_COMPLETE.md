# Door Animation - Implementation

## What Changed

Vrata se sada fizi?ki otvaraju i zatvaraju sa smooth rotacijom umesto samo promene boje.

## Implementation Details

### Door Rotation
- **Hinge Position:** (-8.9, 1.75, -4.0) - ivica vrata
- **Closed Angle:** 0° - vrata zatvorena (paralelna sa zidom)
- **Open Angle:** 90° - vrata otvorena (normalno na zid)
- **Rotation Speed:** 90°/s - vrata se otvaraju za ~1 sekundu

### Animation System
```cpp
// Smooth interpolation
if (m_currentAngle != m_targetAngle)
{
    float diff = m_targetAngle - m_currentAngle;
    float step = m_rotationSpeed * deltaTime;
    
    if (abs(diff) < step)
        m_currentAngle = m_targetAngle;  // Snap to target
    else
        m_currentAngle += (diff > 0 ? step : -step);  // Move toward target
}
```

### Transformation Order
1. **Translate to hinge** - pomeranje na poziciju šarke
2. **Rotate around Y axis** - rotacija oko vertikale
3. **Offset from hinge** - pomeranje centra vrata od šarke
4. **Scale** - veli?ina vrata

```cpp
model = translate(hingePosition)
      * rotate(currentAngle, Y)
      * translate(offsetFromHinge)
      * scale(doorSize);
```

## Visual States

### Booking State
- **Angle:** 0° (closed)
- **Color:** Wood brown (0.5, 0.3, 0.2)

### Entering State
- **Angle:** 90° (open)
- **Animation:** Smooth rotation from 0° to 90°
- **Duration:** ~1 second

### Projection State
- **Angle:** 0° (closed)
- **Animation:** Smooth rotation from 90° to 0°
- **Duration:** ~1 second

### Exiting State
- **Angle:** 90° (open)
- **Animation:** Smooth rotation from 0° to 90°

## Files Modified

1. **Header/Door.h**
   - Added `update(float deltaTime)`
   - Added animation members: `m_currentAngle`, `m_targetAngle`, `m_rotationSpeed`
   - Added `m_hingePosition`
   - Removed color states (single color now)

2. **Source/Door.cpp**
   - Implemented `update()` with smooth interpolation
   - Updated `draw()` with hinge-based rotation
   - Changed `open()/close()` to set target angles

3. **Source/Application.cpp**
   - Added `m_door->update(dt)` call in main loop

## Build Status

? **Build:** Successful  
? **Animation:** Smooth 90° rotation  
? **Timing:** ~1 second open/close  
? **Physics:** Rotation around hinge point  

## Testing

### Test Animation
1. Start application (Booking state)
2. Vrata zatvorena (0°)
3. Reserve seats, press ENTER
4. Watch door smoothly rotate to 90° (~1s)
5. People enter
6. Watch door smoothly close to 0° when entering Projection

### Expected Behavior
- Smooth rotation (no jumps)
- 90° open position
- Rotation around left edge (hinge)
- Animation completes before state transitions

---

**Status:** ? COMPLETE
