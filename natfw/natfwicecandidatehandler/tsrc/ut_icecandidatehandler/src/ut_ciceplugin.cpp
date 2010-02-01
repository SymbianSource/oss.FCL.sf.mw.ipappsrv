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
#include "UT_CIcePlugin.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "natfwcredentials.h"

//  INTERNAL INCLUDES
#include "ciceplugin.h"
#include "natfwconnectivityapidefs.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIcePlugin* UT_CIcePlugin::NewL()
    {
    UT_CIcePlugin* self = UT_CIcePlugin::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIcePlugin* UT_CIcePlugin::NewLC()
    {
    UT_CIcePlugin* self = new( ELeave ) UT_CIcePlugin();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIcePlugin::~UT_CIcePlugin()
    {
    delete iPlugin;
    delete iMultiplexer;
    iLocalCandidates.ResetAndDestroy();
    iCandPairs.ResetAndDestroy();
    }

// Default constructor
UT_CIcePlugin::UT_CIcePlugin()
    {
    }

// Second phase construct
void UT_CIcePlugin::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIcePlugin::SetupL(  )
    {
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    CNATFWPluginApi::TNATFWPluginInitParams params( 
        *this, *iMultiplexer, KTestDomain, KTestIap );
    
    iPlugin = CIcePlugin::NewL( &params );
    
    }

void UT_CIcePlugin::Teardown(  )
    {
    static_cast<CIcePlugin*>( iPlugin )->StreamClosed( KTestStreamIdRtp1 );
    delete iPlugin;
    iPlugin = NULL;
    delete iMultiplexer;
    iMultiplexer = NULL;
    iLocalCandidates.ResetAndDestroy();
    iCandPairs.ResetAndDestroy();
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_ConnectServerLL()
    {
    RSocketServ server;
    TName name;
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        TRAPD( err, iPlugin->ConnectServerL( server, name ));
        if ( err == KErrCouldNotConnect )
        	{
        	User::Leave( KErrNoMemory );
        	}
        else if ( err != KErrNone )
        		{
        		User::Leave( err );
        		}
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iPlugin->ConnectServerL( server, name ) );
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( 
            MNATFWPluginObserver::EServerConnected == iPluginEvent );
        }
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_FetchCandidatesLL(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPlugin->FetchCandidatesL(
            KTestStreamCollectionId1, KTestStreamIdRtp1, 1, KAfInet );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iPlugin->FetchCandidatesL(
            KTestStreamCollectionId1, KTestStreamIdRtp1, 1, KAFUnspec ) );
        
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( iLocalCandidates.Count() );
        }
    }
    
void UT_CIcePlugin::UT_CNATFWICEPlugin_FetchCandidatesLL2(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPlugin->FetchCandidatesL(
            KTestStreamCollectionId1, KTestStreamIdRtp1, 1, KAfInet6 );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iPlugin->FetchCandidatesL(
            KTestStreamCollectionId1, KTestStreamIdRtp1, 1, KAfInet ) );
        
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( iLocalCandidates.Count() );
        
        Teardown();
        SetupL();
        
        EUNIT_ASSERT_NO_LEAVE( iPlugin->FetchCandidatesL(
            KTestStreamCollectionId1, KTestStreamIdRtp1, 1, KAfInet6 ) );
        
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( iLocalCandidates.Count() );       
        }
    }    

void UT_CIcePlugin::UT_CNATFWICEPlugin_SetReceivingStateLL()
    {
    UT_CNATFWICEPlugin_FetchCandidatesLL();
    
    TInt ind( KErrNotFound );
    TInt count = iLocalCandidates.Count();
    
    for ( TInt i = 0; i < count; ++i )
        {
    // Activate receiving
        CNATFWCandidate* candidate = iLocalCandidates[i];
        iPlugin->SetReceivingStateL( 
            *iLocalCandidates[i], EStreamingStateActive );
     
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;        
        
        EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
        EUNIT_ASSERT( 
            MNATFWPluginObserver::EReceivingActivated == iPluginEvent );
        EUNIT_ASSERT( KErrNone == iEventErrCode );
        
    // Deactivate receiving    
        iPlugin->SetReceivingStateL( 
            *iLocalCandidates[i], EStreamingStatePassive );
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        
        EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
        EUNIT_ASSERT( 
            MNATFWPluginObserver::EReceivingDeactivated == iPluginEvent );
        EUNIT_ASSERT( KErrNone == iEventErrCode );
        }
    
    if ( count )
        {
    // Activate after deactivated
        iPlugin->SetReceivingStateL( 
            *iLocalCandidates[0], EStreamingStateActive );
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        
        EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
        EUNIT_ASSERT( 
            MNATFWPluginObserver::EReceivingActivated == iPluginEvent );
        EUNIT_ASSERT( KErrNone == iEventErrCode );
        
        if ( !TIceTestUtils::AllocTestEnabled() )
            {
        // activate peer reflexive candidate
            CNATFWCandidate* peerCandidate 
                = CNATFWCandidate::NewLC( *iLocalCandidates[0] );
            peerCandidate->SetType( CNATFWCandidate::EPeerReflexive );
            peerCandidate->SetStreamId( KTestStreamIdRtp1 );
            EUNIT_ASSERT_NO_LEAVE( 
                iPlugin->SetReceivingStateL( 
                *peerCandidate, EStreamingStateActive ) );
            iUseWaitLoop = ETrue;
            CActiveScheduler::Start();
            iUseWaitLoop = EFalse;
            EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
            EUNIT_ASSERT( 
                MNATFWPluginObserver::EReceivingActivated == iPluginEvent );
            EUNIT_ASSERT( KErrNone == iEventErrCode );
            
        // Deactivate peer reflexive candidate
            EUNIT_ASSERT_NO_LEAVE( 
                iPlugin->SetReceivingStateL( 
                *peerCandidate, EStreamingStatePassive ) );
            iUseWaitLoop = ETrue;
            CActiveScheduler::Start();
            iUseWaitLoop = EFalse;
            EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
            EUNIT_ASSERT( 
                MNATFWPluginObserver::EReceivingDeactivated == iPluginEvent );
            EUNIT_ASSERT( KErrNone == iEventErrCode );
            
            CleanupStack::PopAndDestroy( peerCandidate );
            }
        }
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_SetSendingStateL(  )
    {
    UT_CNATFWICEPlugin_FetchCandidatesLL();
    
    TInetAddr remoteAddr = INET_ADDR( 192, 168, 1, 1 );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        if ( iLocalCandidates.Count() )
            {
            iPlugin->SetSendingStateL( 
                *iLocalCandidates[0], EStreamingStateActive, remoteAddr );
            }
        }
    else
        {
        TInt count = iLocalCandidates.Count();
        for ( TInt i = 0; i < count; i++ )
            {
            // Activate
            iPlugin->SetSendingStateL( 
                *iLocalCandidates[i], EStreamingStateActive, remoteAddr );
            iUseWaitLoop = ETrue;
            CActiveScheduler::Start();
            iUseWaitLoop = EFalse;
            EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
            EUNIT_ASSERT( 
                MNATFWPluginObserver::ESendingActivated == iPluginEvent );
            EUNIT_ASSERT( KErrNone == iEventErrCode );
            
            // Passivate
            iPlugin->SetSendingStateL( 
                *iLocalCandidates[i], EStreamingStatePassive, remoteAddr );
            iUseWaitLoop = ETrue;
            CActiveScheduler::Start();
            iUseWaitLoop = EFalse;
            EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
            EUNIT_ASSERT( 
                MNATFWPluginObserver::ESendingDeactivated == iPluginEvent );
            EUNIT_ASSERT( KErrNone == iEventErrCode );
            }
        }
    
    if ( !TIceTestUtils::AllocTestEnabled() )
        {
    // activate peer reflexive candidate
        CNATFWCandidate* peerCandidate 
            = CNATFWCandidate::NewLC( *iLocalCandidates[0] );
        peerCandidate->SetType( CNATFWCandidate::EPeerReflexive );
        peerCandidate->SetStreamId( KTestStreamIdRtp1 );
        EUNIT_ASSERT_NO_LEAVE( iPlugin->SetSendingStateL( 
            *peerCandidate, EStreamingStateActive, remoteAddr ) );
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
        EUNIT_ASSERT( 
            MNATFWPluginObserver::ESendingActivated == iPluginEvent );
        EUNIT_ASSERT( KErrNone == iEventErrCode );
        
    // Deactivate peer reflexive candidate
        EUNIT_ASSERT_NO_LEAVE( iPlugin->SetSendingStateL( 
            *peerCandidate, EStreamingStatePassive, remoteAddr ) );
        iUseWaitLoop = ETrue;
        CActiveScheduler::Start();
        iUseWaitLoop = EFalse;
        EUNIT_ASSERT( KTestStreamIdRtp1 == iEventStreamId );
        EUNIT_ASSERT( 
            MNATFWPluginObserver::ESendingDeactivated == iPluginEvent );
        EUNIT_ASSERT( KErrNone == iEventErrCode );        
        
        CleanupStack::PopAndDestroy( peerCandidate ); 
        }
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_GetConnectionIdLL()
    {
    UT_CNATFWICEPlugin_FetchCandidatesLL();

    TUint connectionId;
    TInt ind( KErrNotFound );
    TInt count = iLocalCandidates.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        ind = iLocalCandidates[i]->Type();
        EUNIT_ASSERT_NO_LEAVE( iPlugin->GetConnectionIdL( *iLocalCandidates[i], connectionId ) );
        }
        
    CNATFWCandidate* hostIpv6Candidate = CNATFWCandidate::NewLC();
    hostIpv6Candidate->SetType( CNATFWCandidate::EHost );
    TInetAddr ipv6Addr( KAfInet6);
    hostIpv6Candidate->SetTransportAddrL( ipv6Addr );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iPlugin->GetConnectionIdL(
        *hostIpv6Candidate, connectionId ), KErrNotFound );
    CleanupStack::PopAndDestroy( hostIpv6Candidate );             
    }
  

void UT_CIcePlugin::UT_CNATFWICEPlugin_SetOperationModeLL(  )
    {
    EUNIT_ASSERT_LEAVE(
        iPlugin->SetRoleL( static_cast<TNATFWIceRole>( 0 ) ) );
    
    EUNIT_ASSERT_NO_LEAVE(
        iPlugin->SetRoleL( EIceRoleControlling ) );
    
    // TODO: Role change due to new offer-answer exchange
    EUNIT_ASSERT_NO_LEAVE(
        iPlugin->SetRoleL( EIceRoleControlled ) );
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_SetIdentificationLL(  )
    {
    CNATFWCredentials* identification1 = CNATFWCredentials::NewLC();
    identification1->SetUsernameL( _L8("alice") );
    identification1->SetPasswordL( _L8("pass") );
    identification1->SetStreamCollectionId( KTestStreamCollectionId1 );
    identification1->SetStreamId( KTestStreamIdRtp1 );
    identification1->SetDirection( CNATFWCredentials::EInbound );

    CNATFWCredentials* identification2 = CNATFWCredentials::NewLC();
    identification2->SetUsernameL( _L8("bob") );
    identification2->SetPasswordL( _L8("ssap") );
    identification2->SetStreamCollectionId( KTestStreamCollectionId1 );
    identification2->SetStreamId( KTestStreamIdRtp1 );
    identification2->SetDirection( CNATFWCredentials::EOutbound );
    
    CNATFWCandidate* dummyParam = CNATFWCandidate::NewLC();
    dummyParam->SetStreamId( KTestStreamIdRtp1 );
    dummyParam->SetStreamCollectionId( KTestStreamCollectionId1 );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iPlugin->SetCredentialsL( *dummyParam, *identification1 );
        iPlugin->SetCredentialsL( *dummyParam, *identification2 );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iPlugin->SetCredentialsL( *dummyParam, *identification1 ) );
        EUNIT_ASSERT_NO_LEAVE(
            iPlugin->SetCredentialsL( *dummyParam, *identification2 ) );
        }
    
    CleanupStack::PopAndDestroy( dummyParam );
    CleanupStack::PopAndDestroy( identification2 );
    CleanupStack::PopAndDestroy( identification1 );
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_PerformConnectivityChecksLL(  )
    {
    // TODO: Fetchcandidates must generate some local candidates
    UT_CNATFWICEPlugin_FetchCandidatesLL();
    UT_CNATFWICEPlugin_SetIdentificationLL();
    EUNIT_ASSERT_NO_LEAVE(
        iPlugin->SetRoleL( EIceRoleControlling ) );
    
    RPointerArray<CNATFWCandidate> candArray;
    CleanupResetAndDestroyPushL( candArray );

    CNATFWCandidate* remoteCandRtp = CNATFWCandidate::NewLC();
    remoteCandRtp->SetType( CNATFWCandidate::EHost );
    remoteCandRtp->SetStreamCollectionId( KTestStreamCollectionId1 );
    remoteCandRtp->SetStreamId( KTestStreamIdRtp1 );
    TInetAddr privateAddr( KLocalAddr );
    privateAddr.SetPort( KLocalPort );
    privateAddr.SetFamily( KAfInet );
    remoteCandRtp->SetTransportAddrL( privateAddr );
    remoteCandRtp->SetBase( privateAddr );
    remoteCandRtp->SetFoundationL( _L8("1") );
    remoteCandRtp->SetComponentId( 1 );
    remoteCandRtp->SetTransportProtocol( KProtocolInetUdp );
    remoteCandRtp->SetPriority( 2130706178 );
    
    CNATFWCandidate* remoteCandRtcp = CNATFWCandidate::NewLC( *remoteCandRtp );
    remoteCandRtcp->SetStreamId( KTestStreamIdRtcp1 );
    privateAddr.SetPort( KLocalPort + 1 );
    remoteCandRtcp->SetTransportAddrL( privateAddr );
    remoteCandRtcp->SetBase( privateAddr );
    remoteCandRtcp->SetComponentId( 2 );
    remoteCandRtcp->SetPriority( 2130706178 - 1 );
    
    candArray.AppendL( remoteCandRtcp );
    CleanupStack::Pop( remoteCandRtcp );
    candArray.AppendL( remoteCandRtp );
    CleanupStack::Pop( remoteCandRtp );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // TODO: no local candidates due to OOM (test case problem)
        EUNIT_ASSERT_LEAVE(
            iPlugin->PerformConnectivityChecksL( candArray ) );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iPlugin->PerformConnectivityChecksL( candArray ) );
        }

    CleanupStack::PopAndDestroy( &candArray );
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_UpdateIceProcessing1LL()
    {
    // test with null array
    RPointerArray<CNATFWCandidatePair> peerSelectedPairs;
    CleanupResetAndDestroyPushL( peerSelectedPairs );
    EUNIT_ASSERT_LEAVE( iPlugin->UpdateIceProcessingL( peerSelectedPairs ) );
    
    CleanupStack::PopAndDestroy( &peerSelectedPairs );
    }


void UT_CIcePlugin::UT_CNATFWICEPlugin_UpdateIceProcessing2LL()
    {
    // test with null array
    RPointerArray<CNATFWCandidate> newRemoteCands;
    CleanupResetAndDestroyPushL( newRemoteCands );
    EUNIT_ASSERT_LEAVE( iPlugin->UpdateIceProcessingL( newRemoteCands ) );
    
    CleanupStack::PopAndDestroy( &newRemoteCands );
    }


void UT_CIcePlugin::UT_CNATFWICEPlugin_PluginIdentifierL()
    {
    const TDesC8& identifier = iPlugin->PluginIdentifier();
    EUNIT_ASSERT( KNatPluginIdNokiaIce() == identifier );
    }

void UT_CIcePlugin::UT_CNATFWICEPlugin_PluginEventOccuredL()
    {
    // internal events must not include plugin parameter
    CIcePlugin* plugin = static_cast<CIcePlugin*>( iPlugin );
    
    // ELocalCandidateFound
    CNATFWCandidate* localCandidate = CNATFWCandidate::NewL();
    
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::ELocalCandidateFound, KErrNone, 
            localCandidate );
    if ( !TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT( 0 != iLocalCandidates.Count() );
        }
    
    // EFetchingCompleted
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::EFetchingCompleted, KErrNone, 
            NULL );
    EUNIT_ASSERT( EFetchingCompleted == iPluginEvent );
    
    // ESendingActivated
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::ESendingActivated, KErrNone, 
            NULL );
    EUNIT_ASSERT( MNATFWPluginObserver::ESendingActivated == iPluginEvent );
    
    // ESendingDeactivated
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::ESendingDeactivated, KErrNone, 
            NULL );
    EUNIT_ASSERT( MNATFWPluginObserver::ESendingDeactivated == iPluginEvent );
    
    // ECandidatePairFound
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewL();
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::ECandidatePairFound, KErrNone, 
            pair );
    if ( !TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT( 0 != iCandPairs.Count() );
        }
    
    // EConnChecksCompleted
    plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::EConnChecksCompleted, KErrNone, 
            NULL );
    EUNIT_ASSERT( EConnChecksCompleted == iPluginEvent );
    
    // EGeneralError
    plugin->PluginEventOccured( iPlugin, KTestStreamIdRtp1, 
            MIceNatPluginEventObs::EGeneralError, KErrUnknown, 
            NULL );
    
    // unknown event
    EUNIT_ASSERT_PANIC(
        plugin->PluginEventOccured( NULL, KTestStreamIdRtp1, 
            static_cast<MIceNatPluginEventObs::TNatPluginEvent>( -1 ),
            KErrNone, NULL ), "USER", 0 );
    }

void UT_CIcePlugin::Error( 
        const CNATFWPluginApi& /*aPlugin*/,
        TUint /*aStreamId*/, TInt /*aErrorCode*/ )
    {
    
    }

void UT_CIcePlugin::Notify( 
        const CNATFWPluginApi& /*aPlugin*/,
        TUint aStreamId, TNATFWPluginEvent aEvent,
        TInt aErrCode )
    {
    iEventStreamId = aStreamId;
    iPluginEvent = aEvent;
    iEventErrCode = aErrCode;
    if ( iUseWaitLoop )
        {
        CActiveScheduler::Stop();
        }
    }

void UT_CIcePlugin::NewCandidatePairFound( 
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidatePair* aCandidatePair )
    {
    TInt err = iCandPairs.Append( aCandidatePair );
    if ( err )
        {
        delete aCandidatePair;
        }
    }

void UT_CIcePlugin::NewLocalCandidateFound( 
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidate* aLocalCandidate )
    {
    TInt err = iLocalCandidates.Append( aLocalCandidate );
    if ( err )
        {
        delete aLocalCandidate;
        }
    }

void UT_CIcePlugin::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }
    
void UT_CIcePlugin::IcmpError( TUint /*aSessionId*/, 
        TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIcePlugin,
    "UT_CIcePlugin",
    "UNIT" )

EUNIT_TEST(
    "ConnectServerL - test ",
    "CIcePlugin",
    "ConnectServerL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_ConnectServerLL, Teardown)

EUNIT_TEST(
    "FetchCandidatesL - test ",
    "CIcePlugin",
    "FetchCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_FetchCandidatesLL, Teardown)

EUNIT_TEST(
    "FetchCandidatesL - test 2 ",
    "CIcePlugin",
    "FetchCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_FetchCandidatesLL2, Teardown)

EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CIcePlugin",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_SetReceivingStateLL, Teardown)

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CIcePlugin",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_SetSendingStateL, Teardown)

EUNIT_TEST(
    "GetConnectionIdL - test ",
    "CIcePlugin",
    "GetConnectionIdL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_GetConnectionIdLL, Teardown)

EUNIT_TEST(
    "SetRoleL - test ",
    "CIcePlugin",
    "SetRoleL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_SetOperationModeLL, Teardown)

EUNIT_TEST(
    "SetCredentialsL - test ",
    "CIcePlugin",
    "SetCredentialsL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_SetIdentificationLL, Teardown)

EUNIT_TEST(
    "PerformConnectivityChecksL - test ",
    "CIcePlugin",
    "PerformConnectivityChecksL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_PerformConnectivityChecksLL, Teardown)

EUNIT_TEST(
    "UpdateIceProcessing1L - test ",
    "CIcePlugin",
    "UpdateIceProcessing1L",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_UpdateIceProcessing1LL, Teardown)

EUNIT_TEST(
    "UpdateIceProcessing2L - test ",
    "CIcePlugin",
    "UpdateIceProcessing2L",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_UpdateIceProcessing2LL, Teardown)

EUNIT_TEST(
    "Pluginidentifier - test ",
    "CIcePlugin",
    "Pluginidentifier",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_PluginIdentifierL, Teardown)

EUNIT_TEST(
    "PluginEventOccured - test ",
    "CIcePlugin",
    "PluginEventOccured ",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWICEPlugin_PluginEventOccuredL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
