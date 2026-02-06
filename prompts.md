# PROMPTS.md — Copilot Implementation Prompts (3D Cinema)

This document contains copy?paste prompts to send to GitHub Copilot sequentially. Each phase builds on the previous phases.

General rules for ALL phases:

- Target: C++14 + OpenGL 3.3+ with GLFW + GLAD + GLM + stb_image.
- Do not introduce a new framework (no SDL, no engine).
- Keep code modular: headers in `Header/`, sources in `Source/`, assets in `Assets/`.
- `ESC` must exit immediately in every phase once the window exists.
- Maintain a stable 75 FPS cap once the frame limiter is introduced.

---

## Phase 0 — Project & Engine Setup

**Goal (1–2 sentences):** Establish a clean C++14 OpenGL workspace with predictable file layout and a minimal “engine skeleton” (app loop + logging) that other phases extend.

**Copilot prompts:**

```text
Create/organize the project into `Source/`, `Header/`, and `Assets/` folders. Add a build configuration (CMakeLists or update the existing VS project) for C++14 and OpenGL dependencies: GLFW, GLAD, GLM. Add stb_image as a single-header library under `Header/`.

Create minimal engine-style modules (empty/placeholder implementations are OK for now): `Application`, `Time`, and `Log`:
- `Application` owns init/shutdown and the main loop entry point.
- `Time` provides `deltaTime` and `now()` using `glfwGetTime()`.
- `Log` provides simple console logging macros/wrappers.

Update `Source/Main.cpp` to instantiate `Application` and run it.

Do NOT implement rendering yet; just structure and compile.
```

```text
Add a `third_party.md` (or similar) listing GLFW/GLAD/GLM/stb_image usage and where they are included in the project. Keep it short, no code.
```

**Constraints & acceptance criteria:**

- Project builds successfully (C++14).
- App starts and exits cleanly (even if it does nothing yet).
- No rendering features required in this phase.

---

## Phase 1 — Window, Context, FPS Limiter, Fullscreen, Exit

**Goal (1–2 sentences):** Create the OpenGL context and a stable main loop: fullscreen window, 75 FPS limiter, and guaranteed immediate exit on `ESC`.

**Copilot prompts:**

```text
Using the Phase 0 skeleton, implement `Window` (in `Header/Window.h` + `Source/Window.cpp`) based on GLFW:
- Always create a fullscreen window on the primary monitor using the monitor’s current video mode.
- Request an OpenGL 3.3 core profile context.
- Initialize GLAD after context creation.
- Provide accessors: `GLFWwindow* handle()`, `int width()`, `int height()`, and `bool shouldClose()`.
- Add `pollEvents()` and `swapBuffers()`.

In `Application`, create the window during init, destroy it during shutdown.

Implement `ESC` to call `glfwSetWindowShouldClose(window, true)` and verify it works from the running loop.
```

```text
Implement `FrameLimiter` that caps to 75 FPS using a target frame time (1/75). Integrate it into the main loop in `Application`:
- Measure `deltaTime` each frame.
- Sleep/spin to enforce the cap (avoid long sleeps that cause jitter).

Add a very small FPS print to console once per second to confirm the cap.

In the loop, call `glClearColor` and `glClear(GL_COLOR_BUFFER_BIT)` so the window visibly updates.
```

**Constraints & acceptance criteria:**

- Fullscreen window on startup.
- `ESC` always exits (no matter what else is added later).
- Frame rate is capped near 75 FPS (console check acceptable).
- No camera/scene/models yet.

---

## Phase 2 — Camera (Perspective, Mouse Look, Movement, Collision)

**Goal (1–2 sentences):** Add a first-person camera with mouse look and arrow-key movement in the view direction, constrained within a configurable hall AABB.

**Copilot prompts:**

```text
Add `Input` (or `InputManager`) module that wraps GLFW input for:
- key down / key pressed (edge) queries
- mouse position and per-frame mouse delta

Implement a `Camera` class using GLM:
- Stores position, yaw, pitch.
- Provides `viewMatrix()` and `projectionMatrix(aspect)`.
- Mouse look: update yaw/pitch from mouse delta; clamp pitch to avoid flipping.
- Movement: arrow keys move forward/back and strafe left/right in the camera’s view direction (not fixed axis). Use `deltaTime`.

Add `Collision/AABB` struct and clamp camera position inside `hallBounds` with a small padding so the camera never passes through walls/floor/ceiling.

Wire camera update into the `Application` loop.
```

```text
Add a minimal debug draw for a single cube in front of the camera (hardcoded transform is fine) so camera motion can be visually verified. Keep it simple: one basic shader and one cube mesh.
```

**Constraints & acceptance criteria:**

- Mouse look rotates view smoothly; pitch clamped.
- Arrow keys move relative to view direction.
- Camera cannot leave `hallBounds` (test by pushing into each boundary).
- Still fullscreen + 75 FPS + ESC exit.

---

## Phase 3 — Basic Scene (Floor, Walls, Ceiling, Screen)

**Goal (1–2 sentences):** Establish the cinema hall geometry and coordinate conventions: floor, walls, ceiling, door placeholder, and a screen quad that starts white.

**Copilot prompts:**

```text
Create a `Scene` module to hold renderable objects and update/draw them each frame.

Implement basic geometry rendering using the existing cube mesh:
- Build floor, 4 walls, and ceiling as scaled cubes (or separate plane mesh if you prefer, but keep it minimal).
- Add a screen rectangle (quad) on the front wall. It must start white.
- Add a door placeholder (a scaled cube) at a consistent location.

Define a world coordinate system and document it in a short `README_scene.md`:
- Which axis is up (Y), forward/back (Z), left/right (X).

Add textures for floor/walls/screen if texture pipeline already exists; otherwise use solid colors and plan to texture later.
```

**Constraints & acceptance criteria:**

- User can walk around inside the hall without clipping (camera collision still applies).
- Screen is visible, positioned on a wall, initially white.
- No seat logic yet.

---

## Phase 4 — Lighting System (Room Light + Screen Light)

**Goal (1–2 sentences):** Introduce a basic lighting model with a configurable room light and a separate screen light that can be enabled during projection.

**Copilot prompts:**

```text
Add a minimal lighting system:
- Define `Light` data (position, color, intensity, enabled).
- Update the main 3D shader to support one point light (Phong or Blinn-Phong).

Add two lights in the scene:
1) `roomLight`: positioned near the ceiling, warm color, enabled by default.
2) `screenLight`: positioned at/near the screen, white color, disabled by default.

Expose the ability to enable/disable each light via code (no keybinds required yet). Ensure unlit fallback is reasonable when a light is disabled.
```

**Constraints & acceptance criteria:**

- Scene is visibly lit when `roomLight.enabled = true`.
- Disabling room light makes the scene noticeably darker.
- Screen light can be enabled independently.

---

## Phase 5 — Seat System (3D Seats, Rows, Steps, States)

**Goal (1–2 sentences):** Create the 3D seat grid with stepped rows and seat state storage (Free/Reserved/Purchased), rendered as textured cuboids.

**Copilot prompts:**

```text
Implement `SeatState` enum: `Free`, `Reserved`, `Purchased`.

Implement `Seat` struct/class:
- `row`, `col`
- `SeatState state`
- `glm::vec3 position` (seat center)
- `AABB bounds` for picking (will be used in Phase 6)

Implement `SeatGrid`:
- Fixed grid size: 5 rows x 10 cols.
- Each row farther from screen is +1 step higher (increase Y by `stepHeight` per row).
- Seats are spaced consistently in X and Z.

Render each seat as a cuboid “chair” (can be one cuboid for now). Use distinct colors by state (Free=green, Reserved=yellow, Purchased=red) until chair textures are in place.

Do not implement clicking or purchasing yet; only data + rendering.
```

**Constraints & acceptance criteria:**

- 50 seats are visible in the hall.
- Row stepping is visually obvious (back rows higher).
- Each seat has a stable world position and bounds.

---

## Phase 6 — Seat Interaction (Mouse Ray Picking, Reserve/Unreserve)

**Goal (1–2 sentences):** Allow the user to reserve/unreserve seats by clicking on them via ray casting from the camera through the mouse cursor.

**Copilot prompts:**

```text
Implement `RayPicker`:
- Convert current mouse position (pixels) to a world-space ray using the camera view/projection matrices.
- Implement ray vs AABB intersection.
- Provide `Seat* pickSeat(const Ray&)` returning the closest intersected seat.

Add left mouse click interaction:
- On click (edge-triggered), pick a seat.
- If seat is `Free`, set to `Reserved`.
- If seat is `Reserved`, set to `Free`.
- If seat is `Purchased`, do nothing.

Only allow this interaction when the app is in an “interactive booking mode” boolean (temporary). The real state machine will come in Phase 10.
```

**Constraints & acceptance criteria:**

- Clicking a seat reliably toggles Free?Reserved.
- Purchased seats cannot be toggled.
- Picking chooses the nearest seat if multiple overlap in ray direction.

---

## Phase 7 — Purchase Logic (1–9 Keys, Adjacent Seat Search)

**Goal (1–2 sentences):** Implement the exact adjacency purchase algorithm: keys 1–9 buy N adjacent Free seats scanning from the rightmost seat in the last row backwards.

**Copilot prompts:**

```text
In `SeatGrid`, implement `bool purchaseAdjacent(int N)` with this exact scan order:
- Start from last row (row index = rows-1) to first row.
- Within a row, scan from rightmost col (cols-1) to left.
- Find the first block of exactly N consecutive `Free` seats.
- When found, mark those N seats as `Purchased` and return true.
- If not found anywhere, return false and change nothing.

Wire keys `1`..`9` (edge-triggered) to call `purchaseAdjacent(N)`.
Only allow purchasing when in booking mode (same temporary boolean as Phase 6).
```

```text
Add a small on-screen or console feedback when purchase fails (no adjacent block found). Keep it minimal.
```

**Constraints & acceptance criteria:**

- The scan order matches the 2D logic (rightmost of last row, scanning backwards).
- Only `Free` seats are purchased.
- Reserving seats does not count as Free for purchase.

---

## Phase 8 — People System (Humanoid Models, Paths, Movement)

**Goal (1–2 sentences):** Spawn viewers at the door and animate them to their target seats (and back later), using a clear staged path that includes step climbing.

**Copilot prompts:**

```text
Implement `Person` and `PeopleManager`:
- `Person` stores `glm::vec3 position`, `Seat* targetSeat`, and a movement `stage` enum.
- Spawning: on demand, create K people at the door position (K chosen elsewhere for now).
- Assign each person a target seat chosen from (Reserved ? Purchased). Avoid assigning the same seat to multiple people.

Movement path (entering):
1) Move along aisle direction to the target row (change Z toward seat row Z).
2) Climb steps to target row height (change Y to seat row Y).
3) Move across the row to the seat (change X toward seat X).
4) Mark seated when close enough.

Use constant speed movement based on `deltaTime` (acceleration is optional but not required).

Rendering: initially render people as colored cubes. Keep humanoid model loading as a separate prompt below.
```

```text
Add model loading support to render humanoid viewer models:
- Load at least 15 distinct humanoid models from `Assets/Models/` (OBJ is fine).
- Each spawned person chooses a model index 0..14; models may repeat across people.

If model loading is too heavy right now, implement it behind a compile-time or runtime flag so cubes still work.
```

**Constraints & acceptance criteria:**

- People move in ordered stages (aisle ? climb ? across row ? seated) at a consistent speed.
- People reach the intended target seats and stop.
- No two people are assigned the same target seat.
- Cube fallback remains functional if models are missing.

---

## Phase 9 — Projection Simulation (Screen Textures, Timing, Film Loop)

**Goal (1–2 sentences):** Implement film playback on the screen using at least 20 textures over exactly 20 seconds, then reset screen to white.

**Copilot prompts:**

```text
Implement `Screen` component:
- Holds a list of at least 20 textures (`film_frame_00`..`film_frame_19`).
- Has `bool playing`, `float timer`, `int currentFrame`.
- While playing, choose the frame based on elapsed time: 20 seconds total; 20 frames => 1 second per frame.
- After 20 seconds: stop, reset to white texture.

Render the screen quad using a textured shader path.
```

```text
Integrate screen light behavior (from Phase 4) with playback:
- When `Screen.playing == true`, enable `screenLight`.
- When playback ends, disable `screenLight`.

Keep room light control for Phase 10 (state machine), but ensure screen light toggling works now.
```

**Constraints & acceptance criteria:**

- Screen cycles actual textures (not random solid colors).
- Total film duration is 20 seconds.
- Screen resets to white when finished.

---

## Phase 10 — State Machine (Idle ? Entry ? Projection ? Exit ? Reset)

**Goal (1–2 sentences):** Convert the app into a deterministic state machine controlling booking, entry animation, projection timing, exit animation, and full reset.

**Copilot prompts:**

```text
Introduce `AppState` enum with states:
- `Booking` (interactive seat reserve/purchase)
- `Entering` (door open, people move to seats)
- `Projection` (door closed, room light off, screen playing)
- `Exiting` (door open, people return to door)

Add a `StateController` (or integrate into `Application`) that:
- Starts in `Booking`.
- On `Enter` key: if there is at least one Reserved or Purchased seat, spawn a random number of people from 1..occupiedCount and transition to `Entering`.
- When all people report seated: transition to `Projection` and start film playback.
- When film ends (20s): transition to `Exiting` and command people to walk back via reverse path.
- When all people exited: clear people, reset all seats to `Free`, transition back to `Booking`.

Door behavior: create a `Door` component with `open/closed` state and a visible color/state change. (Animation optional.)

Lighting by state:
- Booking/Entering/Exiting: room light ON, screen light OFF.
- Projection: room light OFF, screen light ON (screen plays film).

Ensure seat reserve/purchase is only active in `Booking`.
```

**Constraints & acceptance criteria:**

- Full loop works: Booking ? Entering ? Projection ? Exiting ? Booking.
- Room light OFF during projection; screen light ON during projection.
- After exit: seats reset to Free and booking interaction works again.

---

## Phase 11 — Rendering Features (Depth Test Toggle, Back-Face Culling Toggle)

**Goal (1–2 sentences):** Add runtime toggles for depth testing and back-face culling and validate their correctness in the 3D hall.

**Copilot prompts:**

```text
Add runtime key toggles:
- `D` toggles `glEnable/glDisable(GL_DEPTH_TEST)`.
- `C` toggles `glEnable/glDisable(GL_CULL_FACE)` and sets `glCullFace(GL_BACK)`.

Add a small on-screen/console status print for both toggles.

Validation requirement:
- When depth test is ON, toggling back-face culling must cause no visible difference in the scene.
- Ensure your meshes are closed/consistent winding so this holds.
```

**Constraints & acceptance criteria:**

- Both toggles work during runtime in all states.
- With depth test ON, culling toggle produces no visible difference.

---

## Phase 12 — HUD Overlay (Student Name/Index, Transparency)

**Goal (1–2 sentences):** Render a semi-transparent textured HUD nametag in a screen corner independent from the 3D camera.

**Copilot prompts:**

```text
Implement `HUD` overlay rendering:
- Render a textured quad in NDC/screen space using an orthographic projection.
- Load `Assets/Textures/hud_nametag.png` (contains Name Surname Index) with alpha.
- Draw it in a corner (e.g., bottom-right) with blending enabled.
- Disable depth testing for HUD draw and draw it last.

Ensure HUD is visible in every app state, including projection.
```

**Constraints & acceptance criteria:**

- HUD texture is semi-transparent (alpha) and readable.
- HUD position remains fixed on screen while camera moves.

---

## Phase 13 — Polishing & Validation (Constraints, Edge Cases, Testing)

**Goal (1–2 sentences):** Harden the application against edge cases, verify all requirements, and add small diagnostics to ensure the demo is reliable.

**Copilot prompts:**

```text
Perform a pass to validate and harden behavior with explicit checks:
- `ESC` exits immediately in every state.
- Fullscreen remains active.
- Frame limiter remains 75 FPS.
- Seat interactions are disabled outside `Booking`.
- Pressing `Enter` in `Booking` with 0 occupied seats does nothing.
- People spawning never exceeds occupied seat count and never duplicates target seats.
- Film always lasts 20 seconds and resets the screen to white afterward.
- After Exiting completes, seats reset to Free and people list clears.

Add a minimal debug UI (console logs acceptable) that prints:
- Current app state
- Occupied seat count
- People count
- Depth/culling toggles

Do not add new features; only validation, bug fixes, and small diagnostics.
```

```text
Add a short `TESTING.md` checklist that mirrors the acceptance criteria for each phase and includes a full end-to-end scenario to run manually.
```

**Constraints & acceptance criteria:**

- No regressions: all prior phases still work.
- End-to-end run is repeatable for multiple cycles without restarting the app.
- The debug output clearly indicates current state and key counts for troubleshooting.
