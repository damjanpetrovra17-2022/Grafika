# Phase 11 Testing Checklist

## Quick Test Guide

This checklist helps verify Phase 11 implementation is working correctly.

---

## ? Pre-Test Setup

- [ ] Build successful (no compilation errors)
- [ ] Application starts and shows fullscreen window
- [ ] Console window visible for log messages

---

## ?? Test 1: Initial State Verification

**Steps:**
1. Start the application
2. Check console output immediately after startup

**Expected Console Output:**
```
[RENDER] Initial state: DepthTest=ON Culling=OFF
```

**Pass Criteria:**
- [ ] Console shows initial render state
- [ ] DepthTest=ON by default
- [ ] Culling=OFF by default

---

## ?? Test 2: Depth Test Toggle (Key D)

**Steps:**
1. Press `D` key
2. Check console output
3. Press `D` key again
4. Check console output

**Expected Console Output:**
```
[RENDER] DepthTest: OFF
[RENDER] DepthTest: ON
```

**Visual Verification:**
1. With depth test OFF, walk around the hall
2. You should see weird overlap (distant objects rendering on top of near objects)
3. Press `D` to turn depth test back ON
4. Objects should render correctly again

**Pass Criteria:**
- [ ] Console shows toggle messages
- [ ] Depth test OFF causes visible rendering issues
- [ ] Depth test ON restores correct rendering

---

## ?? Test 3: Back-Face Culling Toggle (Key C)

**Steps:**
1. Ensure depth test is ON (check debug print shows "Depth=ON")
2. Look at the front wall/screen
3. Press `C` key
4. Check console output
5. Compare visual appearance
6. Press `C` key again
7. Check console output

**Expected Console Output:**
```
[RENDER] Culling: ON (GL_BACK, CCW)
[RENDER] Culling: OFF
```

**Visual Verification:**
- With depth test ON, toggling culling should produce **NO visible difference**
- All walls, floor, ceiling, seats should look identical with culling ON or OFF

**Pass Criteria:**
- [ ] Console shows toggle messages
- [ ] With depth ON, culling toggle causes NO visible change (CRITICAL)

---

## ?? Test 4: Debug Print Includes Toggles

**Steps:**
1. Start application
2. Wait 1-2 seconds for debug print
3. Check console output

**Expected Console Output (example):**
```
[STATE] Booking | occupied=0 people=0 playing=0 | Depth=ON Cull=OFF
```

**Pass Criteria:**
- [ ] Debug print appears every second
- [ ] Shows "Depth=ON/OFF"
- [ ] Shows "Cull=ON/OFF"

---

## ?? Test 5: Toggles Work in All States

**Steps:**
1. Start in Booking state
2. Press `D` and `C` - verify toggles work
3. Reserve some seats (left-click on seats)
4. Press `ENTER` to start cinema cycle
5. **During Entering state:** Press `D` and `C`
6. **During Projection state:** Press `D` and `C`
7. **During Exiting state:** Press `D` and `C`

**Pass Criteria:**
- [ ] Toggles work in Booking state
- [ ] Toggles work in Entering state
- [ ] Toggles work in Projection state
- [ ] Toggles work in Exiting state
- [ ] Console feedback appears in all states

---

## ?? Test 6: Validation Requirement (CRITICAL)

**Goal:** Verify that toggling back-face culling with depth test ON produces no visible difference.

**Steps:**
1. Start application
2. Verify depth test is ON (check console or press `D` twice)
3. Position camera to see multiple objects (walls, floor, seats)
4. Note the current appearance
5. Press `C` to enable culling
6. Compare appearance - should be IDENTICAL
7. Press `C` to disable culling
8. Compare appearance - should still be IDENTICAL

**Why This Works:**
- Camera is inside the hall (sees front faces of walls)
- All meshes have consistent CCW winding order
- Back faces are behind front faces (already occluded by depth test)
- Culling removes geometry that's already invisible

**Pass Criteria:**
- [ ] **NO visible difference when toggling culling with depth ON**
- [ ] Scene renders correctly with culling ON
- [ ] Scene renders identically with culling OFF

---

## ?? Test 7: Edge Case - Depth OFF + Culling Toggle

**Purpose:** Verify culling actually works (diagnostic test)

**Steps:**
1. Press `D` to turn depth test OFF
2. Press `C` several times to toggle culling
3. Observe visual differences

**Expected:**
- With depth OFF + culling OFF: You should see back faces through walls (weird overlap)
- With depth OFF + culling ON: Back faces removed (cleaner but still has depth issues)
- This proves culling is actually working

**Pass Criteria:**
- [ ] Visual difference observed when toggling culling with depth OFF
- [ ] Proves culling functionality is working
- [ ] Return depth test to ON after this test (`D` key)

---

## ?? Test 8: Other Features Still Work

**Verify no regressions:**

**Camera:**
- [ ] Arrow keys move camera
- [ ] Mouse look rotates camera
- [ ] Camera collision still works (can't walk through walls)

**Seat Interaction (Booking state):**
- [ ] Left-click on seat toggles Free ? Reserved
- [ ] Keys 1-9 purchase adjacent seats
- [ ] Console feedback for seat actions

**State Machine:**
- [ ] ENTER starts cinema cycle
- [ ] Door opens/closes
- [ ] People spawn and walk to seats
- [ ] Film plays for 20 seconds
- [ ] People exit
- [ ] Cycle repeats

**Exit:**
- [ ] ESC key exits immediately from any state

**Pass Criteria:**
- [ ] All previous Phase features still work
- [ ] No regressions introduced

---

## ?? Final Validation

### All Tests Must Pass:

- [ ] Test 1: Initial State ?
- [ ] Test 2: Depth Test Toggle ?
- [ ] Test 3: Culling Toggle ?
- [ ] Test 4: Debug Print ?
- [ ] Test 5: All States ?
- [ ] Test 6: **Validation Requirement** ? (CRITICAL)
- [ ] Test 7: Edge Case ?
- [ ] Test 8: No Regressions ?

---

## ?? Acceptance Criteria Summary

| Criterion | Test | Status |
|-----------|------|--------|
| D toggles depth test | Test 2 | ? |
| Depth OFF shows visual issues | Test 2 | ? |
| C toggles culling | Test 3 | ? |
| Console feedback on toggle | Tests 2, 3 | ? |
| Debug print includes flags | Test 4 | ? |
| Works in all states | Test 5 | ? |
| **Culling toggle = no diff (depth ON)** | **Test 6** | ? |
| No regressions | Test 8 | ? |

---

## ?? Troubleshooting

### Issue: No console output when pressing D or C

**Solution:**
- Check that Input::isKeyPressed() is working (try other keys like ENTER)
- Verify handleRenderToggles() is called in main loop
- Check console window is visible

### Issue: Visible difference when toggling culling (depth ON)

**Possible Causes:**
1. Mesh winding is inconsistent (check DebugCube.cpp)
2. glFrontFace() not set to GL_CCW
3. Camera is outside the hall (not inside)

**Solution:**
- Verify DebugCube vertices are CCW
- Check glFrontFace(GL_CCW) is called when enabling culling
- Move camera inside hall bounds

### Issue: Depth test toggle doesn't show visual effect

**Solution:**
- Make sure you're looking at overlapping objects (e.g., multiple walls)
- Walk forward toward a wall to see overlap more clearly
- Check glClear() includes GL_DEPTH_BUFFER_BIT

---

## ? Sign-Off

**Tester Name:** _______________  
**Date:** _______________  
**All Tests Passed:** ? YES  ? NO  

**Notes:**
```
_________________________________________________________________
_________________________________________________________________
_________________________________________________________________
```

---

**Phase 11 Testing Complete!**

If all tests pass, Phase 11 is ready for integration into the final project.
