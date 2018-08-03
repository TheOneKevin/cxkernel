@ECHO OFF

ECHO Obtaining POSIX path...

REM Convert Windows path to POSIX:
REM Replace backslash with forward slash
REM Replace C:/ with /mnt/c/
SET P=%~dp0%
FOR %%I IN ("%P%") DO SET "GP=%%~fI\"
FOR /F %%I IN ('POWERSHELL -COMMAND "ECHO %GP% | sed -e 's/\\/\//g' -e 's/C:/\/mnt\/c/g'"') DO SET DIR=%%I

ECHO Starting Ubuntu WSL and configuring project...
REM Config twice
UBUNTU -c "cd %DIR% && ./configure"
UBUNTU -c "export PATH=$PATH:~/barebones-toolchain/cross/x86_64/bin && cd %DIR% && ./configure"

REM PAUSE