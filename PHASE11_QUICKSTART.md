# Phase 11 - Quick Start Guide

## ?? How to Test Phase 11 Immediately

### Step 1: Build and Run
```
1. Build the project (should succeed with no errors)
2. Run the application
3. Console window should appear with fullscreen 3D cinema hall
```

### Step 2: Verify Initial State
**Look for this console message:**
```
[RENDER] Initial state: DepthTest=ON Culling=OFF
```

? **Depth test is ON by default** (correct 3D rendering)  
? **Culling is OFF by default** (safe mode)

---

### Step 3: Test Depth Test Toggle (Key D)

**Action:** Press `D` key

**Expected Console Output:**
```
[RENDER] DepthTest: OFF
```

**Visual Effect:**
- Walk around the hall with arrow keys
- You should see **weird overlap** - distant objects rendering on top of near objects
- This proves depth test is actually disabled

**Action:** Press `D` again

**Expected Console Output:**
```
[RENDER] DepthTest: ON
```

**Visual Effect:**
- Rendering returns to normal
- Objects at correct depth

? **Test PASS if:** Console shows toggle messages AND visual effect is obvious

---

### Step 4: Test Culling Toggle (Key C)

**CRITICAL: Make sure depth test is ON first!**

**Action:** Press `D` twice if needed to ensure depth test is ON

**Action:** Look at the front wall/screen

**Action:** Press `C` key

**Expected Console Output:**
```
[RENDER] Culling: ON (GL_BACK, CCW)
```

**Visual Effect:**
- Should be **NO visible difference**
- Walls, floor, ceiling, seats should look identical

**Action:** Press `C` again

**Expected Console Output:**
```
[RENDER] Culling: OFF
```

**Visual Effect:**
- Should **STILL** be no visible difference

? **Test PASS if:** Console shows toggle messages AND no visual change with depth ON

---

### Step 5: Verify Debug Print

**Action:** Wait 1-2 seconds (app prints debug info once per second)

**Expected Console Output (example):**
```
[STATE] Booking | occupied=0 people=0 playing=0 | Depth=ON Cull=OFF
```

? **Test PASS if:** Debug print includes "Depth=ON/OFF" and "Cull=ON/OFF"

---

## ? 5-Minute Acceptance Test

If you only have 5 minutes, run this minimal test:

```
1. Build and run                           ? No errors
2. Check console for initial state         ? "DepthTest=ON Culling=OFF"
3. Press D                                 ? Console: "DepthTest: OFF"
4. Press D again                           ? Console: "DepthTest: ON"
5. Press C                                 ? Console: "Culling: ON"
6. Look at scene                           ? NO visible difference
7. Press C again                           ? Console: "Culling: OFF"
8. Look at scene                           ? STILL no visible difference
9. Wait for debug print                    ? Shows "Depth=ON Cull=OFF"
```

**All steps successful?** ? Phase 11 is working correctly! ?

---

## ?? Key Bindings Quick Reference

| Key | Effect |
|-----|--------|
| **D** | Toggle depth test ON/OFF |
| **C** | Toggle back-face culling ON/OFF |
| ESC | Exit application |
| Arrow Keys | Move camera |
| Mouse | Look around |

---

## ?? Quick Troubleshooting

### "No console output when pressing D or C"
- Check that console window is visible
- Try pressing ENTER key first (to confirm input works)
- Make sure you're pressing D or C (not other keys)

### "Visible difference when toggling culling"
- This is only expected when **depth test is OFF**
- Make sure depth test is ON (check debug print shows "Depth=ON")
- Press D twice if needed to ensure depth test is enabled

### "Can't see visual effect when toggling depth test"
- Make sure you're inside the hall (looking at walls/seats)
- Walk forward toward a wall with depth test OFF
- The overlap should be obvious

---

## ?? Success Criteria

**Phase 11 is COMPLETE if all of the following are true:**

- [x] Build succeeds with no errors
- [x] Console shows "[RENDER] Initial state: DepthTest=ON Culling=OFF"
- [x] Pressing D toggles depth test (console confirms)
- [x] Pressing C toggles culling (console confirms)
- [x] With depth ON, culling toggle = NO visible difference
- [x] Debug print shows "Depth=ON/OFF Cull=ON/OFF"
- [x] Toggles work in all app states (Booking/Entering/Projection/Exiting)

**If all checked:** Phase 11 is ready! ??

---

## ?? Console Output Reference

**Expected messages you should see:**

```
[RENDER] Initial state: DepthTest=ON Culling=OFF
[RENDER] DepthTest: OFF
[RENDER] DepthTest: ON
[RENDER] Culling: ON (GL_BACK, CCW)
[RENDER] Culling: OFF
[STATE] Booking | occupied=0 people=0 playing=0 | Depth=ON Cull=OFF
```

---

## ?? Estimated Testing Time

- **Minimal Test:** 2 minutes (toggle keys, check console)
- **Basic Test:** 5 minutes (above + visual verification)
- **Full Test:** 15 minutes (all states + edge cases)
- **Complete Validation:** 30 minutes (full test checklist)

**Recommendation:** Start with the 5-minute test, then run full test if needed.

---

## ?? Next Steps After Testing

**If Phase 11 tests pass:**
1. Mark Phase 11 as complete ?
2. Commit changes to version control
3. Proceed to Phase 12 (HUD Overlay)

**If Phase 11 tests fail:**
1. Check PHASE11_TEST_CHECKLIST.md for detailed troubleshooting
2. Review PHASE11_COMPLETE.md for implementation details
3. Verify code changes match PHASE11_SUMMARY.md

---

**Ready to test? Start the app and press D and C!** ??
