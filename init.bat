git submodule init
git submodule update
if exist Dependencies rmdir /s /Q Dependencies
mkdir Dependencies
cd Dependencies
if exist dependencies.h del dependencies.h
( echo [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12 && echo.Invoke-WebRequest https://github.com/glfw/glfw/releases/download/3.3/glfw-3.3.bin.WIN32.zip -OutFile glfw-3.3.zip) > glfw.ps1
Powershell.exe -executionpolicy remotesigned -File glfw.ps1
Powershell.exe "Expand-Archive glfw-3.3.zip"
del glfw-3.3.zip
del glfw.ps1
cd glfw-3.3
move glfw-3.3.bin.WIN32\include .\
move glfw-3.3.bin.WIN32\lib-vc2017 .\
move glfw-3.3.bin.WIN32\LICENSE.md .\
rename lib-vc2017 lib
rmdir /s /Q glfw-3.3.bin.WIN32