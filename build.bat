@echo off
echo --- BATTENING DOWN THE HATCHES ---
gcc Scavenger.c -o Scavenger.exe

if %errorlevel% neq 0 (
    echo [!] Compilation failed. Take a screenshot and DM me!
    pause
    exit /b
)

echo [+] Build successful! Launching Scavenger...
Scavenger.exe
