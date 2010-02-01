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
#include "UT_CIceConnectivityCheck.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"

//  INTERNAL INCLUDES
#include "ciceconnectivitycheck.h"
#include "cicesessiondata.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "ciceconnectionhandler.h"
#include "cicenatplugincontainer.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceConnectivityCheck* UT_CIceConnectivityCheck::NewL()
    {
    UT_CIceConnectivityCheck* self = UT_CIceConnectivityCheck::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceConnectivityCheck* UT_CIceConnectivityCheck::NewLC()
    {
    UT_CIceConnectivityCheck* self = new( ELeave ) UT_CIceConnectivityCheck();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceConnectivityCheck::~UT_CIceConnectivityCheck()
    {
    delete iConnCheck;
    delete iLocalCand;
    delete iRemoteCand;
    delete iValidatedPair;
    delete iCandStorage;
    delete iConnHandler;
    delete iMultiplexer;
    delete iInboundIdentRtp;
    delete iOutboundIdentRtp;

    delete iPluginContainer;
    }

// Default constructor
UT_CIceConnectivityCheck::UT_CIceConnectivityCheck()
    {
    }

// Second phase construct
void UT_CIceConnectivityCheck::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    iLocalCand = CNATFWCandidate::NewL();
    iLocalCand->SetType( CNATFWCandidate::EHost );
    iLocalCand->SetStreamCollectionId( KTestStreamCollectionId1 );
    iLocalCand->SetStreamId( KTestStreamIdRtp1 );
    TInetAddr privateAddr( KLocalAddr );
    privateAddr.SetPort( KLocalPort );
    privateAddr.SetFamily( KAfInet );
    iLocalCand->SetTransportAddrL( privateAddr );
    iLocalCand->SetBase( privateAddr );
    iLocalCand->SetFoundationL( _L8("1") );
    iLocalCand->SetComponentId( 1 ); // RTP
    iLocalCand->SetTransportProtocol( KProtocolInetUdp );
    iLocalCand->SetPriority( 2130706178 );
    
    iRemoteCand = CNATFWCandidate::NewL( *iLocalCand );
    TInetAddr remotePrivateAddr( KRemoteAddr );
    remotePrivateAddr.SetPort( KRemotePort );
    remotePrivateAddr.SetFamily( KAfInet );
    iRemoteCand->SetTransportAddrL( remotePrivateAddr );
    iRemoteCand->SetFoundationL( _L8("10") );
    
    iInboundIdentRtp = CNATFWCredentials::NewL();
    iInboundIdentRtp->SetStreamCollectionId( KTestStreamCollectionId1 );
    iInboundIdentRtp->SetStreamId( KTestStreamIdRtp1 );
    iInboundIdentRtp->SetDirection( CNATFWCredentials::EInbound );
    iInboundIdentRtp->SetUsernameL( _L8("alic") );
    iInboundIdentRtp->SetPasswordL( _L8("pass") );
    
    iOutboundIdentRtp = CNATFWCredentials::NewL();
    iOutboundIdentRtp->SetStreamCollectionId( KTestStreamCollectionId1 );
    iOutboundIdentRtp->SetStreamId( KTestStreamIdRtp1 );
    iOutboundIdentRtp->SetDirection( CNATFWCredentials::EOutbound );
    iOutboundIdentRtp->SetUsernameL( _L8("bob") );
    iOutboundIdentRtp->SetPasswordL( _L8("ssap") );
    }

//  METHODS

void UT_CIceConnectivityCheck::SetupL(  )
    {
    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
	iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
	
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
        
	iConnHandler = CIceConnectionHandler::NewL( 
		*iMultiplexer, *this, *iCandStorage, *iPluginContainer, *this );
    
    iConnCheck = CIceConnectivityCheck::NewL( 
        *this, *iLocalCand, *iRemoteCand, *iConnHandler, *iCandStorage );
    
    iCandStorage->AddCredentialsL( *iInboundIdentRtp );
    iCandStorage->AddCredentialsL( *iOutboundIdentRtp );
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    }

void UT_CIceConnectivityCheck::Teardown(  )
    {
    delete iCandStorage;
    iCandStorage = NULL;
    delete iConnCheck;
    iConnCheck = NULL;
    delete iConnHandler;
    iConnHandler = NULL;
    delete iMultiplexer;
    iMultiplexer = NULL;
    delete iValidatedPair;
    iValidatedPair = NULL;
    delete iPluginContainer;
    iPluginContainer = NULL;
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_ComparePrioritiesL(  )
    {
    //EUNIT_ASSERT( EFalse );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_MatchAddressesL(  )
    {
    //EUNIT_ASSERT( EFalse );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_CreateValidatedPairLL()
    {
    CNATFWCandidatePair* pair( NULL );
    TInetAddr mappedAddr( KLocalAddr );
    mappedAddr.SetPort( KLocalPort );
    mappedAddr.SetFamily( KAfInet ); 
       
    if ( !TIceTestUtils::AllocTestEnabled() )
        {
        iCandStorage->AddLocalCandidateL( CNATFWCandidate::NewL( *iLocalCand ) );
        CNATFWCandidatePair* pair = iConnCheck->CreateValidatedPairL( mappedAddr );
        delete pair;

        CNATFWCandidatePair* pair2 = iConnCheck->CreateValidatedPairL( mappedAddr );
        delete pair2;
        }
    else
        {
        EUNIT_ASSERT_LEAVE( pair = iConnCheck->CreateValidatedPairL( mappedAddr ) );
        delete pair;
        }
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_PerformConnCheckLL(  )
    {
    // Test succesfull case
    const TUint priority = 100;
    
    iCandStorage->AddLocalCandidateL( CNATFWCandidate::NewL( *iLocalCand ) );
    
    iConnCheck->PerformConnCheckL( 
        CIceConnectivityCheck::EIceCheckTypePeriodic,
        priority, KTaTimerValueForChecks );
    EUNIT_ASSERT( iConnCheck->iRemotePriority == 0 );
    EUNIT_ASSERT( iConnCheck->iRemoteFavored == EFalse );
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckInProgress == iConnCheck->State() );
    
    TInetAddr localAddr = iLocalCand->TransportAddr();
    TInetAddr peerAddr = iRemoteCand->TransportAddr();
    TInetAddr mappedAddr( KLocalAddr );
    mappedAddr.SetPort( KLocalPort );
    mappedAddr.SetFamily( KAfInet );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
        delete iValidatedPair;
        iValidatedPair = NULL;
        
        iConnCheck->PerformNominationL( 0 );
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
        delete iValidatedPair;
        iValidatedPair = NULL;
        
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->PerformNominationL( 0 ) );
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        }
    
    EUNIT_ASSERT( KErrNone == iCheckResult );
    EUNIT_ASSERT( iValidatedPair && iValidatedPair->Selected() );
    EUNIT_ASSERT( iValidatedPair->LocalCandidate().Priority() 
        == iLocalCand->Priority() );
    EUNIT_ASSERT( iValidatedPair->RemoteCandidate().Priority() 
        == iRemoteCand->Priority() );
        
    // remote has seen some our local candidate, which was signaled
    EUNIT_ASSERT( iValidatedPair->LocalCandidate()
        .TransportAddr().CmpAddr( mappedAddr ) );
    
    delete iValidatedPair;
    iValidatedPair = NULL;
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_PerformConnCheckL_TL(  )
    {
    // Test succesfull case
    const TUint priority = 100;
    const TUint remotePriority = 200;
    TBool remoteFavored = ETrue;
    
    iCandStorage->AddLocalCandidateL( CNATFWCandidate::NewL( *iLocalCand ) );
    
// PROACTIVE SELECTION USED BY REMOTE PARTY
    iConnCheck->SetRemoteCheckInfo( remotePriority, remoteFavored );
    iConnCheck->PerformConnCheckL( 
        CIceConnectivityCheck::EIceCheckTypeTriggered,
        priority, KTaTimerValueForChecks );
    EUNIT_ASSERT( iConnCheck->iRemotePriority == remotePriority );
    EUNIT_ASSERT( iConnCheck->iRemoteFavored == remoteFavored );
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckInProgress == iConnCheck->State() );
    
    TInetAddr localAddr = iLocalCand->TransportAddr();
    TInetAddr peerAddr = iRemoteCand->TransportAddr();
    TInetAddr mappedAddr( KLocalAddr );
    mappedAddr.SetPort( KLocalPort );
    mappedAddr.SetFamily( KAfInet );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        }

    EUNIT_ASSERT( KErrNone == iCheckResult );
    EUNIT_ASSERT( iValidatedPair && iValidatedPair->Selected() );
    
    // remote has seen some our local candidate, which was signaled
    EUNIT_ASSERT( iValidatedPair->LocalCandidate()
        .TransportAddr().CmpAddr( mappedAddr ) );
    
    delete iValidatedPair;
    iValidatedPair = NULL;
        
// INTROSPECTIVE SELECTION USED BY REMOTE PARTY
    iConnCheck->iState = CIceConnectivityCheck::EIceCheckFrozen;
    delete iConnCheck->iValidatedPair; iConnCheck->iValidatedPair = NULL;
    remoteFavored = EFalse;
    iConnCheck->SetRemoteCheckInfo( remotePriority, remoteFavored );
    iConnCheck->PerformConnCheckL( 
        CIceConnectivityCheck::EIceCheckTypeTriggered,
        priority, KTaTimerValueForChecks );

    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        }

    EUNIT_ASSERT( KErrNone == iCheckResult );
    EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
    EUNIT_ASSERT( 
        iValidatedPair->RemoteCandidate().Priority() == remotePriority );
    
    delete iValidatedPair;
    iValidatedPair = NULL;
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_PerformConnCheckL2L(  )
    {
    // Test succesfull case
    const TUint priority = 100;
    
    iConnCheck->PerformConnCheckL( 
        CIceConnectivityCheck::EIceCheckTypePeriodic,
        priority, KTaTimerValueForChecks );
    EUNIT_ASSERT( iConnCheck->iRemotePriority == 0 );
    EUNIT_ASSERT( iConnCheck->iRemoteFavored == EFalse );
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckInProgress == iConnCheck->State() );
    
    TInetAddr localAddr = iLocalCand->TransportAddr();
    TInetAddr peerAddr = iRemoteCand->TransportAddr();
    TInetAddr mappedAddr( KMappedAddr );    // Peer reflexive address
    mappedAddr.SetPort( KMappedPort );
    mappedAddr.SetFamily( KAfInet );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
        delete iValidatedPair;
        iValidatedPair = NULL;
        
        iConnCheck->PerformNominationL( 0 );
        iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
        delete iValidatedPair;
        iValidatedPair = NULL;
        
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->PerformNominationL( 0 ) );
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        }

    
    EUNIT_ASSERT( KErrNone == iCheckResult );
    EUNIT_ASSERT( iValidatedPair && iValidatedPair->Selected() );
    // For peer reflx candidate priority is gotten from request priority
    EUNIT_ASSERT( iValidatedPair->LocalCandidate().Priority() == priority );
    EUNIT_ASSERT( iValidatedPair->RemoteCandidate().Priority() 
        == iRemoteCand->Priority() );
    
    // remote has seen some our local candidate, which was signaled
    EUNIT_ASSERT( iValidatedPair->LocalCandidate()
        .TransportAddr().CmpAddr( mappedAddr ) );
    
    delete iValidatedPair;
    iValidatedPair = NULL;
    }
    
void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_PerformConnCheckL3L()
    {
    const TUint priority = 100;
    const TUint remotePriority = 100;
    const TBool remoteFavored = ETrue;
    
    iConnCheck->SetRemoteCheckInfo( remotePriority, remoteFavored );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT_LEAVE( iConnCheck->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic,
            priority, KTaTimerValueForChecks ) );        
        }
    else
        {
        iConnCheck->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic,
            priority, KTaTimerValueForChecks );        

        TInetAddr fromAddr = iRemoteCand->TransportAddr();
        TInetAddr destAddr = iLocalCand->TransportAddr();
        TInetAddr mappedAddr( KMappedAddr );
        mappedAddr.SetPort( KMappedPort );
        mappedAddr.SetFamily( KAfInet );
        
        // Invalid addresses
        fromAddr.SetPort( 1000 );
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, fromAddr, destAddr, mappedAddr ) );
        fromAddr.SetPort( KRemotePort );
        EUNIT_ASSERT( KErrNone != iCheckResult );
        EUNIT_ASSERT( !iValidatedPair );
        iConnCheck->iState = CIceConnectivityCheck::EIceCheckInProgress;
        /*
        // no way to check currently that destination address matches
        destAddr.SetPort( 1000 );
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, fromAddr, destAddr, mappedAddr ) );
        destAddr.SetPort( KLocalPort );
        EUNIT_ASSERT( KErrNone != iCheckResult );
        EUNIT_ASSERT( !iValidatedPair );
        iConnCheck->iState = CIceConnectivityCheck::EICECheckInProgress;
        */
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrUnknown, fromAddr, destAddr, mappedAddr ) );
        EUNIT_ASSERT( KErrNone != iCheckResult );
        EUNIT_ASSERT( !iValidatedPair );
        
        // Wrong state
        iConnCheck->iState = CIceConnectivityCheck::EIceCheckWaiting;
        EUNIT_ASSERT_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, fromAddr, destAddr, mappedAddr ) );

        delete iConnCheck;
        iConnCheck = NULL;
        iLocalCand->SetType( CNATFWCandidate::EServerReflexive );
        iConnCheck = CIceConnectivityCheck::NewL( 
            *this, *iLocalCand, *iRemoteCand, *iConnHandler, *iCandStorage );    
        
        EUNIT_ASSERT_SPECIFIC_LEAVE( iConnCheck->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic,
            priority, KTaTimerValueForChecks ), KErrArgument );
        }
    }
    
void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_PerformConnCheckL4L()
    {
    const TUint priority = 100;
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT_SPECIFIC_LEAVE( iConnCheck->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic,
            priority, KTaTimerValueForChecks ), KErrNoMemory );         
        }
        
    else
        {
        iLocalCand->SetType( CNATFWCandidate::ERelay );
        delete iConnCheck;
        iConnCheck = NULL;
        iConnCheck = CIceConnectivityCheck::NewL( 
            *this, *iLocalCand, *iRemoteCand, *iConnHandler, *iCandStorage );    

        CNATFWPluginApi* turnPlugin
                = iPluginContainer->PluginByType( KNatPluginTypeTurn() );
        turnPlugin->FetchCandidateL( 1, 100, iLocalCand->TransportAddr() );
        
        // Test succesfull case
        iCandStorage->AddLocalCandidateL( CNATFWCandidate::NewL( *iLocalCand ) );

        iConnCheck->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic,
            priority, KTaTimerValueForChecks );       
        EUNIT_ASSERT( iConnCheck->iRemotePriority == 0 );
        EUNIT_ASSERT( iConnCheck->iRemoteFavored == EFalse );
        EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckInProgress == iConnCheck->State() );        

        TInetAddr localAddr = iLocalCand->TransportAddr();
        TInetAddr peerAddr = iRemoteCand->TransportAddr();
        TInetAddr mappedAddr( KLocalAddr );
        mappedAddr.SetPort( KLocalPort );
        mappedAddr.SetFamily( KAfInet );

        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
        EUNIT_ASSERT( iValidatedPair && !iValidatedPair->Selected() );
        delete iValidatedPair;
        iValidatedPair = NULL;
        
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->PerformNominationL( 0 ) );
        EUNIT_ASSERT_NO_LEAVE( iConnCheck->ConnCheckCompletedL( 
            KErrNone, localAddr, peerAddr, mappedAddr ) );
    
        EUNIT_ASSERT( KErrNone == iCheckResult );
        EUNIT_ASSERT( iValidatedPair && iValidatedPair->Selected() );
        /*EUNIT_ASSERT( iValidatedPair->LocalCandidate().Priority() 
            == iLocalCand->Priority() );*/
        EUNIT_ASSERT( iValidatedPair->RemoteCandidate().Priority() 
            == iRemoteCand->Priority() );
            
        // remote has seen some our local candidate, which was signaled
        EUNIT_ASSERT( iValidatedPair->LocalCandidate()
            .TransportAddr().CmpAddr( mappedAddr ) );
           
        delete iValidatedPair;
        iValidatedPair = NULL;          
        }
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_FoundationL(  )
    {
    const TDesC8& foundation = iConnCheck->CandidatePair().Foundation();
    EUNIT_ASSERT( foundation.Size() );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_ComponentIdL(  )
    {
    TUint componentId = iConnCheck->ComponentId();
    EUNIT_ASSERT( 
        componentId == iLocalCand->ComponentId()
        && componentId == iRemoteCand->ComponentId() );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_StateL(  )
    {
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckFrozen == iConnCheck->State() );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_InitializeCheckL(  )
    {
    iConnCheck->Initialize();
    EUNIT_ASSERT( CIceConnectivityCheck::EIceCheckWaiting 
        == iConnCheck->iState );
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_ValidatedPairLL(  )
    {
    EUNIT_ASSERT( NULL == iConnCheck->ValidatedPair() );
    iConnCheck->iValidatedPair = reinterpret_cast<CNATFWCandidatePair*>( this );
    EUNIT_ASSERT( NULL != iConnCheck->ValidatedPair() );
    iConnCheck->iValidatedPair = NULL;
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_ConnCheckCompletedLL(  )
    {
    EUNIT_ASSERT( ETrue );  // Tested with ..PerformConnCheckL..
    }

void UT_CIceConnectivityCheck::UT_CICEConnectivityCheck_StreamIdL()
    {
    EUNIT_ASSERT( KTestStreamIdRtp1 == iConnCheck->StreamId() );
    }

void UT_CIceConnectivityCheck::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }
    
void UT_CIceConnectivityCheck::IcmpError( TUint /*aSessionId*/, 
        TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    
    }
    
    
void UT_CIceConnectivityCheck::CheckCompletedL( 
    TInt aCompletionCode,
    const CIceConnectivityCheck& /*aCheck*/,
    CNATFWCandidatePair* aValidatedPair )
    {
    iCheckResult = aCompletionCode;
    iValidatedPair = aValidatedPair;
    }
    

void UT_CIceConnectivityCheck::NominationCompletedL( 
    TInt aCompletionCode,
    const CIceConnectivityCheck& /*aCheck*/,
    const CNATFWCandidatePair& aValidatedPair )
    {
    iCheckResult = aCompletionCode;
    iValidatedPair = CNATFWCandidatePair::NewL( aValidatedPair );
    }
    
void UT_CIceConnectivityCheck::StreamClosed( TUint /*aStreamId*/ )
    {
    
    }

void UT_CIceConnectivityCheck::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, 
        TUint /*aStreamId*/, TNatPluginEvent aEventCode, 
        TInt /*aErrorCode*/, TAny* aEventData, TUint /*aStreamConnectionId*/,
        TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ELocalCandidateFound:
            {
            CNATFWCandidate* candidate =
                reinterpret_cast<CNATFWCandidate*>( aEventData );
            delete candidate;
            }
            break;
        case MIceNatPluginEventObs::EFetchingCompleted:
        case MIceNatPluginEventObs::ESendingActivated:
        case MIceNatPluginEventObs::ESendingDeactivated:
        case MIceNatPluginEventObs::ECandidatePairFound:
        case MIceNatPluginEventObs::EConnChecksCompleted:
        case MIceNatPluginEventObs::EGeneralError:
            EUNIT_ASSERT( EFalse );
            break;
        default:
            EUNIT_ASSERT( EFalse );
        }
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceConnectivityCheck,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ComparePriorities - test ",
    "CIceConnectivityCheck",
    "ComparePriorities",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_ComparePrioritiesL, Teardown)

EUNIT_TEST(
    "MatchAddresses - test ",
    "CIceConnectivityCheck",
    "MatchAddresses",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_MatchAddressesL, Teardown)


EUNIT_TEST(
    "CreateValidatedPairL - test ",
    "CIceConnectivityCheck",
    "CreateValidatedPairL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_CreateValidatedPairLL, Teardown)


EUNIT_TEST(
    "PerformConnCheckL - PERIODIC ",
    "CIceConnectivityCheck",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_PerformConnCheckLL, Teardown)

EUNIT_TEST(
    "PerformConnCheckL - TRIGGERED ",
    "CIceConnectivityCheck",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_PerformConnCheckL_TL, Teardown)
    
EUNIT_TEST(
    "PerformConnCheckL - peer reflexive ",
    "CIceConnectivityCheck",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_PerformConnCheckL2L, Teardown)
    
EUNIT_TEST(
    "PerformConnCheckL - failure cases ",
    "CIceConnectivityCheck",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_PerformConnCheckL3L, Teardown)

EUNIT_TEST(
    "PerformConnCheckL - relay",
    "CIceConnectivityCheck",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_PerformConnCheckL4L, Teardown)
    
EUNIT_TEST(
    "Foundation - test ",
    "CIceConnectivityCheck",
    "Foundation",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_FoundationL, Teardown)

EUNIT_TEST(
    "ComponentId - test ",
    "CIceConnectivityCheck",
    "ComponentId",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_ComponentIdL, Teardown)

EUNIT_TEST(
    "State - test ",
    "CIceConnectivityCheck",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_StateL, Teardown)

EUNIT_TEST(
    "InitializeCheckL - test ",
    "CIceConnectivityCheck",
    "InitializeCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_InitializeCheckL, Teardown)

EUNIT_TEST(
    "ValidatedPairL - test ",
    "CIceConnectivityCheck",
    "ValidatedPairL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_ValidatedPairLL, Teardown)

EUNIT_TEST(
    "ConnCheckCompletedL - test ",
    "CIceConnectivityCheck",
    "ConnCheckCompletedL",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_ConnCheckCompletedLL, Teardown)

EUNIT_TEST(
    "StreamIdL - test ",
    "CIceConnectivityCheck",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CICEConnectivityCheck_StreamIdL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
