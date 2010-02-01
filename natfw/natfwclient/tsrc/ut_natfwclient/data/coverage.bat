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
echo This BAT instruments natfwclient unit tests
echo.
echo ======================================================================
echo.

:SET_PATHS
echo.
echo ************************************************
echo   Set paths.
echo   Remember to modify paths according to SDK.
echo ************************************************
echo.
set UDEB_PATH=%EPOCROOT%epoc32\release\winscw\udeb
set NATFW_PATH=%EPOCROOT%s60\mw\ipappservices\natfw
set SRC_PATH=%NATFW_PATH%\natfwclient\src
set TOOLS_PATH=%NATFW_PATH%\natfwclient\tsrc\ut_natfwclient\data
set GROUP_PATH=%NATFW_PATH%\natfwclient\group
set NATFWCLIENT_TEST_GROUP_PATH=%NATFW_PATH%\natfwclient\tsrc\ut_natfwclient\group
set EUNIT_RESULT_PATH=%EPOCROOT%epoc32\WINSCW\C\shared\EUnit\Logs
:set EUNIT_RESULT_PATH=%EPOCROOT%epoc32\WINSCW\C\QualityKit\EUnit\Logs
if errorlevel == 1 goto END_ERROR

:INSTRUMENT_AND_BUILD_TEST_PRJ
echo.
echo ************************************************
echo   Instrument Test project
echo ************************************************
echo.
call cd %NATFWCLIENT_TEST_GROUP_PATH%
call cd
echo delete old stuff if exists ( Mon.dat, MON.sym, profile.txt and CTCHTML directory )
call del Mon.dat
call del MON.sym
call del profile.txt
call del CTCHTML /Q
echo.
call bldmake bldfiles
call abld reallyclean winscw udeb
call ctcwrap -i d -C "EXCLUDE=*" -C "NO_EXCLUDE=%SRC_PATH%\natfwcandidatepair.cpp,%SRC_PATH%\natfwclient.cpp,%SRC_PATH%\natfwcredentials.cpp,%SRC_PATH%\natfwsession.cpp,%SRC_PATH%\natfwcandidate.cpp,%SRC_PATH%\natfwstream.cpp,%SRC_PATH%\cnatfwasynccallback.cpp,%SRC_PATH%\tnatfwcallbackinfo.cpp" abld build winscw udeb

call copy %UDEB_PATH%\ut_cnatfwclient.dll %UDEB_PATH%\z\sys\bin\ut_cnatfwclient.dll
if errorlevel == 1 goto END_ERROR
goto END

:END_ERROR
set NATFW_TEST_ERROR=1
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
echo.
EXIT /B 0