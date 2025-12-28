#!/bin/bash
cd "$(dirname "$0")"

echo "=================================================="
echo "      BUILDING: PERSONAL FINANCE MANAGER"
echo "=================================================="

mkdir -p build
cd build

echo "[1/3] Configuring..."
cmake ..

echo "[2/3] Compiling..."
make

if [ $? -eq 0 ]; then
    echo "[3/3] Launching..."
    echo "=================================================="
    ./PersonalFinanceManager
else
    echo "[ERROR] Build Failed."
    exit 1
fi
