# PHASE 10 COMPLETE - Cinema State Machine

## Overview

Phase 10 implements a deterministic state machine that controls the full cinema cycle:

```
Booking ? Entering ? Projection ? Exiting ? Reset ? Booking (loop)
```

## State Definitions

### 1. Booking State
- **Seat interaction:** ENABLED (click to toggle Reserved/Free)
- **Purchase keys (1-9):** ENABLED
- **People:** None spawned
- **Door:** CLOSED
- **Room Light:** ON
- **Screen Light:** OFF
- **Screen:** White (not playing)
- **Exit condition:** Press ENTER with occupiedCount > 0

### 2. Entering State
- **Seat interaction:** DISABLED
- **Purchase keys:** DISABLED
- **People:** Spawned (random K where 1 ? K ? occupiedCount)
- **Door:** OPEN
- **Room Light:** ON
- **Screen Light:** OFF
- **Screen:** White
- **Exit condition:** All people seated (allSeated() == true)

### 3. Projection State
- **Seat interaction:** DISABLED
- **Purchase keys:** DISABLED
- **People:** Seated, watching film
- **Door:** CLOSED
- **Room Light:** OFF
- **Screen Light:** ON
- **Screen:** Playing film (20 seconds, 100 frames)
- **Exit condition:** Film ends (isPlaying() == false)

### 4. Exiting State
- **Seat interaction:** DISABLED
- **Purchase keys:** DISABLED
- **People:** Walking back to door (reverse path)
- **Door:** OPEN
- **Room Light:** ON
- **Screen Light:** OFF
- **Screen:** White
- **Exit condition:** All people exited (allExited() == true)

### 5. Reset State
- **Actions performed:**
  - Clear all people
  - Reset all seats to Free
  - Stop/reset screen to white
  - Close door
- **Exit condition:** Immediate transition to Booking

## Key Bindings

| Key | State | Action |
|-----|-------|--------|
| **ENTER** | Booking only | Start cinema cycle (if seats occupied) |
| **ESC** | All states | Exit application immediately |
| **Left Click** | Booking only | Toggle seat (Free ? Reserved) |
| **1-9** | Booking only | Purchase N adjacent seats |
| **WASD** | All states | Camera movement |
| **Mouse** | All states | Camera look |

## Lighting Behavior

| State | Room Light | Screen Light |
|-------|------------|--------------|
| Booking | ON | OFF |
| Entering | ON | OFF |
| Projection | OFF | ON |
| Exiting | ON | OFF |
| Reset | ON | OFF |

## People Movement

### Entering (Door ? Seat)
1. **Stage 1 (ToRowDepth):** Walk along aisle (Z changes)
2. **Stage 2 (ClimbToRowHeight):** Climb to row height (Y changes)
3. **Stage 3 (ToSeatX):** Walk across row to seat (X changes)
4. **Stage 4 (Seated):** Stop at seat

### Exiting (Seat ? Door)
1. **Stage 1 (ToSeatX):** Walk back to aisle (X changes)
2. **Stage 2 (ClimbToRowHeight):** Descend to door height (Y changes)
3. **Stage 3 (ToRowDepth):** Walk along aisle to door (Z changes)
4. **Stage 4 (Exited):** Reached door

## Door Component

- **Position:** Left wall, near front (-8.9, 1.75, -5.0)
- **Size:** 0.1m × 2.5m × 2.0m
- **Color when closed:** Brown (0.4, 0.25, 0.15)
- **Color when open:** Green (0.2, 0.5, 0.2)

## Console Output

Debug prints once per second:
```
[STATE] Booking | occupied=5 people=0 playing=0
[STATE] Entering | occupied=5 people=3 playing=0
[STATE] Projection | occupied=5 people=3 playing=1
[STATE] Exiting | occupied=5 people=3 playing=0
[STATE] Reset | occupied=5 people=3 playing=0
[STATE] Booking | occupied=0 people=0 playing=0
```

## Test Scenario

### Full Loop Test

1. **Start Application**
   - State: Booking
   - All seats Free (green)
   - Room light ON
   - Screen white

2. **Reserve/Purchase Seats**
   - Click seats to reserve (yellow)
   - Or press 1-9 to purchase adjacent (red)
   - Verify occupied count increases

3. **Press ENTER**
   - Console: "ENTER pressed - starting cinema cycle with X occupied seats"
   - Door opens (turns green)
   - People spawn at door
   - State: Entering

4. **Watch People Enter**
   - People walk along aisle
   - People climb to row heights
   - People cross rows to seats
   - When all seated ? State: Projection

5. **Watch Film (20 seconds)**
   - Door closes
   - Room light OFF (dark)
   - Screen light ON
   - Film frames play
   - After 20s ? State: Exiting

6. **Watch People Exit**
   - Door opens
   - Room light ON
   - People leave seats
   - People walk back to door
   - When all exited ? State: Reset

7. **Reset**
   - People cleared
   - All seats reset to Free
   - Screen white
   - Door closed
   - State: Booking

8. **Repeat**
   - Can select new seats and start again
   - Full loop works indefinitely

## Files Created/Modified

### New Files
- `Header/AppState.h` - State enum
- `Header/Door.h` - Door component header
- `Source/Door.cpp` - Door component implementation

### Modified Files
- `Header/Application.h` - Added state machine members
- `Source/Application.cpp` - Complete state machine implementation
- `CMakeLists.txt` - Added Door.cpp and AppState.h

### Existing (already had support)
- `Header/Person.h` - Already had Exiting support
- `Source/Person.cpp` - Already had updateExiting()
- `Header/PeopleManager.h` - Already had allSeated(), allExited(), startExiting()
- `Source/PeopleManager.cpp` - Already had exit support

## Acceptance Criteria

? Full loop works repeatedly without restart  
? Room light OFF during projection, ON otherwise  
? Screen plays during projection (20s), resets to white after  
? People count never exceeds occupied seat count  
? No duplicate seat assignments  
? Seat interactions disabled outside Booking  
? ESC exits immediately in every state  
? ENTER only works in Booking with occupied seats  

## Architecture

```
Application
    ??? AppState (enum)
    ??? updateStateMachine(dt)
    ?   ??? updateBookingState()
    ?   ??? updateEnteringState()
    ?   ??? updateProjectionState()
    ?   ??? updateExitingState()
    ?   ??? updateResetState()
    ??? enterState(newState)
    ?   ??? setLightingForState(state)
    ??? Door
    ?   ??? open()/close()
    ?   ??? draw()
    ??? PeopleManager
    ?   ??? spawnPeopleRandom()
    ?   ??? startExiting()
    ?   ??? allSeated()
    ?   ??? allExited()
    ??? Screen
        ??? startPlayback()
        ??? stopAndResetToWhite()
        ??? isPlaying()
```

## Summary

Phase 10 successfully implements the complete cinema simulation cycle with:
- Deterministic state machine
- Proper lighting transitions
- People movement (entering and exiting)
- Film playback
- Seat management
- Door visualization
- Full loop capability

**Status:** ? COMPLETE
