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
#include "UT_CIceCheckList.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "cicechecklist.h"
#include "cicesessiondata.h"
#include "ciceconnectionhandler.h"
#include "ciceconnectivitycheck.h"
#include "cicecheckprioritizer.h"
#include "cicenatplugincontainer.h"
#include "cicevalidlist.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceCheckList* UT_CIceCheckList::NewL()
    {
    UT_CIceCheckList* self = UT_CIceCheckList::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceCheckList* UT_CIceCheckList::NewLC()
    {
    UT_CIceCheckList* self = new( ELeave ) UT_CIceCheckList();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceCheckList::~UT_CIceCheckList()
    {
    delete iCheckPrioritizer;
    delete iCheckList;
	delete iCandStorage;
    delete iConnHandler;
    delete iMultiplexer;
    delete iLocalCandRtp;
    delete iLocalCandRtcp;
    delete iRemoteCandRtp;
    delete iRemoteCandRtcp;
    iSelectedPairs.ResetAndDestroy();
    delete iInboundIdentRtp;
    delete iOutboundIdentRtp;
    delete iInboundIdentRtcp;
    delete iOutboundIdentRtcp;
    delete iPluginContainer;
    }

// Default constructor
UT_CIceCheckList::UT_CIceCheckList()
    {
    }

// Second phase construct
void UT_CIceCheckList::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    iCheckPrioritizer = CIceCheckPrioritizer::NewL();
    
    // LOCAL RTP
    iLocalCandRtp = CNATFWCandidate::NewL();
    iLocalCandRtp->SetType( CNATFWCandidate::EHost );
    iLocalCandRtp->SetStreamCollectionId( KTestStreamCollectionId1 );
    iLocalCandRtp->SetStreamId( KTestStreamIdRtp1 );
    TInetAddr privateAddr( KLocalAddr );
    privateAddr.SetPort( KLocalPort );
    privateAddr.SetFamily( KAfInet );
    iLocalCandRtp->SetTransportAddrL( privateAddr );
    iLocalCandRtp->SetBase( privateAddr );
    iLocalCandRtp->SetFoundationL( _L8("1") );
    iLocalCandRtp->SetComponentId( 1 ); // RTP
    iLocalCandRtp->SetTransportProtocol( KProtocolInetUdp );
    iLocalCandRtp->SetPriority( 2130706178 );
    
    // LOCAL RTCP
    iLocalCandRtcp = CNATFWCandidate::NewL( *iLocalCandRtp );
    iLocalCandRtcp->SetStreamId( KTestStreamIdRtcp1 );
    privateAddr.SetPort( KLocalPort + 1 );
    iLocalCandRtcp->SetTransportAddrL( privateAddr );
    iLocalCandRtcp->SetBase( privateAddr );
    iLocalCandRtcp->SetComponentId( 2 );
    iLocalCandRtcp->SetPriority( 2130706178 - 1 );
    
    // REMOTE RTP
    iRemoteCandRtp = CNATFWCandidate::NewL( *iLocalCandRtp );
    TInetAddr remotePrivateAddr( KRemoteAddr );
    remotePrivateAddr.SetPort( KRemotePort );
    remotePrivateAddr.SetFamily( KAfInet );
    iRemoteCandRtp->SetTransportAddrL( remotePrivateAddr );
    iRemoteCandRtp->SetFoundationL( _L8("10") );
    iRemoteCandRtp->SetPriority( 2130706180 );
    
    // REMOTE RTCP
    iRemoteCandRtcp = CNATFWCandidate::NewL( *iRemoteCandRtp );
    remotePrivateAddr.SetPort( KRemotePort + 1 );
    iRemoteCandRtcp->SetTransportAddrL( remotePrivateAddr );
    iRemoteCandRtcp->SetStreamId( KTestStreamIdRtcp1 );
    iRemoteCandRtcp->SetComponentId( 2 );
    iRemoteCandRtcp->SetPriority( 2130706180 - 1 );
    
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
    
    iInboundIdentRtcp = CNATFWCredentials::NewL( *iInboundIdentRtp );
    iInboundIdentRtcp->SetStreamId( KTestStreamIdRtcp1 );
    iOutboundIdentRtcp = CNATFWCredentials::NewL( *iOutboundIdentRtp );
    iOutboundIdentRtcp->SetStreamId( KTestStreamIdRtcp1 );
    }

//  METHODS

void UT_CIceCheckList::SetupControllingL()
    {
    iCheckForLastCompSucceed = EFalse;
    iCheckListCompleted = EFalse;
    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtp ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtp ) );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtcp ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtcp ) );
    
    iCandStorage->AddCredentialsL( *iInboundIdentRtp );
    iCandStorage->AddCredentialsL( *iOutboundIdentRtp );
    iCandStorage->AddCredentialsL( *iInboundIdentRtcp );
    iCandStorage->AddCredentialsL( *iOutboundIdentRtcp );
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    iCandStorage->SetRole( EIceRoleControlling );
    
	iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    
	iConnHandler = CIceConnectionHandler::NewL( 
		*iMultiplexer, *this, *iCandStorage, *iPluginContainer, *this );
    
	iCheckList = CIceCheckList::NewL( 
	    *this, KTestStreamCollectionId1, *iCandStorage, *iConnHandler );
    }

void UT_CIceCheckList::SetupControlledL()
    {
    iCheckForLastCompSucceed = EFalse;
    iCheckListCompleted = EFalse;
    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtp ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtp ) );
    iCandStorage->AddLocalCandidateL( 
        CNATFWCandidate::NewL( *iLocalCandRtcp ) );
    iCandStorage->AddRemoteCandidateL( 
        CNATFWCandidate::NewL( *iRemoteCandRtcp ) );

    iCandStorage->AddCredentialsL( *iInboundIdentRtp );
    iCandStorage->AddCredentialsL( *iOutboundIdentRtp );
    iCandStorage->AddCredentialsL( *iInboundIdentRtcp );
    iCandStorage->AddCredentialsL( *iOutboundIdentRtcp );
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    iCandStorage->SetRole( EIceRoleControlled );

	iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    
	iConnHandler = CIceConnectionHandler::NewL( 
		*iMultiplexer, *this, *iCandStorage, *iPluginContainer, *this );
    
	iCheckList = CIceCheckList::NewL( 
	    *this, KTestStreamCollectionId1, *iCandStorage, *iConnHandler );
    }

void UT_CIceCheckList::Teardown(  )
    {
	delete iCheckList;
	iCheckList = NULL;
	delete iCandStorage;
	iCandStorage = NULL;
    delete iConnHandler;
    iConnHandler = NULL;
    delete iMultiplexer;
    iMultiplexer = NULL;
    iSelectedPairs.ResetAndDestroy();
    delete iPluginContainer;
    iPluginContainer = NULL;
    }

void UT_CIceCheckList::UT_CICECheckList_NewLL()
    {
    EUNIT_ASSERT( iCheckList->iChecks.Count() == 2 );
    }

void UT_CIceCheckList::UT_CICECheckList_TimerFiredL(  )
    {
    EUNIT_ASSERT( 1 == CIceCheckList::TimerFired( iCheckList ) );
    EUNIT_ASSERT_PANIC( CIceCheckList::TimerFired( NULL ), "USER", 0 );
    }

void UT_CIceCheckList::UT_CICECheckList_StateL(  )
    {
    EUNIT_ASSERT( !iCheckList->State() );
    }

void UT_CIceCheckList::UT_CICECheckList_StreamCollectionIdL(  )
    {
    EUNIT_ASSERT( 
        KTestStreamCollectionId1 == iCheckList->StreamCollectionId() );
    }

void UT_CIceCheckList::UT_CICECheckList_InitializeCheckListLL(  )
    {
    iCheckList->InitializeCheckListL();
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckWaiting 
        == iCheckList->iChecks[0]->State() );
    }

void UT_CIceCheckList::UT_CICECheckList_InitializeCheckListL_1L(  )
    {
    iCheckList->InitializeCheckListL( 
        iCheckList->iChecks[0]->CandidatePair().Foundation() );
    EUNIT_ASSERT( 
        CIceConnectivityCheck::EIceCheckWaiting 
        == iCheckList->iChecks[0]->State() );
    }


// RTP + RTCP, CONTROLLING CLIENT, PERIODIC CHECKS
void UT_CIceCheckList::UT_CICECheckList_StartPerformChecksLL()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    EUNIT_ASSERT_NO_LEAVE( iCheckList->StartPerformChecksL( interval ) );
    iCheckList->iTimer->Cancel();
    iCheckList->OnTimerFiredL();
    
    CNATFWCandidatePair* validPairRtcp = CNATFWCandidatePair::NewLC(
        *iLocalCandRtcp, *iRemoteCandRtcp );
    validPairRtcp->SetSelected( ETrue );
    
    CNATFWCandidatePair* validPairRtp = CNATFWCandidatePair::NewLC(
        *iLocalCandRtp, *iRemoteCandRtp );
    validPairRtp->SetSelected( ETrue );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RTP candidate completes succesfully
        iCheckList->iChecks[0]->iState 
            = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[0]->iCandidatePair->SetSelected( ETrue );
        iCheckList->iChecks[0]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtp );
        
        CleanupStack::Pop( validPairRtp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[0], validPairRtp );
        
        CleanupStack::PopAndDestroy( validPairRtcp );        
        }
    else
        {
        EUNIT_ASSERT_LEAVE( iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[0], NULL ) );
        
        // RTP candidate completes succesfully
        iCheckList->iChecks[0]->iState 
            = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[0]->iCandidatePair->SetSelected( ETrue );
        iCheckList->iChecks[0]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtp );
        
        CleanupStack::Pop( validPairRtp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[0], validPairRtp );
        
        // RTCP candidate of same type must be set to waiting state
        EUNIT_ASSERT( 
            CIceConnectivityCheck::EIceCheckWaiting
            == iCheckList->iChecks[1]->State() );
        
        // RTCP candidate completes succesfully
        iCheckList->OnTimerFiredL();
        iCheckList->iChecks[1]->iState 
            = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[1]->iCandidatePair->SetSelected( ETrue );
        iCheckList->iChecks[1]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtcp );
        
        CleanupStack::Pop( validPairRtcp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[1], validPairRtcp );
        EUNIT_ASSERT( iCheckForLastCompSucceed );
        EUNIT_ASSERT( iCheckListCompleted );
        }
    }


// RTP + RTCP, CONTROLLED CLIENT, PERIODIC CHECKS
void UT_CIceCheckList::UT_CICECheckList_StartPerformChecksL_2L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    EUNIT_ASSERT_NO_LEAVE( iCheckList->StartPerformChecksL( interval ) );
    iCheckList->iTimer->Cancel();
    iCheckList->OnTimerFiredL();
    
    CNATFWCandidatePair* validPairRtcp = CNATFWCandidatePair::NewLC(
        *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp = CNATFWCandidatePair::NewLC(
        *iLocalCandRtp, *iRemoteCandRtp );
    
    EUNIT_ASSERT_LEAVE( iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], NULL ) );

    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RTP candidate completes succesfully
        iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[0]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtp );
        
        CleanupStack::Pop( validPairRtp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[0], validPairRtp );
        CleanupStack::PopAndDestroy( validPairRtcp );
        }
    else
        {
        // RTP candidate completes succesfully
        iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[0]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtp );
        CleanupStack::Pop( validPairRtp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[0], validPairRtp );
        
        // RTCP candidate of same type must be set to waiting state
        EUNIT_ASSERT( 
            CIceConnectivityCheck::EIceCheckWaiting
            == iCheckList->iChecks[1]->State() );
        
        // RTCP candidate completes succesfully
        iCheckList->OnTimerFiredL();
        iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
        iCheckList->iChecks[1]->iValidatedPair 
            = CNATFWCandidatePair::NewL( *validPairRtcp );
        CleanupStack::Pop( validPairRtcp );
        iCheckList->CheckCompletedL( 
            KErrNone, *iCheckList->iChecks[1], validPairRtcp );
        EUNIT_ASSERT( iCheckForLastCompSucceed );
        }
    }


void UT_CIceCheckList::UT_CICECheckList_RestartCheckListLL(  )
    {
    UT_CICECheckList_StartPerformChecksLL();
    
    TTimeIntervalMicroSeconds32 interval( 20000 );
    iCheckList->iState = CIceCheckList::EIceCheckListRunning;
    EUNIT_ASSERT_LEAVE( iCheckList->RestartCheckListL( interval ) );
    
    iCheckList->iState = CIceCheckList::EIceCheckListCompleted;
    EUNIT_ASSERT_NO_LEAVE( iCheckList->RestartCheckListL( interval ) );
    }


// UPDATE SCENARIO 1: Controlling peer selects candidate pairs to use.
// Local client has NOT yet validated these pairs.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_1L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct peer selected candidate pairs
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    selectedPairs.AppendL( validPairRtp );
    CleanupStack::Pop( validPairRtp );
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    selectedPairs.AppendL( validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    
    // do updating
    TInt numOfChecks( iCheckList->iChecks.Count() );
    iCheckList->UpdateCheckListL( selectedPairs );
    EUNIT_ASSERT( numOfChecks == iCheckList->iChecks.Count() );
    EUNIT_ASSERT( selectedPairs.Count() 
        == iCheckList->iSelectedPairs.Count() );
    
// RTP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        CNATFWCandidatePair::NewL( *validPairRtp ) );
    
// RTCP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        CNATFWCandidatePair::NewL( *validPairRtcp ) );
    EUNIT_ASSERT( iCheckForLastCompSucceed );
    EUNIT_ASSERT( iCheckListCompleted );
    EUNIT_ASSERT( 0 == iCheckList->iSelectedPairs.Count() );
    
    CleanupStack::PopAndDestroy( &selectedPairs );
    }

// UPDATE SCENARIO 2: Controlling peer selects candidate pairs to use.
// Local client has validated these pairs.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_2L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct selected candidate pairs    
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    selectedPairs.AppendL( validPairRtp );
    CleanupStack::Pop( validPairRtp );
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    selectedPairs.AppendL( validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    
// RTP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        CNATFWCandidatePair::NewL( *validPairRtp ) );
    
// RTCP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        CNATFWCandidatePair::NewL( *validPairRtcp ) );
    EUNIT_ASSERT( iCheckForLastCompSucceed );
    
    // do updating
    TInt numOfChecks( iCheckList->iChecks.Count() );
    iCheckList->UpdateCheckListL( selectedPairs );
    EUNIT_ASSERT( numOfChecks == iCheckList->iChecks.Count() );
    EUNIT_ASSERT( 0 == iCheckList->iSelectedPairs.Count() );
    EUNIT_ASSERT( iCheckListCompleted );

    CleanupStack::PopAndDestroy( &selectedPairs );
    }

// UPDATE SCENARIO 3: Controlling peer selects candidate pairs to use.
// Local client has validated pairs partially.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_3L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct selected candidate pairs    
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    selectedPairs.AppendL( validPairRtp );
    CleanupStack::Pop( validPairRtp );
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    selectedPairs.AppendL( validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    
// RTP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        CNATFWCandidatePair::NewL( *validPairRtp ) );
    
    // do updating
    TInt numOfChecks( iCheckList->iChecks.Count() );
    iCheckList->UpdateCheckListL( selectedPairs );
    EUNIT_ASSERT( numOfChecks == iCheckList->iChecks.Count() );
    EUNIT_ASSERT( 2 == iCheckList->iSelectedPairs.Count() );
    EUNIT_ASSERT( !iCheckListCompleted );
    
// RTCP CANDIDATE COMPLETES SUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        CNATFWCandidatePair::NewL( *validPairRtcp ) );
    EUNIT_ASSERT( iCheckForLastCompSucceed );
    EUNIT_ASSERT( iCheckListCompleted );
    EUNIT_ASSERT( 0 == iCheckList->iSelectedPairs.Count() );
    
    CleanupStack::PopAndDestroy( &selectedPairs );
    }


// UPDATE SCENARIO 4: Controlling peer selects candidate pairs to use.
// None of pairs are in-progress and at least one is failed. 
// ==> fail check list.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_4L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct selected candidate pairs    
    RPointerArray<CNATFWCandidatePair> selectedPairs;
    CleanupResetAndDestroyPushL( selectedPairs );
    
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    selectedPairs.AppendL( validPairRtp );
    CleanupStack::Pop( validPairRtp );
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    selectedPairs.AppendL( validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    
// RTP CANDIDATE COMPLETES UNSUCCESFULLY
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckFailed;
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrUnknown, *iCheckList->iChecks[0], 
        CNATFWCandidatePair::NewL( *validPairRtp ) );
    
    // do updating
    TInt numOfChecks( iCheckList->iChecks.Count() );
    iCheckList->UpdateCheckListL( selectedPairs );
    EUNIT_ASSERT( numOfChecks == iCheckList->iChecks.Count() );
    EUNIT_ASSERT( 0 == iCheckList->iSelectedPairs.Count() );
    EUNIT_ASSERT( iCheckListCompleted );
    
    CleanupStack::PopAndDestroy( &selectedPairs );
    }


// UPDATE SCENARIO: Controlling remote peer signals new remote candidates
// while local client is still doing own checks.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_5L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct updated remote candidate array
    RPointerArray<CNATFWCandidate> remoteCands;
    CleanupResetAndDestroyPushL( remoteCands );
    CNATFWCandidate* copyRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    remoteCands.AppendL( copyRemoteCandRtp );
    CleanupStack::Pop( copyRemoteCandRtp );
    
    CNATFWCandidate* copyRemoteCandRtcp
        = CNATFWCandidate::NewLC( *iRemoteCandRtcp );
    remoteCands.AppendL( copyRemoteCandRtcp );
    CleanupStack::Pop( copyRemoteCandRtcp );
    
    // new remote candidate
    CNATFWCandidate* newRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    TInetAddr newRtpAddr( remoteCands[0]->TransportAddr() );
    newRtpAddr.SetPort( newRtpAddr.Port() + 20 );
    newRemoteCandRtp->SetTransportAddrL( newRtpAddr );
    newRemoteCandRtp->SetPriority( iRemoteCandRtp->Priority() + 1 );
    remoteCands.AppendL( newRemoteCandRtp );
    CleanupStack::Pop( newRemoteCandRtp );
    
    // update session data
    iCandStorage->AddRemoteCandidatesL( remoteCands );
    
    // no checks in succeeded state
    TInt numOfChecks( iCheckList->iChecks.Count() );
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckList->UpdateCheckListL( remoteCands );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iCheckList->UpdateCheckListL( remoteCands ) );
        EUNIT_ASSERT( numOfChecks != iCheckList->iChecks.Count() );
        }
    
    CleanupStack::PopAndDestroy( &remoteCands );
    }

// UPDATE SCENARIO: Controlling remote peer signals new remote candidates
// while local client is still doing own checks. RTP component has succeeded
// when session is updated.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_6L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct updated remote candidate array
    RPointerArray<CNATFWCandidate> remoteCands;
    CleanupResetAndDestroyPushL( remoteCands );
    CNATFWCandidate* copyRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    remoteCands.AppendL( copyRemoteCandRtp );
    CleanupStack::Pop( copyRemoteCandRtp );
    
    CNATFWCandidate* copyRemoteCandRtcp
        = CNATFWCandidate::NewLC( *iRemoteCandRtcp );
    remoteCands.AppendL( copyRemoteCandRtcp );
    CleanupStack::Pop( copyRemoteCandRtcp );
    
    // new remote candidate
    CNATFWCandidate* newRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    TInetAddr newRtpAddr( remoteCands[0]->TransportAddr() );
    newRtpAddr.SetPort( newRtpAddr.Port() + 20 );
    newRemoteCandRtp->SetTransportAddrL( newRtpAddr );
    newRemoteCandRtp->SetPriority( iRemoteCandRtp->Priority() + 1 );
    remoteCands.AppendL( newRemoteCandRtp );
    CleanupStack::Pop( newRemoteCandRtp );
    
    // update session data
    iCandStorage->AddRemoteCandidatesL( remoteCands );
    
    // RTP component has succeeded => unfreeze RTCP
    for ( TInt i( iCheckList->iChecks.Count() - 1 ); 0 <= i; --i )
        {
        if ( iCheckList->iChecks[i]->ComponentId() == KComponentIdRtp )
            {
            iCheckList->iChecks[i]->iState 
                = CIceConnectivityCheck::EIceCheckSucceed;
            }
        }
    
    TInt numOfChecks( iCheckList->iChecks.Count() );
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckList->UpdateCheckListL( remoteCands );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iCheckList->UpdateCheckListL( remoteCands ) );
        EUNIT_ASSERT( numOfChecks != iCheckList->iChecks.Count() );
        }
    
    CleanupStack::PopAndDestroy( &remoteCands );
    }


// UPDATE SCENARIO: Controlling remote peer signals new remote candidates
// while local client is still doing own checks. All components 
// have succeeded.
void UT_CIceCheckList::UT_CICECheckList_UpdateCheckListL_7L()
    {
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    // construct updated remote candidate array
    RPointerArray<CNATFWCandidate> remoteCands;
    CleanupResetAndDestroyPushL( remoteCands );
    CNATFWCandidate* copyRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    remoteCands.AppendL( copyRemoteCandRtp );
    CleanupStack::Pop( copyRemoteCandRtp );
    
    CNATFWCandidate* copyRemoteCandRtcp
        = CNATFWCandidate::NewLC( *iRemoteCandRtcp );
    remoteCands.AppendL( copyRemoteCandRtcp );
    CleanupStack::Pop( copyRemoteCandRtcp );
    
    // new remote candidate
    CNATFWCandidate* newRemoteCandRtp
        = CNATFWCandidate::NewLC( *iRemoteCandRtp );
    TInetAddr newRtpAddr( remoteCands[0]->TransportAddr() );
    newRtpAddr.SetPort( newRtpAddr.Port() + 20 );
    newRemoteCandRtp->SetTransportAddrL( newRtpAddr );
    newRemoteCandRtp->SetPriority( iRemoteCandRtp->Priority() + 1 );
    remoteCands.AppendL( newRemoteCandRtp );
    CleanupStack::Pop( newRemoteCandRtp );
    
    // update session data
    iCandStorage->AddRemoteCandidatesL( remoteCands );
    
    // All components are succeeded
    for ( TInt i( iCheckList->iChecks.Count() - 1 ); 0 <= i; --i )
        {
        iCheckList->iChecks[i]->iState 
            = CIceConnectivityCheck::EIceCheckSucceed;
        }
    
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    validPairRtp->SetPriority( 100 );
    iCheckList->iValidList->AddValidPairL( *validPairRtp );
    CleanupStack::PopAndDestroy( validPairRtp );
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    validPairRtp->SetPriority( 200 );
    iCheckList->iValidList->AddValidPairL( *validPairRtcp );
    CleanupStack::PopAndDestroy( validPairRtcp );
        
    TInt numOfChecks( iCheckList->iChecks.Count() );
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckList->UpdateCheckListL( remoteCands );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iCheckList->UpdateCheckListL( remoteCands ) );
        EUNIT_ASSERT( numOfChecks != iCheckList->iChecks.Count() );
        EUNIT_ASSERT( iCheckForLastCompSucceed );
        }
    
    CleanupStack::PopAndDestroy( &remoteCands );
    }


void UT_CIceCheckList::UT_CICECheckList_CheckCompletedLL(  )
    {
    CIceConnectivityCheck* check = NULL;
    EUNIT_ASSERT_LEAVE( 
        iCheckList->CheckCompletedL( KErrNone, *check, NULL ) );
    }

// RTP + RTCP, CONTROLLING CLIENT, TRIGGERED CHECKS
// TRIGGERED CHECKS AFTER PERIODIC CHECKS COMPLETED SUCCESSFULLY
void UT_CIceCheckList::UT_CICECheckList_STUNRequestReceivedLL(  )
    {
// MAKE PERIODIC CHECKS
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    
    // RTP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtp, EIceRoleControlling );
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    CleanupStack::Pop( validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], validPairRtp );
    
    // RTCP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtcp, EIceRoleControlling );
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], validPairRtcp );
    
// RESENT CHECKS COMPLETES
    
    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        *iCheckList->iChecks[0]->iValidatedPair );

    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        *iCheckList->iChecks[1]->iValidatedPair );
    
    EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
    EUNIT_ASSERT( iCheckListCompleted );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            100, EFalse );
        
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, EFalse );
        }
    else
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                100, EFalse ) );
        
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                100, EFalse ) );
        }
    }

// RTP + RTCP, CONTROLLED CLIENT
// TRIGGERED CHECKS AFTER PERIODIC CHECKS COMPLETED SUCCESSFULLY
void UT_CIceCheckList::UT_CICECheckList_STUNRequestReceivedL_1L(  )
    {
// MAKE PERIODIC CHECKS
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    
    // RTP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtp, EIceRoleControlled );
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    CleanupStack::Pop( validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], validPairRtp );
    
    // RTCP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtcp, EIceRoleControlled );
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], validPairRtcp );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            100, ETrue );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, ETrue );
        EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
        EUNIT_ASSERT( iCheckListCompleted );
        }
    else
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                100, ETrue ) );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        
        EUNIT_ASSERT_NO_LEAVE( 
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, ETrue ) );
        EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
        EUNIT_ASSERT( iCheckListCompleted );
        }
    }

// RTP + RTCP, CONTROLLED CLIENT
// TRIGGERED CHECK BEFORE PERIODIC CHECKS DONE
void UT_CIceCheckList::UT_CICECheckList_STUNRequestReceivedL_2L(  )
    {
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            100, ETrue );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        EUNIT_ASSERT( !iCheckListCompleted );
        }
    else
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                100, ETrue ) );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        EUNIT_ASSERT( !iCheckListCompleted );
        }
    
    // Triggered check for RTP candidate completes succesfully
    iCheckPrioritizer->PrioritizePair( *validPairRtp, EIceRoleControlled );
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[0]->iCandidatePair->SetSelected( ETrue );
    
    validPairRtp->SetSelected( ETrue );
    CleanupStack::Pop( validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], validPairRtp );
    EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, ETrue );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        EUNIT_ASSERT( !iCheckListCompleted );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, ETrue ) );
        EUNIT_ASSERT( iSelectedPairs.Count() == 0 );
        EUNIT_ASSERT( !iCheckListCompleted );
        }
    
    // Triggered check for RTCP candidate completes succesfully
    iCheckPrioritizer->PrioritizePair( *validPairRtcp, EIceRoleControlled );
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckList->iChecks[1]->iCandidatePair->SetSelected( ETrue );

    validPairRtcp->SetSelected( ETrue );
    CleanupStack::Pop( validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], validPairRtcp );
    EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
    EUNIT_ASSERT( iCheckListCompleted );
    }

// RTP + RTCP, CONTROLLING CLIENT. TRIGGERED CHECKS RECEIVED WHILE
// NOMINATION ONGOING
void UT_CIceCheckList::UT_CICECheckList_STUNRequestReceivedL3L(  )
    {
// MAKE PERIODIC CHECKS
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    
    // RTP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtp, EIceRoleControlling );
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    CleanupStack::Pop( validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], validPairRtp );
    
    // RTCP candidate completes succesfully
    iCheckList->OnTimerFiredL();
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtcp, EIceRoleControlling );
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], validPairRtcp );
    iCheckList->iTimer->Cancel();
    
// FIRST NOMINATION CHECK IS TRIGGERED
    iCheckList->OnTimerFiredL();

// SECOND NOMINATION CHECK IS TRIGGERED
    iCheckList->OnTimerFiredL();

    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            100, EFalse );
        
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, EFalse );
        }
    else
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                100, EFalse ) );
        
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                100, EFalse ) );
        }

// RESENT CHECKS COMPLETES
    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        *iCheckList->iChecks[0]->iValidatedPair );

    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        *iCheckList->iChecks[1]->iValidatedPair );
    
    EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
    EUNIT_ASSERT( iCheckListCompleted );
    }


// RTP + RTCP, CONTROLLING CLIENT. TRIGGERED CHECKS RECEIVED WHILE
// ORDINARY CHECK IS INPROGRESS
void UT_CIceCheckList::UT_CICECheckList_STUNRequestReceivedL4L(  )
    {
// MAKE ORDINARY CHECKS
    TTimeIntervalMicroSeconds32 interval( 20000 ); // 20ms
    iCheckList->StartPerformChecksL( interval );
    iCheckList->iTimer->Cancel();
    
    CNATFWCandidatePair* validPairRtcp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtcp, *iRemoteCandRtcp );
    CNATFWCandidatePair* validPairRtp 
        = CNATFWCandidatePair::NewLC( *iLocalCandRtp, *iRemoteCandRtp );
    
// FIRST ORDINARY CHECK IS TRIGGERED
    iCheckList->OnTimerFiredL();

// SECOND ORDINARY CHECK IS TRIGGERED
    iCheckList->OnTimerFiredL();

    if ( TIceTestUtils::AllocTestEnabled() )
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            iRemoteCandRtp->TransportAddr(),
            100, EFalse );
        
        iCheckList->STUNRequestReceivedL( 
            iLocalCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            iRemoteCandRtcp->TransportAddr(),
            100, EFalse );
        }
    else
        {
        // RECEIVE TRIGGERED CHECKS, NO RELAY USED
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                iRemoteCandRtp->TransportAddr(),
                100, EFalse ) );
        
        EUNIT_ASSERT_NO_LEAVE( 
            iCheckList->STUNRequestReceivedL( 
                iLocalCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                iRemoteCandRtcp->TransportAddr(),
                100, EFalse ) );
        }

// ORDINARY CHECK FOR RTP COMPLETES SUCCESFULLY
    iCheckList->iChecks[0]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtp, EIceRoleControlling );
    iCheckList->iChecks[0]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtp );
    CleanupStack::Pop( validPairRtp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[0], validPairRtp );
    
// ORDINARY CHECK FOR RTCP COMPLETES SUCCESFULLY
    iCheckList->iChecks[1]->iState = CIceConnectivityCheck::EIceCheckSucceed;
    iCheckPrioritizer->PrioritizePair( *validPairRtcp, EIceRoleControlling );
    iCheckList->iChecks[1]->iValidatedPair 
        = CNATFWCandidatePair::NewL( *validPairRtcp );
    CleanupStack::Pop( validPairRtcp );
    iCheckList->CheckCompletedL( 
        KErrNone, *iCheckList->iChecks[1], validPairRtcp );
    iCheckList->iTimer->Cancel();

// NOMINATION CHECKS ARE SCHEDULED

// RESENT CHECKS COMPLETES
    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[0], 
        *iCheckList->iChecks[0]->iValidatedPair );

    iCheckList->NominationCompletedL( 
        KErrNone, *iCheckList->iChecks[1], 
        *iCheckList->iChecks[1]->iValidatedPair );
    
    EUNIT_ASSERT( iSelectedPairs.Count() == 2 );
    EUNIT_ASSERT( iCheckListCompleted );
    }

void UT_CIceCheckList::UT_CICECheckList_RecomputePairPrioritiesL()
    {
    TInt64 priorityBeforeUpdate 
        = iCheckList->iChecks[0]->CandidatePair().Priority();
    EIceRoleControlled == iCandStorage->Role()
        ? iCandStorage->SetRole( EIceRoleControlling )
        : iCandStorage->SetRole( EIceRoleControlled );
    
    iCheckList->RecomputePairPriorities();
    
    TInt64 priorityAfterUpdate 
        = iCheckList->iChecks[0]->CandidatePair().Priority();
    EUNIT_ASSERT( 
        1 == Abs( priorityBeforeUpdate - priorityAfterUpdate ) );
    }

void UT_CIceCheckList::ChecklistCompletedL( 
    const CIceCheckList& /*aChecklist*/, 
    const RPointerArray<CNATFWCandidatePair>& aSelectedPairs )
    {
    iCheckListCompleted = ETrue;
    
    TInt count( aSelectedPairs.Count() );
    for ( TInt i( 0 ); i< count; ++i )
        {
        iSelectedPairs.AppendL( aSelectedPairs[i] );
        }
    }

void UT_CIceCheckList::ChecklistCompletedL( 
        const CIceCheckList& /*aChecklist*/,
        const RArray<TUint>& /*aFailedComps*/,
        TInt /*aErrCode*/ )
    {
    iCheckListCompleted = ETrue;
    }

void UT_CIceCheckList::ComponentsHaveValidPairsL( 
    const CIceCheckList& /*aChecklist*/, const TDesC8& /*aFoundation*/ )
    {
    iCheckForLastCompSucceed = ETrue;
    }
    

void UT_CIceCheckList::RoleChangeNeeded( TNATFWIceRole aDesiredRole )
    {
    iRole = aDesiredRole;
    }

void UT_CIceCheckList::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/,
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }

    
void UT_CIceCheckList::IcmpError( TUint /*aSessionId*/, 
        TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    
    }
    
void UT_CIceCheckList::StreamClosed( TUint /*aStreamId*/ )
    {
    
    }

void UT_CIceCheckList::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, 
        TUint /*aStreamId*/, TNatPluginEvent aEventCode, 
        TInt /*aErrorCode*/, TAny* /*aEventData*/, TUint /*aStreamConnectionId*/,
        TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ELocalCandidateFound:
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
    UT_CIceCheckList,
    "UT_CIceCheckList",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CIceCheckList",
    "NewL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_NewLL, Teardown)

EUNIT_TEST(
    "TimerFired - test ",
    "CIceCheckList",
    "TimerFired",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_TimerFiredL, Teardown)

EUNIT_TEST(
    "State - test ",
    "CIceCheckList",
    "State",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_StateL, Teardown)

EUNIT_TEST(
    "StreamCollectionId - test ",
    "CIceCheckList",
    "StreamCollectionId",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_StreamCollectionIdL, Teardown)

EUNIT_TEST(
    "InitializeCheckListL - test ",
    "CIceCheckList",
    "InitializeCheckListL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_InitializeCheckListLL, Teardown)

EUNIT_TEST(
    "InitializeCheckListL - test ",
    "CIceCheckList",
    "InitializeCheckListL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_InitializeCheckListL_1L, Teardown)

EUNIT_TEST(
    "StartPerformChecksL - CONTROLLING ",
    "CIceCheckList",
    "StartPerformChecksL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_StartPerformChecksLL, Teardown)

EUNIT_TEST(
    "StartPerformChecksL - CONTROLLED ",
    "CIceCheckList",
    "StartPerformChecksL",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_StartPerformChecksL_2L, Teardown)

EUNIT_TEST(
    "RestartCheckListL - test ",
    "CIceCheckList",
    "RestartCheckListL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_RestartCheckListLL, Teardown)

EUNIT_TEST(
    "UpdateCheckListL_1 - test ",
    "CIceCheckList",
    "UpdateCheckListL_1",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_1L, Teardown)

EUNIT_TEST(
    "UpdateCheckListL_2 - test ",
    "CIceCheckList",
    "UpdateCheckListL_2",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_2L, Teardown)

EUNIT_TEST(
    "UpdateCheckListL_3 - test ",
    "CIceCheckList",
    "UpdateCheckListL_3",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_3L, Teardown)
    
EUNIT_TEST(
    "UpdateCheckListL_4 - test ",
    "CIceCheckList",
    "UpdateCheckListL_4",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_4L, Teardown)

EUNIT_TEST(
    "UpdateCheckListL_5 - test ",
    "CIceCheckList",
    "UpdateCheckListL_5",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_5L, Teardown)
    
EUNIT_TEST(
    "UpdateCheckListL_6 - test ",
    "CIceCheckList",
    "UpdateCheckListL_6",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_6L, Teardown)

EUNIT_TEST(
    "UpdateCheckListL_7 - test ",
    "CIceCheckList",
    "UpdateCheckListL_7",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_UpdateCheckListL_7L, Teardown)
    
EUNIT_TEST(
    "CheckCompletedL - test ",
    "CIceCheckList",
    "CheckCompletedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_CheckCompletedLL, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - CONTROLLING ",
    "CIceCheckList",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_STUNRequestReceivedLL, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - CONTROLLED",
    "CIceCheckList",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_STUNRequestReceivedL_1L, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - CONTROLLED",
    "CIceCheckList",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_STUNRequestReceivedL_2L, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - CONTROLLING",
    "CIceCheckList",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_STUNRequestReceivedL3L, Teardown)

EUNIT_TEST(
    "STUNRequestReceivedL - CONTROLLING",
    "CIceCheckList",
    "STUNRequestReceivedL",
    "FUNCTIONALITY",
    SetupControllingL, UT_CICECheckList_STUNRequestReceivedL4L, Teardown)

EUNIT_TEST(
    "RecomputePairPrioritiesL - CONTROLLED",
    "CIceCheckList",
    "RecomputePairPrioritiesL",
    "FUNCTIONALITY",
    SetupControlledL, UT_CICECheckList_RecomputePairPrioritiesL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
