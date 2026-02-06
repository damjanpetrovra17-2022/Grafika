# Complete Guide: Building kostur.exe with CMake in Visual Studio 2022

## ? Problem Fixed
Your CMakeLists.txt has been updated to create `kostur.exe` (not `Kostur.exe` or `KosturProject.exe`).

---

## ?? B) Cleanup + Rebuild Steps

### Step 1: Clean Build Artifacts (Safe Deletion)

**In Visual Studio 2022:**
1. Close Visual Studio if it's open
2. Delete these folders/files from your workspace root:
   ```
   C:\Users\damja\Grafika\Damjan Petrov RA17-2022\build\
   C:\Users\damja\Grafika\Damjan Petrov RA17-2022\out\
   C:\Users\damja\Grafika\Damjan Petrov RA17-2022\.vs\
   C:\Users\damja\Grafika\Damjan Petrov RA17-2022\CMakeCache.txt
   C:\Users\damja\Grafika\Damjan Petrov RA17-2022\CMakeFiles\
   ```

**Via PowerShell (run from workspace root):**
```powershell
Remove-Item -Path "build" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "out" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path ".vs" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "CMakeCache.txt" -Force -ErrorAction SilentlyContinue
Remove-Item -Path "CMakeFiles" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "CMakeSettings.json" -Force -ErrorAction SilentlyContinue
```

?? **DO NOT DELETE:**
- `_legacy_vs\` folder (old .sln/.vcxproj - keep for reference)
- `packages\` folder (NuGet packages for GLEW/GLFW)
- `Source\`, `Header\`, `glm\` folders (your source code)

---

### Step 2: Reopen and Reconfigure in Visual Studio 2022

1. **Open Folder in VS2022:**
   - Launch Visual Studio 2022
   - File ? Open ? Folder...
   - Select: `C:\Users\damja\Grafika\Damjan Petrov RA17-2022`
   - Wait for CMake to detect `CMakeLists.txt`

2. **Delete Cache and Reconfigure:**
   - Go to: **Project** ? **Delete Cache and Reconfigure**
   - OR: Right-click `CMakeLists.txt` in Solution Explorer ? **Delete Cache and Reconfigure**
   - Watch the Output window (View ? Output ? Show output from: CMake)

3. **Verify CMake Configuration:**
   Look for these messages in the CMake output:
   ```
   -- Target executable name: kostur
   -- Output directory (Debug): .../build/Debug
   -- Output directory (Release): .../build/Release
   -- Using NuGet packages for GLEW and GLFW
   ```

---

### Step 3: Select Startup Target and Build

1. **Select the Startup Item:**
   - In the toolbar, find the dropdown that says "Select Startup Item"
   - Click it and choose: **kostur.exe**
   - (Not "KosturProject.exe" or anything else)

2. **Build the Project:**
   - Press **F7** or **Ctrl+Shift+B**
   - OR: Build ? Build All

3. **Run the Application:**
   - Press **F5** (with debugging) or **Ctrl+F5** (without debugging)

---

## ?? C) Verification Checklist

### ? Where kostur.exe Should Appear

After building successfully, the executable will be located at:

**Debug build:**
```
C:\Users\damja\Grafika\Damjan Petrov RA17-2022\out\build\x64-Debug\Debug\kostur.exe
```

**Release build:**
```
C:\Users\damja\Grafika\Damjan Petrov RA17-2022\out\build\x64-Release\Release\kostur.exe
```

**Note:** The exact path depends on your CMake configuration. Check the Output window for the actual build directory.

### ? Verify CMake Target Exists

**In Visual Studio:**
1. Open Solution Explorer (Ctrl+Alt+L)
2. Switch to "CMake Targets View" (icon at top of Solution Explorer)
3. Expand the project
4. You should see: **kostur** (not Kostur or KosturProject)

**Via Command Line:**
```powershell
cd "C:\Users\damja\Grafika\Damjan Petrov RA17-2022"
cmake --build out/build/x64-Debug --target kostur
```

### ? Quick Verification Commands

Run these from your workspace root:

```powershell
# Check if CMake configured successfully
Get-ChildItem -Path "out\build" -Recurse -Filter "kostur.exe"

# Check CMake cache for target name
Select-String -Path "out\build\x64-Debug\CMakeCache.txt" -Pattern "kostur"
```

---

## ?? Common Errors and Fixes

### Error 1: "No CMake targets found"
**Symptom:** CMake configures but no targets appear

**Fix:**
```
1. Ensure CMakeLists.txt has add_executable(kostur ...)
2. Delete cache and reconfigure
3. Check for CMake errors in Output window
```

### Error 2: "Could not find OpenGL"
**Symptom:** `find_package(OpenGL REQUIRED)` fails

**Fix:**
Visual Studio 2022 should include OpenGL by default. If it fails:
1. Ensure Windows SDK is installed (via Visual Studio Installer)
2. Modify `CMakeLists.txt` to set `OpenGL_GL_PREFERENCE` to `GLVND` or `LEGACY`

### Error 3: "GLEW or GLFW libraries not found"
**Symptom:** Fatal error about missing packages

**Fix:**
```powershell
# From workspace root, restore NuGet packages
.\nuget.exe restore packages.config -PackagesDirectory packages

# If nuget.exe doesn't exist, download it first:
Invoke-WebRequest -Uri "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" -OutFile "nuget.exe"
```

### Error 4: "Cannot find source file Source/Main.cpp"
**Symptom:** CMake can't find source files

**Fix:**
1. Verify files exist: `Get-ChildItem Source\*.cpp`
2. Check file paths in `CMakeLists.txt` match actual filesystem (case-sensitive on some systems)
3. Ensure you're opening the correct root folder in VS

### Error 5: "Multiple targets named 'Kostur' or 'kostur'"
**Symptom:** Conflicting target names

**Fix:**
1. Delete the old build directory completely
2. Do NOT open the old `.sln` file in `_legacy_vs\`
3. Use ONLY "Open Folder" in VS2022 with CMakeLists.txt

### Error 6: "Wrong executable name (Kostur.exe instead of kostur.exe)"
**Symptom:** Target builds but has wrong name

**Fix:**
The updated `CMakeLists.txt` explicitly sets the target to lowercase `kostur`. If you still see `Kostur.exe`:
1. Delete the entire `out\` or `build\` folder
2. Delete cache and reconfigure
3. Rebuild from scratch

---

## ?? Alternative: Using vcpkg for GLFW/GLEW (Optional)

If you want to migrate from NuGet packages to vcpkg (recommended for CMake projects):

### Install vcpkg:
```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Install dependencies:
```powershell
.\vcpkg install glfw3:x64-windows
.\vcpkg install glew:x64-windows
```

### Update CMakeLists.txt:
Replace the manual library linking section with:
```cmake
# Use vcpkg's find_package
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(kostur PRIVATE 
    OpenGL::GL
    glfw
    GLEW::GLEW
)
```

### Configure VS with vcpkg:
In VS2022: Project ? CMake Settings ? Edit JSON ? Add:
```json
"cmakeToolchain": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

---

## ?? Summary

**What was fixed:**
1. ? Changed `add_executable(${PROJECT_NAME} ...)` to `add_executable(kostur ...)`
2. ? Changed project name to `KosturProject` (doesn't affect exe name)
3. ? Replaced `GLOB_RECURSE` with explicit file lists (more reliable)
4. ? Added all missing `.cpp` files (Rectangle.cpp, Camera.cpp, etc.)
5. ? Added error checking for missing libraries
6. ? Set explicit output directories
7. ? Added diagnostic messages

**Your workflow:**
1. Clean build folders
2. Open folder in VS2022
3. Delete Cache and Reconfigure
4. Select `kostur.exe` as startup item
5. Build and Run (F5)

**Expected result:**
`kostur.exe` in `out\build\x64-Debug\Debug\kostur.exe`

---

## ?? Next Steps

1. Verify the build works with the checklist above
2. If errors occur, check the "Common Errors" section
3. Consider migrating to vcpkg for cleaner dependency management
4. Add the old `_legacy_vs\` folder to `.gitignore` if not already ignored

Good luck! ??
