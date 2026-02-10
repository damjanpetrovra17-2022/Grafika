cmake_minimum_required(VERSION 3.16)

project(KosturProject
    VERSION 1.0
    LANGUAGES CXX
)

# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly list all source files (more reliable than GLOB)
set(SOURCE_FILES
    Source/Application.cpp
    Source/AppTime.cpp
    Source/Camera.cpp
    Source/Crosshair.cpp
    Source/DebugCube.cpp
    Source/Door.cpp
    Source/FrameLimiter.cpp
    Source/HUD.cpp
    Source/HumanMesh.cpp
    Source/Input.cpp
    Source/Log.cpp
    Source/Main.cpp
    Source/PeopleManager.cpp
    Source/Person.cpp
    Source/RayPicker.cpp
    Source/Scene.cpp
    Source/Screen.cpp
    Source/SeatGrid.cpp
    Source/SeatMesh.cpp
    Source/Util.cpp
    Source/Window.cpp
    Rectangle.cpp
    Shader.cpp
)

# Explicitly list headers for IntelliSense
set(HEADER_FILES
    Header/AABB.h
    Header/Application.h
    Header/AppState.h
    Header/AppTime.h
    Header/Camera.h
    Header/Crosshair.h
    Header/DebugCube.h
    Header/Door.h
    Header/FrameLimiter.h
    Header/HUD.h
    Header/HumanMesh.h
    Header/Input.h
    Header/Light.h
    Header/Log.h
    Header/PeopleManager.h
    Header/Person.h
    Header/Ray.h
    Header/RayPicker.h
    Header/Scene.h
    Header/Screen.h
    Header/Seat.h
    Header/SeatGrid.h
    Header/SeatMesh.h
    Header/stb_image.h
    Header/Util.h
    Header/Window.h
    Rectangle.h
    Shader.h
)

# Create executable with EXPLICIT name "kostur"
add_executable(kostur
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# Include directories - GLM first to avoid conflicts with Header/Time.h
target_include_directories(kostur PRIVATE
    ${CMAKE_SOURCE_DIR}/glm
    ${CMAKE_SOURCE_DIR}/packages/glew-2.2.0.2.2.0.1/build/native/include
    ${CMAKE_SOURCE_DIR}/packages/glfw.3.4.0/build/native/include
    ${CMAKE_SOURCE_DIR}/Header
    ${CMAKE_SOURCE_DIR}
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(kostur PRIVATE OpenGL::GL)

# Link GLEW and GLFW from NuGet packages
if(EXISTS "${CMAKE_SOURCE_DIR}/packages/glew-2.2.0.2.2.0.1/build/native/lib/Release/x64/glew32s.lib" AND
   EXISTS "${CMAKE_SOURCE_DIR}/packages/glfw.3.4.0/build/native/lib/static/v143/x64/glfw3.lib")
    target_link_libraries(kostur PRIVATE
        ${CMAKE_SOURCE_DIR}/packages/glew-2.2.0.2.2.0.1/build/native/lib/Release/x64/glew32s.lib
        ${CMAKE_SOURCE_DIR}/packages/glfw.3.4.0/build/native/lib/static/v143/x64/glfw3.lib
    )
    message(STATUS "Using NuGet packages for GLEW and GLFW")
else()
    message(FATAL_ERROR "GLEW or GLFW libraries not found. Please restore NuGet packages first.")
endif()

# Define GLEW_STATIC for static linking
target_compile_definitions(kostur PRIVATE GLEW_STATIC)

# Platform-specific settings
if (WIN32)
    target_compile_definitions(kostur PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Set output directory for easier finding of the .exe
set_target_properties(kostur PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

# Create a custom target that always copies Assets
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        ${CMAKE_BINARY_DIR}/Debug/Assets
    COMMENT "Copying Assets folder to build directory"
    VERBATIM
)

# Make sure kostur depends on CopyAssets so assets are copied before running
add_dependencies(kostur CopyAssets)

# Print helpful information
message(STATUS "Target executable name: kostur")
message(STATUS "Output directory (Debug): ${CMAKE_BINARY_DIR}/Debug")
message(STATUS "Output directory (Release): ${CMAKE_BINARY_DIR}/Release")
message(STATUS "Assets will be copied on every build")
