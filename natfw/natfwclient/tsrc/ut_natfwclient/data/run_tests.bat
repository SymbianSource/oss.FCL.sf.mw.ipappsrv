rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:  
rem

@echo off
echo.
echo ======================================================================
echo.
echo This runs natfw client unit tests.
echo
echo Test results: 
echo      epoc32\WINSCW\C\shared\EUnit\Logs\EUnit_log.xml
echo.
echo ======================================================================
echo.

:CHECK_EPOCROOT
echo.
echo ************************************************
echo   Verify epoc root
echo ************************************************
echo.
set EPOCROOT
if errorlevel == 1 goto END_ERROR 

:SET_PATHS
echo.
echo ************************************************
echo   Set paths.
echo   Remember to modify paths according to SDK.
echo ************************************************
echo.
set NATFW_TEST_ERROR=0
set UDEB_PATH=%EPOCROOT%epoc32\release\winscw\udeb
set NATFW_PATH=%EPOCROOT%s60\mw\ipappservices\natfw
set TOOLS_PATH=%NATFW_PATH%\natfwclient\tsrc\ut_natfwclient\data
set GROUP_PATH=%NATFW_PATH%\natfwclient\group
set EUNIT_RESULT_PATH=%EPOCROOT%epoc32\WINSCW\C\shared\EUnit\Logs
:set EUNIT_RESULT_PATH=%EPOCROOT%epoc32\WINSCW\C\QualityKit\EUnit\Logs
if errorlevel == 1 goto END_ERROR 

call coverage.bat
if %NATFW_TEST_ERROR% == 1 goto END_ERROR

:RUN_TESTS
echo.
echo ************************************************
echo   Run tests
echo   Wait!
echo ************************************************
echo.
call %UDEB_PATH%\eunitexerunner.exe /d alloc ut_cnatfwclient.dll
if errorlevel == 1 goto END_ERROR

:PROCESS_RESULTS
echo.
echo ************************************************
echo   Process results
echo ************************************************
echo.
call cd %EUNIT_RESULT_PATH%
EUnit_log.xml
call cd %NATFWCLIENT_TEST_GROUP_PATH%
call ctcpost -p profile.txt
call ctc2html -i profile.txt
if errorlevel == 1 goto END_ERROR
goto END

:END_ERROR
echo.
echo ************************************************
echo Execution failed. 
echo ************************************************
goto FINAL_END

:END
echo.
echo ================================================
echo Tests run succesfully!
echo ================================================

:FINAL_END
call cd %TOOLS_PATH%
echo.
