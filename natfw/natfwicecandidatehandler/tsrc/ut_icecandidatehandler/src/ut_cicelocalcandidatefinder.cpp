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
#include "UT_CIceLocalCandidateFinder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>
#include "natfwcandidate.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "cicesessiondata.h"

//  INTERNAL INCLUDES
#include "cicelocalcandidatefinder.h"
#include "cicenatplugincontainer.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceLocalCandidateFinder* UT_CIceLocalCandidateFinder::NewL()
    {
    UT_CIceLocalCandidateFinder* self = UT_CIceLocalCandidateFinder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceLocalCandidateFinder* UT_CIceLocalCandidateFinder::NewLC()
    {
    UT_CIceLocalCandidateFinder* self = new( ELeave ) UT_CIceLocalCandidateFinder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceLocalCandidateFinder::~UT_CIceLocalCandidateFinder()
    {
    delete iPluginContainer;
    }

// Default constructor
UT_CIceLocalCandidateFinder::UT_CIceLocalCandidateFinder()
    {
    }

// Second phase construct
void UT_CIceLocalCandidateFinder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceLocalCandidateFinder::SetupL(  )
    {
    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    iCandStorage->SetTaTimerValue( KTaTimerValueForGathering );
    
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this ); 
    iSessionId = iMultiplexer->CreateSessionL( KTestIap, 5090, 5500 );
    iStreamId = iMultiplexer->CreateStreamL( iSessionId, 0, KProtocolInetUdp );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    
    iFinder = CIceLocalCandidateFinder::NewL( 
        *this, *iCandStorage, *iPluginContainer );
    }

void UT_CIceLocalCandidateFinder::Teardown(  )
    {
    delete iFinder;
    iFinder = NULL;
    delete iCandStorage;
    iCandStorage = NULL;
    delete iPluginContainer;
    iPluginContainer = NULL;
    delete iMultiplexer;
    iMultiplexer = NULL;
    }

void UT_CIceLocalCandidateFinder::UT_CNATFWLocalCandidateFinder_ResolveNextCandidateL(  )
    {
    EUNIT_ASSERT( 0 == iFinder->ResolveNextCandidate( NULL ) );
    EUNIT_ASSERT( 1 == iFinder->ResolveNextCandidate( iFinder ) );
    }

void UT_CIceLocalCandidateFinder::UT_CNATFWLocalCandidateFinder_FetchCandidatesLL(  )
    {
    TBool ipv6After( EFalse );
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iFinder->FetchCandidatesL( 
            KTestStreamCollectionId1, iStreamId, KComponentIdRtp, KAfInet, ipv6After );
        WaitForEvent( MIceNatPluginEventObs::EFetchingCompleted );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iFinder->FetchCandidatesL( 
            KTestStreamCollectionId1, iStreamId, KComponentIdRtp, KAfInet, ipv6After ) );
        WaitForEvent( MIceNatPluginEventObs::EFetchingCompleted );
        }
    }


void UT_CIceLocalCandidateFinder::UT_CNATFWLocalCandidateFinder_PluginEventOccuredL()
    {
    const CNATFWPluginApi* plugin 
        = reinterpret_cast<const CNATFWPluginApi*>( this );
    
    // test handling of unknown event
    EUNIT_ASSERT_PANIC(
        iFinder->PluginEventOccured( plugin, iStreamId,
            MIceNatPluginEventObs::EGeneralError, KErrNone, NULL ),
            "USER", 0 );
    }


void UT_CIceLocalCandidateFinder::WaitForEvent( 
        MIceNatPluginEventObs::TNatPluginEvent aEvent )
    {
    iEventToWait = aEvent;
    CActiveScheduler::Start();
    }
    
// From observer interface

// from MNATFWConnectionMultiplexerObserver
void UT_CIceLocalCandidateFinder::Notify( 
        TUint /*aSessionId*/, TUint /*aStreamId*/,
        TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }

void UT_CIceLocalCandidateFinder::IcmpError( 
        TUint /*aSessionId*/, TUint /*aStreamId*/, 
        const TInetAddr& /*aAddress*/ )
    {
    
    }

HBufC8* UT_CIceLocalCandidateFinder::IncomingMessageL( 
    TUint /*aStreamId*/, const TDesC8& /*aMessage*/, 
    const TInetAddr& /*aLocalAddr*/, const TInetAddr& /*aFromAddr*/,
    TInetAddr& /*aPeerRemoteAddr*/, TBool& /*aConsumed*/ )
    {
    return NULL;
    }

void UT_CIceLocalCandidateFinder::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint /*aStreamConnectionId*/,
        TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ELocalCandidateFound:
            {
            CNATFWCandidate* candidate =
                reinterpret_cast<CNATFWCandidate*>( aEventData );
            EUNIT_ASSERT( 
                candidate->StreamCollectionId() == KTestStreamCollectionId1
                && candidate->StreamId() == iStreamId
                && candidate->ComponentId() == KComponentIdRtp );
            
            delete candidate;
            }
            break;
        case MIceNatPluginEventObs::EFetchingCompleted:
        case MIceNatPluginEventObs::ESendingActivated:
        case MIceNatPluginEventObs::ESendingDeactivated:
        case MIceNatPluginEventObs::ECandidatePairFound:
        case MIceNatPluginEventObs::EConnChecksCompleted:
            {
            if ( iEventToWait == aEventCode )
                {
                iEventErrCode = aErrorCode;
                iEventStreamId = aStreamId;
                
                CActiveScheduler::Stop();
                }
            }
            break;
        
        case MIceNatPluginEventObs::EGeneralError:
            iEventStreamId = aStreamId;
            iEventErrCode = aErrorCode;            
            break;
        
        default:
            EUNIT_ASSERT( EFalse );
        }
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceLocalCandidateFinder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TimerFiredL - test ",
    "CIceLocalCandidateFinder",
    "TimerFiredL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWLocalCandidateFinder_ResolveNextCandidateL, Teardown)

EUNIT_TEST(
    "FetchCandidatesL - test ",
    "CIceLocalCandidateFinder",
    "FetchCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWLocalCandidateFinder_FetchCandidatesLL, Teardown)

EUNIT_TEST(
    "PluginEventOccured - test ",
    "CIceLocalCandidateFinder",
    "PluginEventOccured",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWLocalCandidateFinder_PluginEventOccuredL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
