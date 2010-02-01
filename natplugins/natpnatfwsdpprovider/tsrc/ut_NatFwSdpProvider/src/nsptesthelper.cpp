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

#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "nspmediastreamcomponent.h"
#include "nsptesthelper.h"

_LIT8( KHost, "192.168.0.5" );
_LIT8( KServerReflex, "193.65.183.237" );
_LIT8( KPeerReflex, "190.0.22.14" );
_LIT8( KRelay, "172.23.0.196" );
_LIT8( KLoopback, "127.0.0.1" );



// METHODS:
// ---------------------------------------------------------------------------
// NSPTestHelper::NSPTestHelper
// ---------------------------------------------------------------------------
// 
NSPTestHelper::NSPTestHelper()
    {
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::~NSPTestHelper
// ---------------------------------------------------------------------------
// 
NSPTestHelper::~NSPTestHelper()
    {
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::ClearCRepositoryL
// ---------------------------------------------------------------------------
// 
void NSPTestHelper::ClearCRepositoryL( CRepository& aRep )
    {
    RArray<TUint32> domainKeys;
    CleanupClosePushL( domainKeys );
    
    TInt error = aRep.FindL( 0x00000000, 0x00000000, domainKeys );
    
    if ( KErrNone == error )
        {
        for ( TInt index = 0; index < domainKeys.Count(); index++ )
            {
            aRep.Delete( domainKeys[index] );
            }
        }
    
    CleanupStack::PopAndDestroy(); // domainKeys
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::CreateDomainKeyL
// ---------------------------------------------------------------------------
// 
void NSPTestHelper::CreateDomainKeyL( CRepository& aRep, const TDesC8& aDomain )
    {
    TInt error( KErrNone );
    TUint32 currentDomainKey( KErrNone );
    TUint32 tmp( 0 );
    TBool updateValue( EFalse );
    
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    
    aRep.FindEqL( KUNSAFProtocolsDomainMask,
            KUNSAFProtocolsFieldTypeMask, aDomain, keys );
    
    if ( keys.Count() )
        {
        tmp = keys[0];
        updateValue = ETrue;
        }
    
    if ( !updateValue )
        {
        currentDomainKey = tmp|KUNSAFProtocolsDomainMask;
        currentDomainKey &= KUNSAFProtocolsKeyMask;
        currentDomainKey |= KUNSAFProtocolsDomainMask;
        
        error = aRep.Create( currentDomainKey, aDomain );
        }
    
    User::LeaveIfError( error );
    CleanupStack::PopAndDestroy(); // keys
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::SetCenrepTDesC8toAProtocolL
// ---------------------------------------------------------------------------
// 
void NSPTestHelper::SetCenrepTDesC8toAProtocolL( CRepository& aRep,
        const TUint32& aKey, const TDesC8& aValue )
    {
    TInt error = aRep.Set( aKey, aValue );
    
    if ( KErrNotFound == error )
        {
        error = aRep.Create( aKey, aValue );
        }
    
    User::LeaveIfError( error );
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::GetAddr
// ---------------------------------------------------------------------------
// 
const TDesC8& NSPTestHelper::GetAddr( CNATFWCandidate::TCandidateType aType )
    {
    switch( aType )
        {
        case CNATFWCandidate::EHost:
            return KHost();
        
        case CNATFWCandidate::EServerReflexive:
            return KServerReflex();
        
        case CNATFWCandidate::EPeerReflexive:
            return KPeerReflex();
        
        case CNATFWCandidate::ERelay:
            return KRelay();
        
        default:
            return KLoopback();
        }
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::CreateCandL
// ---------------------------------------------------------------------------
// 
CNATFWCandidate* NSPTestHelper::CreateCandL( const TDesC8& aFoundation,
        TUint aComponentId, TUint aProtocol, TUint aPriority,
        CNATFWCandidate::TCandidateType aType, TUint aSessionId, 
        TUint aStreamId, TBool /*aSelected*/ )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    
    candidate->SetFoundationL( aFoundation );
    candidate->SetComponentId( aComponentId );
    candidate->SetTransportProtocol( (TUint)aProtocol );
    candidate->SetPriority( aPriority );
    candidate->SetTransportDomainAddrL( GetAddr( aType ), aStreamId );
    candidate->SetType( aType );
    candidate->SetSessionId( aSessionId );
    candidate->SetStreamId( aStreamId );
    
    CleanupStack::Pop( candidate );
    return candidate;
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::CreateCandL
// ---------------------------------------------------------------------------
// 
CNATFWCandidate* NSPTestHelper::CreateCandL(
        const CNSPMediaStreamComponent& aStreamComp,
        const TDesC8& aFoundation, TUint aProtocol,
        CNATFWCandidate::TCandidateType aType, TUint aSessionId, 
        TBool aSelected )
    {
    return CreateCandL( aFoundation, aStreamComp.ComponentId(), aProtocol,
            KPriorityMultiplier*aStreamComp.StreamId(),
            aType, aSessionId, aStreamComp.StreamId(), aSelected );
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::CreateCandLC
// ---------------------------------------------------------------------------
// 
CNATFWCandidate* NSPTestHelper::CreateCandLC(
        const CNSPMediaStreamComponent& aStreamComp,
        const TDesC8& aFoundation, TUint aProtocol,
        CNATFWCandidate::TCandidateType aType, TUint aSessionId, 
        TBool aSelected )
    {
    CNATFWCandidate* candidate = 
            CreateCandL( aFoundation, aStreamComp.ComponentId(), aProtocol,
            KPriorityMultiplier*aStreamComp.StreamId(),
            aType, aSessionId, aStreamComp.StreamId(), aSelected );
    CleanupStack::PushL( candidate );
    return candidate;
    }


// ---------------------------------------------------------------------------
// NSPTestHelper::CreateCandPairL
// ---------------------------------------------------------------------------
// 
CNATFWCandidatePair* NSPTestHelper::CreateCandPairL(
        const CNSPMediaStreamComponent& aLocalStreamComp,
        const TDesC8& aLocalFoundation, TUint aLocalProtocol,
        CNATFWCandidate::TCandidateType aLocalType, TUint aLocalSessionId,
        TBool aLocalSelected,
        const CNSPMediaStreamComponent& aRemoteStreamComp,
        const TDesC8& aRemoteFoundation, TUint aRemoteProtocol,
        CNATFWCandidate::TCandidateType aRemoteType, TUint aRemoteSessionId,
        TBool aRemoteSelected )
    {
    CNATFWCandidate* local = CreateCandL(
            aLocalStreamComp, aLocalFoundation, aLocalProtocol,
            aLocalType, aLocalSessionId, aLocalSelected );
    CleanupStack::PushL( local );
    CNATFWCandidate* remote = CreateCandL(
            aRemoteStreamComp, aRemoteFoundation, aRemoteProtocol,
            aRemoteType, aRemoteSessionId, aRemoteSelected );
    CleanupStack::PushL( remote );
    CNATFWCandidatePair* candidatePair = CNATFWCandidatePair::NewL( *local, *remote );
    CleanupStack::PopAndDestroy( remote );
    CleanupStack::PopAndDestroy( local );
    return candidatePair;
    }

// ---------------------------------------------------------------------------
// NSPTestHelper::CreateCandPairLC
// ---------------------------------------------------------------------------
// 
CNATFWCandidatePair* NSPTestHelper::CreateCandPairLC(
        const CNSPMediaStreamComponent& aLocalStreamComp,
        const TDesC8& aLocalFoundation, TUint aLocalProtocol,
        CNATFWCandidate::TCandidateType aLocalType, TUint aLocalSessionId,
        TBool aLocalSelected,
        const CNSPMediaStreamComponent& aRemoteStreamComp,
        const TDesC8& aRemoteFoundation, TUint aRemoteProtocol,
        CNATFWCandidate::TCandidateType aRemoteType, TUint aRemoteSessionId,
        TBool aRemoteSelected )
    {
    CNATFWCandidate* local = CreateCandL(
            aLocalStreamComp, aLocalFoundation, aLocalProtocol,
            aLocalType, aLocalSessionId, aLocalSelected );
    CleanupStack::PushL( local );
    CNATFWCandidate* remote = CreateCandL(
            aRemoteStreamComp, aRemoteFoundation, aRemoteProtocol,
            aRemoteType, aRemoteSessionId, aRemoteSelected );
    CleanupStack::PushL( remote );
    CNATFWCandidatePair* candidatePair = CNATFWCandidatePair::NewL( *local, *remote );
    CleanupStack::PopAndDestroy( remote );
    CleanupStack::PopAndDestroy( local );
    CleanupStack::PushL( candidatePair );
    return candidatePair;
    }

//  END OF FILE
