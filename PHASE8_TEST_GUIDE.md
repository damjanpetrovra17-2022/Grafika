# PHASE 8 QUICK TEST GUIDE

## How to Test Phase 8 - People System

### Prerequisites
- Application built successfully
- ESC key works to exit

### Test 1: Basic Spawning

**Steps:**
1. Run `kostur.exe`
2. Reserve some seats (click on green seats to make them yellow)
3. Press **P** key

**Expected Result:**
```
Console: "Spawned X people for Y occupied seats"
```
- Colored cuboids appear at door position (left wall, near Z=-5)
- Each person has different random color
- Number of people ? number of occupied seats

**Visual Check:**
- People are visible as vertical cuboids (0.4×1.7×0.4)
- Different colors (random RGB)
- Standing at door location

---

### Test 2: Movement Stages

**Steps:**
1. Reserve seats in different rows (front and back)
2. Press **P** to spawn people
3. Watch movement unfold

**Expected Stages:**

**Stage 1 - ToRowDepth (Aisle Walk):**
- People move along left side of hall
- Z coordinate changes toward row position
- X stays at -7.0 (aisle)
- Y stays near door height

**Stage 2 - ClimbToRowHeight (Step Climbing):**
- People stop moving in Z
- Y coordinate increases
- Back row people climb higher than front row
- X and Z remain constant

**Stage 3 - ToSeatX (Row Crossing):**
- People move horizontally across row
- X coordinate changes toward seat X
- Y and Z remain constant
- People approach their seats

**Stage 4 - Seated:**
- People stop moving
- People appear "sitting" on seats
- Slight Y offset above seat top

---

### Test 3: Seat Assignment Uniqueness

**Steps:**
1. Purchase exactly 3 seats (use keys 1, 2, or 3)
2. Press **P** multiple times (spawn several batches)

**Expected:**
- Each spawn assigns different random seats
- People never target the same seat simultaneously
- All 3 people reach different seats

**Console Check:**
```
"Spawned 2 people for 3 occupied seats"  (random 1-3)
"Spawned 1 people for 3 occupied seats"  (random 1-3)
"Spawned 3 people for 3 occupied seats"  (all 3)
```

---

### Test 4: Clear Functionality

**Steps:**
1. Spawn some people (press P)
2. Wait for movement to start
3. Press **O** key

**Expected:**
```
Console: "Cleared all people"
```
- All people disappear instantly
- Screen shows only seats and hall
- Can spawn new people again

---

### Test 5: Step Climbing Verification

**Steps:**
1. Purchase ONLY back row seats (row 4)
2. Press **P**
3. Watch people climb

**Expected:**
- People walk along aisle normally (Stage 1)
- People climb significantly upward (Stage 2)
  - Y goes from ~1.7 to ~2.2 (0.5m climb)
  - Climbing is visible and smooth
- People reach elevated platform at back

**Visual:**
```
     ????  ? Row 4 (highest)
    ????  ? People climbing here
   /
  /
 /        ? Aisle path
?? ? Door
```

---

### Test 6: Mixed Row Heights

**Steps:**
1. Purchase seats in rows 0, 2, and 4 (front, middle, back)
2. Press **P** to spawn 3+ people
3. Observe different climb heights

**Expected:**
- Person targeting row 0: minimal climb
- Person targeting row 2: medium climb  
- Person targeting row 4: maximum climb
- All reach correct row heights

---

### Test 7: Constant Speed Movement

**Steps:**
1. Spawn people
2. Watch one person throughout entire journey
3. Observe movement smoothness

**Expected:**
- No sudden jumps or teleports
- Constant velocity in each stage
- Smooth transitions between stages
- Framerate stays at 75 FPS

---

### Test 8: Multiple Spawn Cycles

**Steps:**
1. Spawn people (P)
2. Wait for all to reach seats
3. Clear (O)
4. Spawn again (P)
5. Repeat 3-4 times

**Expected:**
- Each cycle works identically
- No memory leaks or slowdowns
- People always spawn at door
- Movement remains smooth

---

### Debug Console Output

**What to Look For:**

```
[INFO] Spawned X people for Y occupied seats
```
- X = number spawned (random between 1 and Y)
- Y = total occupied seats

```
[INFO] Cleared all people
```
- Confirms clear command executed

```
[INFO] No occupied seats - cannot spawn people
```
- If you press P with no reserved/purchased seats

---

### Visual Validation Checklist

**People Appearance:**
- [ ] People are visible as colored cuboids
- [ ] Different random colors per person
- [ ] Height is taller than width/depth (1.7 vs 0.4)
- [ ] People are lit by room light (not solid color)

**Movement Path:**
- [ ] Stage 1: Left aisle walk (Z changes)
- [ ] Stage 2: Upward climb (Y changes)
- [ ] Stage 3: Row crossing (X changes)
- [ ] Stage 4: Seated (no movement)

**Seating:**
- [ ] People stop at correct seats
- [ ] People appear "sitting" on seats
- [ ] No floating or intersecting seats
- [ ] Each person has unique target

---

### Performance Check

**Monitor During Test:**
- FPS should stay at 75
- No stuttering or lag
- ESC exits immediately
- Camera movement unaffected

**With 50 People:**
- Application should remain smooth
- No visible performance degradation

---

### Common Issues

**Issue: No people spawn when pressing P**
- Check: Are any seats reserved/purchased?
- Console should say: "No occupied seats"
- Fix: Reserve some seats first

**Issue: People don't move**
- Check: Is deltaTime working? (should see debug output every 100 frames)
- Check: Are people actually spawned? (console message)

**Issue: People teleport instead of walk**
- Check: Speed value (should be 2.0 units/sec)
- Check: EPSILON threshold (should be 0.05)

**Issue: Multiple people go to same seat**
- Bug: Seat assignment not unique
- Should never happen (shuffle guarantees uniqueness)

---

### Success Criteria

All tests pass if:
? People spawn at door on P key
? People move in staged order (Z ? Y ? X)
? Step climbing is visible for elevated rows
? Each person reaches unique target seat
? People stop when seated
? O key clears all people
? Application remains at 75 FPS
? ESC still exits

---

### Next Steps

After verifying Phase 8:
1. Phase 9: Implement screen projection (20 textures, 20 seconds)
2. Phase 10: State machine (Booking ? Entering ? Projection ? Exiting)

---

**Phase 8 Test Status:** Ready for manual testing
**Build:** ? Successful
**Run:** `.\out\build\x64-Debug\Debug\kostur.exe`
