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
#include "UT_CIceNatPluginContainer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>   // for using EUNIT_ALLOC_TEST

//  INTERNAL INCLUDES
#include "cicenatplugincontainer.h"
#include "cicesessiondata.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlertestdefs.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceNatPluginContainer* UT_CIceNatPluginContainer::NewL()
    {
    UT_CIceNatPluginContainer* self = UT_CIceNatPluginContainer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceNatPluginContainer* UT_CIceNatPluginContainer::NewLC()
    {
    UT_CIceNatPluginContainer* self = new( ELeave ) UT_CIceNatPluginContainer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceNatPluginContainer::~UT_CIceNatPluginContainer()
    {
    delete iSessionData;
    delete iMultiplexer;
    delete iPluginContainer;
    }

// Default constructor
UT_CIceNatPluginContainer::UT_CIceNatPluginContainer()
    {
    }

// Second phase construct
void UT_CIceNatPluginContainer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CIceNatPluginContainer::SetupL(  )
    {
    iEventToWait = MIceNatPluginEventObs::TNatPluginEvent( -1 );
    iSessionData = CIceSessionData::NewL( KTestDomain, KTestIap );
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    iPluginContainer 
        = CIceNatPluginContainer::NewL( *iSessionData, *iMultiplexer );
    }

void UT_CIceNatPluginContainer::Teardown(  )
    {
    ClearTestData();
    
    delete iSessionData;
    iSessionData = NULL;
    
    delete iMultiplexer;
    iMultiplexer = NULL;
    
    delete iPluginContainer;
    iPluginContainer = NULL;
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_ConnectPluginsLL()
    {
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EServerConnected );
    
    RSocketServ server;
    TName name;
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT_NO_SPECIFIC_LEAVE(
            iPluginContainer->ConnectPluginsL( server, name ), KErrNoMemory );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iPluginContainer->ConnectPluginsL( server, name ) );
        }
    
    for ( TInt i(0); i < iPluginContainer->Count(); ++i )
        {
        WaitForEvent( MIceNatPluginEventObs::EServerConnected );
        }
    
    iPluginContainer->UnregObserver( *this );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_CountL(  )
    {
    EUNIT_ASSERT( 0 != iPluginContainer->Count() );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_RegObserverForEventLL(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPluginContainer->RegObserverForEventL( *this, 
            MIceNatPluginEventObs::EFetchingCompleted );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iPluginContainer->RegObserverForEventL( *this, 
            MIceNatPluginEventObs::EFetchingCompleted ) );
        
        EUNIT_ASSERT_LEAVE( 
            iPluginContainer->RegObserverForEventL( *this, 
            MIceNatPluginEventObs::EFetchingCompleted ) );
        }
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_UnRegObserverForEventL(  )
    {
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ELocalCandidateFound );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EFetchingCompleted );
    
    // unregister EFetchingCompleted -event
    iPluginContainer->UnregObserverForEvent( *this, 
        MIceNatPluginEventObs::EFetchingCompleted );
    EUNIT_ASSERT( 1 == iPluginContainer->iObserversForEvent[ 
        MIceNatPluginEventObs::ELocalCandidateFound ]->Count() );
    
    // try to unregister already unregistered observer
    /*EUNIT_ASSERT_PANIC(
        iPluginContainer->UnregObserverForEvent( *this, 
            MIceNatPluginEventObs::EFetchingCompleted ), 
        "USER", 0 );*/
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_UnregObserverL()
    {
    RArray<MIceNatPluginEventObs::TNatPluginEvent> events;
    CleanupClosePushL( events );
    
    events.AppendL( MIceNatPluginEventObs::EFetchingCompleted );
    events.AppendL( MIceNatPluginEventObs::ELocalCandidateFound );
    
    iPluginContainer->UnregObserver( *this );
    
    CleanupStack::PopAndDestroy( &events );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_ReleasePluginsL(  )
    {
    EUNIT_ASSERT( 3 >= iPluginContainer->iPlugins.Count() );
    
    iPluginContainer->ReleasePlugins( EFalse );
    // host plugin should be preserved
    EUNIT_ASSERT( 1 == iPluginContainer->iPlugins.Count() );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPluginContainer->ResolvePluginsL();
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iPluginContainer->ResolvePluginsL() );
        }
    
    iPluginContainer->ReleasePlugins( ETrue );
    // host & turn plugins should be preserved
    EUNIT_ASSERT( 2 <= iPluginContainer->iPlugins.Count() );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_PluginByIdentifierL(  )
    {
    _LIT8( KInvalidIdentifier, "" );
    CNATFWPluginApi* nullPlugin =
        iPluginContainer->PluginByIdentifier( KInvalidIdentifier() );
    EUNIT_ASSERT( !nullPlugin );
    
    CNATFWPluginApi* stunPlugin =
        iPluginContainer->PluginByIdentifier( KNatPluginIdNokiaStun() );
    EUNIT_ASSERT( stunPlugin );
    
    CNATFWPluginApi* turnPlugin =
        iPluginContainer->PluginByIdentifier( KNatPluginIdNokiaTurn() );
    EUNIT_ASSERT( turnPlugin );
    
    CNATFWPluginApi* icePlugin =
        iPluginContainer->PluginByIdentifier( KNatPluginIdNokiaIce() );
    EUNIT_ASSERT( !icePlugin );
    
    CNATFWPluginApi* hostPlugin =
        iPluginContainer->PluginByIdentifier( KNatPluginIdNokiaHost() );
    EUNIT_ASSERT( hostPlugin );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_PluginByTypeL(  )
    {
    _LIT8( KInvalidIdentifier, "" );
    CNATFWPluginApi* nullPlugin =
        iPluginContainer->PluginByType( KInvalidIdentifier() );
    EUNIT_ASSERT( !nullPlugin );
    
    CNATFWPluginApi* stunPlugin =
        iPluginContainer->PluginByType( KNatPluginTypeStun() );
    EUNIT_ASSERT( stunPlugin );
    
    CNATFWPluginApi* turnPlugin =
        iPluginContainer->PluginByType( KNatPluginTypeTurn() );
    EUNIT_ASSERT( turnPlugin );
    
    CNATFWPluginApi* icePlugin =
        iPluginContainer->PluginByType( KNatPluginTypeIce() );
    EUNIT_ASSERT( !icePlugin );
    
    CNATFWPluginApi* hostPlugin =
        iPluginContainer->PluginByType( KNatPluginTypeHost() );
    EUNIT_ASSERT( hostPlugin );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_ErrorL(  )
    {
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ELocalCandidateFound );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EFetchingCompleted );
    
    iPluginContainer->Error( *iPluginContainer->iPlugins[0], 
        KTestStreamIdRtp1, KErrUnknown );
    EUNIT_ASSERT( 0 == iNumOfCallbacks );
    
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EGeneralError );
    
    iPluginContainer->Error( *iPluginContainer->iPlugins[0], 
        KTestStreamIdRtp1, KErrUnknown );
    EUNIT_ASSERT( 
        KTestStreamIdRtp1 == iStreamId && KErrUnknown == iErrorCode 
        && 1 == iNumOfCallbacks );
    
    iPluginContainer->UnregObserver( *this );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_NotifyL(  )
    {
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ELocalCandidateFound );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EFetchingCompleted  );
    
    MNATFWPluginObserver::TNATFWPluginEvent event( 
        MNATFWPluginObserver::EConnChecksCompleted );
    iPluginContainer->Notify( *iPluginContainer->iPlugins[0], 
        KTestStreamIdRtp1, event, KErrCancel );
    EUNIT_ASSERT( 0 == iNumOfCallbacks );
    
    event = MNATFWPluginObserver::EFetchingCompleted;
    iPluginContainer->Notify( *iPluginContainer->iPlugins[0], 
        KTestStreamIdRtp1, event, KErrCancel );
    EUNIT_ASSERT( 
        KTestStreamIdRtp1 == iStreamId && KErrCancel == iErrorCode 
        && EFetchingCompleted == iPluginEvent && 1 == iNumOfCallbacks );
    
    // test handling of invalid event code
    event = static_cast<MNATFWPluginObserver::TNATFWPluginEvent>( -1 );
    iNumOfCallbacks = 0;
    iPluginContainer->Notify( *iPluginContainer->iPlugins[0], 
        KTestStreamIdRtp1, event, KErrCancel );
    EUNIT_ASSERT( 0 == iNumOfCallbacks );
    
    iPluginContainer->UnregObserver( *this );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_NewCandidatePairFoundL(  )
    {
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ECandidatePairFound );
    
    // test handling of NULL object
    iPluginContainer->NewCandidatePairFound( 
        *iPluginContainer->iPlugins[0], NULL );
    
    // test handling of valid object
    CNATFWCandidatePair* pair1 = CNATFWCandidatePair::NewLC();
    CNATFWCandidatePair* pair2 = CNATFWCandidatePair::NewLC( *pair1 );
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPluginContainer->NewCandidatePairFound( 
            *iPluginContainer->iPlugins[0], pair1 );
        EUNIT_ASSERT( 0 == iNumOfCallbacks );
        }
    else
        {
        iPluginContainer->NewCandidatePairFound( 
            *iPluginContainer->iPlugins[0], pair1 );
        EUNIT_ASSERT( 1 == iNumOfCallbacks && *iCandidatePair == *pair2 );
        }
    
    CleanupStack::PopAndDestroy( pair2 );
    CleanupStack::Pop( pair1 );
    iPluginContainer->UnregObserver( *this );
    }

void UT_CIceNatPluginContainer::UT_CIceNatPluginContainer_NewLocalCandidateFoundL(  )
    {
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC( *candidate1 );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPluginContainer->RegObserverForEventL( *this, 
            MIceNatPluginEventObs::ELocalCandidateFound );
        
        iPluginContainer->NewLocalCandidateFound( 
            *iPluginContainer->iPlugins[0], candidate1 );
        EUNIT_ASSERT( 0 == iNumOfCallbacks );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iPluginContainer->RegObserverForEventL( *this, 
                MIceNatPluginEventObs::ELocalCandidateFound ) );
        
        iPluginContainer->NewLocalCandidateFound( 
            *iPluginContainer->iPlugins[0], candidate1 );
        EUNIT_ASSERT( 1 == iNumOfCallbacks && *iCandidate == *candidate2 );
        }
    
    CleanupStack::Pop( candidate2 );
    CleanupStack::Pop( candidate1 );
    
    iPluginContainer->UnregObserver( *this );
    delete candidate2;
    }


void UT_CIceNatPluginContainer::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
        TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }
    
void UT_CIceNatPluginContainer::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/,
        const TInetAddr& /*aAddress*/ )
    {
    
    }

void UT_CIceNatPluginContainer::PluginEventOccured(
        const CNATFWPluginApi* /*aPlugin*/, TUint aStreamId, 
        TNatPluginEvent aEventCode, TInt aErrorCode, TAny* aEventData,
        TUint /*aStreamConnectionId*/, TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    iNumOfCallbacks++;
    iStreamId = aStreamId;
    iPluginEvent = aEventCode;
    iErrorCode = aErrorCode;
    if ( MIceNatPluginEventObs::ECandidatePairFound == aEventCode )
        {
        iCandidatePair = reinterpret_cast<CNATFWCandidatePair*>( aEventData );
        }
    
    if ( MIceNatPluginEventObs::ELocalCandidateFound == aEventCode )
        {
        iCandidate = reinterpret_cast<CNATFWCandidate*>( aEventData );
        }
    
    if ( iEventToWait == aEventCode )
        {
        CActiveScheduler::Stop();
        }
    }

void UT_CIceNatPluginContainer::ClearTestData()
    {
    iStreamId = 0;
    iErrorCode = KErrNone;
    iNumOfCallbacks = 0;
    delete iCandidate;
    iCandidate = NULL;
    delete iCandidatePair;
    iCandidatePair = NULL;
    }

void UT_CIceNatPluginContainer::WaitForEvent( 
        MIceNatPluginEventObs::TNatPluginEvent aEvent )
    {
    iEventToWait = aEvent;
    CActiveScheduler::Start();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceNatPluginContainer,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConnectPluginsL - test ",
    "CIceNatPluginContainer",
    "ConnectPluginsL",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_ConnectPluginsLL, Teardown)

EUNIT_TEST(
    "Count - test ",
    "CIceNatPluginContainer",
    "Count",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_CountL, Teardown)

EUNIT_TEST(
    "RegObserverForEventL - test ",
    "CIceNatPluginContainer",
    "RegObserverForEventL",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_RegObserverForEventLL, Teardown)

EUNIT_TEST(
    "UnregObserverForEvents - test ",
    "CIceNatPluginContainer",
    "UnregObserverForEvents",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_UnRegObserverForEventL, Teardown)

EUNIT_TEST(
    "UnregObserver - test ",
    "CIceNatPluginContainer",
    "UnregObserver",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_UnregObserverL, Teardown)

EUNIT_TEST(
    "ReleasePlugins - test ",
    "CIceNatPluginContainer",
    "ReleasePlugins",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_ReleasePluginsL, Teardown)

EUNIT_TEST(
    "PluginByIdentifier - test ",
    "CIceNatPluginContainer",
    "PluginByIdentifier",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_PluginByIdentifierL, Teardown)

EUNIT_TEST(
    "PluginByType - test ",
    "CIceNatPluginContainer",
    "PluginByType",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_PluginByTypeL, Teardown)

EUNIT_TEST(
    "Error - test ",
    "CIceNatPluginContainer",
    "Error",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_ErrorL, Teardown)

EUNIT_TEST(
    "Notify - test ",
    "CIceNatPluginContainer",
    "Notify",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_NotifyL, Teardown)

EUNIT_TEST(
    "NewCandidatePairFound - test ",
    "CIceNatPluginContainer",
    "NewCandidatePairFound",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_NewCandidatePairFoundL, Teardown)

EUNIT_TEST(
    "NewLocalCandidateFound - test ",
    "CIceNatPluginContainer",
    "NewLocalCandidateFound",
    "FUNCTIONALITY",
    SetupL, UT_CIceNatPluginContainer_NewLocalCandidateFoundL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
