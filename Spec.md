# SPEC.md – 3D Cinema Hall Application Specification

---

## 1. Requirements Overview

### 1.1 MUST Have (Mandatory)

| ID | Requirement |
|----|-------------|
| M1 | Fullscreen window at all times |
| M2 | ESC key exits application immediately from any state |
| M3 | Frame limiter capped at 75 FPS |
| M4 | Runtime toggle for depth testing |
| M5 | Runtime toggle for back-face culling |
| M6 | At least one light source in the scene |
| M7 | Toggling back-face culling produces no visible difference while depth test is ON |
| M8 | Semi-transparent HUD quad in corner displaying Name/Surname/Index texture |
| M9 | 3D cinema hall with textured cuboid chairs |
| M10 | Rows are stepped: each row farther from screen is +1 step higher |
| M11 | Screen is a rectangle on the front wall, starts white |
| M12 | Perspective camera with arrow-key movement in view direction |
| M13 | Mouse look (camera rotation via mouse movement) |
| M14 | Camera constrained by hall walls, floor, and ceiling (collision bounds) |
| M15 | Seat selection via mouse ray casting (ray pick) |
| M16 | Seat states: Free, Reserved, Purchased |
| M17 | Click toggles Free ↔ Reserved (only during booking phase) |
| M18 | Keys 1–9 purchase N adjacent Free seats (rightmost-last-row scan, backwards) |
| M19 | Enter key starts simulation if any seats are Reserved or Purchased |
| M20 | Door opens; random count of people (1 to Reserved+Purchased) spawn at door |
| M21 | People walk to assigned seats: first along aisle (Y movement + step climb), then across row (X movement) |
| M22 | Movement may be constant speed OR accelerated; constant speed where spec says constant is acceptable; accelerated may replace constant but NOT vice versa |
| M23 | After all seated: door closes, room light OFF, projection starts |
| M24 | Screen displays ≥20 textures as film frames over 20 seconds total |
| M25 | After film: screen white, door opens, people exit via reverse path |
| M26 | When all exited: door closes, seats reset to Free, return to booking state |
| M27 | Room light OFF while hall is closed AND during projection |
| M28 | Screen emits light (light source at screen position) during projection |
| M29 | Humanoid viewer models (≥15 unique characters; instances may repeat) |

### 1.2 SHOULD Have (Recommended)

| ID | Requirement |
|----|-------------|
| S1 | Smooth camera interpolation / damping |
| S2 | Visual feedback on hovered seat (highlight color) |
| S3 | Door animation (swing or slide) |
| S4 | Ambient sound or visual cue when phase changes |
| S5 | Debug overlay showing FPS, current state, toggle statuses |

### 1.3 NICE-TO-HAVE (Optional Enhancements)

| ID | Requirement |
|----|-------------|
| N1 | Shadow mapping for room light |
| N2 | Post-processing effects (bloom on screen during projection) |
| N3 | Animated humanoid walking cycle |
| N4 | Multiple camera modes (free-fly, fixed angles) |
| N5 | Configurable hall size via external config file |

---

## 2. Application State Machine

### 2.1 States

| State | Description |
|-------|-------------|
| `BOOKING` | User can reserve/purchase seats; camera free to move; room light ON |
| `ENTERING` | Door open; people spawning and walking to seats; room light ON |
| `PROJECTION` | Door closed; room light OFF; screen light ON; film playing |
| `EXITING` | Door open; people walking out; room light ON; screen white |

### 2.2 State Diagram

---

## 3. Component Interfaces

### 3.1 Window

#### Requirements

- Fullscreen mode at native resolution
- Respond to ESC key for exiting

#### Tasks

- Create `Window` class
- Implement fullscreen toggle
- Capture and handle ESC key event

### 3.2 Frame Limiter

#### Requirements

- Cap framerate at 75 FPS

#### Tasks

- Create `FrameLimiter` class
- Implement mechanism to limit FPS
- Integrate with main render loop

### 3.3 Shader

#### Requirements

- Load and compile GLSL shaders
- Handle shader program linking

#### Tasks

- Create `Shader` class
- Implement shader loading and compilation
- Implement program linking and error logging

### 3.4 Texture

#### Requirements

- Load PNG textures using stb_image
- Support texture binding to shader units

#### Tasks

- Create `Texture` class
- Implement texture loading using stb_image
- Implement texture binding interface

### 3.5 Mesh

#### Requirements

- Create and manage VAO/VBO for mesh data
- Support loading of cube mesh

#### Tasks

- Create `Mesh` class
- Implement VAO/VBO creation and management
- Implement cube mesh loading

### 3.6 Renderer

#### Requirements

- Handle rendering of scenes
- Support toggling of depth testing and back-face culling

#### Tasks

- Create `Renderer` class
- Implement rendering pipeline
- Integrate depth test and culling toggles

### 3.7 Camera

#### Requirements

- Implement perspective projection
- Support camera movement and mouse look
- Enforce collision bounds with hall geometry

#### Tasks

- Create camera class/struct
- Implement perspective projection matrix
- Implement camera movement and rotation
- Implement collision detection and response

### 3.8 Light

#### Requirements

- Support for positional and directional lights
- Control over light intensity and color

#### Tasks

- Create `Light` class/struct
- Implement light position, color, intensity, and enable/disable functions
- Integrate lighting calculations into the shader

### 3.9 HUD

#### Requirements

- Render semi-transparent HUD with name tag
- Display in a corner of the screen

#### Tasks

- Create `HUD` class
- Implement HUD quad rendering
- Load and bind name tag texture

---

## 4. Implementation Roadmap (2D → 3D)

### Phase 0: Project Setup
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 0.1 | Create new project structure | Directories exist; CMake/vcxproj compiles empty main | ☐ Build succeeds |
| 0.2 | Integrate GLM, GLEW, GLFW, stb_image | Includes resolve; links without error | ☐ Blank window opens |
| 0.3 | Implement `Window` class with fullscreen | Window opens fullscreen at native resolution | ☐ Fullscreen verified |
| 0.4 | Implement `FrameLimiter` (75 FPS) | FPS counter shows ~75 FPS | ☐ Measured FPS ≤ 75 |
| 0.5 | ESC exits from main loop | Pressing ESC closes window | ☐ ESC works |

### Phase 1: Rendering Foundation
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 1.1 | Implement `Shader` class | Load, compile, link shaders; log errors | ☐ Shader compiles |
| 1.2 | Implement `Texture` class | Load PNG via stb_image; bind to unit | ☐ Texture loads |
| 1.3 | Implement `Mesh` class | Create cube VAO/VBO | ☐ Cube renders |
| 1.4 | Implement `Renderer` with depth test toggle | `glEnable/glDisable(GL_DEPTH_TEST)` via key | ☐ Toggle key works |
| 1.5 | Add back-face culling toggle | `glEnable/glDisable(GL_CULL_FACE)` via key | ☐ Toggle key works |
| 1.6 | Verify M7: culling toggle invisible with depth ON | Visual inspection shows no difference | ☐ No visible change |

### Phase 2: Camera System
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 2.1 | Implement perspective projection | Cube appears with correct perspective | ☐ Perspective correct |
| 2.2 | Arrow key movement in view direction | Camera moves forward/back/strafe with arrows | ☐ Movement smooth |
| 2.3 | Mouse look (yaw/pitch) | Camera rotates with mouse delta | ☐ Look around works |
| 2.4 | Implement collision bounds struct | AABB for hall defined | ☐ Bounds defined |
| 2.5 | Clamp camera position to bounds | Camera cannot exit hall AABB | ☐ Cannot walk through walls |

### Phase 3: Cinema Hall Geometry
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 3.1 | Create floor mesh with texture | Textured floor visible | ☐ Floor renders |
| 3.2 | Create walls (4) and ceiling | Enclosed room visible | ☐ Walls/ceiling render |
| 3.3 | Create stepped platforms for rows | 5 rows, each +0.2 Y higher | ☐ Steps visible |
| 3.4 | Place screen quad on front wall | White rectangle on wall | ☐ Screen visible |
| 3.5 | Place door mesh on side/back wall | Brown door visible | ☐ Door visible |

### Phase 4: Seat System
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 4.1 | Implement `Seat` struct | State, position, AABB | ☐ Struct compiles |
| 4.2 | Implement `SeatGrid` with 5×10 layout | 50 seats created | ☐ Count correct |
| 4.3 | Render seats as textured cuboids | Chairs visible in rows | ☐ Chairs render |
| 4.4 | Implement `RayPicker` | Mouse coords to world ray | ☐ Ray calculated |
| 4.5 | Ray-AABB intersection | Detect which seat is hit | ☐ Correct seat detected |
| 4.6 | Toggle Free ↔ Reserved on click | Seat color changes | ☐ Toggle works |
| 4.7 | Implement 1-9 purchase logic | N adjacent seats marked Purchased | ☐ Adjacency logic correct |

### Phase 5: State Machine
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 5.1 | Implement `StateMachine` enum + transitions | States defined | ☐ Compiles |
| 5.2 | BOOKING state active on start | Seat interaction works | ☐ Initial state correct |
| 5.3 | Enter key triggers ENTERING | State changes; seat interaction disabled | ☐ Transition works |
| 5.4 | All-seated triggers PROJECTION | State changes | ☐ Transition works |
| 5.5 | 20-second timer triggers EXITING | State changes | ☐ Transition works |
| 5.6 | All-exited triggers BOOKING | State resets; seats Free | ☐ Full loop works |

### Phase 6: Person Animation
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 6.1 | Implement `Person` struct with stages | Struct compiles | ☐ Compiles |
| 6.2 | Spawn people at door position | Cubes appear at door | ☐ Spawn works |
| 6.3 | Movement stage 1: walk along aisle (Z axis) | People move toward row | ☐ Aisle movement |
| 6.4 | Movement stage 2: climb step (Y axis) | People rise to row height | ☐ Step climb |
| 6.5 | Movement stage 3: walk across row (X axis) | People reach seat | ☐ Row movement |
| 6.6 | Constant speed movement | Position changes by fixed delta per frame | ☐ Speed constant |
| 6.7 | Reverse path for exiting | People return to door | ☐ Exit path correct |
| 6.8 | Replace cubes with humanoid models | OBJ models render | ☐ Models visible |
| 6.9 | Load 15 unique humanoid models | All 15 load without error | ☐ 15 models loaded |

### Phase 7: Lighting
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 7.1 | Implement `Light` struct | Position, color, intensity, enabled | ☐ Compiles |
| 7.2 | Add room light (point or directional) | Scene is lit | ☐ Light visible |
| 7.3 | Room light OFF during PROJECTION | Scene darker during film | ☐ Light off |
| 7.4 | Screen emissive during PROJECTION | Screen appears bright | ☐ Screen glows |
| 7.5 | Light OFF while hall closed (PROJECTION) | Correct per spec | ☐ Verified |

### Phase 8: Film Playback
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 8.1 | Load 20 film frame textures | All load without error | ☐ 20 textures loaded |
| 8.2 | Cycle textures on screen (20s / 20 = 1s each) | Frame changes every second | ☐ Frame timing correct |
| 8.3 | After 20s, screen white | Screen texture resets | ☐ White screen |

### Phase 9: HUD
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 9.1 | Implement `HUD` class with ortho projection | Quad renders in screen space | ☐ HUD renders |
| 9.2 | Load nametag texture | Texture shows Name/Surname/Index | ☐ Texture correct |
| 9.3 | Semi-transparent blending | Alpha < 1.0 visible | ☐ Transparency works |
| 9.4 | Position in corner | Quad in bottom-right (or chosen corner) | ☐ Position correct |

### Phase 10: Polish & Verification
| Step | Task | Acceptance Criteria | Test Checklist |
|------|------|---------------------|----------------|
| 10.1 | Full playthrough test | All states cycle correctly | ☐ End-to-end pass |
| 10.2 | Verify M7 (culling toggle) | No visible change with depth ON | ☐ Verified |
| 10.3 | Verify 75 FPS cap | FPS counter steady | ☐ Verified |
| 10.4 | Verify ESC exits from all states | Tested in each state | ☐ Verified |
| 10.5 | Code cleanup, comments | Readable code | ☐ Done |

---

## 5. Camera Movement, Mouse Look & Collision

### 5.1 Camera Parameters

---

## [Appendices and References]

### A. OpenGL and GLSL Resources

#### A.1 Learning Resources

- "OpenGL Programming Guide" (The Red Book)
- "OpenGL Shading Language" (The Orange Book)
- Online tutorials (ogl-tutorials.org, learnopengl.com)

#### A.2 Reference Resources

- OpenGL API documentation (opengl.org)
- GLSL reference card (khronos.org)

### B. Performance Considerations

- Profile CPU/GPU usage; optimize render loop
- Batch draw calls; minimize state changes
- Use efficient data formats and structures

### C. Troubleshooting Tips

- Common shader compilation/linking errors
- Debugging OpenGL applications (glGetError, etc.)
- Performance profiling tools (e.g., RenderDoc, gDEBugger)

### D. Future Enhancements

- Support for more complex geometries (OBJ, FBX)
- Advanced lighting models (phy
