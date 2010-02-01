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
* Description:  Common constants for TesterCore
*
*/

#ifndef __TESTERCONSTANTS_H__
#define __TESTERCONSTANTS_H__

// INCLUDES
#include <e32std.h>

/// CTRL delimiters
const TUint8 KTcField		( 0xFE );
const TUint8 KTcType		( 0xF7 );
const TUint8 KTcArrayBegin	( 0xF3 );
const TUint8 KTcArrayEnd	( 0xF2 );
const TUint8 KTcArrayValue	( 0xF5 );
const TUint8 KTcStructBegin	( 0xFA );
const TUint8 KTcStructEnd	( 0xF9 );
const TUint8 KTcScope		( '.' );
const TUint8 KTcPduEnd		( '$' );

/// PDU type headers
_LIT8( KTcReqPdu, "REQ" );
_LIT8( KTcResPdu, "RES" );
_LIT8( KTcErrPdu, "ERR" );
_LIT8( KTcResetPdu, "_RESET_" );
_LIT8( KTcStartPdu, "_START_" );
_LIT8( KTcEndPdu, "_END_" );
_LIT8( KTcOkPdu, "_OK_" );
_LIT8( KTcCTRLVersion, "CTRL/2.0" );

// CTRL paramter names and values
_LIT8( KTcTrue, "true" );
_LIT8( KTcFalse, "false" );
_LIT8( KTcActionReturnCode, "ActionReturnCode" );
_LIT8( KTcTestClientIds, "TestClientIds" );
_LIT8( KTcParamLog, "Log" );

_LIT8( KTcOptional, "optional");
_LIT8( KTcMandatory, "mandatory");
const TInt KTcOptionalLength( 10 );
_LIT8( KLogOk, " => OK\n" );
_LIT8( KLogNotFound, " => NOT FOUND\n" );
_LIT8( KLogNotAllowed, " => NOT ALLOWED\n" );

/// Maximum length for an object name in registry
const TInt KTcMaxObjectName( 16 );

/// Maximum length for type name
const TInt KTcMaxTypeName( 32 );

/// Maximum length for "current test case" and "current request name"
const TInt KTcMaxInfoName( 32 );

/// Maximum length for an error message
const TInt KTcMaxErrorMsg( 128 );

// Minimum overhead but with 50% less allocations than with granularity of 1
const TInt KTcGranularity( 2 );

// Longest string is -2147483648, which is 11 characters
// (but 11 is not even, so lets choose 12 instead)
const TInt KTcMaxIntConversion( 12 );

/// Maximum CTRL request size. Large enough to test with messages that are
/// slightly over 64kB.
const TInt KTcRequestMaxLength( 70000 );

/// Maximum CTRL response size (40kb!), allow for large SIP messages and logs to be carried
const TInt KTcResponseMaxLength( 40960 );

// Tester plugin directory path
//_LIT( KTcDllPath, "\\system\\apps\\TestClient\\plugins\\" );

// Tester plugin name match string
//_LIT( KTcDllWild, "*Tester.dll" );

const TUid KTesterInterfaceUid = { 0x10203352 };

// Error messages sent to TTCN system
_LIT8( KTcErrUnknownContextMsg, "Start failed, unknown test context." );
_LIT8( KTcErrNoActiveTestCaseMsg, "No active test case.");
_LIT8( KTcErrReceiveOverflowMsg, "Receive buffer full." );
_LIT8( KTcErrRequestCorruptMsg, "Unrecognized or malformed request." );
_LIT8( KTcErrMandatoryIdNotFoundMsg, "Mandatory TestClient ID not found in request." );
_LIT8( KTcErrMandatoryParameterNotFoundMsg, "Mandatory Parameter not found in request." );
_LIT8( KTcErrMandatoryHeaderNotFoundMsg, "Mandatory Header not found in request." );
_LIT8( KTcErrMandatoryStructureItemNotFoundMsg, "Mandatory Structure item not found in request." );
_LIT8( KTcErrHeaderNotAllowedMsg, "Header not allowed." );
_LIT8( KTcErrObjectNotFoundMsg, "Invalid TestClient ID (object not found)." );
_LIT8( KTcErrTestClientInternalMsg, "Internal error %d in CTcTestRunner::ExecuteL(), or test context creation failed." );
_LIT8( KTcErrNoMemoryMsg, "Not enough memory to perform requested operation." );
_LIT8( KTcErrUnableToRetrieveLog, "<unable to retrieve log>" );

// Custom error codes
const TInt KTcErrBase( -90 );
const TInt KTcErrUnknownContext( -91 );
const TInt KTcErrNoActiveTestCase( -92 );
const TInt KTcErrReceiveOverflow( -93 );
const TInt KTcErrRequestCorrupt( -94 );
const TInt KTcErrMandatoryIdNotFound( -95 );
const TInt KTcErrMandatoryParameterNotFound( -96 );
const TInt KTcErrMandatoryHeaderNotFound( -97 );
const TInt KTcErrMandatoryStructureItemNotFound( -98 );
const TInt KTcErrHeaderNotAllowed( -99 );
const TInt KTcErrObjectNotFound( -100 );
const TInt KTcErrMax( KTcErrObjectNotFound );

#endif	// __TESTERCONSTANTS_H__
