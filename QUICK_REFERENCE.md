# Kostur CMake Quick Reference

## ?? Goal
Build `kostur.exe` using CMake in Visual Studio 2022

---

## ? What Was Fixed

**CMakeLists.txt changes:**
- ? Changed from `add_executable(${PROJECT_NAME} ...)` to `add_executable(kostur ...)`
- ? Explicit file listing instead of GLOB (more reliable)
- ? Added all .cpp files including Camera.cpp, DebugCube.cpp, etc.
- ? Set explicit output directories
- ? Added library existence checks

**Result:** Executable will be named exactly `kostur.exe` (lowercase)

---

## ?? Quick Start (Visual Studio 2022)

### First Time Setup:
```
1. File ? Open ? Folder
2. Navigate to: C:\Users\damja\Grafika\Damjan Petrov RA17-2022
3. Click "Select Folder"
4. Wait for CMake to configure (watch Output window)
5. In toolbar: Select Startup Item ? kostur.exe
6. Press F5 to run
```

### If Rebuild Needed:
```
1. Project ? Delete Cache and Reconfigure
2. OR run: .\clean_build.ps1 (then reopen in VS)
```

---

## ?? File Locations

### Source Code:
```
Source/Main.cpp          ? Entry point
Source/Application.cpp   ? Main app logic
Source/Window.cpp        ? Window management
Source/Camera.cpp        ? Camera system
Source/Input.cpp         ? Input handling
Shader.cpp               ? Shader utilities
Rectangle.cpp            ? Rectangle rendering
```

### Dependencies:
```
packages/glfw.3.4.0/...           ? Window library
packages/glew-2.2.0.2.2.0.1/...  ? OpenGL extensions
glm/                              ? Math library
```

### Build Output:
```
out/build/x64-Debug/Debug/kostur.exe      ? Debug build
out/build/x64-Release/Release/kostur.exe  ? Release build
```

---

## ??? Troubleshooting Commands

### Verify Setup:
```powershell
.\verify_kostur_setup.ps1
```

### Clean Build:
```powershell
.\clean_build.ps1
```

### Manual Clean:
```powershell
Remove-Item build, out, .vs, CMakeCache.txt, CMakeFiles -Recurse -Force
```

### Check Target Exists:
```powershell
cmake --build out/build/x64-Debug --target help
# Should list "kostur" as a target
```

### Find the .exe:
```powershell
Get-ChildItem -Path out -Recurse -Filter "kostur.exe"
```

---

## ?? Common Issues

### Issue: "No targets available"
**Solution:** 
- Ensure you opened the FOLDER (not a .sln file)
- Delete cache and reconfigure

### Issue: "GLEW/GLFW not found"
**Solution:**
```powershell
.\nuget.exe restore packages.config -PackagesDirectory packages
```

### Issue: "Wrong executable name (Kostur.exe not kostur.exe)"
**Solution:**
- Delete build folders completely
- Reopen project in VS2022
- The CMakeLists.txt now explicitly uses lowercase "kostur"

### Issue: "Can't find Main.cpp"
**Solution:**
- Verify you're in the correct directory (should have Source/ and Header/ folders)
- Check that CMakeLists.txt is in the root

---

## ?? Verification Checklist

Before building:
- [ ] CMakeLists.txt exists in workspace root
- [ ] `add_executable(kostur ...)` line exists in CMakeLists.txt
- [ ] All .cpp files exist in Source/ and root
- [ ] packages/ folder exists with GLFW and GLEW
- [ ] No stale build/ or out/ directories (or cleaned)

After opening in VS2022:
- [ ] CMake configured successfully (check Output window)
- [ ] "kostur.exe" appears in Startup Item dropdown
- [ ] No errors in Error List window

After building:
- [ ] Build succeeded (0 errors)
- [ ] kostur.exe exists in out/build/.../Debug/ or Release/
- [ ] Can run with F5

---

## ?? Advanced: Switch to vcpkg (Optional)

If you want cleaner dependency management:

### Install vcpkg:
```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Install libraries:
```powershell
.\vcpkg install glfw3:x64-windows glew:x64-windows
```

### Update CMakeLists.txt:
Replace the packages/ section with:
```cmake
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(kostur PRIVATE OpenGL::GL glfw GLEW::GLEW)
```

### Configure VS:
Project ? CMake Settings ? Add:
```
CMAKE_TOOLCHAIN_FILE = C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

---

## ?? Help

If you still can't get kostur.exe to build:

1. Run `.\verify_kostur_setup.ps1` and share output
2. Check CMake output window in VS for errors
3. Verify you're using "Open Folder" not "Open Solution"
4. Ensure Visual Studio 2022 has "Desktop development with C++" workload installed

---

**Last Updated:** Based on CMakeLists.txt fix that explicitly names the target `kostur`
