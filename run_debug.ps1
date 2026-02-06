# Test script for running kostur.exe
Write-Host "=== Running Kostur Application ===" -ForegroundColor Cyan
Write-Host ""

$exePath = "out\build\x64-Debug\Debug\kostur.exe"

if (Test-Path $exePath) {
    Write-Host "Found executable at: $exePath" -ForegroundColor Green
    Write-Host "Starting application..." -ForegroundColor Yellow
    Write-Host ""
    
    # Run the application
    & $exePath
    
    Write-Host ""
    Write-Host "Application exited." -ForegroundColor Cyan
} else {
    Write-Host "ERROR: Executable not found at: $exePath" -ForegroundColor Red
    Write-Host "Please build the project first." -ForegroundColor Yellow
}
