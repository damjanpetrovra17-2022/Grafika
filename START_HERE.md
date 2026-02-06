# ?? START HERE - Building kostur.exe

## Quick Start (2 Minutes)

### Step 1: Verify Setup
```powershell
.\verify_kostur_setup.ps1
```

? If you see "Setup looks good!" ? Continue to Step 2  
? If you see errors ? Fix them first (usually NuGet packages)

### Step 2: Open in Visual Studio 2022
1. Launch Visual Studio 2022
2. **File** ? **Open** ? **Folder**
3. Select: `C:\Users\damja\Grafika\Damjan Petrov RA17-2022`
4. Wait for CMake to configure (watch Output window)

### Step 3: Build and Run
1. In toolbar: Click **"Select Startup Item"** dropdown
2. Choose: **kostur.exe** (should be the only option)
3. Press **F5** (or click green ? play button)

### Expected Result
? Build succeeds  
? `kostur.exe` appears in `out\build\x64-Debug\Debug\`  
? Application window opens  

---

## If Something Goes Wrong

### "No targets found" or "Can't select kostur.exe"
```
1. Project ? Delete Cache and Reconfigure
2. Wait for CMake to finish
3. Try again
```

### "GLEW or GLFW not found"
```powershell
.\nuget.exe restore packages.config -PackagesDirectory packages
```

### Still not working?
```powershell
# Clean everything and start fresh
.\clean_build.ps1

# Then reopen in VS2022
```

---

## Where is kostur.exe?

After building, find it at:
```
out\build\x64-Debug\Debug\kostur.exe
```

Or search:
```powershell
Get-ChildItem -Recurse -Filter "kostur.exe"
```

---

## ?? More Help

- **Complete guide:** See `CMAKE_BUILD_GUIDE.md`
- **Quick commands:** See `QUICK_REFERENCE.md`  
- **What was changed:** See `SOLUTION_SUMMARY.md`

---

## ? That's It!

You now have a working CMake setup that builds `kostur.exe`.

**Don't forget:** Always use "Open Folder" in VS2022, never open the old .sln files!
