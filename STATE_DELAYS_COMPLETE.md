# State Machine Delays - Implementation

## Overview

Dodati su delay-i izme?u otvaranja/zatvaranja vrata i akcija za realisti?niji flow.

## Delay Timeline

### 1. Booking ? Entering
```
0.0s: ENTER pressed
0.0s: Door starts opening (animate -90°)
1.0s: Door fully open (animation complete)
1.5s: People spawn at door
....: People walk to seats
X.Xs: All people seated ? Transition to Projection
```

### 2. Entering ? Projection
```
0.0s: All people seated
0.0s: Door starts closing (animate 0°)
1.0s: Door fully closed (animation complete)
1.5s: Film starts playing
20s: Film ends ? Transition to Exiting
```

### 3. Projection ? Exiting
```
0.0s: Film finished
0.0s: Door starts opening (animate -90°)
0.0s: People start exiting immediately
....: People walk to door
X.Xs: All people exited ? Transition to Reset
```

### 4. Exiting ? Reset
```
0.0s: All people exited
0.0s: Door starts closing (animate 0°)
1.0s: Door fully closed (animation complete)
1.5s: Reset complete ? Transition to Booking
```

## Implementation Details

### Constants
```cpp
const float DOOR_OPEN_DELAY = 1.5f;   // Wait for door to open
const float DOOR_CLOSE_DELAY = 1.5f;  // Wait for door to close
```

### State Flags
- **peopleSpawned** - Prevents multiple spawns in Entering
- **filmStarted** - Prevents multiple film starts in Projection
- **doorClosing** - Tracks door closing in Reset

### Entering State Logic
```cpp
if (m_stateTimer < DOOR_OPEN_DELAY)
    return;  // Wait for door

if (!peopleSpawned && m_stateTimer >= DOOR_OPEN_DELAY)
{
    // Spawn people once
    spawnPeople();
    peopleSpawned = true;
}

if (allSeated())
{
    peopleSpawned = false;  // Reset for next cycle
    enterState(Projection);
}
```

### Projection State Logic
```cpp
if (m_stateTimer < DOOR_CLOSE_DELAY)
    return;  // Wait for door

if (!filmStarted && m_stateTimer >= DOOR_CLOSE_DELAY)
{
    // Start film once
    screen->startPlayback();
    filmStarted = true;
}

if (!screen->isPlaying() && filmStarted)
{
    filmStarted = false;  // Reset for next cycle
    enterState(Exiting);
}
```

### Reset State Logic
```cpp
if (m_stateTimer < DOOR_CLOSE_DELAY)
{
    if (!doorClosing)
    {
        door->close();
        doorClosing = true;
    }
    return;  // Wait for door
}

// Door closed, reset everything
doorClosing = false;
clear();
resetSeats();
enterState(Booking);
```

## Visual Flow

```
BOOKING
  ? ENTER key
  ?
ENTERING
  0.0s: Door opens
  1.5s: People spawn
  X.Xs: People seated
  ?
PROJECTION
  0.0s: Door closes
  1.5s: Film starts
  21.5s: Film ends
  ?
EXITING
  0.0s: Door opens + People exit
  X.Xs: All exited
  ?
RESET
  0.0s: Door closes
  1.5s: Reset complete
  ?
BOOKING (loop)
```

## Timing Summary

| Transition | Door Action | Delay | Next Action |
|------------|-------------|-------|-------------|
| Booking ? Entering | Open | 1.5s | Spawn people |
| Entering ? Projection | Close | 1.5s | Start film |
| Projection ? Exiting | Open | 0s | Exit immediately |
| Exiting ? Reset | Close | 1.5s | Reset & Booking |

## Console Output

### Example Run
```
[STATE] Transition: Booking -> Entering
Door opening... waiting for people to enter
... (1.5s delay)
Door fully open - spawned 3 people for 5 occupied seats

[STATE] Transition: Entering -> Projection
Door closing... preparing projection
... (1.5s delay)
Door fully closed - film playback started (20 seconds)

[STATE] Transition: Projection -> Exiting
Door opening... people exiting

[STATE] Transition: Exiting -> Reset
Door closing... preparing reset
... (1.5s delay)
Reset complete - returning to Booking

[STATE] Transition: Reset -> Booking
```

## Files Modified

**Source/Application.cpp:**
1. `enterState()` - Removed immediate spawning/film start
2. `updateEnteringState()` - Added delay and spawn logic
3. `updateProjectionState()` - Added delay and film start logic
4. `updateResetState()` - Added delay before returning to Booking

## Build Status

? **Build:** Successful  
? **Door Delays:** 1.5s open/close wait  
? **People Spawn:** After door opens  
? **Film Start:** After door closes  
? **Reset:** After door closes  

## Testing

### Test Full Cycle
1. **Booking:** Select seats, press ENTER
2. **Entering:** 
   - See door open (1s animation)
   - Wait 1.5s
   - See people spawn
   - Watch people walk to seats
3. **Projection:**
   - See door close (1s animation)
   - Wait 1.5s
   - See film start
   - Wait 20s for film
4. **Exiting:**
   - See door open
   - See people exit immediately
   - Wait for all to exit
5. **Reset:**
   - See door close (1s animation)
   - Wait 1.5s
   - Return to Booking

### Expected Behavior
- No people spawn while door is opening
- No film starts while door is closing
- Smooth, realistic transitions
- Clear visual feedback for each phase

---

**Status:** ? COMPLETE
