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
#include "UT_CIceSessionData.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"

//  INTERNAL INCLUDES
#include "cicesessiondata.h"
#include "cicestreamcollection.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceSessionData* UT_CIceSessionData::NewL()
    {
    UT_CIceSessionData* self = UT_CIceSessionData::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceSessionData* UT_CIceSessionData::NewLC()
    {
    UT_CIceSessionData* self = new( ELeave ) UT_CIceSessionData();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceSessionData::~UT_CIceSessionData()
    {
    }

// Default constructor
UT_CIceSessionData::UT_CIceSessionData()
    {
    }

// Second phase construct
void UT_CIceSessionData::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CIceSessionData::SetupL()
    {
    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    }

void UT_CIceSessionData::Teardown(  )
    {
    delete iCandStorage;
    iCandStorage = NULL;
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_AddPeerReflexiveCandidateLL(  )
    {
    EUNIT_ASSERT_LEAVE( 
        iCandStorage->AddPeerReflexiveCandidateL( NULL, EFalse ) );
    
    CNATFWCandidate* localCandidate = CNATFWCandidate::NewLC();
    CNATFWCandidate* remoteCandidate = CNATFWCandidate::NewLC();
    
    localCandidate->SetType( CNATFWCandidate::EPeerReflexive );
    localCandidate->SetComponentId( KComponentIdRtp );
    remoteCandidate->SetType( CNATFWCandidate::EPeerReflexive );
    remoteCandidate->SetComponentId( KComponentIdRtp );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        CleanupStack::Pop( remoteCandidate );
        iCandStorage->AddPeerReflexiveCandidateL( remoteCandidate, ETrue );
        CleanupStack::Pop( localCandidate );
        iCandStorage->AddPeerReflexiveCandidateL( localCandidate, EFalse );
        }
    else
        {
        CNATFWCandidate* redundantLocalCandidate 
            = CNATFWCandidate::NewL( *localCandidate );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddPeerReflexiveCandidateL( 
            redundantLocalCandidate, EFalse ) );
        
        CleanupStack::Pop( remoteCandidate );
        EUNIT_ASSERT_NO_LEAVE( iCandStorage->AddPeerReflexiveCandidateL( 
            remoteCandidate, ETrue ) );
        
        CleanupStack::Pop( localCandidate );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddPeerReflexiveCandidateL( localCandidate, EFalse ) );
        
        CNATFWCandidate* remoteCandidateRtcp
            = CNATFWCandidate::NewLC( *remoteCandidate );
        remoteCandidateRtcp->SetComponentId( KComponentIdRtcp );
        TInetAddr addr;
        addr.SetPort( 3000 );
        remoteCandidateRtcp->SetTransportAddrL( addr );
        
        // add RTCP candidate
        CleanupStack::Pop( remoteCandidateRtcp );
        EUNIT_ASSERT_NO_LEAVE( iCandStorage->AddPeerReflexiveCandidateL( 
            remoteCandidateRtcp, ETrue ) );
        
        // there is equal candidate already
        CNATFWCandidate* equalRemoteCandidate
            = CNATFWCandidate::NewL( *remoteCandidate );
        EUNIT_ASSERT_NO_LEAVE( iCandStorage->AddPeerReflexiveCandidateL( 
            equalRemoteCandidate, ETrue ) );
        }
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_FindLocalCandidate_1L()
    {
    TInetAddr addr( KTestAddressRtp1, KTestPortRtp1 );
    const CNATFWCandidate* candidate = iCandStorage->FindLocalCandidate( addr );
    EUNIT_ASSERT( !candidate );
    
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    candidate = iCandStorage->FindLocalCandidate( addr );
    EUNIT_ASSERT( candidate );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_FindLocalCandidate_2L()
    {
    const CNATFWCandidate* candidate 
        = iCandStorage->FindLocalCandidate( 
          KTestStreamCollectionId1, KComponentIdRtp, CNATFWCandidate::EHost );
    EUNIT_ASSERT( !candidate );

    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    
    candidate = iCandStorage->FindLocalCandidate( 
          KTestStreamCollectionId1, KComponentIdRtp, CNATFWCandidate::ERelay );
    EUNIT_ASSERT( !candidate );
    
    candidate = iCandStorage->FindLocalCandidate( 
          KTestStreamCollectionId1, KComponentIdRtcp, CNATFWCandidate::EHost );
    EUNIT_ASSERT( !candidate );
    
    candidate = iCandStorage->FindLocalCandidate( 
          KTestStreamCollectionId2, KComponentIdRtp, CNATFWCandidate::EHost );
    EUNIT_ASSERT( !candidate );
    
    // success case
    candidate = iCandStorage->FindLocalCandidate( 
          KTestStreamCollectionId1, KComponentIdRtp, CNATFWCandidate::EHost );
    EUNIT_ASSERT( candidate );
    }
 
void UT_CIceSessionData::UT_CNATFWCandidateStorage_FindRemoteCandidateL()
    {
    TInetAddr addr( KTestAddressRtp1, KTestPortRtp1 );
    const CNATFWCandidate* candidate = iCandStorage->FindRemoteCandidate( addr );
    EUNIT_ASSERT( !candidate );
    
    UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
    candidate = iCandStorage->FindRemoteCandidate( addr );
    EUNIT_ASSERT( candidate );
    }
 
void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetLocalCandidatesL(  )
    {
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    
    RPointerArray<CNATFWCandidate> candidateArray;
    CleanupClosePushL( candidateArray );
    iCandStorage->GetLocalCandidates( candidateArray );
    EUNIT_ASSERT( candidateArray.Count() );
    CleanupStack::PopAndDestroy( &candidateArray );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetLocalCandidates_1L(  )
    {
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    
    RPointerArray<CNATFWCandidate> candidateArray;
    CleanupClosePushL( candidateArray );
    iCandStorage->GetLocalCandidates( 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetLocalCandidates( KTestStreamCollectionId1, candidateArray );
    EUNIT_ASSERT( candidateArray.Count() );
    
    CleanupStack::PopAndDestroy( &candidateArray );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetLocalCandidates_2L(  )
    {
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    
    RPointerArray<CNATFWCandidate> candidateArray;
    CleanupClosePushL( candidateArray );
    iCandStorage->GetLocalCandidates( 100, 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetLocalCandidates( KTestStreamCollectionId1, 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetLocalCandidates( 100, KTestStreamIdRtp1, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetLocalCandidates( 
        KTestStreamCollectionId1, KTestStreamIdRtp1, candidateArray );
    EUNIT_ASSERT( candidateArray.Count() );
    
    CleanupStack::PopAndDestroy( &candidateArray );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_DeleteCollectionL(  )
    {
    UT_CNATFWCandidateStorage_CreateCollectionL();
    iCandStorage->DeleteCollection( 100 );
    EUNIT_ASSERT( iCandStorage->iStreamCollections.Count() == 1 );
    
    iCandStorage->DeleteCollection( KTestStreamCollectionId1 );
    EUNIT_ASSERT( iCandStorage->iStreamCollections.Count() == 0 );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_AddIdentificationLL(  )
    {
    CNATFWCredentials* identification1 = CNATFWCredentials::NewLC();
    identification1->SetStreamCollectionId( KTestStreamCollectionId1 );
    identification1->SetStreamId( KTestStreamIdRtp1 );
    identification1->SetDirection( CNATFWCredentials::EInbound );
    identification1->SetUsernameL( _L8("alice") );
    identification1->SetPasswordL( _L8("alicepass") );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCandStorage->AddCredentialsL( *identification1 );
        CleanupStack::PopAndDestroy( identification1 );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddCredentialsL( *identification1 ) );
        // Storage must filter duplicates
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddCredentialsL( *identification1 ) );
        
        CNATFWCredentials* identification2 = CNATFWCredentials::NewL();
        CleanupStack::PushL( identification2 );
        identification2->SetStreamCollectionId( KTestStreamCollectionId1 );
        identification2->SetStreamId( KTestStreamIdRtp1 );
        identification2->SetDirection( CNATFWCredentials::EOutbound);
        identification2->SetUsernameL( _L8("bob") );
        identification2->SetPasswordL( _L8("bobpass") );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddCredentialsL( *identification2 ) );
        const CNATFWCredentials* inboundId = iCandStorage->Credentials( 
            KTestStreamIdRtp1, CNATFWCredentials::EInbound );
        const CNATFWCredentials* outboundId = iCandStorage->Credentials(
            KTestStreamIdRtp1, CNATFWCredentials::EOutbound );
        
        EUNIT_ASSERT( inboundId->Username() == _L8("alice:bob") );
        EUNIT_ASSERT( outboundId->Username() == _L8("bob:alice") );
        
        // Update identifications
        identification1->SetUsernameL( _L8("alice2") );
        identification1->SetPasswordL( _L8("alicepass2") );
        identification2->SetUsernameL( _L8("bob2") );
        identification2->SetPasswordL( _L8("bobpass2") );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddCredentialsL( *identification1 ) );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddCredentialsL( *identification2 ) );
        
        inboundId = iCandStorage->Credentials( 
            KTestStreamIdRtp1, CNATFWCredentials::EInbound );
        outboundId = iCandStorage->Credentials( 
            KTestStreamIdRtp1, CNATFWCredentials::EOutbound );
        
        EUNIT_ASSERT( inboundId->Username() == _L8("alice2:bob2") );
        EUNIT_ASSERT( outboundId->Username() == _L8("bob2:alice2") );
        
        CleanupStack::PopAndDestroy( identification2 );
        CleanupStack::PopAndDestroy( identification1 );
        }
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_IdentificationsL()
    {
    const RPointerArray<CNATFWCredentials>& identifications
        = iCandStorage->Credentials();
    EUNIT_ASSERT( identifications.Count() == 0 );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_IdentificationL()
    {
    const CNATFWCredentials* credential
        = iCandStorage->Credentials( 
        KTestStreamIdRtp1, CNATFWCredentials::EInbound );
    EUNIT_ASSERT( NULL == credential );
    
    UT_CNATFWCandidateStorage_AddIdentificationLL();
    credential
        = iCandStorage->Credentials( 
        KTestStreamIdRtp1, CNATFWCredentials::EInbound );
    EUNIT_ASSERT( credential );
    }
         
void UT_CIceSessionData::UT_CNATFWCandidateStorage_AddRemoteCandidateLL(  )
    {
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    
    // TODO: Appropriate candidate type
    candidate1->SetType( CNATFWCandidate::EHost );
    candidate2->SetType( CNATFWCandidate::EHost );
    TInetAddr addr1( KTestAddressRtp1, KTestPortRtp1 );
    TInetAddr addr2( KTestAddressRtp1, KTestPortRtcp1 );
    candidate1->SetBase( addr1 );
    candidate1->SetTransportAddrL( addr1 );
    candidate2->SetBase( addr2 );
    candidate2->SetTransportAddrL( addr1 );
    
    candidate1->SetStreamCollectionId( KTestStreamCollectionId1 );
    candidate2->SetStreamCollectionId( KTestStreamCollectionId1 );
    candidate1->SetStreamId( KTestStreamIdRtp1 );
    candidate2->SetStreamId( KTestStreamIdRtp1 );
    candidate1->SetComponentId( KComponentIdRtp );
    candidate2->SetComponentId( KComponentIdRtcp );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        EUNIT_ASSERT_LEAVE( 
            iCandStorage->AddRemoteCandidateL( NULL ) );
        
        CleanupStack::Pop( candidate1 );
        iCandStorage->AddRemoteCandidateL( candidate1 );
        CleanupStack::Pop( candidate2 );
        iCandStorage->AddRemoteCandidateL( candidate2 );
        }
    else
        {
        EUNIT_ASSERT_LEAVE( 
            iCandStorage->AddRemoteCandidateL( NULL ) );
        
        CleanupStack::Pop( candidate1 );        
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddRemoteCandidateL( candidate1 ) );
        
        CleanupStack::Pop( candidate2 );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddRemoteCandidateL( candidate2 ) );
        }
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetRemoteCandidatesLL(  )
    {
    UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
    
    RPointerArray<CNATFWCandidate> candidateArray;
    CleanupClosePushL( candidateArray );
    iCandStorage->GetRemoteCandidatesL( 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetRemoteCandidatesL( KTestStreamCollectionId1, candidateArray );
    EUNIT_ASSERT( candidateArray.Count() );
    
    CleanupStack::PopAndDestroy( &candidateArray );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_AddRemoteCandidatesLL()
    {
    RPointerArray<CNATFWCandidate> remoteCandidates;
    CleanupResetAndDestroyPushL( remoteCandidates );
    
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    
    TInetAddr addr1( INET_ADDR( 192, 168, 1, 1 ), 5000 );
    TInetAddr addr2( INET_ADDR( 192, 168, 1, 2 ), 5000 );
    candidate1->SetType( CNATFWCandidate::EHost );
    candidate1->SetBase( addr1 );
    candidate1->SetTransportAddrL( addr1 );
    
    candidate2->SetType( CNATFWCandidate::EHost );
    candidate2->SetBase( addr1 );
    candidate2->SetTransportAddrL( addr2 );
    
    remoteCandidates.AppendL( candidate1 );
    CleanupStack::Pop( candidate1 );
    remoteCandidates.AppendL( candidate2 );
    CleanupStack::Pop( candidate2 );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCandStorage->AddRemoteCandidatesL( remoteCandidates );
        }
    else
        {
        // Invalid input
        RPointerArray<CNATFWCandidate> nullArray;
        CleanupClosePushL( nullArray );
        EUNIT_ASSERT_LEAVE( 
            iCandStorage->AddRemoteCandidatesL( nullArray ) );
        CleanupStack::PopAndDestroy( &nullArray );
        
        // redundant candidates in input array
        TInetAddr addr1( INET_ADDR( 192, 168, 1, 1 ), 5000 );
        TInetAddr addr2( INET_ADDR( 192, 168, 1, 2 ), 5000 );
        candidate1->SetBase( addr1 );
        candidate1->SetTransportAddrL( addr1 );
        candidate2->SetBase( addr1 );
        candidate2->SetTransportAddrL( addr1 );
        
        EUNIT_ASSERT_LEAVE( 
            iCandStorage->AddRemoteCandidatesL( remoteCandidates ) );
        
        // Correct input
        candidate2->SetTransportAddrL( addr2 );
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddRemoteCandidatesL( remoteCandidates ) );
        
        // adding redundant candidates is ok
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->AddRemoteCandidatesL( remoteCandidates ) );
        }
    
    CleanupStack::PopAndDestroy( &remoteCandidates );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetRemoteCandidatesL_1L(  )
    {
    UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
    
    RPointerArray<CNATFWCandidate> candidateArray;
    CleanupClosePushL( candidateArray );
    iCandStorage->GetRemoteCandidatesL( 100, 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetRemoteCandidatesL( KTestStreamCollectionId1, 100, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetRemoteCandidatesL( 100, KTestStreamIdRtp1, candidateArray );
    EUNIT_ASSERT( !candidateArray.Count() );
    
    iCandStorage->GetRemoteCandidatesL( 
        KTestStreamCollectionId1, KComponentIdRtp, candidateArray );
    EUNIT_ASSERT( candidateArray.Count() );
    
    CleanupStack::PopAndDestroy( &candidateArray );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_AddLocalCandidateLL(  )
    {
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    
    candidate1->SetType( CNATFWCandidate::EHost );
    TInetAddr addr1( KTestAddressRtp1, KTestPortRtp1 );
    candidate1->SetTransportAddrL( addr1 );
    candidate1->SetComponentId( KComponentIdRtp );
    candidate1->SetType( CNATFWCandidate::EHost );
    
    candidate2->SetType( CNATFWCandidate::EHost );
    TInetAddr addr2( KTestAddressRtcp1, KTestPortRtcp1 );
    candidate2->SetTransportAddrL( addr2 );
    candidate2->SetComponentId( KComponentIdRtp );
    candidate2->SetType( CNATFWCandidate::EHost );
    
    candidate1->SetStreamCollectionId( KTestStreamCollectionId1 );
    candidate2->SetStreamCollectionId( KTestStreamCollectionId1 );
    candidate1->SetStreamId( KTestStreamIdRtp1 );
    candidate2->SetStreamId( KTestStreamIdRtp1 );
    
    TBool isRedundant( EFalse );
    EUNIT_ASSERT_LEAVE( 
        isRedundant = iCandStorage->AddLocalCandidateL( NULL ) );
    EUNIT_ASSERT( !isRedundant );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        CleanupStack::Pop( candidate1 );
        isRedundant = iCandStorage->AddLocalCandidateL( candidate1 );
        EUNIT_ASSERT( !isRedundant );
        
        CleanupStack::Pop( candidate2 );
        isRedundant = iCandStorage->AddLocalCandidateL( candidate2 );
        EUNIT_ASSERT( !isRedundant );
        }
    else
        {
        CleanupStack::Pop( candidate1 );
        EUNIT_ASSERT_NO_LEAVE( 
            isRedundant = iCandStorage->AddLocalCandidateL( candidate1 ) );
        EUNIT_ASSERT( !isRedundant );
        
        CleanupStack::Pop( candidate2 );
        EUNIT_ASSERT_NO_LEAVE( 
            isRedundant = iCandStorage->AddLocalCandidateL( candidate2 ) );
        EUNIT_ASSERT( !isRedundant );
        
        CNATFWCandidate* redundantCandidate 
            = CNATFWCandidate::NewLC( *candidate1 );
        CleanupStack::Pop( redundantCandidate );
        EUNIT_ASSERT_NO_LEAVE( 
            isRedundant = iCandStorage->AddLocalCandidateL( redundantCandidate ) );
        EUNIT_ASSERT( isRedundant );
        }
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetStreamCollectionIdsLL(  )
    {
    RArray<TUint> streamCollIds;
    CleanupClosePushL( streamCollIds );
    
    iCandStorage->GetStreamCollectionIdsL( streamCollIds );
    CleanupStack::PopAndDestroy( &streamCollIds );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_CreateCollectionL(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
        EUNIT_ASSERT( iCandStorage->iStreamCollections.Count() == 1 );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->CreateCollectionL( KTestStreamCollectionId1 ) );
        EUNIT_ASSERT( iCandStorage->iStreamCollections.Count() == 1 );
        
        // implementation should silently discard duplicate creation
        EUNIT_ASSERT_NO_LEAVE( 
            iCandStorage->CreateCollectionL( KTestStreamCollectionId1 ) );
        EUNIT_ASSERT( iCandStorage->iStreamCollections.Count() == 1 );
        }
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_CollectionLL()
    {
    EUNIT_ASSERT_LEAVE( 
        iCandStorage->CollectionL( KTestStreamCollectionId1 ) );
    
    UT_CNATFWCandidateStorage_CreateCollectionL();
    EUNIT_ASSERT_NO_LEAVE( 
        iCandStorage->CollectionL( KTestStreamCollectionId1 ) );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_GetCollectionIdForComponentL()
    {
    iCandStorage->CreateCollectionL( KTestStreamCollectionId1 );
    CIceStreamCollection* collection 
        = iCandStorage->CollectionL( KTestStreamCollectionId1 );
    collection->AddMediaComponentL( KTestStreamIdRtp1, KComponentIdRtp );
    
    TUint collId( 0 );
    TInt err = iCandStorage->GetCollectionIdForComponent( 
        KTestStreamIdRtp1, collId );
    EUNIT_ASSERT( KErrNone == err && ( collId != 0 ) );
    
    collId = 0;
    err = iCandStorage->GetCollectionIdForComponent( 
        KTestStreamIdRtcp1, collId );
    EUNIT_ASSERT( KErrNotFound == err && ( collId == 0 ) );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_CleanupStreamDataL()
    {
    UT_CNATFWCandidateStorage_CreateCollectionL();
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
    UT_CNATFWCandidateStorage_AddIdentificationLL();
    
    // unknown stream
    iCandStorage->CleanupStreamData( KTestStreamIdRtp2 );
    EUNIT_ASSERT( iCandStorage->iCredentialsList.Count() != 0 );
    EUNIT_ASSERT( iCandStorage->iLocalCandidateList.Count() != 0 );
    EUNIT_ASSERT( iCandStorage->iRemoteCandidateList.Count() != 0 );
    
    CNATFWCandidate* rtcpCandidate = CNATFWCandidate::NewLC();
    rtcpCandidate->SetType( CNATFWCandidate::EHost );
    TInetAddr addr1( KTestAddressRtcp1, KTestPortRtcp1 );
    rtcpCandidate->SetTransportAddrL( addr1 );
    rtcpCandidate->SetComponentId( KComponentIdRtcp );
    rtcpCandidate->SetStreamId( KTestStreamIdRtcp1 );
    
    // known stream
    iCandStorage->CleanupStreamData( KTestStreamIdRtp1 );
    EUNIT_ASSERT( iCandStorage->iCredentialsList.Count() == 0 );
    EUNIT_ASSERT( iCandStorage->iLocalCandidateList.Count() == 0 );
    EUNIT_ASSERT( iCandStorage->iRemoteCandidateList.Count() == 0 );
    
    CleanupStack::PopAndDestroy( rtcpCandidate );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_RemoveLocalCandidatesL()
    {
    iCandStorage->RemoveLocalCandidates();
    
    UT_CNATFWCandidateStorage_AddLocalCandidateLL();
    iCandStorage->RemoveLocalCandidates();
    EUNIT_ASSERT( iCandStorage->iLocalCandidateList.Count() == 0 );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_RemoveRemoteCandidatesL()
    {
    iCandStorage->RemoveRemoteCandidates();
    
    UT_CNATFWCandidateStorage_AddRemoteCandidateLL();
    iCandStorage->RemoveRemoteCandidates();
    EUNIT_ASSERT( iCandStorage->iRemoteCandidateList.Count() == 0 );
    }

void UT_CIceSessionData::UT_CNATFWCandidateStorage_SetOperationModeLL()
    {
    iCandStorage->SetRole( EIceRoleControlling );
    EUNIT_ASSERT( EIceRoleControlling == iCandStorage->Role() );
    
    iCandStorage->SetRole( EIceRoleControlled );
    EUNIT_ASSERT( EIceRoleControlled == iCandStorage->Role() );
    
    // invalid role
    EUNIT_ASSERT_PANIC( 
        iCandStorage->SetRole( TNATFWIceRole( 0 ) ), "USER", 0 );
    }
 
 void UT_CIceSessionData::UT_CNATFWCandidateStorage_OperationModeL()
    {
    EUNIT_ASSERT( 0 == iCandStorage->Role() );
    
    EUNIT_ASSERT_NO_LEAVE(
        iCandStorage->SetRole( EIceRoleControlling ) );
    EUNIT_ASSERT( EIceRoleControlling == iCandStorage->Role() );
    }
 
 void UT_CIceSessionData::UT_CNATFWCandidateStorage_TieBreakerL()
    {
    EUNIT_ASSERT( 0 == iCandStorage->TieBreaker() );
    
    iCandStorage->SetTieBreaker( 1000 );
    EUNIT_ASSERT( 1000 == iCandStorage->TieBreaker() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceSessionData,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddPeerReflexiveCandidateL - test ",
    "CIceSessionData",
    "AddPeerReflexiveCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_AddPeerReflexiveCandidateLL, Teardown)

EUNIT_TEST(
    "FindLocalCandidate_1L - test ",
    "CIceSessionData",
    "FindLocalCandidate_1L",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_FindLocalCandidate_1L, Teardown)

EUNIT_TEST(
    "FindLocalCandidate_2L - test ",
    "CIceSessionData",
    "FindLocalCandidate_2L",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_FindLocalCandidate_2L, Teardown)
    
EUNIT_TEST(
    "FindRemoteCandidateL - test ",
    "CIceSessionData",
    "FindRemoteCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_FindRemoteCandidateL, Teardown)
    
EUNIT_TEST(
    "GetLocalCandidates - test ",
    "CIceSessionData",
    "GetLocalCandidates",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetLocalCandidatesL, Teardown)

EUNIT_TEST(
    "GetLocalCandidates - test ",
    "CIceSessionData",
    "GetLocalCandidates",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetLocalCandidates_1L, Teardown)

EUNIT_TEST(
    "GetLocalCandidates - test ",
    "CIceSessionData",
    "GetLocalCandidates",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetLocalCandidates_2L, Teardown)


EUNIT_TEST(
    "DeleteCollectionL - test ",
    "CIceSessionData",
    "DeleteCollectionL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_DeleteCollectionL, Teardown)


EUNIT_TEST(
    "AddCredentialsL - test ",
    "CIceSessionData",
    "AddCredentialsL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_AddIdentificationLL, Teardown)

EUNIT_TEST(
    "Identifications - test ",
    "CIceSessionData",
    "Identifications",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_IdentificationsL, Teardown)

EUNIT_TEST(
    "Identification - test ",
    "CIceSessionData",
    "Identification",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_IdentificationL, Teardown)

EUNIT_TEST(
    "AddRemoteCandidateL - test ",
    "CIceSessionData",
    "AddRemoteCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_AddRemoteCandidateLL, Teardown)

EUNIT_TEST(
    "GetRemoteCandidatesL - test ",
    "CIceSessionData",
    "GetRemoteCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetRemoteCandidatesLL, Teardown)

EUNIT_TEST(
    "AddRemoteCandidatesL - test ",
    "CIceSessionData",
    "AddRemoteCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_AddRemoteCandidatesLL, Teardown)

EUNIT_TEST(
    "GetRemoteCandidatesL - test ",
    "CIceSessionData",
    "GetRemoteCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetRemoteCandidatesL_1L, Teardown)

EUNIT_TEST(
    "AddLocalCandidateL - test ",
    "CIceSessionData",
    "AddLocalCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_AddLocalCandidateLL, Teardown)

EUNIT_TEST(
    "GetStreamCollectionIdsL - test ",
    "CIceSessionData",
    "GetStreamCollectionIdsL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetStreamCollectionIdsLL, Teardown)

EUNIT_TEST(
    "CreateCollectionL - test ",
    "CIceSessionData",
    "CreateCollectionL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_CreateCollectionL, Teardown)

EUNIT_TEST(
    "CollectionL - test ",
    "CIceSessionData",
    "CollectionL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_CollectionLL, Teardown)

EUNIT_TEST(
    "GetCollectionIdForComponentL - test ",
    "CIceSessionData",
    "GetCollectionIdForComponentL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_GetCollectionIdForComponentL, Teardown)

EUNIT_TEST(
    "CleanupStreamData - test ",
    "CIceSessionData",
    "CleanupStreamData",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_CleanupStreamDataL, Teardown)

EUNIT_TEST(
    "RemoveLocalCandidatesL - test ",
    "CIceSessionData",
    "RemoveLocalCandidates",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_RemoveLocalCandidatesL, Teardown )

EUNIT_TEST(
    "RemoveRemoteCandidatesL - test ",
    "CIceSessionData",
    "RemoveRemoteCandidates",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_RemoveRemoteCandidatesL, Teardown )

EUNIT_TEST(
    "SetRoleL - test ",
    "CIceSessionData",
    "SetRoleL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_SetOperationModeLL, Teardown )

EUNIT_TEST(
    "OperationModeL - test ",
    "CIceSessionData",
    "OperationMode",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_OperationModeL, Teardown )

EUNIT_TEST(
    "TieBreakerL - test ",
    "CIceSessionData",
    "TieBreaker",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidateStorage_TieBreakerL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
