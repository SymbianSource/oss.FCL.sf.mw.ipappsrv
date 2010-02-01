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
* Description:
*
*/

#include <sdpcodecstringpool.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdporiginfield.h>
#include <sdpmediafield.h>
#include <in_sock.h>
#include "nspcontroller.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspsessiondata.h"
#include "nsputil.h"
#include "ut_nsputil.h"
#include "nsptesthelper.h"
#include "nsptls.h"

const TUint KRejectPort = 0;

// ---------------------------------------------------------------------------
// UT_NSPUtility::NewL
// ---------------------------------------------------------------------------
UT_NSPUtil* UT_NSPUtil::NewL()
    {
    UT_NSPUtil* self = UT_NSPUtil::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::NewLC
// ---------------------------------------------------------------------------
UT_NSPUtil* UT_NSPUtil::NewLC()
    {
    UT_NSPUtil* self = new( ELeave ) UT_NSPUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::~UT_NSPUtil
// ---------------------------------------------------------------------------
UT_NSPUtil::~UT_NSPUtil()
    {
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_NSPUtil
// ---------------------------------------------------------------------------
UT_NSPUtil::UT_NSPUtil()
    {
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::ConstructL
// ---------------------------------------------------------------------------
void UT_NSPUtil::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::SetupL
// ---------------------------------------------------------------------------
void UT_NSPUtil::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    iController = CNSPController::NewL();
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::Teardown
// ---------------------------------------------------------------------------
void UT_NSPUtil::Teardown()
    {
    delete iController;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }


// TESTS:
// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_UpdateConnectionFieldL
// ---------------------------------------------------------------------------
void UT_NSPUtil::UT_UpdateConnectionFieldL()
    {
    _LIT8( KLocalConn, "c=IN IP4 10.0.0.4\r\n" );
    _LIT8( KLocalIPv6, "c=IN IP6 2001:db8::1428:55ab\r\n" );
    _LIT8( KNoneConn,  "c=IN IP4 0.0.0.0\r\n" );
    //_LIT8( KNoneIPv6,  "c=IN IP6 ::\r\n" );
    CSdpConnectionField* connField = CSdpConnectionField::DecodeLC( KLocalConn() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("10.0.0.16") ) );
    NSP_ASSERT( !connField->Address().Compare( _L8("10.0.0.16") ) );
    
    CleanupStack::PopAndDestroy( connField );
    connField = CSdpConnectionField::DecodeLC( KLocalConn() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("2001:db8::1428:57ab") ) );
    NSP_ASSERT( !connField->Address().Compare( _L8("2001:db8::1428:57ab") ) );
    NSP_ASSERT( KAfInet6 == connField->InetAddress()->Family() );
    
    CleanupStack::PopAndDestroy( connField );
    connField = CSdpConnectionField::DecodeLC( KLocalConn() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("www.example.com") ) );
    NSP_ASSERT( !connField->Address().Compare( _L8("www.example.com") ) );
    
    CleanupStack::PopAndDestroy( connField );
    connField = CSdpConnectionField::DecodeLC( KLocalIPv6() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("www.example.com") ) );
    NSP_ASSERT( !connField->Address().Compare( _L8("www.example.com") ) );
    
    CleanupStack::PopAndDestroy( connField );
    connField = CSdpConnectionField::DecodeLC( KLocalIPv6() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("10.0.0.4") ) );
    NSP_ASSERT( !connField->Address().Compare( _L8("10.0.0.4") ) );
    NSP_ASSERT( KAfInet == connField->InetAddress()->Family() );
    
    CleanupStack::PopAndDestroy( connField );
    connField = CSdpConnectionField::DecodeLC( KNoneConn() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("10.0.0.4") ) );
    NSP_ASSERT( connField->InetAddress()->IsUnspecified() );
    
    CleanupStack::PopAndDestroy( connField );
    
    /*
    connField = CSdpConnectionField::DecodeLC( KNoneIPv6() );
    
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateConnectionFieldL( *connField, _L8("10.0.0.4") ) );
    NSP_ASSERT( connField->InetAddress()->IsUnspecified() );
    
    CleanupStack::PopAndDestroy( connField );
    */
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_UpdateOriginFieldL
// ---------------------------------------------------------------------------
void UT_NSPUtil::UT_UpdateOriginFieldL()
    {
    _LIT8( KOrigin, "o=bob 63364672769249125 63364672769249125 IN IP4 192.168.1.101\r\n" );
    _LIT8( KOriginIPv6, "o=bob 63364672769249125 63364672769249125 IN IP6 2001:db8::1428:55ab\r\n" );
    _LIT8( KOriginNone,  "o=bob 63364672769249125 63364672769249125 IN IP4 0.0.0.0\r\n" );
    CSdpOriginField* originField = CSdpOriginField::DecodeLC( KOrigin() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateOriginFieldL( *originField, _L8("10.0.0.16") ) );
    NSP_ASSERT( !originField->Address().Compare( _L8("10.0.0.16") ) );
    
    CleanupStack::PopAndDestroy( originField );
    originField = CSdpOriginField::DecodeLC( KOriginIPv6() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateOriginFieldL( *originField, _L8("10.0.0.16") ) );
    NSP_ASSERT( !originField->Address().Compare( _L8("10.0.0.16") ) );
    NSP_ASSERT( KAfInet == originField->InetAddress()->Family() );
    
    CleanupStack::PopAndDestroy( originField );
    originField = CSdpOriginField::DecodeLC( KOriginNone() );
    
    // OK, ADDRESS NOT CHANGED SINCE IT IS UNSPECIFIED(I.E. OLDWAY HOLD)
    NSP_ASSERT_NO_LEAVE( NSPUtil::UpdateOriginFieldL( *originField, _L8("10.0.0.16") ) );
    NSP_ASSERT( originField->InetAddress()->IsUnspecified() );
    NSP_ASSERT( originField->Address().Compare( _L8("10.0.0.16") ) );
    
    CleanupStack::PopAndDestroy( originField );
    }

// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_RejectLL
// ---------------------------------------------------------------------------
void UT_NSPUtil::UT_RejectLL()
	{
	_LIT8( KAudioStream, "m=audio 2330 RTP/AVP 96\r\n" );
	CSdpMediaField* field = NULL;
	
	// NULL CAUSES LEAVE
	NSP_ASSERT_LEAVE( NSPUtil::RejectL( *field ) );
	
	field = CSdpMediaField::DecodeLC( KAudioStream() );
	
	NSP_ASSERT_NO_LEAVE( NSPUtil::RejectL( *field ) );
	NSP_ASSERT( KRejectPort == field->Port() );
	
	CleanupStack::PopAndDestroy( field );
	}

// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_IsRejectL
// ---------------------------------------------------------------------------
void UT_NSPUtil::UT_IsRejectL()
	{
	_LIT8( KAudioStream, "m=audio 2330 RTP/AVP 96\r\n" );
	_LIT8( KVideoStream, "m=video 2430 RTP/AVP 98\r\n" );
	_LIT8( KReject, "m=video 0 RTP/AVP 98\r\n" );
	CSdpMediaField* field = NULL;
	
	// NOT VALID FIELD ==> NOT REJECT PORT
	NSP_ASSERT( !NSPUtil::IsReject( *field ) );
	
	field = CSdpMediaField::DecodeLC( KAudioStream() );
	
	// VALID FIELD WITH VALID MEDIA PORT, NOT REJECT
	NSP_ASSERT( !NSPUtil::IsReject( *field ) );
	
	CleanupStack::PopAndDestroy( field );
	field = CSdpMediaField::DecodeLC( KVideoStream() );
	
	// VALID FIELD WITH VALID MEDIA PORT, NOT REJECT
	NSP_ASSERT( !NSPUtil::IsReject( *field ) );
	
	CleanupStack::PopAndDestroy( field );
	field = CSdpMediaField::DecodeLC( KReject() );

	// VALID FIELD WITH REJECT PORT
	NSP_ASSERT( NSPUtil::IsReject( *field ) );

	CleanupStack::PopAndDestroy( field );
	}

// ---------------------------------------------------------------------------
// UT_NSPUtil::UT_IsUnspecifiedL
// ---------------------------------------------------------------------------
void UT_NSPUtil::UT_IsUnspecifiedL()
	{
	_LIT8( KIPv4Unspecified, "0.0.0.0" );
	_LIT8( KIPv6Unspecified, "::" );
	_LIT8( KValidIPv4Address, "192.168.0.2" );
	_LIT8( KFqdn, "testdomain.com" );
	TUint port( 2000 );
	
	// OK
	NSP_ASSERT( NSPUtil::IsUnspecifiedL( KIPv4Unspecified(), port ) );
	
	// OK
	NSP_ASSERT( NSPUtil::IsUnspecifiedL( KIPv6Unspecified(), port ) );
	
	// OK, NOT UNSPECIFIED ADDRESS
	NSP_ASSERT( !NSPUtil::IsUnspecifiedL( KValidIPv4Address(), port ) );
	
	// OK, NOT VALID INET ADDRESS
	NSP_ASSERT( !NSPUtil::IsUnspecifiedL( KFqdn(), port ) );
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_NSPUtil,
    "Individual method tests for Util",
    "UNIT" )

NSP_TEST_METHOD(
    "UpdateConnectionFieldL - test",
    "NSPUtil",
    "UT_UpdateConnectionFieldL",
    "FUNCTIONALITY",
    SetupL, UT_UpdateConnectionFieldL, Teardown )

NSP_TEST_METHOD(
    "UpdateOriginFieldL - test",
    "NSPUtil",
    "UT_UpdateOriginFieldL",
    "FUNCTIONALITY",
    SetupL, UT_UpdateOriginFieldL, Teardown )

NSP_TEST_METHOD(
	"RejectL - test",
	"NSPUtil",
	"UT_RejectLL",
	"FUNCTIONALITY",
	SetupL, UT_RejectLL, Teardown )

NSP_TEST_METHOD(
	"IsReject - test",
	"NSPUtil",
	"UT_IsRejectL",
	"FUNCTIONALITY",
	SetupL, UT_IsRejectL, Teardown )

NSP_TEST_METHOD(
	"IsUnspecifiedL - test",
    "NSPUtil",
    "UT_IsUnspecifiedL",
    "FUNCTIONALITY",
    SetupL, UT_IsUnspecifiedL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
