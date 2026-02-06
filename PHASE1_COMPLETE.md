# Phase 1 Complete: Window, Context, FPS Limiter, Fullscreen, Exit

## What was implemented

### 1. Window Module (`Header/Window.h` + `Source/Window.cpp`)
- Creates a **fullscreen window** on the primary monitor using the monitor's current video mode
- Requests **OpenGL 3.3 core profile** context
- Initializes **GLEW** after context creation
- Provides accessors:
  - `GLFWwindow* handle()` - returns the GLFW window handle
  - `int width()` / `int height()` - returns window dimensions
  - `bool shouldClose()` - checks if window should close
- Provides `pollEvents()` and `swapBuffers()` methods
- Implements **ESC key callback** that immediately sets `glfwSetWindowShouldClose(window, true)`

### 2. FrameLimiter Module (`Header/FrameLimiter.h` + `Source/FrameLimiter.cpp`)
- Caps frame rate to **75 FPS** using target frame time (1/75 seconds)
- Uses a hybrid approach to avoid jitter:
  - Sleeps for most of the remaining time
  - Spin-waits for the final millisecond for precision
- Tracks and prints **FPS to console once per second**
- Provides `getDeltaTime()` for frame timing

### 3. Application Updates (`Source/Application.cpp`)
- Creates and initializes the Window during `init()`
- Creates the FrameLimiter with 75 FPS target
- Main loop:
  - Calls `frameLimiter->beginFrame()` at start
  - Polls events (includes ESC handling)
  - Clears screen with `glClearColor` and `glClear(GL_COLOR_BUFFER_BIT)`
  - Swaps buffers
  - Calls `frameLimiter->endFrame()` to enforce FPS cap
- Properly shuts down window on exit

## Acceptance Criteria Verification

| Criteria | Status |
|----------|--------|
| Fullscreen window on startup | ? Uses primary monitor's video mode |
| ESC always exits immediately | ? Key callback sets shouldClose |
| Frame rate capped near 75 FPS | ? Console prints FPS every second |
| No camera/scene/models yet | ? Only clear color rendering |

## Files Added
- `Header/Window.h`
- `Source/Window.cpp`
- `Header/FrameLimiter.h`
- `Source/FrameLimiter.cpp`

## Files Modified
- `Header/Application.h` - Added Window and FrameLimiter members
- `Source/Application.cpp` - Integrated window creation and frame limiting
- `Kostur.vcxproj` - Added new source and header files
- `Kostur.vcxproj.filters` - Added filters for new files

## Notes
- The project uses **GLEW** (via NuGet package) instead of GLAD for OpenGL loading
- VSync is disabled (`glfwSwapInterval(0)`) so the custom frame limiter controls timing
- Window dimensions are determined by the primary monitor's current video mode
