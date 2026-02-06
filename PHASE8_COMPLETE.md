# PHASE 8 COMPLETE - People System

## Overview

Phase 8 implements the people spawning and movement system. People spawn at the door and move to their assigned seats using a 3-stage movement path that includes:
1. Walking along the aisle to the target row depth (Z)
2. Climbing to the target row height (Y) 
3. Moving across the row to the seat (X)
4. Sitting at the seat

## Implementation

### Files Created

**Header Files:**
- `Header/Person.h` - Individual person class with staged movement
- `Header/PeopleManager.h` - Manager for spawning and updating all people

**Source Files:**
- `Source/Person.cpp` - Person movement logic implementation
- `Source/PeopleManager.cpp` - People management and rendering

### Data Structures

#### MovementStage Enum
```cpp
enum class MovementStage
{
    ToRowDepth,       // Stage 1: Move along aisle to target row Z
    ClimbToRowHeight, // Stage 2: Climb to target row Y
    ToSeatX,          // Stage 3: Move across row to seat X
    Seated,           // Stage 4: Seated at target
    Exited            // Stage 5: Exited (for Phase 10)
};
```

#### TravelMode Enum
```cpp
enum class TravelMode
{
    Entering,  // Moving from door to seat
    Exiting    // Moving from seat to door (Phase 10)
};
```

#### Person Class
**Key Members:**
- `glm::vec3 m_position` - Current world position
- `glm::vec3 m_doorPos` - Door spawn/exit point
- `Seat* m_targetSeat` - Assigned seat pointer
- `MovementStage m_stage` - Current movement stage
- `TravelMode m_mode` - Entering or Exiting
- `float m_speed` - Movement speed (2.0 units/sec)
- `glm::vec3 m_baseColor` - Random color for visual differentiation

**Key Methods:**
- `update(float deltaTime)` - Updates movement based on current stage
- `isSeated()` - Returns true when person reached seat
- `startExiting()` - Triggers exit sequence (Phase 10)

#### PeopleManager Class
**Key Methods:**
- `spawnPeople(int count, SeatGrid&, doorPos)` - Spawn specific count
- `spawnPeopleRandom(SeatGrid&, doorPos, min, max)` - Spawn random count
- `clear()` - Remove all people
- `update(float dt)` - Update all people movement
- `draw(Shader&, ...)` - Render all people as cuboids
- `allSeated()` - Check if all people are seated
- `allExited()` - Check if all people have exited (Phase 10)

### Movement System

#### Staged Movement Path (Entering)

**Stage 1: ToRowDepth**
```
- Keep X at AISLE_X (-7.0)
- Keep Y at current height
- Move Z toward targetSeat.position.z
- When |current.z - target.z| < EPSILON: advance to Stage 2
```

**Stage 2: ClimbToRowHeight**
```
- Keep X and Z constant
- Move Y toward targetSeat.position.y
- When |current.y - target.y| < EPSILON: advance to Stage 3
```

**Stage 3: ToSeatX**
```
- Keep Y and Z constant  
- Move X toward targetSeat.position.x
- When |current.x - target.x| < EPSILON: snap to seat and mark Seated
```

**Stage 4: Seated**
```
- Final position = seatPos + (0, personHeight*0.5, 0)
- Person appears sitting on seat
- Movement stops
```

#### Movement Constants
```cpp
static constexpr float EPSILON = 0.05f;      // Arrival threshold
static constexpr float AISLE_X = -7.0f;      // Left aisle position
float m_speed = 2.0f;                        // Movement speed (units/sec)
```

#### Movement Algorithm
```cpp
bool moveToward(float& current, float target, float speed, float dt)
{
    float diff = target - current;
    
    if (abs(diff) < EPSILON)
    {
        current = target;
        return true;  // Reached
    }
    
    float step = speed * dt;
    current += (diff > 0 ? step : -step);
    return false;
}
```

### Seat Assignment

#### Uniqueness Guarantee

**Algorithm:**
1. Collect all seats where state is Reserved OR Purchased
2. Clamp spawn count to <= occupied seats count
3. Shuffle seat list using std::mt19937 RNG
4. Assign first K seats to people
5. Each person gets unique seat (no duplicates)

**Code:**
```cpp
// Collect occupied seats
std::vector<Seat*> occupiedSeats;
for each seat:
    if (state == Reserved || state == Purchased)
        occupiedSeats.push_back(seat);

// Shuffle for randomness
std::shuffle(occupiedSeats.begin(), occupiedSeats.end(), rng);

// Assign first K seats (guarantees uniqueness)
for (int i = 0; i < count; ++i)
    spawn person with targetSeat = occupiedSeats[i];
```

### Rendering

#### Cuboid Representation
People are rendered as colored cuboids using the existing DebugCube mesh:

**Dimensions:**
```cpp
static constexpr float PERSON_WIDTH = 0.4f;   // X
static constexpr float PERSON_HEIGHT = 1.7f;  // Y (human height)
static constexpr float PERSON_DEPTH = 0.4f;   // Z
```

**Color:**
- Each person gets a random base color (RGB in range 0.3-0.9)
- Uses Phong shader with room lighting
- People are lit like other scene objects

**Rendering Code:**
```cpp
for each person:
    model = translate(person.position)
    model = scale(PERSON_WIDTH, PERSON_HEIGHT, PERSON_DEPTH)
    shader.setMat4("model", model)
    shader.setVec3("uBaseColor", person.color)
    cubeMesh.draw()
```

### Integration

#### Application Changes

**Added Members:**
```cpp
std::unique_ptr<PeopleManager> m_peopleManager;
```

**Main Loop Updates:**
```cpp
// Update people movement
m_peopleManager->update(Time::deltaTime());

// Draw people after seats
m_peopleManager->draw(*m_phongShader, view, projection, viewPos, *m_debugCube);
```

### Debug Controls

#### Key Bindings

**P Key - Spawn Random People**
```
- Counts occupied seats (Reserved + Purchased)
- If occupiedCount > 0:
    - Generates random K in range [1, occupiedCount]
    - Spawns K people at door
    - Each assigned unique random seat
- Console: "Spawned X people for Y occupied seats"
```

**O Key - Clear All People**
```
- Removes all spawned people
- Clears internal people vector
- Console: "Cleared all people"
```

**Edge-Triggered:**
Both keys use `Input::isKeyPressed()` for one-shot activation (not continuous).

#### Door Position
```cpp
glm::vec3 doorPos(-8.5f, 1.7f, -5.0f);  // Near left wall door
```

### Testing

#### Test Scenario 1: Basic Spawning
1. Start application
2. Reserve or purchase some seats (click seats, or use 1-9 keys)
3. Press **P** to spawn people
4. **Expected:**
   - People appear at door position
   - People count logged to console
   - People are visible as colored cuboids

#### Test Scenario 2: Movement Stages
1. Spawn people (press P)
2. Watch people movement:
   - **Stage 1:** People move along left aisle (Z changes)
   - **Stage 2:** People climb up steps (Y increases)
   - **Stage 3:** People move across row (X changes to seat position)
   - **Stage 4:** People stop at seats
3. **Expected:**
   - Smooth constant-speed movement
   - No teleporting or jittering
   - People reach correct target seats

#### Test Scenario 3: Seat Assignment
1. Purchase 5 specific seats (e.g., front row)
2. Press P multiple times
3. **Expected:**
   - Each spawn assigns different random seats
   - No two people target the same seat
   - All people go to occupied seats only

#### Test Scenario 4: Clear Functionality
1. Spawn people
2. Wait for some to reach seats
3. Press **O** to clear
4. **Expected:**
   - All people disappear immediately
   - Console: "Cleared all people"
   - Can spawn new people again

#### Test Scenario 5: Step Climbing
1. Purchase seats in back rows (rows 3-4)
2. Spawn people
3. **Expected:**
   - People visibly climb higher as they approach back rows
   - Y position increases in Stage 2 (ClimbToRowHeight)
   - People arrive at elevated platform level

### Status Queries

**PeopleManager provides:**
```cpp
bool allSeated() const;       // True when all people seated
bool allExited() const;       // True when all people exited (Phase 10)
int getPeopleCount() const;   // Current number of people
```

**Usage in Phase 10:**
```cpp
if (peopleManager.allSeated())
    // Trigger projection start

if (peopleManager.allExited())
    // Reset to booking mode
```

### Exit System (Phase 10 Ready)

#### Reverse Path Structure

**Exiting stages (reverse of entering):**
1. **ToSeatX:** Move from seat.x back to AISLE_X
2. **ClimbToRowHeight:** Move down to door height
3. **ToRowDepth:** Move along aisle back to door.z
4. **Exited:** Marked as exited

**Trigger:**
```cpp
peopleManager.startExiting();  // Commands all people to exit
```

**Implementation Status:**
- Structure complete in `Person::updateExiting()`
- Can be activated via `startExiting()` method
- Ready for Phase 10 state machine integration

### Performance

**Rendering Cost:**
- 1 draw call per person
- Uses shared DebugCube mesh (no extra geometry)
- Phong shader (same as seats/scene)

**Typical Load:**
- 50 seats ? max 50 people
- ~50 draw calls for people rendering
- Negligible CPU cost for movement updates

### Known Limitations

1. **No Collision Between People**
   - People can overlap during movement
   - Acceptable for Phase 8 (simple movement demo)

2. **Fixed Aisle Position**
   - All people use same aisle (X = -7.0)
   - Could be extended with multiple aisles

3. **Cuboid Models**
   - People rendered as simple cuboids
   - Humanoid model loading deferred (can add later)

4. **No Gravity**
   - People move in straight lines per axis
   - Step climbing is instant height change

### Acceptance Criteria

? **People spawn at door position**
- Door position defined near left wall
- People appear at correct location

? **Staged movement works**
- Z ? Y ? X order enforced
- Each stage completes before next begins

? **Step climbing visible**
- Y increases for farther rows
- People arrive at correct platform height

? **No duplicate seats**
- Unique seat assignment guaranteed
- Shuffle algorithm ensures randomness

? **People reach correct seats**
- Final position matches target seat
- People stop at Seated stage

? **Cuboid rendering works**
- People visible and lit
- Different colors for visual differentiation

? **Debug keys functional**
- P spawns people (edge-triggered)
- O clears people (edge-triggered)
- Console feedback provided

? **App remains stable**
- ESC still exits
- 75 FPS limiter maintained
- No crashes or hangs

### Integration with Future Phases

**Phase 9 (Projection):**
- No changes needed to people system
- People remain seated during film

**Phase 10 (State Machine):**
- Use `allSeated()` to trigger projection
- Use `startExiting()` after film ends
- Use `allExited()` to reset to booking
- Clear people on reset

### Files Modified

**New Files (4):**
1. `Header/Person.h`
2. `Source/Person.cpp`
3. `Header/PeopleManager.h`
4. `Source/PeopleManager.cpp`

**Modified Files (3):**
1. `Header/Application.h` - Added PeopleManager member
2. `Source/Application.cpp` - Integration + debug keys
3. `CMakeLists.txt` - Added new source files

**Lines Added:** ~400 total

### Build Status

? **Build Successful**
- All files compile without errors
- No warnings
- CMake configuration updated

### Summary

Phase 8 - People System is **COMPLETE**. The system provides:

- ? Person class with staged movement logic
- ? PeopleManager for spawning and rendering
- ? Unique seat assignment with shuffle algorithm
- ? Three-stage movement path (Z ? Y ? X)
- ? Step climbing animation
- ? Cuboid rendering fallback
- ? Debug controls (P spawn, O clear)
- ? Ready for Phase 10 state machine integration
- ? Exit system structure in place

**Next Phase:** Phase 9 - Projection Simulation (screen textures + timing)

---

**Date:** Auto-generated
**Phase:** 8 - People System
**Status:** ? COMPLETE
**Build:** ? Successful
**Ready for:** Phase 9 and Phase 10 integration
