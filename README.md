# README - 3D Cinema Project Setup

## Project Overview

This is a C++14 OpenGL project being converted from a 2D cinema seat management application to a full 3D cinema hall simulation with:
- First-person camera navigation
- 3D seat selection and purchasing
- Animated people/viewers
- Film projection simulation
- Full state machine (Booking ? Entering ? Projection ? Exiting)

## Current Status: Phase 0 Complete ?

The project structure and engine skeleton are ready. See `PHASE0_COMPLETE.md` for details.

## Building the Project

### Requirements
- Visual Studio 2019 or later
- Windows SDK
- NuGet Package Manager

### Dependencies (Already Configured)
- GLFW 3.4.0 (Window/Input)
- GLEW 2.2.0 (OpenGL loader)
- stb_image.h (Image loading)

### Build Steps
1. Open `Kostur.vcxproj` or the solution file in Visual Studio
2. Restore NuGet packages (should happen automatically)
3. Build configuration: Debug | x64
4. Build ? Build Solution (or F7)

## Running the Application

Currently (Phase 0), the application:
- Initializes the engine skeleton
- Logs to console
- Exits immediately (no window yet)

This is expected behavior - windowing comes in Phase 1.

## Next Phase

See `prompts.md` ? **Phase 1** for the next implementation steps.

## File Organization

- `Source/` - C++ implementation files
- `Header/` - C++ header files  
- `Assets/` - Textures, models, shaders (to be populated)
- `prompts.md` - Phase-by-phase implementation guide
- `Spec.md` - Full 3D specification
- `third_party.md` - Dependency documentation

## Questions?

Check the specification in `Spec.md` or follow the prompts in `prompts.md`.
