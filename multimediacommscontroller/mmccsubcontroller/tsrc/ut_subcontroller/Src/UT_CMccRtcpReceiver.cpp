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




//  CLASS HEADER
#include "UT_CMccRtcpReceiver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccrtcpreceiver.h"
#include "MmccEvents.h"
#include "mccinternalevents.h"
#include "MccInternalDef.h"
#include "mccresourcepool_stub.h"
#include "mcctesteventhandler.h"
#include "mccrtpmanager.h"

// CONSTANS
const TRtpSSRC KSsrcSdes( 42 );
const TRtpSSRC KSsrcApp( 43 );
const TRtpSSRC KSsrcBye( 44 );
const TRtpSSRC KSsrcSr( 45 );
const TRtpSSRC KSsrcRr( 46 );
const TInt KStreamId( 42 );
const TInt KWrongStreamId( 43 );

#define MCCEVENTDATA_TO_RTCPEVENTDATA( event )\
    (*reinterpret_cast<const TMccRtcpEventDataPackage*>( &event.iEventData ))()

#define MMFEVENT_TO_MCCEVENT( event )\
    *reinterpret_cast<const TMccEvent*>( reinterpret_cast<const TMccInternalEvent&>( event ).iMccEvent );
    
// CONSTRUCTION
UT_CMccRtcpReceiver* UT_CMccRtcpReceiver::NewL()
    {
    UT_CMccRtcpReceiver* self = UT_CMccRtcpReceiver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtcpReceiver* UT_CMccRtcpReceiver::NewLC()
    {
    UT_CMccRtcpReceiver* self = new( ELeave ) UT_CMccRtcpReceiver();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtcpReceiver::~UT_CMccRtcpReceiver()
    {
    }

// Default constructor
UT_CMccRtcpReceiver::UT_CMccRtcpReceiver()
    : iRtpApi( NULL ),
      iRtcpReceiver( NULL ),
      iExpectedPackets( 0 ),
      iStream( KStreamId )
    {
    }

// Second phase construct
void UT_CMccRtcpReceiver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccRtcpReceiver::SetupL()
    {
    iRtpApi = CRtpAPI::NewL( *this );
    iResources = CMccResourcePoolStub::NewL();
    iHandler = CMccTestEventHandler::NewL();
    iMccSessionId = 1; 
    iRtpManager = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iRtcpReceiver = CMccRtcpReceiver::NewL( iRtpManager, *this, *iResources, *iRtpApi );
    } 

void UT_CMccRtcpReceiver::Teardown()
    {
    EUNIT_ASSERT_EQUALS( iExpectedPackets, 0 );
    delete iRtcpReceiver;
    iRtcpReceiver = NULL;
    delete iRtpApi;
    iRtpApi = NULL;
    delete iResources;
    iResources = NULL;
    delete iHandler;
    iHandler = NULL;
    delete iRtpManager;
    iRtpManager = NULL;
    }

// INHERITED FUNCTIONS

TInt UT_CMccRtcpReceiver::SendEventToClient( const TMMFEvent& aEvent )
    {
    RDebug::Print( _L( "UT_CMccRtcpReceiver::SendMccEventToClient" ) );
    
    const TMccEvent& event = MMFEVENT_TO_MCCEVENT( aEvent );
    
    iExpectedPackets--;

    RDebug::Print( _L( "UT_CMccRtcpReceiver::SendMediaSignallingToClient" ) );
    
    const TMccRtcpEventData& eventData = MCCEVENTDATA_TO_RTCPEVENTDATA( event );
    switch ( eventData.iRtcpPacketType )
        {
        case KRtcpSdesPacket:
            break;
        case KRtcpByePacket:
            EUNIT_ASSERT_EQUALS( iTmpSsrc, KSsrcBye );
            break;
        case KRtcpAppPacket:
            EUNIT_ASSERT_EQUALS( iTmpSsrc, KSsrcApp );
            break;
        case KRtcpSrPacket:
            EUNIT_ASSERT_EQUALS( iTmpSsrc, KSsrcSr );
            break;
        case KRtcpRrPacket:
            EUNIT_ASSERT_EQUALS( iTmpSsrc, KSsrcRr );
            break;
        case KRtcpPacketUndefined:
            // Perhaps Non-RTCP data
            break;
        default:
            EUNIT_ASSERT_DESC( EFalse, "Wrong RTCP packet type" );
            break;
        }
        
    return KErrNone;
    }

void UT_CMccRtcpReceiver::ErrorNotify( TInt /*aErrCode*/ )
    {
    RDebug::Print( _L( "UT_CMccRtcpReceiver::ErrorNotify" ) );
    }

// TEST CASE FUNCTIONS

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_SdesReceivedL()
    {
    iExpectedPackets++;
    iTmpSsrc = KSsrcSdes;
    TRtpSdesParams params;
    _LIT8(KCName, "foofoo");
    params.iCName.Set( KCName() );
    _LIT8(KLocation, "foo2foo2");
    params.iLocation.Set( KLocation() );
    iRtcpReceiver->SdesReceived( iTmpSsrc, params );
    }

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_ByeReceivedL()
    {
    iExpectedPackets++;
    iTmpSsrc = KSsrcBye;
    TBuf8<5> reason;
    reason.Copy( _L8( "foo" ) );
    iRtcpReceiver->ByeReceived( iStream, iTmpSsrc, reason );

    // Wrong stream ID should NOT cause a packet received event
   // iRtcpReceiver->ByeReceived( KWrongStreamId, iTmpSsrc, reason );
    }

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_AppReceivedL()
    {
    iExpectedPackets++;
    iTmpSsrc = KSsrcApp;
    TRtcpApp app;
    iRtcpReceiver->AppReceived( iStream, iTmpSsrc, app );
    }

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_SrReceivedL()
    {
    iExpectedPackets++;
    iTmpSsrc = KSsrcSr;
    TTimeStamps timeStamps;
    iRtcpReceiver->SrReceived( iStream, iTmpSsrc, timeStamps );
    }

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_RrReceivedL()
    {
    iExpectedPackets++;
    iTmpSsrc = KSsrcRr;
    iRtcpReceiver->RrReceived( iStream, iTmpSsrc );
    }

void UT_CMccRtcpReceiver::UT_CMccRtcpReceiver_NonRTPDataReceivedL()
    {
    TUint dummyPort( 5000 );
    TBool rtpPort( ETrue );
    TBuf8<5> data;
    data.Append( _L8( "foo42" ) );
    
    // This should not do anything
    iRtcpReceiver->NonRTPDataReceived( dummyPort, rtpPort, data );

    rtpPort = EFalse;

    // This should do something
    iExpectedPackets++;
    iRtcpReceiver->NonRTPDataReceived( dummyPort, rtpPort, data );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtcpReceiver,
    "MccRtcpReceiver test",
    "UNIT" )

EUNIT_TEST(
    "SdesReceived - test ",
    "CMccRtcpReceiver",
    "SdesReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_SdesReceivedL, Teardown )

EUNIT_TEST(
    "ByeReceived - test ",
    "CMccRtcpReceiver",
    "ByeReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_ByeReceivedL, Teardown )

EUNIT_TEST(
    "AppReceived - test ",
    "CMccRtcpReceiver",
    "AppReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_AppReceivedL, Teardown )

EUNIT_TEST(
    "SrReceived - test ",
    "CMccRtcpReceiver",
    "SrReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_SrReceivedL, Teardown )

EUNIT_TEST(
    "RrReceived - test ",
    "CMccRtcpReceiver",
    "RrReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_RrReceivedL, Teardown )

EUNIT_TEST(
    "NonRTPDataReceived - test ",
    "CMccRtcpReceiver",
    "NonRTPDataReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtcpReceiver_NonRTPDataReceivedL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
