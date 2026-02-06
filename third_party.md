# Third-Party Dependencies

This project uses the following third-party libraries:

## GLFW 3.4.0
- **Purpose**: Window creation, OpenGL context management, and input handling
- **Location**: Installed via NuGet packages (`packages/glfw.3.4.0/`)
- **License**: zlib/libpng license
- **Usage**: Window and context creation, event polling

## GLAD (OpenGL Loader)
- **Purpose**: Load OpenGL function pointers
- **Location**: To be integrated (generated from https://glad.dav1d.de/)
- **License**: MIT/Public Domain
- **Usage**: OpenGL function loading after context creation
- **Note**: Will need to generate for OpenGL 3.3+ Core Profile

## GLM (OpenGL Mathematics)
- **Purpose**: Mathematics library for graphics (vectors, matrices, transformations)
- **Location**: To be integrated (header-only library)
- **License**: MIT License
- **Usage**: Camera transforms, projection matrices, vector/matrix math
- **Note**: Will be added as a header-only library

## stb_image.h
- **Purpose**: Image loading (PNG, JPG, etc.)
- **Location**: `Header/stb_image.h`
- **License**: MIT License / Public Domain
- **Usage**: Loading textures and images
- **Implementation**: Single-header library, already included

## Integration Notes

- GLFW is currently integrated via NuGet
- GLAD needs to be generated and added to the project
- GLM needs to be downloaded and added to `Header/` directory
- stb_image is already present in the project

## C++ Standard

- **Version**: C++14
- **Compiler**: MSVC (Visual Studio)
