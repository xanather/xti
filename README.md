# Making Touch Input Great Again
The pre-existing Windows virtual touch keyboard is terrible for typing on for productivity.
+ Mainly designed for programmers.
+ Has basic extensible JSON config.

## Known limitations
- Don't change scaling or DPI of the system after starting.
- The program expects only one primary screen attached (the tablet screen).
- Must run as administrator otherwise Windows will deny access to certain functions.

## Installing
Supports either x64 or Arm64 machines.
1. Install Qt with desktop feature
2. Install C++ Visual Studio feature (or Visual Studio C++ Build Tools)
3. Install CMake
4. Run build.ps1
5. Copy the set of .dll's and exe binaries in /release-output to where ever you want. Run the exe as admin.

## Before Running
1. Create a config file in user profile directory at ~/xti.json. See example-xti.json at root of repository for example usage.

## Developing
This is a C++ CMake QT Creator project https://en.wikipedia.org/wiki/Qt_Creator. Simply open up the CMakeLists.txt file.
