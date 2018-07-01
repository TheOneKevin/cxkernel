@ECHO OFF

REM Detect Windows version
FOR /f "TOKENS=4-6 DELIMS=[XP. " %%i IN ('VER') DO (
	SET VERSION=%%i.%%j
	SET /A BUILD=%%k
)

IF "%VERSION%" == "10.0" (
	IF %BUILD% GEQ 16299 (
		ECHO Windows 10 Fall Creators Update detected!
		GOTO :WIN_VER
	)
)

:FAIL
	ECHO You need to have Windows 10 Fall Creators Update installed to use the Windows Subsystem for Linux!
	PAUSE
	EXIT -1

:WIN_VER
	ECHO Detecting administrative privileges...
	NET SESSION >NUL 2>&1
	IF %ERRORLEVEL% == 0 (
		ECHO Administrative privileges confirmed
	) ELSE (
		ECHO Administrative privileges required.
		GOTO :ADMIN_FAIL
	)

ECHO Checking for Windows Subsystem for Linux...
WHERE /q wsl
IF %ERRORLEVEL% NEQ 0 ( GOTO :NO_WSL ) ELSE ( GOTO :CHECK_UBUNTU )

:NO_WSL
	REM If the WSL is not found, then enable it
	ECHO Please install the Windows Subsystem for Linux.
	:CHOICE1
	set /P c=Do you want us to install WSL for you ^(Y/N^)?^ 
	IF /I "%c%" EQU "Y" (
		ECHO The Windows Subsystem for Linux install will require a reboot to finish.
		ECHO Run this script again after the reboot to complete the installation.
		ECHO Press any key to continue installation...
		PAUSE >NUL
		ECHO Checking for and installing WSL...
		
		REM The following PowerShell command is a condensed version of the following:
		REM
		REM & {
		REM 	if((Get-WindowsOptionalFeature -Online -FeatureName "Microsoft-Windows-Subsystem-Linux").State -eq "Enabled") {
		REM 		ECHO "WSL has already been installed (this is likely a bug).";
		REM 	} else {
		REM 		ECHO "WSL has not been installed, WSL will be installed now.";
		REM 		ECHO "WSL installation will requre a reboot.";
		REM 		Enable-WindowsOptionalFeature -Online -FeatureName "Microsoft-Windows-Subsystem-Linux";
		REM 	}
		REM }
		
		POWERSHELL -COMMAND "& { if((Get-WindowsOptionalFeature -Online -FeatureName \"Microsoft-Windows-Subsystem-Linux\").State -eq \"Enabled\") { ECHO \"WSL has already been installed ^(this is likely a bug^).\"; } else { ECHO \"WSL has not been installed, WSL will be installed now.\"; ECHO \"WSL installation will requre a reboot.\"; Enable-WindowsOptionalFeature -Online -FeatureName \"Microsoft-Windows-Subsystem-Linux\"; } }"
		GOTO :CHECK_UBUNTU
	)
	IF /I "%c%" EQU "N" (
		ECHO Setup requires WSL to be installed and enabled. Please do that yourself, visit:
		ECHO https://docs.microsoft.com/en-us/windows/wsl/install-win10 for more info.
		GOTO :END
	)
	GOTO :CHOICE1


:CHECK_UBUNTU
	ECHO Checking for Ubuntu (WSL)...
	WHERE /q ubuntu.exe
	IF %ERRORLEVEL% NEQ 0 ( GOTO :NO_UBUNTU ) ELSE ( GOTO :SUCCESS )

:NO_UBUNTU
	ECHO No Ubuntu installation found, opening Windows Store webpage
	ECHO Press any key to open...
	PAUSE >NUL
	EXPLORER "https://www.microsoft.com/en-ca/store/p/ubuntu/9nblggh4msv6?SilentAuth=1&wa=wsignin1.0&rtc=1"

:SUCCESS
	ECHO Windows properly configured for WSL, please run lnenv.bat after running Ubuntu.exe at least once.
	PAUSE
	EXIT 0

:END
	REM EXIT /B exits the current script
	REM but doesn't close the terminal itself
	PAUSE
	EXIT -1

:ADMIN_FAIL
	SET CDIR=%~dp0
	POWERSHELL -COMMAND "& { Start-Process \"%CDIR%winenv.bat\" -verb RunAs}"
	EXIT -1