# Phase 2 Complete: Camera (Perspective, Mouse Look, Movement, Collision)

## What was implemented

### 1. Input Module (`Header/Input.h` + `Source/Input.cpp`)
- Wraps GLFW input for keyboard and mouse
- **Key queries:**
  - `isKeyDown(int key)` - Check if key is currently held
  - `isKeyPressed(int key)` - Edge-triggered (just pressed this frame)
- **Mouse queries:**
  - `getMouseX()` / `getMouseY()` - Current mouse position
  - `getMouseDeltaX()` / `getMouseDeltaY()` - Per-frame mouse delta
  - `isMouseButtonDown()` / `isMouseButtonPressed()` - Mouse button states
- Captures cursor for FPS-style camera (`GLFW_CURSOR_DISABLED`)

### 2. AABB Collision (`Header/AABB.h`)
- Simple axis-aligned bounding box struct
- Methods:
  - `contains(point)` - Check if point is inside
  - `clamp(point)` - Clamp point to stay within bounds
  - `center()` / `size()` - Utility methods

### 3. Camera Class (`Header/Camera.h` + `Source/Camera.cpp`)
- First-person camera using GLM
- **Stores:** position, yaw, pitch
- **Provides:**
  - `viewMatrix()` - Returns the view matrix
  - `projectionMatrix(aspect)` - Returns perspective projection matrix
- **Mouse look:**
  - Updates yaw/pitch from mouse delta
  - Pitch clamped to ±89° to avoid gimbal lock/flipping
  - Configurable mouse sensitivity
- **Movement:**
  - Arrow keys (and WASD) move forward/back and strafe left/right
  - Movement is relative to camera's view direction (XZ plane)
  - Uses `deltaTime` for frame-independent movement
  - Configurable move speed
- **Collision:**
  - Camera position clamped inside `hallBounds` AABB
  - Configurable padding to keep camera away from walls

### 4. Debug Visualization (`Header/DebugCube.h` + `Source/DebugCube.cpp`)
- Simple cube mesh for visual debugging
- 36 vertices (12 triangles, 6 faces)
- Uses OpenGL VAO/VBO

### 5. Basic Shader (`Assets/Shaders/basic.vert` + `basic.frag`)
- Vertex shader: MVP transformation
- Fragment shader: Solid color output
- Updated `Shader.h` with `setMat4()` and `setVec3(glm::vec3)` methods

### 6. Application Integration
- Camera created with starting position at back of hall
- Hall bounds set to 18x4.5x18 meter space
- Debug cubes rendered:
  - Red cube at center
  - Gray floor plane
  - Colored corner markers

## File Changes

### Files Added
- `Header/Input.h` + `Source/Input.cpp`
- `Header/Camera.h` + `Source/Camera.cpp`
- `Header/AABB.h`
- `Header/DebugCube.h` + `Source/DebugCube.cpp`
- `Assets/Shaders/basic.vert`
- `Assets/Shaders/basic.frag`

### Files Modified
- `Header/Application.h` - Added Camera, DebugCube, Shader members
- `Source/Application.cpp` - Integrated camera, input, and debug rendering
- `Shader.h` - Added GLM matrix/vector uniform setters
- `CMakeLists.txt` - Updated to use NuGet packages for GLEW/GLFW

### Files Renamed (to avoid conflict with `<ctime>`)
- `Header/Time.h` ? `Header/AppTime.h`
- `Source/Time.cpp` ? `Source/AppTime.cpp`

## Coordinate System

```
    Y (up)
    |
    |
    +------ X (right)
   /
  /
 Z (forward, toward camera)

Screen is at -Z, camera starts at +Z looking toward -Z
```

## Hall Bounds
- X: -9.0 to +9.0 (18m wide)
- Y: 0.5 to 5.0 (4.5m height, with 0.5m floor offset)
- Z: -9.0 to +9.0 (18m deep)
- Padding: 0.3m from each wall

## Controls
- **Mouse:** Look around (yaw/pitch)
- **Arrow Keys / WASD:** Move forward/back, strafe left/right
- **ESC:** Exit application

## Acceptance Criteria Verification

| Criteria | Status |
|----------|--------|
| Mouse look rotates view smoothly | ? Implemented with configurable sensitivity |
| Pitch clamped to avoid flipping | ? Clamped to ±89° |
| Arrow keys move relative to view direction | ? XZ plane movement based on front vector |
| Camera cannot leave hallBounds | ? Position clamped with padding |
| Still fullscreen + 75 FPS + ESC exit | ? All maintained from Phase 1 |

## Build System
- Using CMake with Visual Studio 2022 generator
- GLEW and GLFW linked from NuGet packages
- GLM included from `glm/` directory
- Build command: `cmake --build build --config Debug`
