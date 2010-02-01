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
#include "UT_CIceCheckHandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natfwconnectivityapidefs.h"

//  INTERNAL INCLUDES
#include "cicecheckhandler.h"
#include "cicechecklist.h"
#include "cicesessiondata.h"
#include "ciceconnectionhandler.h"
#include "cicenatplugincontainer.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceCheckHandler* UT_CIceCheckHandler::NewL()
    {
    UT_CIceCheckHandler* self = UT_CIceCheckHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceCheckHandler* UT_CIceCheckHandler::NewLC()
    {
    UT_CIceCheckHandler* self = new( ELeave ) UT_CIceCheckHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceCheckHandler::~UT_CIceCheckHandler()
    {
    delete iLocalCandRtpHost;
    delete iLocalCandRtpRflx;
    delete iRemoteCandRtpHost;
    delete iRemoteCandRtpRflx;
    delete iInboundIdent;
    delete iOutboundIdent;
    
    delete iCheckHandler;
    delete iCandStorage;
    delete iConnHandler;
    delete iMultiplexer;
    delete iPluginContainer;
    }

// Default constructor
UT_CIceCheckHandler::UT_CIceCheckHandler()
    {
    }

// Second phase construct
void UT_CIceCheckHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    iLocalCandRtpHost = CNATFWCandidate::NewL();
    iLocalCandRtpHost->SetType( CNATFWCandidate::EHost );
    iLocalCandRtpHost->SetStreamCollectionId( KTestStreamCollectionId1 );
    iLocalCandRtpHost->SetStreamId( KTestStreamIdRtp1 );
    TInetAddr privateAddr = INET_ADDR( 10, 0, 1, 1 );
    privateAddr.SetPort( 8998 );
    privateAddr.SetFamily( KAfInet );
    iLocalCandRtpHost->SetTransportAddrL( privateAddr );
    iLocalCandRtpHost->SetBase( privateAddr );
    iLocalCandRtpHost->SetFoundationL( _L8("1") );
    iLocalCandRtpHost->SetComponentId( 1 ); // RTP
    iLocalCandRtpHost->SetTransportProtocol( KProtocolInetUdp );
    iLocalCandRtpHost->SetPriority( 2130706178 );
    
    iLocalCandRtpRflx = CNATFWCandidate::NewL( *iLocalCandRtpHost );
    iLocalCandRtpRflx->SetType( CNATFWCandidate::EServerReflexive );
    TInetAddr publicAddr = INET_ADDR( 192, 0, 2, 3 );
    publicAddr.SetPort( 45664 );
    publicAddr.SetFamily( KAfInet );
    iLocalCandRtpRflx->SetTransportAddrL( publicAddr );
    iLocalCandRtpRflx->SetBase( privateAddr );
    iLocalCandRtpRflx->SetFoundationL( _L8("2") );
    iLocalCandRtpRflx->SetPriority( 1694498562 );
    
    iRemoteCandRtpHost = CNATFWCandidate::NewL( *iLocalCandRtpHost );
    TInetAddr remotePrivateAddr = INET_ADDR( 16, 0, 0, 1 );
    remotePrivateAddr.SetPort( 3856 );
    remotePrivateAddr.SetFamily( KAfInet );
    iRemoteCandRtpHost->SetTransportAddrL( remotePrivateAddr );
    iRemoteCandRtpHost->SetFoundationL( _L8("10") );
    
    iRemoteCandRtpRflx = CNATFWCandidate::NewL( *iLocalCandRtpRflx );
    TInetAddr remotePublicAddr = INET_ADDR( 168, 0, 0, 1 );
    remotePublicAddr.SetPort( 62001 );
    remotePublicAddr.SetFamily( KAfInet );
    iRemoteCandRtpRflx->SetTransportAddrL( remotePublicAddr );
    iRemoteCandRtpRflx->SetFoundationL( _L8("11") );
    
    iInboundIdent = CNATFWCredentials::NewL();
    iInboundIdent->SetStreamCollectionId( KTestStreamCollectionId1 );
    iInboundIdent->SetStreamId( KTestStreamIdRtp1 );
    iInboundIdent->SetDirection( CNATFWCredentials::EInbound );
    iInboundIdent->SetUsernameL( _L8("alice") );
    iInboundIdent->SetPasswordL( _L8("pass") );
    
    iOutboundIdent = CNATFWCredentials::NewL();
    iOutboundIdent->SetStreamCollectionId( KTestStreamCollectionId1 );
    iOutboundIdent->SetStreamId( KTestStreamIdRtp1 );
    iOutboundIdent->SetDirection( CNATFWCredentials::EOutbound );
    iOutboundIdent->SetUsernameL( _L8("bob") );
    iOutboundIdent->SetPasswordL( _L8("ssap") );
    
    }

//  METHODS

void UT_CIceCheckHandler::SetupControllingL(  )
    {
	iCandidatePairFound = EFalse;
	iErrorWithStream = KErrNone;
	iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
	iCandStorage->SetTaTimerValue( KTaTimerValueForGathering );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtpHost ) );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtpRflx ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtpHost ) );
    iCandStorage->AddRemoteCandidateL(  
        CNATFWCandidate::NewL( *iRemoteCandRtpRflx ) );

    iCandStorage->AddCredentialsL( *iInboundIdent );
    iCandStorage->AddCredentialsL( *iOutboundIdent );
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    iCandStorage->SetRole( EIceRoleControlling );
    
	iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    
	iConnHandler = CIceConnectionHandler::NewL( 
		*iMultiplexer, *this, *iCandStorage, *iPluginContainer, *this );

	iCheckHandler 
		= CIceCheckHandler::NewL( *iCandStorage, *iMultiplexer, *iConnHandler, *this );
    }

void UT_CIceCheckHandler::SetupControlledL(  )
    {
	iCandidatePairFound = EFalse;
	iErrorWithStream = KErrNone;
	iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    iCandStorage->SetTaTimerValue( KTaTimerValueForGathering );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtpHost ) );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtpRflx ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtpHost ) );
    iCandStorage->AddRemoteCandidateL(  
        CNATFWCandidate::NewL( *iRemoteCandRtpRflx ) );

    iCandStorage->AddCredentialsL( *iInboundIdent );
    iCandStorage->AddCredentialsL( *iOutboundIdent );
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    iCandStorage->SetRole( EIceRoleControlled );
    
	iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    
	iConnHandler = CIceConnectionHandler::NewL( 
		*iMultiplexer, *this, *iCandStorage, *iPluginContainer, *this );

	iCheckHandler 
		= CIceCheckHandler::NewL( *iCandStorage, *iMultiplexer, *iConnHandler, *this );
    }
    
void UT_CIceCheckHandler::Teardown(  )
    {
    delete iCheckHandler;
    iCheckHandler = NULL;
    
    delete iCandStorage;
    iCandStorage = NULL;
    
    delete iConnHandler;
    iConnHandler = NULL;
    
    delete iMultiplexer;
    iMultiplexer = NULL;
    
    delete iPluginContainer;
    iPluginContainer = NULL;
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_SetIdentificationLL(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->SetCredentialsL( *iInboundIdent );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iCheckHandler->SetCredentialsL( *iInboundIdent ) );
        }
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_PerformConnectivityChecksLL()
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->PerformConnectivityChecksL();
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iCheckHandler->PerformConnectivityChecksL() );
        
        TBuf8<10> foundation;
        foundation.Append( iLocalCandRtpHost->Foundation() );
        foundation.Append( iRemoteCandRtpHost->Foundation() );
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckHandler->ComponentsHaveValidPairsL(
                *iCheckHandler->iCheckLists[0], foundation ) );
        
        CNATFWCandidatePair* pair = CNATFWCandidatePair::NewL(
            *iLocalCandRtpHost, *iRemoteCandRtpHost );
        RPointerArray<CNATFWCandidatePair> selectedPairs;
        CleanupResetAndDestroyPushL( selectedPairs );
        selectedPairs.AppendL( pair );
        iCheckHandler->iCheckLists[0]->iState 
            = CIceCheckList::EIceCheckListCompleted;
        iCheckHandler->ChecklistCompletedL( 
            *iCheckHandler->iCheckLists[0], selectedPairs );
        selectedPairs.Close();
        CleanupStack::Pop( &selectedPairs );
        }
    }

// UPDATE SCENARIO 1: Controlling peer selects candidate pairs to use.
void UT_CIceCheckHandler::UT_CICECheckHandler_UpdateIceProcessing1LL()
    {
    iCheckHandler->PerformConnectivityChecksL();
    
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    // test with null array
    EUNIT_ASSERT_LEAVE( 
        iCheckHandler->UpdateIceProcessingL( selectedPairs ) );
    
    // construct peer selected candidate pairs
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtpHost, *iRemoteCandRtpHost );
    selectedPairs.AppendL( validPairRtp );
    CleanupStack::Pop( validPairRtp );
    
    // test with invalid collection identifier
    TUint validCollectionId = validPairRtp->LocalCandidate().StreamCollectionId();
    const_cast<CNATFWCandidate&>( validPairRtp->LocalCandidate() ).
        SetStreamCollectionId( validCollectionId + 5 );
    EUNIT_ASSERT_LEAVE( 
        iCheckHandler->UpdateIceProcessingL( selectedPairs ) );
    const_cast<CNATFWCandidate&>( validPairRtp->LocalCandidate() ).
        SetStreamCollectionId( validCollectionId );
    
    // test with valid input
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->UpdateIceProcessingL( selectedPairs );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iCheckHandler->UpdateIceProcessingL( selectedPairs ) );
        }
    
    CleanupStack::PopAndDestroy( &selectedPairs );
    }

// UPDATE SCENARIO 2: Controlling remote peer signals new remote candidates.
void UT_CIceCheckHandler::UT_CICECheckHandler_UpdateIceProcessing2LL()
    {
    iCheckHandler->PerformConnectivityChecksL();
    
    RPointerArray<CNATFWCandidate> remoteCands;
    CleanupResetAndDestroyPushL( remoteCands );

    // test with null array
    EUNIT_ASSERT_LEAVE( 
        iCheckHandler->UpdateIceProcessingL( remoteCands ) );

    // construct updated remote candidate array
    CNATFWCandidate* copyRemoteCandRtpHost
        = CNATFWCandidate::NewLC( *iRemoteCandRtpHost );
    remoteCands.AppendL( copyRemoteCandRtpHost );
    CleanupStack::Pop( copyRemoteCandRtpHost );
    
    CNATFWCandidate* copyRemoteCandRtpRflx
        = CNATFWCandidate::NewLC( *iRemoteCandRtpRflx );
    remoteCands.AppendL( copyRemoteCandRtpRflx );
    CleanupStack::Pop( copyRemoteCandRtpRflx );
    
    // new remote candidate
    CNATFWCandidate* remoteCandRtpRelay
        = CNATFWCandidate::NewLC( *iRemoteCandRtpHost );
    remoteCandRtpRelay->SetType( CNATFWCandidate::ERelay );
    TInetAddr newRtpAddr( iRemoteCandRtpHost->TransportAddr() );
    newRtpAddr.SetPort( newRtpAddr.Port() + 20 );
    remoteCandRtpRelay->SetTransportAddrL( newRtpAddr );
    remoteCandRtpRelay->SetPriority( 
        iRemoteCandRtpHost->Priority() + 1 );
    remoteCands.AppendL( remoteCandRtpRelay );
    CleanupStack::Pop( remoteCandRtpRelay );
    
    // test with valid input
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->UpdateIceProcessingL( remoteCands );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE(
            iCheckHandler->UpdateIceProcessingL( remoteCands ) );
        }
    
    CleanupStack::PopAndDestroy( &remoteCands );
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_CleanupCollectionDataL()
    {
    iCheckHandler->CleanupCollectionData( KTestStreamCollectionId1 );
    
    UT_CICECheckHandler_PerformConnectivityChecksLL();
    
    // invalid collection
    iCheckHandler->CleanupCollectionData( KTestStreamCollectionId2 );
    EUNIT_ASSERT( iCheckHandler->iCheckLists.Count() != 0 );
    
    // valid collection
    iCheckHandler->CleanupCollectionData( KTestStreamCollectionId1 );
    EUNIT_ASSERT( iCheckHandler->iCheckLists.Count() == 0 );
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_STUNRequestReceivedL_1L(  )
    {
    UT_CICECheckHandler_PerformConnectivityChecksLL();
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->STUNRequestReceivedL(
            iLocalCandRtpHost->TransportAddr(),
            iRemoteCandRtpHost->TransportAddr(),
            iRemoteCandRtpHost->TransportAddr(),
            100, ETrue );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckHandler->STUNRequestReceivedL(
                iLocalCandRtpHost->TransportAddr(),
                iRemoteCandRtpHost->TransportAddr(),
                iRemoteCandRtpHost->TransportAddr(),
                100, ETrue ) );
        }
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_STUNRequestReceivedL_2L(  )
    {
    // Must buffer triggered checks
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckHandler->STUNRequestReceivedL(
            iLocalCandRtpHost->TransportAddr(),
            iRemoteCandRtpHost->TransportAddr(),
            iRemoteCandRtpHost->TransportAddr(),
            100, ETrue );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckHandler->STUNRequestReceivedL(
                iLocalCandRtpHost->TransportAddr(),
                iRemoteCandRtpHost->TransportAddr(),
                iRemoteCandRtpHost->TransportAddr(),
                100, ETrue ) );
        }
    
    UT_CICECheckHandler_PerformConnectivityChecksLL();
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_ChecklistCompletedL(  )
    {
    iCheckHandler->PerformConnectivityChecksL();
    
// SUCCESS COMPLETION CASES
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    EUNIT_ASSERT_LEAVE( iCheckHandler->ChecklistCompletedL( 
        *iCheckHandler->iCheckLists[0], selectedPairs ) );
    
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC(
        *iLocalCandRtpHost, *iRemoteCandRtpHost );
    selectedPairs.AppendL( pair );
    CleanupStack::Pop( pair );
    
    iCheckHandler->ChecklistCompletedL( 
        *iCheckHandler->iCheckLists[0], selectedPairs );
    EUNIT_ASSERT( CIceCheckHandler::EIceCompleted != iCheckHandler->iState );
    selectedPairs.Reset();
    
    pair = CNATFWCandidatePair::NewLC(
        *iLocalCandRtpHost, *iRemoteCandRtpHost );
    selectedPairs.AppendL( pair );
    CleanupStack::Pop( pair );
    iCheckHandler->iCheckLists[0]->iState 
        = CIceCheckList::EIceCheckListCompleted;
    iCheckHandler->ChecklistCompletedL( 
        *iCheckHandler->iCheckLists[0], selectedPairs );
    EUNIT_ASSERT( CIceCheckHandler::EIceCompleted == iCheckHandler->iState );
    selectedPairs.Reset();
    
// FAILED COMPLETION CASES
    RArray<TUint> components;
    CleanupClosePushL( components );
    
    EUNIT_ASSERT_LEAVE( iCheckHandler->ChecklistCompletedL( 
        *iCheckHandler->iCheckLists[0], components, KErrCouldNotConnect ) );
    
    components.AppendL( KTestStreamIdRtp1 );
    iCheckHandler->iCheckLists[0]->iState 
        = CIceCheckList::EIceCheckListFailed;
    
    iCheckHandler->ChecklistCompletedL( 
        *iCheckHandler->iCheckLists[0], components, KErrCouldNotConnect );
    EUNIT_ASSERT( CIceCheckHandler::EIceCompleted == iCheckHandler->iState );
    
    CleanupStack::PopAndDestroy( &components );
    selectedPairs.Close();
    CleanupStack::Pop( &selectedPairs );
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_ComponentsHaveValidPairsLL(  )
    {
    // Tested with UT_CICECheckHandler_PerformConnectivityChecksLL
    EUNIT_ASSERT( ETrue );
    }


void UT_CIceCheckHandler::UT_CICECheckHandler_HandleNewCollectionsLL()
    {
    iCheckHandler->PerformConnectivityChecksL();
    
    // Setup new media stream with host & reflexive addresses
    
// LOCAL HOST CANDIDATE RTP
    CNATFWCandidate* localCand1 = CNATFWCandidate::NewLC( *iLocalCandRtpHost );
    localCand1->SetStreamCollectionId( KTestStreamCollectionId2 );
    localCand1->SetStreamId( KTestStreamIdRtp2 );
    TInetAddr privateAddr = localCand1->TransportAddr();
    privateAddr.SetPort( 8900 );
    localCand1->SetTransportAddrL( privateAddr );
    localCand1->SetBase( privateAddr );
    CleanupStack::Pop( localCand1 );
    iCandStorage->AddLocalCandidateL( localCand1 );
    
// LOCAL SERVER REFLEXIVE CANDIDATE RTP
    CNATFWCandidate* localCand2 = CNATFWCandidate::NewLC( *iLocalCandRtpRflx );
    localCand2->SetStreamCollectionId( KTestStreamCollectionId2 );
    localCand2->SetStreamId( KTestStreamIdRtp2 );
    TInetAddr publicAddr = localCand2->TransportAddr();
    publicAddr.SetPort( 45666 );
    localCand2->SetTransportAddrL( publicAddr  );
    localCand2->SetBase( publicAddr );
    CleanupStack::Pop( localCand2 );
    iCandStorage->AddLocalCandidateL( localCand2 );
    
// REMOTE HOST CANDIDATE RTP
    CNATFWCandidate* remoteCand1 = CNATFWCandidate::NewLC( *iRemoteCandRtpHost );
    remoteCand1->SetStreamCollectionId( KTestStreamCollectionId2 );
    remoteCand1->SetStreamId( KTestStreamIdRtp2 );
    TInetAddr remotePrivateAddr = remoteCand1->TransportAddr();
    remotePrivateAddr.SetPort( 3858 );
    remoteCand1->SetTransportAddrL( remotePrivateAddr );
    remoteCand1->SetBase( remotePrivateAddr );
    CleanupStack::Pop( remoteCand1 );
    iCandStorage->AddRemoteCandidateL( remoteCand1 );
    
// REMOTE SERVER REFLEXIVE CANDIDATE RTP
    CNATFWCandidate* remoteCand2 = CNATFWCandidate::NewLC( *iRemoteCandRtpRflx );
    remoteCand2->SetStreamCollectionId( KTestStreamCollectionId2 );
    remoteCand2->SetStreamId( KTestStreamIdRtp2 );
    TInetAddr remotePublicAddr = remoteCand2->TransportAddr();
    remotePublicAddr.SetPort( 62003 );
    remoteCand2->SetTransportAddrL( remotePublicAddr );
    remoteCand2->SetBase( remotePrivateAddr );
    CleanupStack::Pop( remoteCand2 );
    iCandStorage->AddRemoteCandidateL( remoteCand2 );
    
    iCandStorage->CreateCollectionL( KTestStreamCollectionId2 );
    
    // Mediastream level pass & username
    CNATFWCredentials* inboundId = CNATFWCredentials::NewLC();
    inboundId->SetStreamCollectionId( KTestStreamCollectionId2 );
    inboundId->SetStreamId( KTestStreamIdRtp2 );
    inboundId->SetDirection( CNATFWCredentials::EInbound );
    inboundId->SetUsernameL( _L8("alice2") );
    inboundId->SetPasswordL( _L8("pass2") );
    
    CNATFWCredentials* outboundId = CNATFWCredentials::NewLC();
    outboundId->SetStreamCollectionId( KTestStreamCollectionId2 );
    outboundId->SetStreamId( KTestStreamIdRtp2 );
    outboundId->SetDirection( CNATFWCredentials::EOutbound );
    outboundId->SetUsernameL( _L8("bob2") );
    outboundId->SetPasswordL( _L8("ssap2") );
    iCandStorage->AddCredentialsL( *inboundId );
    iCandStorage->AddCredentialsL( *outboundId );
    CleanupStack::PopAndDestroy( outboundId );
    CleanupStack::PopAndDestroy( inboundId );
    
    iCheckHandler->PerformConnectivityChecksL();
    }

void UT_CIceCheckHandler::UT_CICECheckHandler_HandleIceRestartsLL()
    {
    UT_CICECheckHandler_PerformConnectivityChecksLL();
    
    CNATFWCredentials* inboundId1 = CNATFWCredentials::NewLC();
    inboundId1->SetStreamCollectionId( KTestStreamCollectionId1 );
    inboundId1->SetStreamId( KTestStreamIdRtp1 );
    inboundId1->SetDirection( CNATFWCredentials::EInbound );
    inboundId1->SetUsernameL( _L8("alice2") );
    inboundId1->SetPasswordL( _L8("pass2") );
    
    CNATFWCredentials* outboundId1 = CNATFWCredentials::NewLC();
    outboundId1->SetStreamCollectionId( KTestStreamCollectionId1 );
    outboundId1->SetStreamId( KTestStreamIdRtp1 );
    outboundId1->SetDirection( CNATFWCredentials::EOutbound );
    outboundId1->SetUsernameL( _L8("bob2") );
    outboundId1->SetPasswordL( _L8("ssap2") );
    
    iCandStorage->AddCredentialsL( *inboundId1 );
    iCandStorage->AddCredentialsL( *outboundId1 );
    CleanupStack::PopAndDestroy( outboundId1 );
    CleanupStack::PopAndDestroy( inboundId1 );
    
    iCheckHandler->iCheckLists[0]->iState 
        = CIceCheckList::EIceCheckListCompleted;
    iCheckHandler->PerformConnectivityChecksL();
    }
 
void UT_CIceCheckHandler::UT_CICECheckHandler_RoleChangeNeededL()
    {
    UT_CICECheckHandler_PerformConnectivityChecksLL();
    
    iCheckHandler->RoleChangeNeeded( EIceRoleControlling );
    EUNIT_ASSERT( 
        iCheckHandler->iSessionData.Role() == EIceRoleControlling );
    
    iCheckHandler->RoleChangeNeeded( EIceRoleControlled );
    EUNIT_ASSERT( 
        iCheckHandler->iSessionData.Role() == EIceRoleControlled );
    }

void UT_CIceCheckHandler::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }
    
void UT_CIceCheckHandler::IcmpError( TUint /*aSessionId*/, 
        TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    
    }

void UT_CIceCheckHandler::StreamClosed( TUint /*aStreamId*/ )
    {
    
    }

void UT_CIceCheckHandler::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, 
        TUint /*aStreamId*/, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint /*aStreamConnectionId*/,
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
            {
            CNATFWCandidatePair* pair =
                reinterpret_cast<CNATFWCandidatePair*>( aEventData );
            delete pair;
            iCandidatePairFound = ETrue;
            }
            break;
        case MIceNatPluginEventObs::EConnChecksCompleted:
            break;
        case MIceNatPluginEventObs::EGeneralError:        
            iErrorWithStream = aErrorCode;
            break;
        
        default:
            EUNIT_ASSERT( EFalse );
        }
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceCheckHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetIdentification - test ",
    "CIceCheckHandler",
    "SetIdentification",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_SetIdentificationLL, Teardown)

EUNIT_TEST(
    "PerformConnectivityChecksL - test ",
    "CIceCheckHandler",
    "PerformConnectivityChecksL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_PerformConnectivityChecksLL, Teardown)

EUNIT_TEST(
    "UpdateIceProcessing1L - test ",
    "CIceCheckHandler",
    "UpdateIceProcessing1L",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckHandler_UpdateIceProcessing1LL, Teardown)
    
EUNIT_TEST(
    "UpdateIceProcessing2L- test ",
    "CIceCheckHandler",
    "UpdateIceProcessing2L",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckHandler_UpdateIceProcessing2LL, Teardown)

EUNIT_TEST(
    "CleanupCollectionData - test ",
    "CIceCheckHandler",
    "CleanupCollectionData",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_CleanupCollectionDataL, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - PERIODIC FIRST ",
    "CIceCheckHandler",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_STUNRequestReceivedL_1L, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - TRIGGERED FIRST ",
    "CIceCheckHandler",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_STUNRequestReceivedL_2L, Teardown)

EUNIT_TEST(
    "ChecklistCompleted - test ",
    "CIceCheckHandler",
    "ChecklistCompleted",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_ChecklistCompletedL, Teardown)

EUNIT_TEST(
    "CheckForLastComponentSucceeded - test ",
    "CIceCheckHandler",
    "CheckForLastComponentSucceeded",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_ComponentsHaveValidPairsLL, Teardown)

EUNIT_TEST(
    "HandleNewCollectionsL - test ",
    "CIceCheckHandler",
    "HandleNewCollectionsL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_HandleNewCollectionsLL, Teardown)

EUNIT_TEST(
    "HandleIceRestartsL - test ",
    "CIceCheckHandler",
    "HandleIceRestartsL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_HandleIceRestartsLL, Teardown)

EUNIT_TEST(
    "RoleChangeNeededL - test ",
    "CIceCheckHandler",
    "RoleChangeNeededL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckHandler_RoleChangeNeededL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
