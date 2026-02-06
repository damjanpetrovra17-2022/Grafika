# Kostur Build Verification Script
# Run this from the workspace root: .\verify_kostur_setup.ps1

Write-Host "=== Kostur Build Environment Verification ===" -ForegroundColor Cyan
Write-Host ""

# Check workspace root
$workspaceRoot = Get-Location
Write-Host "Workspace: $workspaceRoot" -ForegroundColor Green

# 1. Check CMakeLists.txt exists
Write-Host "`n[1/7] Checking CMakeLists.txt..." -ForegroundColor Yellow
if (Test-Path "CMakeLists.txt") {
    $content = Get-Content "CMakeLists.txt" -Raw
    if ($content -match 'add_executable\(kostur') {
        Write-Host "  ? CMakeLists.txt exists with 'kostur' target" -ForegroundColor Green
    } else {
        Write-Host "  ? CMakeLists.txt exists but 'kostur' target not found!" -ForegroundColor Red
        Write-Host "    Expected: add_executable(kostur ...)" -ForegroundColor Red
    }
} else {
    Write-Host "  ? CMakeLists.txt not found!" -ForegroundColor Red
}

# 2. Check source files
Write-Host "`n[2/7] Checking source files..." -ForegroundColor Yellow
$requiredSources = @(
    "Source\Main.cpp",
    "Source\Application.cpp",
    "Source\Window.cpp",
    "Shader.cpp",
    "Rectangle.cpp"
)

$missingFiles = @()
foreach ($file in $requiredSources) {
    if (Test-Path $file) {
        Write-Host "  ? $file" -ForegroundColor Green
    } else {
        Write-Host "  ? $file MISSING!" -ForegroundColor Red
        $missingFiles += $file
    }
}

if ($missingFiles.Count -eq 0) {
    Write-Host "  All required source files present" -ForegroundColor Green
}

# 3. Check NuGet packages
Write-Host "`n[3/7] Checking NuGet packages..." -ForegroundColor Yellow
$glfwLib = "packages\glfw.3.4.0\build\native\lib\static\v143\x64\glfw3.lib"
$glewLib = "packages\glew-2.2.0.2.2.0.1\build\native\lib\Release\x64\glew32s.lib"

if (Test-Path $glfwLib) {
    Write-Host "  ? GLFW library found" -ForegroundColor Green
} else {
    Write-Host "  ? GLFW library NOT found at: $glfwLib" -ForegroundColor Red
    Write-Host "    Run: .\nuget.exe restore packages.config -PackagesDirectory packages" -ForegroundColor Yellow
}

if (Test-Path $glewLib) {
    Write-Host "  ? GLEW library found" -ForegroundColor Green
} else {
    Write-Host "  ? GLEW library NOT found at: $glewLib" -ForegroundColor Red
    Write-Host "    Run: .\nuget.exe restore packages.config -PackagesDirectory packages" -ForegroundColor Yellow
}

# 4. Check for old build artifacts
Write-Host "`n[4/7] Checking for stale build artifacts..." -ForegroundColor Yellow
$staleArtifacts = @()
if (Test-Path "build") { $staleArtifacts += "build\" }
if (Test-Path "out") { $staleArtifacts += "out\" }
if (Test-Path "CMakeCache.txt") { $staleArtifacts += "CMakeCache.txt" }
if (Test-Path "CMakeFiles") { $staleArtifacts += "CMakeFiles\" }

if ($staleArtifacts.Count -gt 0) {
    Write-Host "  ? Found stale build artifacts: $($staleArtifacts -join ', ')" -ForegroundColor Yellow
    Write-Host "  Recommendation: Delete these before rebuilding in VS2022" -ForegroundColor Yellow
} else {
    Write-Host "  ? No stale build artifacts (clean slate)" -ForegroundColor Green
}

# 5. Check GLM
Write-Host "`n[5/7] Checking GLM library..." -ForegroundColor Yellow
if (Test-Path "glm\glm\glm.hpp") {
    Write-Host "  ? GLM header library found" -ForegroundColor Green
} else {
    Write-Host "  ? GLM not found at glm\glm\glm.hpp" -ForegroundColor Red
}

# 6. Check for old .sln/.vcxproj (should not interfere)
Write-Host "`n[6/7] Checking for legacy Visual Studio files..." -ForegroundColor Yellow
if (Test-Path "_legacy_vs\Kostur.sln") {
    Write-Host "  ? Legacy .sln found in _legacy_vs\ (this is OK, do NOT open it)" -ForegroundColor Cyan
}
if (Test-Path "Bioskop3D.sln") {
    Write-Host "  ? Old Bioskop3D.sln found in root (safe to delete)" -ForegroundColor Yellow
}

# 7. Final recommendations
Write-Host "`n[7/7] Final Status" -ForegroundColor Yellow
if ($missingFiles.Count -eq 0 -and (Test-Path $glfwLib) -and (Test-Path $glewLib)) {
    Write-Host "  ? Setup looks good!" -ForegroundColor Green
    Write-Host ""
    Write-Host "=== Next Steps ===" -ForegroundColor Cyan
    Write-Host "1. Open Visual Studio 2022" -ForegroundColor White
    Write-Host "2. File ? Open ? Folder ? Select this directory" -ForegroundColor White
    Write-Host "3. Wait for CMake to configure" -ForegroundColor White
    Write-Host "4. Project ? Delete Cache and Reconfigure (if needed)" -ForegroundColor White
    Write-Host "5. Select 'kostur.exe' from the Startup Item dropdown" -ForegroundColor White
    Write-Host "6. Press F5 to build and run" -ForegroundColor White
    Write-Host ""
    Write-Host "Expected output: out\build\x64-Debug\Debug\kostur.exe" -ForegroundColor Green
} else {
    Write-Host "  ? Issues found - fix the errors above first" -ForegroundColor Red
}

Write-Host "`n=== Verification Complete ===" -ForegroundColor Cyan
