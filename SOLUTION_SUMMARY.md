# ? SOLUTION SUMMARY: kostur.exe CMake Build

## ?? Problem
You renamed your app from Bioskop3D to "kostur" but were not getting `kostur.exe` anywhere when building with CMake in Visual Studio 2022.

## ? Root Cause
The CMakeLists.txt was using `add_executable(${PROJECT_NAME} ...)` where `PROJECT_NAME` was "Kostur" (capital K), which resulted in `Kostur.exe` instead of `kostur.exe`.

## ?? Solution Applied

### A) Updated CMakeLists.txt

**Key Changes:**
1. ? **Explicit target name:** Changed to `add_executable(kostur ...)` (lowercase, hardcoded)
2. ? **All source files listed:** Replaced GLOB with explicit file lists
3. ? **Complete file list:** Added Camera.cpp, DebugCube.cpp, Input.cpp, AppTime.cpp, AABB.h
4. ? **Library validation:** Added checks to ensure GLEW/GLFW exist before linking
5. ? **Output directories:** Set explicit paths for easier finding of the .exe
6. ? **Diagnostic messages:** Added status messages to confirm target name

**Files Modified:**
- `CMakeLists.txt` (main project file)

**Files Created:**
- `CMAKE_BUILD_GUIDE.md` (comprehensive build guide)
- `QUICK_REFERENCE.md` (quick reference card)
- `verify_kostur_setup.ps1` (setup verification script)
- `clean_build.ps1` (build cleanup script)

---

## ?? B) Cleanup + Rebuild Workflow

### Option 1: Visual Studio 2022 (Recommended)

1. **Clean (if needed):**
   ```powershell
   # Run from workspace root
   .\clean_build.ps1
   ```

2. **Open in VS2022:**
   - File ? Open ? Folder
   - Select: `C:\Users\damja\Grafika\Damjan Petrov RA17-2022`

3. **Reconfigure:**
   - Project ? Delete Cache and Reconfigure
   - Watch Output window for: "Target executable name: kostur"

4. **Build and Run:**
   - Select "kostur.exe" from Startup Item dropdown
   - Press **F5** to build and run

### Option 2: Command Line

```powershell
cd "C:\Users\damja\Grafika\Damjan Petrov RA17-2022"

# Clean
Remove-Item build, out, .vs -Recurse -Force -ErrorAction SilentlyContinue

# Configure
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Debug

# The exe will be at:
# build\Debug\kostur.exe
```

---

## ?? C) Verification Checklist

### ? Pre-Build Checks

Run the verification script:
```powershell
.\verify_kostur_setup.ps1
```

Expected output:
```
? CMakeLists.txt exists with 'kostur' target
? All required source files present
? GLFW library found
? GLEW library found
? Setup looks good!
```

### ? CMake Configuration

When VS2022 configures, you should see in the Output window:
```
-- Target executable name: kostur
-- Output directory (Debug): .../build/Debug
-- Output directory (Release): .../build/Release
-- Using NuGet packages for GLEW and GLFW
-- Configuring done
-- Generating done
```

### ? Target Verification

In Visual Studio Solution Explorer (CMake Targets View):
```
?? KosturProject
  ?? ?? kostur (executable target)
```

In the toolbar dropdown "Select Startup Item":
```
? kostur.exe  ? Should appear here (lowercase)
```

### ? Build Output Location

After successful build, `kostur.exe` will be located at:

**Using VS2022 "Open Folder":**
```
out\build\x64-Debug\Debug\kostur.exe
out\build\x64-Release\Release\kostur.exe
```

**Using command-line CMake:**
```
build\Debug\kostur.exe
build\Release\kostur.exe
```

Verify with:
```powershell
Get-ChildItem -Recurse -Filter "kostur.exe"
```

---

## ?? Common Errors & Fixes

### ? Error: "GLEW or GLFW libraries not found"

**Cause:** NuGet packages not restored

**Fix:**
```powershell
.\nuget.exe restore packages.config -PackagesDirectory packages
```

If `nuget.exe` doesn't exist:
```powershell
Invoke-WebRequest -Uri "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" -OutFile "nuget.exe"
```

---

### ? Error: "No CMake targets found"

**Cause:** Opened wrong folder or .sln file instead of using CMake

**Fix:**
1. Close VS2022
2. Run `.\clean_build.ps1`
3. Reopen VS2022
4. File ? Open ? **Folder** (NOT File ? Open ? Project/Solution)
5. Select workspace root

---

### ? Error: "Cannot find source file"

**Cause:** CMakeLists.txt file paths don't match actual filesystem

**Fix:**
Already fixed in the updated CMakeLists.txt with correct paths:
```cmake
Source/Application.cpp  # (with forward slash)
Shader.cpp             # (root level files)
```

---

### ? Still Getting "Kostur.exe" (capital K) Instead of "kostur.exe"

**Cause:** Old CMake cache not cleared

**Fix:**
```powershell
# Delete everything
.\clean_build.ps1

# Or manually:
Remove-Item build, out, .vs, CMakeCache.txt, CMakeFiles -Recurse -Force

# Then reopen in VS2022
```

---

### ? Error: "OpenGL not found"

**Cause:** Windows SDK not installed or CMake can't find it

**Fix:**
1. Open Visual Studio Installer
2. Modify Visual Studio 2022
3. Ensure "Windows 10/11 SDK" is checked under "Desktop development with C++"
4. Install/Update

---

## ?? What NOT To Do

? **Don't open `_legacy_vs\Kostur.sln`** - This is the old MSBuild project. Ignore it.

? **Don't edit .vcxproj files** - CMakeLists.txt is now the single source of truth.

? **Don't use File ? Open ? Project/Solution** - Use File ? Open ? Folder for CMake projects.

? **Don't manually edit CMakeCache.txt** - Always delete it and reconfigure instead.

? **Don't mix build systems** - Choose either CMake OR .sln, not both. You're now using CMake.

---

## ?? Understanding the Fix

### Before (broken):
```cmake
project(Kostur VERSION 1.0 LANGUAGES CXX)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
# This created Kostur.exe (capital K)
```

### After (fixed):
```cmake
project(KosturProject VERSION 1.0 LANGUAGES CXX)
add_executable(kostur ${SOURCE_FILES})
# This creates kostur.exe (lowercase, explicit)
```

**Why it matters:**
- `${PROJECT_NAME}` is a variable that resolved to "Kostur"
- Hardcoding `kostur` ensures the exact name you want
- The project name "KosturProject" is separate from the executable name

---

## ?? Next Steps After Building

Once you successfully get `kostur.exe`:

1. **Run it:**
   ```powershell
   # From build directory
   .\out\build\x64-Debug\Debug\kostur.exe
   
   # Or just press F5 in Visual Studio
   ```

2. **Verify it works:**
   - Window should open
   - Check for OpenGL rendering
   - Check console output for any errors

3. **Optional: Migrate to vcpkg:**
   - See `CMAKE_BUILD_GUIDE.md` section "Alternative: Using vcpkg"
   - Cleaner dependency management than NuGet for C++ projects

4. **Add to .gitignore:**
   ```
   build/
   out/
   .vs/
   CMakeCache.txt
   CMakeFiles/
   _legacy_vs/
   *.exe
   *.pdb
   ```

---

## ?? Support Files

All documentation is now in your workspace:

- **CMAKE_BUILD_GUIDE.md** - Complete step-by-step guide
- **QUICK_REFERENCE.md** - One-page cheat sheet
- **verify_kostur_setup.ps1** - Run to check if everything is set up
- **clean_build.ps1** - Run to clean build directories
- **This file** - Summary of what was done

---

## ? Success Criteria

You'll know it's working when:

1. ? `.\verify_kostur_setup.ps1` shows all green checkmarks
2. ? VS2022 CMake output shows "Target executable name: kostur"
3. ? Startup dropdown shows "kostur.exe" (lowercase)
4. ? Build completes with 0 errors
5. ? `kostur.exe` file exists in `out\build\...\Debug\`
6. ? Application runs when you press F5

---

## ?? Summary

**Problem:** No kostur.exe being generated  
**Cause:** CMakeLists.txt using wrong target name  
**Solution:** Updated CMakeLists.txt to explicitly use `add_executable(kostur ...)`  
**Result:** kostur.exe will now be generated correctly  

**Total files modified:** 1 (CMakeLists.txt)  
**Helper files created:** 4 (guides + scripts)  
**Time to rebuild:** ~2-5 minutes  

---

**Everything is ready. Follow the steps in section B) and you'll have kostur.exe!** ??
