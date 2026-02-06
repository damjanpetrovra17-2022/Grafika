# Clean Build Script for Kostur
# Run this when you need to start fresh
# Usage: .\clean_build.ps1

Write-Host "=== Kostur Build Cleanup ===" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will delete all CMake build artifacts to give you a fresh start." -ForegroundColor Yellow
Write-Host ""

$itemsToDelete = @(
    @{Path="build"; Description="CMake build directory"},
    @{Path="out"; Description="Visual Studio CMake output"},
    @{Path=".vs"; Description="Visual Studio cache"},
    @{Path="CMakeCache.txt"; Description="CMake cache file"},
    @{Path="CMakeFiles"; Description="CMake generated files"},
    @{Path="CMakeSettings.json"; Description="VS CMake settings (will be regenerated)"}
)

Write-Host "Items to be deleted:" -ForegroundColor Yellow
foreach ($item in $itemsToDelete) {
    if (Test-Path $item.Path) {
        Write-Host "  [EXISTS] $($item.Path) - $($item.Description)" -ForegroundColor Red
    } else {
        Write-Host "  [NOT FOUND] $($item.Path) - $($item.Description)" -ForegroundColor Gray
    }
}

Write-Host ""
$confirmation = Read-Host "Delete these items? (y/N)"

if ($confirmation -eq 'y' -or $confirmation -eq 'Y') {
    Write-Host "`nDeleting..." -ForegroundColor Yellow
    
    foreach ($item in $itemsToDelete) {
        if (Test-Path $item.Path) {
            try {
                Remove-Item -Path $item.Path -Recurse -Force -ErrorAction Stop
                Write-Host "  ? Deleted $($item.Path)" -ForegroundColor Green
            } catch {
                Write-Host "  ? Failed to delete $($item.Path): $_" -ForegroundColor Red
            }
        }
    }
    
    Write-Host "`n? Cleanup complete!" -ForegroundColor Green
    Write-Host ""
    Write-Host "=== Next Steps ===" -ForegroundColor Cyan
    Write-Host "1. Open Visual Studio 2022" -ForegroundColor White
    Write-Host "2. File ? Open ? Folder" -ForegroundColor White
    Write-Host "3. Select: C:\Users\damja\Grafika\Damjan Petrov RA17-2022" -ForegroundColor White
    Write-Host "4. Wait for CMake to configure" -ForegroundColor White
    Write-Host "5. Select 'kostur.exe' from startup dropdown" -ForegroundColor White
    Write-Host "6. Press F5 to build and run" -ForegroundColor White
    
} else {
    Write-Host "`nCleanup cancelled." -ForegroundColor Yellow
}

Write-Host ""
