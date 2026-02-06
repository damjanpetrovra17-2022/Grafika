# Phase 2 Visual Testing Guide

## Quick Start

1. **Build the project:**
   ```bash
   cmake --build build --config Debug
   ```

2. **Run the application:**
   ```bash
   ./build/Debug/kostur.exe
   ```
   OR in Visual Studio: Press F5

3. **You should see:**
   - Fullscreen window
   - Dark blue-gray background
   - Red cube at center
   - Gray floor plane
   - 4 colored cubes at corners (green, blue, yellow, purple)

---

## Visual Test Scenarios

### Test 1: Mouse Look (30 seconds)

**What to do:**
1. Move mouse **left** slowly
2. Move mouse **right** slowly
3. Move mouse **up** slowly
4. Move mouse **down** slowly

**Expected result:**
- ? View rotates smoothly in all directions
- ? Looking up/down stops at steep angle (cannot flip upside down)
- ? Red center cube and corner markers rotate around you
- ? No stuttering or jumps

**Common issues:**
- ? If view jumps on first mouse move ? Input first-frame stability broken
- ? If you can flip upside down ? Pitch clamping not working
- ? If mouse sensitivity too high/low ? Adjust `m_mouseSensitivity` in Camera.cpp

---

### Test 2: Forward Movement (60 seconds)

**What to do:**
1. **Look at the red center cube** (should be visible ahead when you start)
2. **Hold Arrow Up (or W)** and move toward it
3. **Observe:** The cube gets larger as you approach
4. **Stop** when you're close to it
5. **Rotate 90° to the right** with mouse
6. **Hold Arrow Up (or W) again**
7. **Observe:** You move in the NEW direction (not toward the cube)

**Expected result:**
- ? You move **forward in the direction you're looking**
- ? Movement is smooth (not choppy)
- ? After rotating, movement follows the new view direction
- ? This proves movement is relative to camera, not fixed world axes

**Common issues:**
- ? If you always move toward -Z regardless of rotation ? Movement using world axes instead of camera direction
- ? If movement is choppy ? DeltaTime not integrated or frame limiter issue

---

### Test 3: Strafe Movement (60 seconds)

**What to do:**
1. **Look at the red center cube**
2. **Hold Arrow Left (or A)** for 2 seconds
3. **Observe:** The cube moves to your **right** in the view (you move left)
4. **Release and hold Arrow Right (or D)** for 2 seconds
5. **Observe:** The cube moves to your **left** in the view (you move right)
6. **While strafing, rotate the view with mouse**
7. **Observe:** Strafe direction updates as you rotate

**Expected result:**
- ? Strafing moves **perpendicular** to your view direction
- ? Strafe direction rotates with your view
- ? You can strafe while looking around

**Common issues:**
- ? If strafing moves forward/back ? Wrong vector used (front instead of right)
- ? If strafe doesn't rotate with view ? Using world X axis instead of camera right

---

### Test 4: Collision with Walls (90 seconds)

**What to do:**
1. **Move toward the green corner cube** (front-left corner at (-8, 0.5, -8))
2. **Keep holding the movement key** even after you get close
3. **Observe:** You stop moving when you get near the corner
4. **Try to move closer** by holding the key
5. **Verify:** You cannot get any closer (camera is blocked)
6. **Repeat** for the other 3 corners:
   - Blue (front-right)
   - Yellow (back-left)
   - Purple (back-right)

**Expected result:**
- ? Camera stops approximately 0.3 meters from each corner cube
- ? Cannot pass through the boundary
- ? Camera position is clamped smoothly (no jitter)

**Common issues:**
- ? If you can move through corners ? AABB clamp not applied
- ? If camera jitters near boundary ? Clamping applied incorrectly
- ? If you stop too far from corner ? Padding value too large

---

### Test 5: Floor and Ceiling Bounds (30 seconds)

**What to do:**
1. **Note:** In the current implementation, Y movement is locked to ground plane
2. **If vertical movement is added** (Space = up, Ctrl = down):
   - Try to move below the floor (Y < 0.5)
   - Try to move above the ceiling (Y > 5.0)
3. **Verify:** Camera Y is clamped to stay inside the hall

**Expected result:**
- ? Cannot go below Y = 0.8 (floor + padding)
- ? Cannot go above Y = 4.7 (ceiling - padding)

**Current status:**
- Y movement not implemented yet (camera stays at Y = 1.7)
- AABB clamp is ready and will work when vertical movement is added

---

### Test 6: Pitch Clamping (30 seconds)

**What to do:**
1. **Move mouse up continuously** for several seconds
2. **Try to look straight up**
3. **Observe:** View stops at a steep upward angle (cannot flip)
4. **Move mouse down continuously** for several seconds
5. **Try to look straight down**
6. **Observe:** View stops at a steep downward angle

**Expected result:**
- ? Pitch is clamped to approximately ±89°
- ? You cannot flip upside down
- ? Camera up vector remains stable

**Visual check:**
- When looking up: Floor should still be visible at bottom of screen
- When looking down: Sky should still be visible at top of screen
- You should **never** see an inverted view

---

### Test 7: ESC Exit (10 seconds)

**What to do:**
1. **Press ESC** at any time during the tests

**Expected result:**
- ? Application exits **immediately**
- ? No delay or confirmation dialog

**Common issues:**
- ? If ESC doesn't work ? Window callback not set up correctly
- ? If there's a delay ? Event polling issue

---

### Test 8: Fullscreen and FPS (Passive observation)

**What to do:**
- Just observe while doing the other tests

**Expected result:**
- ? Application runs in **fullscreen** mode (entire screen, no window border)
- ? Movement is **smooth** at 75 FPS
- ? No visible stuttering or frame drops

**How to verify FPS:**
- If FrameLimiter has logging enabled, check console output
- OR: Use external tool like FRAPS/RivaTuner to monitor FPS
- Should be stable at ~75 FPS (may fluctuate slightly due to VSync)

---

## Expected Visual Layout

```
           Ceiling (Y = 5.0)
    ???????????????????????????????
    ?  Yellow          Purple     ?  Back wall (Z = 9.0)
    ?  Corner          Corner     ?
    ?                             ?
    ?         Gray Floor          ?
    ?    (large flat cube)        ?
    ?                             ?
    ?      Red Cube               ?
    ?    (at center)              ?
    ?                             ?
    ?  Green           Blue       ?  Front wall (Z = -9.0)
    ?  Corner          Corner     ?
    ???????????????????????????????
           Floor (Y = 0.5)

    Left wall (X = -9.0)      Right wall (X = 9.0)

Camera spawn: (0, 1.7, 8) looking toward -Z
```

---

## Troubleshooting

### Issue: Nothing renders (black screen)

**Possible causes:**
1. Shader compilation failed
2. Camera outside view frustum
3. Depth testing issue

**How to fix:**
- Check console logs for shader errors
- Verify camera position is (0, 1.7, 8)
- Ensure `glEnable(GL_DEPTH_TEST)` is called

---

### Issue: Camera doesn't move

**Possible causes:**
1. Input not updating
2. DeltaTime is zero
3. Camera update not called

**How to fix:**
- Add `printf("deltaTime: %f\n", deltaTime)` to verify time is advancing
- Add `printf("Input::isKeyDown(UP): %d\n", Input::isKeyDown(GLFW_KEY_UP))` to verify input
- Check that `camera->update(deltaTime)` is in the main loop

---

### Issue: Movement feels wrong (too fast/slow)

**How to fix:**
- Adjust `m_moveSpeed` in `Camera.cpp` (default: 3.0)
- Verify deltaTime is in seconds (not milliseconds)

---

### Issue: Mouse sensitivity too high/low

**How to fix:**
- Adjust `m_mouseSensitivity` in `Camera.cpp` (default: 0.1)
- Lower value = slower rotation
- Higher value = faster rotation

---

## Success Criteria

All tests pass if:
- ? Mouse look rotates view smoothly with clamped pitch
- ? Arrow keys move in camera view direction (not fixed world axes)
- ? Strafe moves perpendicular to view
- ? Camera cannot pass through hall boundaries
- ? ESC exits immediately
- ? Fullscreen at 75 FPS

If all 8 tests pass ? **Phase 2 is verified and working correctly** ?

---

**Next:** Proceed to Phase 3 (Basic Scene with textured walls/floor/ceiling)
