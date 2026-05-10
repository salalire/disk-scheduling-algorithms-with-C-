@echo off
:: ============================================================
::  build_server.bat
::
::  TEAM NOTES:
::  Run this file to compile the API server.
::
::  Requirements:
::    - MSYS2 installed with UCRT64 environment
::    - Packages installed via MSYS2:
::        pacman -S mingw-w64-ucrt-x86_64-gcc
::        pacman -S mingw-w64-ucrt-x86_64-asio
::    - crow_all.h (v1.2.1.2) in this folder
::
::  How to compile (run from MSYS2 UCRT64 terminal):
::    g++ server.cpp scan.cpp cscan.cpp Comparison.cpp headmovement.cpp -I. -I/ucrt64/include -lpthread -lws2_32 -lmswsock -std=c++17 -o server.exe
::
::  How to run:
::    ./server.exe
::
::  API will be live at: http://localhost:8080
:: ============================================================

echo Compiling Disk Scheduling API Server...
echo Make sure you are running this from the MSYS2 UCRT64 terminal.
echo.
echo Run this command in MSYS2:
echo g++ server.cpp scan.cpp cscan.cpp Comparison.cpp headmovement.cpp -I. -I/ucrt64/include -lpthread -lws2_32 -lmswsock -std=c++17 -o server.exe
