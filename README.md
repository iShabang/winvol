# winvol
Set the volume level on Windows 10 from the command line

## Build
Requirements:
- Requires MSVC C/C++ Compiler
- Visual Studio build tools

Instructions:
- Open the Visual Studio Developer Command Prompt
- Navigate to the directory containing winvol.cpp
- Run the build script (build.cmd)

## Usage
Pass a value between 0 and 1 to set as the volume.
Example: Set the volume to 40%
```
winvol.exe 0.4
```
