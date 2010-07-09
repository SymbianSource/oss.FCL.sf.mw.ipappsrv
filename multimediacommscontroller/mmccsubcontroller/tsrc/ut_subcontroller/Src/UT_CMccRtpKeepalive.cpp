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
#include "UT_CMccRtpKeepalive.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>   // for using EUNIT_ALLOC_TEST


//  INTERNAL INCLUDES
#include "mccrtpkeepalivecontainer.h"
#include "mccrtpkeepalive.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mccuids.hrh"
#include "mcctesteventhandler.h"
#include "mccrtpmediaclock.h"

#include "mccunittestmacros.h"

// CONSTANS
const TUid KRtpDataSink =  { KImplUidRtpDataSink };
const TUid KRtpDataSource  =  { KImplUidRtpDataSource }; 
    
// CONSTRUCTION
UT_CMccRtpKeepalive* UT_CMccRtpKeepalive::NewL()
    {
    UT_CMccRtpKeepalive* self = UT_CMccRtpKeepalive::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpKeepalive* UT_CMccRtpKeepalive::NewLC()
    {
    UT_CMccRtpKeepalive* self = new( ELeave ) UT_CMccRtpKeepalive();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtpKeepalive::~UT_CMccRtpKeepalive()
    {
    }

// Default constructor
UT_CMccRtpKeepalive::UT_CMccRtpKeepalive()
    {
    }

// Second phase construct
void UT_CMccRtpKeepalive::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccRtpKeepalive::SetupL()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iEventHandler = CMccTestEventHandler::NewL();
    iRtpApi = CRtpAPI::NewL( *iEventHandler );
    TPckgBuf<TInt> params( 30000 );
    iRtpSource = MDataSource::NewSourceL( KRtpDataSource, params );
	iRtpSink = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
	
	TMccCodecInfo codecInfo;
	TMccCodecInfoBuffer infoBuffer( codecInfo );
	CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iRtpSource );
	dSource->ConfigureL( infoBuffer, iRtpMediaClock );
	CMccRtpDataSink* dSink = static_cast<CMccRtpDataSink*>( iRtpSink );
	dSink->ConfigureL( infoBuffer, iRtpMediaClock );
    } 

void UT_CMccRtpKeepalive::Setup2L()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iEventHandler = CMccTestEventHandler::NewL();
    iRtpApi = CRtpAPI::NewL( *iEventHandler );
    TPckgBuf<TInt> params( 30000 );
    iRtpSource = MDataSource::NewSourceL( KRtpDataSource, params );
	iRtpSink = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
	
	TMccCodecInfo codecInfo;
	TMccCodecInfoBuffer infoBuffer( codecInfo );
	CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iRtpSource );
	dSource->ConfigureL( infoBuffer, iRtpMediaClock );
	CMccRtpDataSink* dSink = static_cast<CMccRtpDataSink*>( iRtpSink );
	dSink->ConfigureL( infoBuffer, iRtpMediaClock );
	
	iKeepaliveHandler = 
	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
	    
    EUNIT_ASSERT_EQUALS( iKeepaliveHandler->PayloadType(), 96 );
    } 


void UT_CMccRtpKeepalive::SetupAL(  )
    {
    SetupL();
	    
    iAlloc = ETrue;
    }
      
void UT_CMccRtpKeepalive::SetupA2L(  )
    {
    Setup2L();
    iAlloc = ETrue;
    }
        
void UT_CMccRtpKeepalive::Teardown()
    {
    delete iKeepaliveHandler;
    delete iRtpSource;
    delete iRtpSink;
    delete iRtpApi;
    delete iEventHandler;
    delete iRtpMediaClock;
    REComSession::FinalClose();
    }

// INHERITED FUNCTIONS

// TEST CASE FUNCTIONS

void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_ContainerTestsL()
    {
    if ( !iAlloc )
        {
        CMccRtpKeepaliveContainer* container = 
            CMccRtpKeepaliveContainer::NewL( *iEventHandler, *iRtpApi, TRtpId() );
        CleanupStack::PushL( container );
        
        // Start keepalive
        //
        
        // Interval not defined
        TMccCodecInfo codecInfo;
        codecInfo.iKeepalivePT = 96;
        container->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        // Interval defined
        codecInfo.iKeepaliveInterval = 1000;
        container->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        // Keepalive handler already exists for the payload type
        container->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo, *iRtpMediaClock );
        
        // Remote address set
        EUNIT_ASSERT_EQUALS( container->RemoteAddressSet(), KErrNone );
        
        // Update valid case
        MCC_EUNIT_ASSERT_NO_LEAVE( container->UpdateParamsL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock ) );
        
        // Update with keealive interval 0
        codecInfo.iKeepaliveInterval = 0;
        MCC_EUNIT_ASSERT_NO_LEAVE( container->UpdateParamsL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock ) );
        
        // Stop keepalive
        //
        
        // First user
        container->StopKeepalive( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo );
        
        // Second user
        container->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo );
        
        // Handler not found
        EUNIT_ASSERT_EQUALS( container->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo ),
                             KErrNotFound );
        
        // Stop 
        container->StopAll();
        
        // Remote address set while stopped
        EUNIT_ASSERT_EQUALS( container->RemoteAddressSet(), KErrNotReady );
        
        CleanupStack::PopAndDestroy( container );       
        }
    else
        {
        CMccRtpKeepaliveContainer* container = 
            CMccRtpKeepaliveContainer::NewL( *iEventHandler, *iRtpApi, TRtpId() );
        CleanupStack::PushL( container );
        
        // Interval not defined
        TMccCodecInfo codecInfo;
        codecInfo.iKeepalivePT = 96;
        container->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        // Interval defined
        codecInfo.iKeepaliveInterval = 1000;
        container->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        // Keepalive handler already exists for the payload type
        container->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo, *iRtpMediaClock );
        
        // Remote address set
        EUNIT_ASSERT_EQUALS( container->RemoteAddressSet(), KErrNone );
        
        // Update valid case
        container->UpdateParamsL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        // Update with keealive interval 0
        codecInfo.iKeepaliveInterval = 0;
        container->UpdateParamsL( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo, *iRtpMediaClock );
        
        
        // Stop keepalive
        //
        
        // First user
        container->StopKeepalive( *static_cast<CMccRtpDataSink*>( iRtpSink ), codecInfo );
        
        // Second user
        container->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo );
        
        // Handler not found
        EUNIT_ASSERT_EQUALS( container->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), codecInfo ),
                             KErrNotFound );
        
        // Stop 
        container->StopAll();
        
        // Remote address set while stopped
        EUNIT_ASSERT_EQUALS( container->RemoteAddressSet(), KErrNotReady );
        CleanupStack::PopAndDestroy( container );
        }

    }

void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_StartKeepaliveLL()
    {
    if ( iAlloc )
        {
        CMccRtpKeepalive* keepaliveHandler = 
	        CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
	     
	    CleanupStack::PushL( keepaliveHandler );	     

        // Keepalive is sent when only RTP source exists
        keepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        EUNIT_ASSERT( keepaliveHandler->iCurrentState == CMccRtpKeepalive::EWaitingTimer );
        
        // Try when already sending
        keepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        
        // Keepalive preaudio packet is sent
        keepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), *iRtpMediaClock );
        
        CleanupStack::PopAndDestroy( keepaliveHandler );
        
        // Test starting while remote address is not set
        keepaliveHandler = 
    	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, EFalse );
    	    
    	CleanupStack::PushL( keepaliveHandler );
    	
    	iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );

        CleanupStack::PopAndDestroy( keepaliveHandler );
        }
    else
        {
        // Incorrect payload type
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler = 
    	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 200, 1000, KNullDesC8, ETrue ), KErrArgument );
        
        iKeepaliveHandler = 
    	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
        
        delete iKeepaliveHandler;
        iKeepaliveHandler = NULL;
        
        iKeepaliveHandler = 
    	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
        
        // Keepalive is sent when only RTP source exists
        iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::EWaitingTimer );
        
        // Try when already sending
        iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        
        // Keepalive preaudio packet is sent
        iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSink*>( iRtpSink ), *iRtpMediaClock );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::EWaitingTimer );
        
        delete iKeepaliveHandler;
        iKeepaliveHandler = NULL;
        
        // Test starting while remote address is not set
        iKeepaliveHandler = 
    	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, EFalse );
    	iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESendingPending );

        delete iKeepaliveHandler;
        iKeepaliveHandler = NULL;        
        }
    }
    
void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_UpdateParamsLL()
    {
    if ( iAlloc )
        { 
        CMccRtpKeepalive* keepaliveHandler = 
	        CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
	        
	    CleanupStack::PushL( keepaliveHandler );    
    
        TUint8 keepalivePT(96); 
        TUint32 keepaliveInterval(25000000);    
        const TUint8 KAmrKeepAlivePayload[6] = 
        { 
        0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        TBuf8<6> keepAliveData;
        keepAliveData.Copy( KAmrKeepAlivePayload );
        
        // Valid case
        EUNIT_ASSERT_LEAVE( keepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ) );

        // Wrong payload type
        keepalivePT = 129;
        EUNIT_ASSERT_LEAVE( keepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ) );    
        CleanupStack::PopAndDestroy( keepaliveHandler );
        }
    else
        {
        iKeepaliveHandler = 
     	    CMccRtpKeepalive::NewL( *iEventHandler, *iRtpApi, TRtpId(), 96, 1000, KNullDesC8, ETrue );
        
        TUint8 keepalivePT(96); 
        TUint32 keepaliveInterval(25000000);    
        const TUint8 KAmrKeepAlivePayload[6] = 
        { 
        0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        TBuf8<6> keepAliveData;
        keepAliveData.Copy( KAmrKeepAlivePayload );
        
        // Valid case
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ) );

        // Wrong payload type
        keepalivePT = 129;
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ), KErrArgument );            
        }
    }

void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_StopKeepaliveL()
    {
    TInt refCount( 0 );
    
    if ( iAlloc )
        {
         iKeepaliveHandler->StartKeepaliveL(
            *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );

        
        iKeepaliveHandler->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), refCount );
        EUNIT_ASSERT_EQUALS( refCount, 0 );       
        }
    else
        {
        EUNIT_ASSERT_EQUALS( 
            iKeepaliveHandler->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), refCount ), 
            KErrNotFound );
        
        iKeepaliveHandler->StartKeepaliveL( *static_cast<CMccRtpDataSource*>( iRtpSource ), *iRtpMediaClock );
        EUNIT_ASSERT_EQUALS( iKeepaliveHandler->iUsers.Count(), 1 );
        
        iKeepaliveHandler->StopKeepalive( *static_cast<CMccRtpDataSource*>( iRtpSource ), refCount );
        EUNIT_ASSERT_EQUALS( refCount, 0 );
        }
    }
    
void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_RemoteAddressSetL()
    {
    if ( iAlloc )
        {
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RemoteAddressSet() );
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESendingPending;
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RemoteAddressSet() );
        }
    else
        {
        iKeepaliveHandler->RemoteAddressSet();
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESendingPending;
        iKeepaliveHandler->RemoteAddressSet();       
        }
    }

void UT_CMccRtpKeepalive::UT_CMccRtpKeepalive_RunLL()
    {
    if ( iAlloc )
        {
         // Timer completed
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RunL() );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESending );
        EUNIT_ASSERT( iKeepaliveHandler->IsActive() );
        
        // Sending when already active fails
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        EUNIT_ASSERT_LEAVE( iKeepaliveHandler->RunL() );
        
        // Send completed
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RunL() );
        // Nothing happens as was still active (not possible in real life)
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESending );
        
        // Send completion ok when not active anymore, timer is started
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RunL() );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESendingPending );
        EUNIT_ASSERT( !iKeepaliveHandler->IsActive() );
        
        // Completed in not supported state
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EIdle;
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler->RunL(), KErrArgument );
        
        // RunL error
        // NB: seems that this test case goes so that the keepalive handler is
        // waiting for the timer thus the only state to work in the cancel case
        // is the 'waiting timer'-state.
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrGeneral;
        
        EUNIT_ASSERT_LEAVE( iKeepaliveHandler->RunL() );
        
        //
        TUint8 keepalivePT(96); 
        TUint8 keepaliveInterval(25);    
        const TUint8 KAmrKeepAlivePayload[6] = 
        { 
        0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        TBuf8<6> keepAliveData;
        keepAliveData.Copy( KAmrKeepAlivePayload );
        EUNIT_ASSERT_LEAVE( iKeepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ) );
        
        // Send completion ok when not active anymore, timer is started
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;
        iKeepaliveHandler->iRtpMediaClock = iRtpMediaClock;
        
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->RunL() );
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::EWaitingTimer );
        EUNIT_ASSERT( iKeepaliveHandler->IsActive() );
     
        // Test RunError
        EUNIT_ASSERT_EQUALS( iKeepaliveHandler->RunError( KErrGeneral ), KErrNone );
        EUNIT_ASSERT_EQUALS( iKeepaliveHandler->RunError( KErrNoMemory ), KErrNoMemory );
        }
    else
        {
        // Timer completed
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        iKeepaliveHandler->RunL();
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESending );
        EUNIT_ASSERT( iKeepaliveHandler->IsActive() );
        
        // Sending when already active fails
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler->RunL(), KErrInUse );
        
        // Send completed
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;
        iKeepaliveHandler->RunL();
        // Nothing happens as was still active (not possible in real life)
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESending );
        
        // Send completion ok when not active anymore, timer is started
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;        
        iKeepaliveHandler->RunL();
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::ESendingPending );
        EUNIT_ASSERT( !iKeepaliveHandler->IsActive() );
        
        // Completed in not supported state
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EIdle;
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler->RunL(), KErrArgument );
        
        // RunL error
        // NB: seems that this test case goes so that the keepalive handler is
        // waiting for the timer thus the only state to work in the cancel case
        // is the 'waiting timer'-state.
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::EWaitingTimer;
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrGeneral;
        
        MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iKeepaliveHandler->RunL(), KErrGeneral );
        
        //
        TUint8 keepalivePT(96); 
        TUint8 keepaliveInterval(25);    
        const TUint8 KAmrKeepAlivePayload[6] = 
        { 
        0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        TBuf8<6> keepAliveData;
        keepAliveData.Copy( KAmrKeepAlivePayload );
        MCC_EUNIT_ASSERT_NO_LEAVE( iKeepaliveHandler->UpdateParamsL( keepalivePT, keepaliveInterval, keepAliveData, iRtpMediaClock ) );
        
        // Send completion ok when not active anymore, timer is started
        iKeepaliveHandler->Cancel();
        iKeepaliveHandler->iStatus = KErrNone;
        iKeepaliveHandler->iCurrentState = CMccRtpKeepalive::ESending;
        iKeepaliveHandler->RunL();
        EUNIT_ASSERT( iKeepaliveHandler->iCurrentState == CMccRtpKeepalive::EWaitingTimer );
        EUNIT_ASSERT( iKeepaliveHandler->IsActive() );
     
        // Test RunError
        EUNIT_ASSERT_EQUALS( iKeepaliveHandler->RunError( KErrGeneral ), KErrNone );
        EUNIT_ASSERT_EQUALS( iKeepaliveHandler->RunError( KErrNoMemory ), KErrNoMemory );
        }
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtpKeepalive,
    "CMccKeepalive test",
    "UNIT" )

EUNIT_TEST(
    "ContainerTests - test ",
    "CMccRtpKeepalive",
    "ContainerTests",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpKeepalive_ContainerTestsL, Teardown )
    
EUNIT_TEST(
    "StartKeepaliveL - test ",
    "CMccRtpKeepalive",
    "StartKeepaliveL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpKeepalive_StartKeepaliveLL, Teardown )
   
EUNIT_TEST(
    "UpdateParamsL - test ",
    "CMccRtpKeepalive",
    "UpdateParamsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpKeepalive_UpdateParamsLL, Teardown )    
    
EUNIT_TEST(
    "StopKeepalive - test ",
    "CMccRtpKeepalive",
    "StopKeepalive",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRtpKeepalive_StopKeepaliveL, Teardown )

EUNIT_TEST(
    "RemoteAddressSet - test ",
    "CMccRtpKeepalive",
    "RemoteAddressSet",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRtpKeepalive_RemoteAddressSetL, Teardown )

EUNIT_TEST(
    "RunL - test ",
    "CMccRtpKeepalive",
    "RunL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRtpKeepalive_RunLL, Teardown )

// ALLOC TESTS

#ifdef ALLOC_TEST_ON
          
EUNIT_ALLOC_TEST(
    "ContainerTests - alloc test",
    "CMccRtpKeepalive",
    "ContainerTests",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_ContainerTestsL, Teardown) 

EUNIT_ALLOC_TEST(
    "StartKeepaliveL - alloc test",
    "CMccRtpKeepalive",
    "StartKeepaliveL",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_StartKeepaliveLL, Teardown)

EUNIT_ALLOC_TEST(
    "UpdateParamsL - alloc test",
    "CMccRtpKeepalive",
    "UpdateParamsL",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_UpdateParamsLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "StopKeepalive - alloc test",
    "CMccRtpKeepalive",
    "StopKeepalive",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_StopKeepaliveL, Teardown)

EUNIT_ALLOC_TEST(
    "RemoteAddressSet - alloc test",
    "CMccRtpKeepalive",
    "RemoteAddressSet",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_RemoteAddressSetL, Teardown)
    
EUNIT_ALLOC_TEST(
    "RunL - alloc test",
    "CMccRtpKeepalive",
    "RunL",
    "ERRORHANDLING",
    SetupA2L, UT_CMccRtpKeepalive_RunLL, Teardown)    
           
#endif // ALLOC_TEST
      
EUNIT_END_TEST_TABLE

//  END OF FILE
