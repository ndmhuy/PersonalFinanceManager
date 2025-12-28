@echo off
title Personal Finance Manager - Launcher

:: 1. Force script to run from its own directory
cd /d "%~dp0"

echo ==================================================
echo      BUILDING: PERSONAL FINANCE MANAGER
echo ==================================================

:: 2. Create Build Directory
if not exist build mkdir build
cd build

:: 3. Configure
echo [1/3] Configuring...
cmake ..
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake Configuration Failed.
    echo 1. Make sure CMake is installed.
    echo 2. Make sure CMakeLists.txt is in the project root.
    pause
    exit /b
)

:: 4. Compile
echo [2/3] Compiling...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation Failed. Check your code errors above.
    pause
    exit /b
)

echo [3/3] Launching App...
echo ==================================================

:: 5. SMART LAUNCHER (Handles VS and MinGW layouts)

:: Case A: Visual Studio (Release subfolder)
if exist "Release\PersonalFinanceManager.exe" (
    cd Release
    PersonalFinanceManager.exe
    goto :end
)

:: Case B: MinGW / Standard (Root build folder)
if exist "PersonalFinanceManager.exe" (
    PersonalFinanceManager.exe
    goto :end
)

:: Case C: Debug subfolder (Just in case)
if exist "Debug\PersonalFinanceManager.exe" (
    cd Debug
    PersonalFinanceManager.exe
    goto :end
)

echo.
echo [ERROR] Build succeeded, but cannot find the executable.
echo Look inside the 'build' folder to find 'PersonalFinanceManager.exe'.

:end
pause
