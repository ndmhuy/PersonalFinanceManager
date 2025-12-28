@echo off
title Personal Finance Manager - Launcher
echo ==================================================
echo      BUILDING: PERSONAL FINANCE MANAGER
echo ==================================================

:: 1. Create Build Directory safely
if not exist build mkdir build
cd build

:: 2. Configure
echo [1/3] Configuring...
cmake ..
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake Failed.
    echo Please make sure CMake is installed and added to your PATH.
    pause
    exit /b
)

:: 3. Compile
echo [2/3] Compiling...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation Failed. Check your code for errors.
    pause
    exit /b
)

echo [3/3] Launching App...
echo ==================================================

:: 4. SMART LAUNCHER (Finds the .exe wherever it is)

:: CASE A: Visual Studio Style (Inside Release folder)
if exist "Release\PersonalFinanceManager.exe" (
    cd Release
    PersonalFinanceManager.exe
    goto :end
)

:: CASE B: MinGW / Standard Style (Directly in build folder)
if exist "PersonalFinanceManager.exe" (
    PersonalFinanceManager.exe
    goto :end
)

:: CASE C: Debug Folder (Fallback)
if exist "Debug\PersonalFinanceManager.exe" (
    echo [WARNING] Running Debug version...
    cd Debug
    PersonalFinanceManager.exe
    goto :end
)

:: ERROR: File not found
echo.
echo [ERROR] Build successful, but cannot find PersonalFinanceManager.exe.
echo Check the 'build' folder to see where it went.

:end
pause