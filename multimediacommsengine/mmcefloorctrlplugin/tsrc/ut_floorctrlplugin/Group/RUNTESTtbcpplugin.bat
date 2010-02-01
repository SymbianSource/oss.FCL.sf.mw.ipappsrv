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

@ECHO OFF
ECHO This is run for UT_CFCPlugInTest tests
cd\
cd \FloorControl\FCTBCPPlugIn\Test\Group
ECHO Build the test dll
call bldmake bldfiles
call abld reallyclean
call abld test build winscw udeb
PAUSE
cd\
ECHO Delete the old test dll in the test folder
copy \Epoc32\release\winscw\udeb\UT_CFCPlugInTest.dll \epoc32\RELEASE\winscw\UDEB\z\sys\bin
del \epoc32\RELEASE\winscw\UDEB\z\sys\bin\UT_CFCPlugInTest.dll
ECHO Copy the new one to the test folder
copy \Epoc32\release\winscw\udeb\UT_CFCPlugInTest.dll \epoc32\RELEASE\winscw\UDEB\z\sys\bin
PAUSE
ECHO Run the test and create the test result
call \Epoc32\release\winscw\udeb\eunitexerunner /L XML /E ExeEnv /T 60 UT_CFCPlugInTest.dll

