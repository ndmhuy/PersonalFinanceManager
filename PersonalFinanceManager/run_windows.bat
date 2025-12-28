@echo off
title Personal Finance Manager - Auto Builder
echo ==================================================
echo      BUILDING: PERSONAL FINANCE MANAGER
echo ==================================================

if not exist build mkdir build
cd build

echo [1/3] Configuring...
cmake ..
if %errorlevel% neq 0 ( pause & exit /b )

echo [2/3] Compiling...
cmake --build . --config Release
if %errorlevel% neq 0 ( pause & exit /b )

echo [3/3] Launching App...
echo ==================================================
cd Release
if exist PersonalFinanceManager.exe (
    PersonalFinanceManager.exe
) else (
    echo Executable not found. Check build errors.
    pause
)