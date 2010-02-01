/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Common constants for TestClient
*
*/

#ifndef __TESTCLIENTCONSTANTS_H__
#define __TESTCLIENTCONSTANTS_H__

// INCLUDES
#include <e32std.h>
#include <in_sock.h>

// Default value for TestClient log file name
_LIT( KTcLogFile, "TestClient.log");
// Settings file name.
_LIT( KTcSettingsFile, "c:\\system\\data\\TestClient.ini" );
// Default TCP port number
const TInt KTcDefaultPort( 5070 );
// Default IAP Id
const TInt KTcDefaultIAPId( 1 );

const TInt KTcDefaultBaudrate = 115200;

// Enumeration for label array indices
enum TLabel
	{
	ETestCaseName,
	ETestCaseValue,
	ERequestName,
	ERequestValue,
	EStatusName,
	EStatusValue
	};

// Texts for "Name" labels
_LIT( KTestCase, "Testcase:" );
_LIT( KRequest, "Request:" );
_LIT( KStatus, "Status:" );
_LIT( KNone, "<none>" );

// Bearer types
enum TTcSettingsBearerType
    {
    EBearerBluetooth = 0,
    EBearerTCP,
    EBearerSerial
    };

// Constants for laying out labels on screen
#ifdef __SERIES60_
const TInt KLabelTlX( 10 );
const TInt KLabelTlY( 10 );
const TInt KLabelBrX( 166 );
const TInt KLabelBrY( 25 );
const TInt KLabelSpacing( 20 );
#else
const TInt KLabelTlX( 20 );
const TInt KLabelTlY( 20 );
const TInt KLabelBrX( 366 );
const TInt KLabelBrY( 50 );
const TInt KLabelSpacing( 30 );
#endif

// Maximum length for a label value
const TInt KMaxLabelSize( 64 );

// Baclight trigger once in 10 seconds
const TInt KPeriod10Sec( 10000000 );

// Status texts for the navigation label
_LIT8( KTcNotConnected, "Not connected" );
_LIT8( KTcInitializing, "Initializing" );
_LIT8( KTcListening, "Listening" );
_LIT8( KTcConnected, "Connected" );
_LIT8( KTcConnecting, "Connecting" );

// Watcher timeout, five minutes (quite long because in some test cases
// freezing the tester thread might be conscious
const TInt KWatcherTimeout( 300 );

#endif	// __TESTCLIENTCONSTANTS_H__
