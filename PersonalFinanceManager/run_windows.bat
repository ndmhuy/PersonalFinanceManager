@echo off
setlocal
title Personal Finance Manager - Universal Builder

:: 1. Navigate to script folder
cd /d "%~dp0"

echo ==================================================
echo      BUILDING: PERSONAL FINANCE MANAGER
echo ==================================================

:: 2. CLEANUP
if exist build\CMakeCache.txt (
    echo [INFO] Cleaning up previous failed build configuration...
    del /f /q build\CMakeCache.txt
)
if not exist build mkdir build
cd build

:: 3. AUTO-DETECT COMPILER
:: Check if the user has MinGW (g++) installed
where g++ >nul 2>nul
if %errorlevel% equ 0 (
    echo [DETECTED] MinGW / G++ Compiler found.
    echo [INFO] configuring for MinGW Makefiles...
    cmake -G "MinGW Makefiles" ..
) else (
    echo [DETECTED] No MinGW found. Assuming Visual Studio.
    echo [INFO] Configuring for Visual Studio...
    :: We let CMake pick the newest Visual Studio version automatically
    cmake ..
)

:: 4. CHECK CONFIGURATION STATUS
if %errorlevel% neq 0 (
    echo.
    echo ==============================================================
    echo [CRITICAL ERROR] NO C++ COMPILER FOUND!
    echo ==============================================================
    echo CMake cannot find Visual Studio or MinGW on this computer.
    echo.
    echo TO FIX THIS:
    echo 1. Install "Visual Studio Community" (Free).
    echo 2. During install, check "Desktop development with C++".
    echo    (Just installing VS is not enough, you need the C++ tools!)
    echo.
    pause
    exit /b
)

:: 5. COMPILE
echo.
echo [INFO] Compiling...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build failed. See messages above.
    pause
    exit /b
)

echo.
echo [SUCCESS] Launching Application...
echo ==================================================

:: 6. SMART RUN
if exist "Release\PersonalFinanceManager.exe" (
    cd Release
    PersonalFinanceManager.exe
) else (
    if exist "PersonalFinanceManager.exe" (
        PersonalFinanceManager.exe
    ) else (
        echo [ERROR] Could not find executable file.
    )
)

pause