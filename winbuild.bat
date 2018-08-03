@ECHO OFF

ECHO Obtaining POSIX path...

REM Convert Windows path to POSIX:
REM Replace backslash with forward slash
REM Replace C:/ with /mnt/c/
SET P=%~dp0%
FOR %%I IN ("%P%") DO SET "GP=%%~fI\"
FOR /F %%I IN ('POWERSHELL -COMMAND "ECHO %GP% | sed -e 's/\\/\//g' -e 's/C:/\/mnt\/c/g'"') DO SET DIR=%%I

ECHO Starting Ubuntu WSL and building project...
UBUNTU -c "[ -d ~/barebones-toolchain/cross/x86_64/bin/ ] && { echo 'Barebones toolchain found.'; } || { echo 'Directory ~/barebones-toolchain/cross/x86_64/bin/ is missing. Did you run winconf.bat?'; }"
UBUNTU -c "export PATH=$PATH:~/barebones-toolchain/cross/x86_64/bin && cd %DIR% && make clean; make"
REM PAUSE