@ECHO OFF
CLS
ECHO Trinity Core
:1
start "Trinity Core" /B /MIN /WAIT worldserver.exe -c worldserver.conf
if %errorlevel% == 0 goto end
goto 1
:end