:: ==================================================================================
:: NAME:	scan.bat
:: DESCRIPTION:	This script does basic chck scan
:: ==================================================================================

SET DRIVE=%1

:: Set console.
:: void mode();
:: /************************************************************************************/
:mode
	echo off
	title SCAN TOOL.
	mode con cols=90 lines=36
	color 0A
	cls

	goto checkPermission
goto :eof


:: Checking for Administrator elevation.
:: void permission();
:: /************************************************************************************/
:checkPermission
	openfiles>nul 2>&1

	if %errorlevel% EQU 0 goto doScan

	call :print Checking for Administrator elevation.

	echo.    You are not running as Administrator.
	echo.    This tool cannot do it's job without elevation.
	echo.
	echo.    You need run this tool as Administrator.
	echo.

	echo.Press any key to continue . . .
	pause>nul
goto :eof


:doScan
	echo SCANNING DRIVE %DRIVE%...
	C:\Windows\system32\chkdsk.exe %DRIVE%:
goto :eof