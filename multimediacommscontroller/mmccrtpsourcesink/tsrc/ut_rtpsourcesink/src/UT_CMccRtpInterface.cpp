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
#include "UT_CMccRtpInterface.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "rtpapi.h"
#include "mccrtpinterface.h"
#include "mccrtpdatasink.h"
#include "mccrtpkeepalivecontainer.h"
#include "mccrtpmediaclock.h"

// CONSTRUCTION
UT_CMccRtpInterface* UT_CMccRtpInterface::NewL()
    {
    UT_CMccRtpInterface* self = UT_CMccRtpInterface::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpInterface* UT_CMccRtpInterface::NewLC()
    {
    UT_CMccRtpInterface* self = new( ELeave ) UT_CMccRtpInterface();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtpInterface::~UT_CMccRtpInterface()
    {
    delete iRtpKeepaliveMechanism;
    delete iInterface;
    delete iRtpApi;
    }

// Default constructor
UT_CMccRtpInterface::UT_CMccRtpInterface()
    {
    }

// Second phase construct
void UT_CMccRtpInterface::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// From MRtpErrNotify
void UT_CMccRtpInterface::ErrorNotify( TInt /*aErrCode*/ )
    {
    }


void UT_CMccRtpInterface::SetupL(  )
    {
    iRtpApi = CRtpAPI::NewL( *this );
    
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    CMccRtpDataSink* sink = static_cast<CMccRtpDataSink*>(
        CMccRtpDataSink::NewSinkL( dummyUid, dummyBuf ) );
    
    iInterface = static_cast<MMccRtpInterface*>( sink );
    
    MAsyncEventHandler* eventHandler = NULL;
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( *eventHandler, 
                                         *iRtpApi, 
                                         iRtpSessionId ); 
    
    iRtpMediaClock = CMccRtpMediaClock::NewL();                                      
    } 

void UT_CMccRtpInterface::Teardown(  )
    {
    delete iRtpKeepaliveMechanism;
    iRtpKeepaliveMechanism = NULL;
    
    delete static_cast<CMccRtpDataSink*>( iInterface );
    iInterface = NULL;
    
    delete iRtpApi;
    iRtpApi = NULL;
    
    if ( iRtpMediaClock )
        {
        delete iRtpMediaClock;
        iRtpMediaClock = NULL;
        }
    }

void UT_CMccRtpInterface::UT_CMccRtpInterface_ConfigureLL()
    {
    TMccCodecInfo cInfo;
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iInterface->ConfigureL( TMccCodecInfoBuffer( cInfo ), iRtpMediaClock ), KErrNotReady );
    
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iRtpSessionId;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    iInterface->SetSessionParamsL( params );
    TBuf8<1> kaData;
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iInterface->ConfigureL( kaData, iRtpMediaClock ), KErrArgument );
    
    cInfo.iKeepalivePT = 13;
    cInfo.iKeepaliveInterval = 0;
    cInfo.iKeepaliveData = kaData;
    iInterface->ConfigureL( TMccCodecInfoBuffer( cInfo ), iRtpMediaClock );
    EUNIT_ASSERT( TMccCodecInfo::Compare( cInfo, iInterface->iCodecInfo ) );
    
    TMccCodecInfo backup = iInterface->iCodecInfo;
    cInfo.iKeepaliveInterval = 28;
    iInterface->ConfigureL( TMccCodecInfoBuffer( cInfo ), iRtpMediaClock );
    EUNIT_ASSERT( 
        cInfo.iKeepaliveInterval == iInterface->iCodecInfo.iKeepaliveInterval );
    
    backup = iInterface->iCodecInfo;
    cInfo.iKeepaliveInterval = 0;
    iInterface->ConfigureL( TMccCodecInfoBuffer( cInfo ), iRtpMediaClock );
    EUNIT_ASSERT( TMccCodecInfo::Compare( backup, iInterface->iCodecInfo ) );
    }

void UT_CMccRtpInterface::UT_CMccRtpInterface_SendRTCPReceiverReportL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMccRtpInterface::UT_CMccRtpInterface_SendRTCPSenderReportL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMccRtpInterface::UT_CMccRtpInterface_StartInactivityTimerLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMccRtpInterface::UT_CMccRtpInterface_StopInactivityTimerLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMccRtpInterface::CreateRtcpEvent( TMccEvent& aEvent )
    {
    TMccRtcpEventData rtcpData;
    rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
    rtcpData.iRtcpPacketType = KRtcpAnyPacket;
    rtcpData.iSsrc = 2;
    
    TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
    
    //Create a media signal and send it 
    TMccEvent event( 1, 1, 1, 1, KMccEventCategoryRtcp, KMccRtcpReceived,
                         KErrNone, rtcpDataPackage );
    aEvent = event;
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtpInterface,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConfigureL - test ",
    "CMccRtpInterface",
    "ConfigureL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpInterface_ConfigureLL, Teardown)

#if 0
EUNIT_TEST(
    "SendRTCPReceiverReport - test ",
    "CMccRtpInterface",
    "SendRTCPReceiverReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpInterface_SendRTCPReceiverReportL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReport - test ",
    "CMccRtpInterface",
    "SendRTCPSenderReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpInterface_SendRTCPSenderReportL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccRtpInterface",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpInterface_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccRtpInterface",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpInterface_StopInactivityTimerLL, Teardown)

#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
