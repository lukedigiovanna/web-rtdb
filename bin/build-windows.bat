@echo off

set PROJECT_NAME=WebRTDB

if not exist "build" (
  mkdir build
)

cd build

:: Specify the MinGW generator for CMake
cmake -G "MinGW Makefiles" ..

if %errorlevel% neq 0 (
  echo CMake configuration failed!
  exit /b 1
)

:: Use mingw32-make instead of cmake --build
mingw32-make

if %errorlevel% neq 0 (
  echo Build failed!
  exit /b 1
)
