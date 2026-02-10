@echo off
echo Copying Assets to build directory...
xcopy /E /I /Y "Assets\*" "out\build\x64-Debug\Debug\Assets\"
echo Done!
pause
