/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Storage for session related data.
*
*/




#include <e32math.h>
#include "natfwcredentials.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "mnatfwpluginobserver.h"
#include "cicesessiondata.h"
#include "cicestreamcollection.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlerlogs.h"

const TUint KInitialIdentifierLength = 16;
_LIT8( KColon, ":" );

// ======== MEMBER FUNCTIONS ========

CIceSessionData::CIceSessionData( TUint32 aIapId )
    :
    iIapId( aIapId )
    {
    __ICEDP( "CIceSessionData::CIceSessionData" )
    }


void CIceSessionData::ConstructL( const TDesC8& aDomain )
    {
    __ICEDP( "CIceSessionData::ConstructL" )
    
    iTempIdentifier.CreateL( KInitialIdentifierLength );
    iDomain = aDomain.AllocL();
    }


CIceSessionData* CIceSessionData::NewL(
        const TDesC8& aDomain, TUint32 aIapId )
    {
    __ICEDP( "CIceSessionData::NewL" )

    CIceSessionData* self 
        = CIceSessionData::NewLC( aDomain, aIapId );
    CleanupStack::Pop( self );
    
    return self;
    }


CIceSessionData* CIceSessionData::NewLC(
        const TDesC8& aDomain, TUint32 aIapId )
    {
    __ICEDP( "CIceSessionData::NewLC" )

    CIceSessionData* self 
        = new ( ELeave ) CIceSessionData( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    
    return self;
    }
    

CIceSessionData::~CIceSessionData()
    {
    __ICEDP( "CIceSessionData::~CIceSessionData" )

    iStreamCollections.ResetAndDestroy();  
    iLocalCandidateList.ResetAndDestroy();
    iRemoteCandidateList.ResetAndDestroy();
    iCredentialsList.ResetAndDestroy();
    iTempIdentifier.Close();
    delete iDomain;
    }
    
  
// ---------------------------------------------------------------------------
// CIceSessionData::AddPeerReflexiveCandidateL
// ---------------------------------------------------------------------------
//
void CIceSessionData::AddPeerReflexiveCandidateL( 
        CNATFWCandidate* aPeerCandidate, TBool aIsRemoteCand )
    {
    __ICEDP( "CIceSessionData::AddPeerReflexiveCandidateL" )
    __ASSERT_DEBUG( aPeerCandidate, User::Leave( KErrArgument ) );
    
    CleanupStack::PushL( aPeerCandidate );
    if ( aIsRemoteCand )
        {
        TInt ind( iRemoteCandidateList.Count() );
        while ( ind-- )
            {
            if ( TIceUtils::MatchAddresses( 
                    iRemoteCandidateList[ind]->TransportAddr(),
                    aPeerCandidate->TransportAddr() ) )
                {
                CleanupStack::PopAndDestroy( aPeerCandidate );
                return;
                }
            }
        
        // Assign arbitrary foundation (ICE-17, 7.2.1.3.)
        do 
            {
            const TDesC8& foundation = GenerateRandomIdentifier();
            aPeerCandidate->SetFoundationL( foundation );
            } while ( KErrNotFound != iRemoteCandidateList.Find( 
                aPeerCandidate, CNATFWCandidate::CompareFoundations ) );
        
        iRemoteCandidateList.AppendL( aPeerCandidate );
        CleanupStack::Pop( aPeerCandidate );
        }
    else
        {
        if ( !IsRedundantCandidate( *aPeerCandidate, iLocalCandidateList ) )
            {
            AssignFoundationForCandidateL( *aPeerCandidate );
            iLocalCandidateList.AppendL( aPeerCandidate );
            CleanupStack::Pop( aPeerCandidate );
            }
        else
            {
            CleanupStack::PopAndDestroy( aPeerCandidate );
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CIceSessionData::FindLocalCandidate
// ---------------------------------------------------------------------------
//
const CNATFWCandidate* CIceSessionData::FindLocalCandidate( 
        const TInetAddr& aTransportAddr ) const
    {
    TInt count( iLocalCandidateList.Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        const CNATFWCandidate* curItem = iLocalCandidateList[i];
        
        if ( TIceUtils::MatchAddresses( 
                curItem->TransportAddr(), aTransportAddr ) )
            {
            return curItem;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::FindLocalCandidate
// ---------------------------------------------------------------------------
//
const CNATFWCandidate* CIceSessionData::FindLocalCandidate( 
        TUint aStreamCollectionId,
        TUint aComponentId,
        CNATFWCandidate::TCandidateType aType ) const
    {
    TInt count( iLocalCandidateList.Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        const CNATFWCandidate* curItem = iLocalCandidateList[i];
        
        if ( curItem->StreamCollectionId() == aStreamCollectionId 
            && curItem->ComponentId() == aComponentId
            && curItem->Type() == aType )
            {
            return curItem;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::FindRemoteCandidate
// ---------------------------------------------------------------------------
//
const CNATFWCandidate* CIceSessionData::FindRemoteCandidate( 
        const TInetAddr& aTransportAddr ) const
    {
    TInt count( iRemoteCandidateList.Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        const CNATFWCandidate* curItem = iRemoteCandidateList[i];
        
        if ( TIceUtils::MatchAddresses( 
                curItem->TransportAddr(), aTransportAddr ) )
            {
            return curItem;
            }
        }
    
    return NULL;
    }
    

// ---------------------------------------------------------------------------
// CIceSessionData::GetLocalCandidates
// ---------------------------------------------------------------------------
//
void CIceSessionData::GetLocalCandidates(
                const TUint aStreamCollectionId,
                const TUint aStreamId,
                RPointerArray<CNATFWCandidate>& aLocalCandidates )
    {
    __ICEDP( "CIceSessionData::GetLocalCandidates" )
    
    for ( TInt i = 0; i < iLocalCandidateList.Count(); i++ )
        {
        if ( iLocalCandidateList[i]->StreamCollectionId() 
                == aStreamCollectionId
            && iLocalCandidateList[i]->StreamId() == aStreamId ) 
            {
            aLocalCandidates.Append( iLocalCandidateList[i] );
            }
        }
    }

// ---------------------------------------------------------------------------
// CIceSessionData::GetLocalCandidates
// ---------------------------------------------------------------------------
//    
void CIceSessionData::GetLocalCandidates( 
        const TUint aStreamCollectionId,
        RPointerArray<CNATFWCandidate>& aLocalCandidates )
    {
    __ICEDP( "CIceSessionData::GetLocalCandidates" )
    
    for ( TInt i = 0; i < iLocalCandidateList.Count(); i++ )
        {
        if ( iLocalCandidateList[i]->StreamCollectionId() 
                == aStreamCollectionId ) 
            {
            aLocalCandidates.Append( iLocalCandidateList[i] );
            }
        }
    }
 
// ---------------------------------------------------------------------------
// CIceSessionData::GetLocalCandidates
// ---------------------------------------------------------------------------
//
void CIceSessionData::GetLocalCandidates(
                     RPointerArray<CNATFWCandidate>& aLocalCandidates )
    {
    __ICEDP( "CIceSessionData::GetLocalCandidates" )
    
    for ( TInt i = 0; i < iLocalCandidateList.Count(); i++ )
        {
        aLocalCandidates.Append( iLocalCandidateList[i] );
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::DeleteCollection
// ---------------------------------------------------------------------------
//
void CIceSessionData::DeleteCollection( TUint aStreamCollectionId )
    { 
    __ICEDP( "CIceSessionData::DeleteCollection" )
    
    CIceStreamCollection* collection 
        = FindStreamCollectionById( aStreamCollectionId );
    
    if ( collection )
        {
        TInt ind( iStreamCollections.Find( collection ) );
        iStreamCollections.Remove( ind );
        delete collection;
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::AddCredentialsL
// Existing credentials for a stream is replaced with the new one.
// ---------------------------------------------------------------------------
//    
void CIceSessionData::AddCredentialsL(
        const CNATFWCredentials& aCredentials )
    {
    __ICEDP( "CIceSessionData::AddCredentialsL" )
    
    TBool existingCredentialsFound( EFalse );
    TInt credentialsInd( iCredentialsList.Count() - 1 );
    while ( !existingCredentialsFound && 0 <= credentialsInd )
        {
        CNATFWCredentials* item = iCredentialsList[credentialsInd];
        if ( item->StreamCollectionId()
                == aCredentials.StreamCollectionId()
            && item->StreamId() == aCredentials.StreamId()
            && item->Direction() == aCredentials.Direction() )
            {
            existingCredentialsFound = ETrue;
            item->SetUsernameL( aCredentials.Username() );
            item->SetPasswordL( aCredentials.Password() );
            UpdateUserNamesL( *item );
            }
        
        credentialsInd--;
        }
    
    if ( !existingCredentialsFound )
        {
        CNATFWCredentials* newItem 
            = CNATFWCredentials::NewLC( aCredentials );
        UpdateUserNamesL( *newItem );
        iCredentialsList.AppendL( newItem );
        CleanupStack::Pop( newItem );
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::Credentials
// ---------------------------------------------------------------------------
//
const RPointerArray<CNATFWCredentials>&
    CIceSessionData::Credentials() const
    {
    return iCredentialsList;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::Credentials
// ---------------------------------------------------------------------------
//
const CNATFWCredentials* CIceSessionData::Credentials( 
        TUint aStreamId,
        CNATFWCredentials::TUtilizationDirection aDirection ) const
    {
    TInt count( iCredentialsList.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iCredentialsList[i]->StreamId() == aStreamId
            && iCredentialsList[i]->Direction() == aDirection )
            {
            return iCredentialsList[i];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::AddLocalCandidateL
// ---------------------------------------------------------------------------
//       
TBool CIceSessionData::AddLocalCandidateL(
        CNATFWCandidate* aLocalCandidate )
    {
    __ICEDP( "CIceSessionData::AddLocalCandidateL" )
    __ASSERT_DEBUG( aLocalCandidate, User::Leave( KErrArgument ) );
    
    CleanupStack::PushL( aLocalCandidate );
    TBool redundantCand 
        = IsRedundantCandidate( *aLocalCandidate, iLocalCandidateList );
    
    if ( !redundantCand )
        {
        AssignFoundationForCandidateL( *aLocalCandidate );
        iLocalCandidateList.AppendL( aLocalCandidate );
        CleanupStack::Pop( aLocalCandidate );
        }
    else
        {
        CleanupStack::PopAndDestroy( aLocalCandidate );
        }
    
    return redundantCand;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::AddRemoteCandidateL
// ---------------------------------------------------------------------------
//       
void CIceSessionData::AddRemoteCandidateL( CNATFWCandidate* aRemoteCandidate )
    {
    __ICEDP( "CIceSessionData::AddRemoteCandidateL" )
    CleanupStack::PushL( aRemoteCandidate );
    __ASSERT_DEBUG( NULL != aRemoteCandidate, User::Leave( KErrArgument ) );
    
    if ( IsRedundantCandidate( *aRemoteCandidate, iRemoteCandidateList ) )
        {
        CleanupStack::PopAndDestroy( aRemoteCandidate );
        }
    else
        {
        iRemoteCandidateList.AppendL( aRemoteCandidate );
        CleanupStack::Pop( aRemoteCandidate );
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::AddRemoteCandidatesL
// ---------------------------------------------------------------------------
//               
void CIceSessionData::AddRemoteCandidatesL( 
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    TInt numOfCandidates = aRemoteCandidates.Count();
    __ICEDP_INT1( "CIceSessionData::AddRemoteCandidatesL, COUNT:",
         numOfCandidates )
    __ASSERT_ALWAYS( 0 != numOfCandidates, User::Leave( KErrArgument ) );
    
    // check that there is not redundancy in input array
    RPointerArray<CNATFWCandidate> candidatesToComp;
    CleanupClosePushL( candidatesToComp );
    
    for ( TInt i(0); i < numOfCandidates; ++i )
        {
        candidatesToComp.AppendL( aRemoteCandidates[i] );
        }
    
    for ( TInt i(0); i < numOfCandidates; ++i )
        {
        candidatesToComp.Remove(0);
        
        if ( IsRedundantCandidate( *aRemoteCandidates[i], candidatesToComp ) )
            {
            __ICEDP( "CIceSessionData::AddRemoteCandidatesL, REDUNDANT" )
            User::Leave( KErrArgument );
            }
        }
    CleanupStack::PopAndDestroy( &candidatesToComp );
    
    // add previously unknown candidates to the list
    for ( TInt i(0); i < numOfCandidates; ++i )
        {
        if ( !IsRedundantCandidate( 
                *aRemoteCandidates[i], iRemoteCandidateList ) )
            {
            CNATFWCandidate* candidate 
                = CNATFWCandidate::NewLC( *aRemoteCandidates[i] );
            iRemoteCandidateList.AppendL( candidate );
            CleanupStack::Pop( candidate );
            }
        }
    }

// ---------------------------------------------------------------------------
// CIceSessionData::GetRemoteCandidatesL
// ---------------------------------------------------------------------------
//         
void CIceSessionData::GetRemoteCandidatesL( 
        const TUint aStreamCollectionId,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __ICEDP( "CIceSessionData::GetRemoteCandidatesL" )

    TInt counter = iRemoteCandidateList.Count();
    for ( TInt i = 0; i < counter; ++i )
        {
        if ( iRemoteCandidateList[i]->StreamCollectionId() 
                == aStreamCollectionId )
            {
            aRemoteCandidates.AppendL( iRemoteCandidateList[i] );
            }
        }
    }

// ---------------------------------------------------------------------------
// CIceSessionData::GetRemoteCandidatesL
// ---------------------------------------------------------------------------
//         
void CIceSessionData::GetRemoteCandidatesL( 
                const TUint aStreamCollectionId,
                const TUint aComponentId,
                RPointerArray<CNATFWCandidate>& aRemoteCandidates )
                
    {
    __ICEDP( "CIceSessionData::GetRemoteCandidatesL" )
    
    TInt count = iRemoteCandidateList.Count();
    for ( TInt i = 0; i < count;i++ )
        {
        if ( iRemoteCandidateList[i]->StreamCollectionId() 
                == aStreamCollectionId && 
            iRemoteCandidateList[i]->ComponentId() == aComponentId )
            {
            aRemoteCandidates.AppendL( iRemoteCandidateList[i] );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::GetStreamCollectionIdsL
// ---------------------------------------------------------------------------
//
void CIceSessionData::GetStreamCollectionIdsL( 
        RArray<TUint>& aStreamCollIds ) const
    {
    __ICEDP( "CIceSessionData::GetStreamCollectionIdsL" )
    
    TInt count = iStreamCollections.Count();
    for ( TInt i = 0; i < count; i++ )
         {
         aStreamCollIds.AppendL( 
            iStreamCollections[i]->StreamCollectionId() );
         }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::CreateCollectionL
// ---------------------------------------------------------------------------
//    
void CIceSessionData::CreateCollectionL( TUint aStreamCollectionId )
    {
    __ICEDP( "CIceSessionData::CreateCollectionL" )

    CIceStreamCollection* collection 
        = FindStreamCollectionById( aStreamCollectionId );
    
    if ( !collection )
        {
        CIceStreamCollection* newCollection 
            = CIceStreamCollection::NewLC( aStreamCollectionId );
        iStreamCollections.AppendL( newCollection );
        CleanupStack::Pop( newCollection );
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::CollectionL
// ---------------------------------------------------------------------------
//
CIceStreamCollection* CIceSessionData::CollectionL( 
        TUint aStreamCollectionId )
    {
    CIceStreamCollection* collection 
        = FindStreamCollectionById( aStreamCollectionId );
    __ASSERT_DEBUG( collection, User::Leave( KErrNotFound ) );
    
    return collection;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::GetCollectionIdForComponent
// ---------------------------------------------------------------------------
//
TInt CIceSessionData::GetCollectionIdForComponent( 
        TUint aStreamId, TUint& aCollectionId )
    {
    TInt count( iStreamCollections.Count() );
    
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iStreamCollections[i]->HasMediaComponent( aStreamId ) )
            {
            aCollectionId = iStreamCollections[i]->StreamCollectionId();
            return KErrNone;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::CleanupStreamData
// ---------------------------------------------------------------------------
//
void CIceSessionData::CleanupStreamData( TUint aStreamId )
    {
    __ICEDP( "CIceSessionData::CleanupStreamData" )
    
    for ( TInt i( iCredentialsList.Count() - 1 ); i >= 0; --i )
        {
        if ( iCredentialsList[i]->StreamId() == aStreamId )
            {
            delete iCredentialsList[i];
            iCredentialsList.Remove( i );
            }
        }
    
    for ( TInt i( iLocalCandidateList.Count() - 1 ); i >= 0; --i )
        {
        if ( iLocalCandidateList[i]->StreamId() == aStreamId )
            {
            delete iLocalCandidateList[i];
            iLocalCandidateList.Remove( i );
            }
        }
    
    for ( TInt i( iRemoteCandidateList.Count() - 1 ); i >= 0; --i )
        {
        if ( iRemoteCandidateList[i]->StreamId() == aStreamId )
            {
            delete iRemoteCandidateList[i];
            iRemoteCandidateList.Remove( i );
            }
        }
    
    CIceStreamCollection* collection( NULL );
    for ( TInt i( iStreamCollections.Count() - 1 ); i >= 0; --i )
        {
        collection = iStreamCollections[i];
        if ( collection->HasMediaComponent( aStreamId ) )
            {
            collection->RemoveMediaComponent( aStreamId );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceSessionData::RemoveLocalCandidates
// ---------------------------------------------------------------------------
//
void CIceSessionData::RemoveLocalCandidates()
    {
    __ICEDP( "CIceSessionData::RemoveLocalCandidates" )
    
    iLocalCandidateList.ResetAndDestroy();
    }
 
 
// ---------------------------------------------------------------------------
// CIceSessionData::RemoveRemoteCandidates
// ---------------------------------------------------------------------------
//
void CIceSessionData::RemoveRemoteCandidates()
    {
    __ICEDP( "CIceSessionData::RemoveRemoteCandidates" )
    
    iRemoteCandidateList.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CIceSessionData::SetRole
// ---------------------------------------------------------------------------
//
void CIceSessionData::SetRole( TNATFWIceRole aRole )
    {
    __ICEDP( "CIceSessionData::SetRole" )
    ASSERT( EIceRoleControlling == aRole || EIceRoleControlled == aRole );
    
    iRole = aRole;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::Role
// ---------------------------------------------------------------------------
//
TNATFWIceRole CIceSessionData::Role() const
    {
    __ICEDP( "CIceSessionData::Role" )
    
    return iRole;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::SetTieBreaker
// ---------------------------------------------------------------------------
//
void CIceSessionData::SetTieBreaker( TUint64 aTieBreaker )
    {
    iTieBreaker = aTieBreaker;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::TieBreaker
// ---------------------------------------------------------------------------
//
TUint64 CIceSessionData::TieBreaker() const
    {
    __ICEDP( "CIceSessionData::TieBreaker" )
    
    return iTieBreaker;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::Domain
// ---------------------------------------------------------------------------
//
const TDesC8& CIceSessionData::Domain() const
    {
    return *iDomain;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::IapId
// ---------------------------------------------------------------------------
//
TUint32 CIceSessionData::IapId() const
    {
    return iIapId;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::SetTaTimerValue
// ---------------------------------------------------------------------------
//
void CIceSessionData::SetTaTimerValue( TUint aTaTimerValue )
    {
    ASSERT( aTaTimerValue != 0 );
    
    iTaTimerValue = aTaTimerValue;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::TaTimerValue
// ---------------------------------------------------------------------------
//
TUint CIceSessionData::TaTimerValue() const
    {
    ASSERT( iTaTimerValue != 0 );
    
    return iTaTimerValue;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::IsRedundantCandidate
// ---------------------------------------------------------------------------
//
TBool CIceSessionData::IsRedundantCandidate( 
        const CNATFWCandidate& aCandidate,
        const RPointerArray<CNATFWCandidate>& aCandidateArray ) const
    {
    __ICEDP( "CIceSessionData::IsRedundantCandidate" )
    
    TInt ind = aCandidateArray.Count();
    
    while ( ind-- )
        {
        CNATFWCandidate* cToComp( aCandidateArray[ind] );
        
        if ( TIceUtils::MatchAddresses( 
                aCandidate.TransportAddr(), cToComp->TransportAddr() ) &&
             TIceUtils::MatchAddresses( 
                aCandidate.Base(), cToComp->Base() ) )
            {
            return ETrue;
            }
        }
    
    return EFalse;    
    }


// ---------------------------------------------------------------------------
// CIceSessionData::AssignFoundationForCandidateL
// Assigns foundation for local candidate.
// ---------------------------------------------------------------------------
//
void CIceSessionData::AssignFoundationForCandidateL( 
        CNATFWCandidate& aCandidate )
    {
    __ICEDP( "CIceSessionData::AssignFoundationForCandidateL" )
    
    TInt ind = iLocalCandidateList.Count();
    
    while ( ind-- )
        {
        CNATFWCandidate* candToCompare( iLocalCandidateList[ind] );
        
        if ( aCandidate.Type() == candToCompare->Type() && 
             aCandidate.Base().Match( candToCompare->Base() ) &&
             aCandidate.TransportProtocol() 
                == candToCompare->TransportProtocol() )
            {
            aCandidate.SetFoundationL( candToCompare->Foundation() );
            return;
            }
        }
    
    const TDesC8& foundation = GenerateRandomIdentifier();
    aCandidate.SetFoundationL( foundation );
    }


// ---------------------------------------------------------------------------
// CIceSessionData::FindStreamCollectionById
// ---------------------------------------------------------------------------
//
CIceStreamCollection* 
    CIceSessionData::FindStreamCollectionById( TUint aCollectionId )
    {
    TInt ind( iStreamCollections.Count() );
    while ( ind-- )
        {
        if ( iStreamCollections[ind]->StreamCollectionId() == aCollectionId )
            {
            return iStreamCollections[ind];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::GenerateRandomIdentifier
// ---------------------------------------------------------------------------
//
const TDesC8& CIceSessionData::GenerateRandomIdentifier()
    {
    TUint32 randomNum = TUint32( Math::Random() + TUint32( Math::Random() ) );
    _LIT8( KFormatString, "%u" );
    iTempIdentifier.Format( KFormatString, randomNum );
    return iTempIdentifier;
    }


// ---------------------------------------------------------------------------
// CIceSessionData::UpdateUserNamesL
// Both inbound & outbound credentials are always updated.
// ---------------------------------------------------------------------------
//
void CIceSessionData::UpdateUserNamesL( 
        CNATFWCredentials& aCredentials )
    {
    CNATFWCredentials::TUtilizationDirection direction =
        CNATFWCredentials::EInbound == aCredentials.Direction()
        ? CNATFWCredentials::EOutbound
        : CNATFWCredentials::EInbound;
    
    const CNATFWCredentials* counterpart 
        = Credentials( aCredentials.StreamId(), direction );
    
    if ( counterpart )
        {
        // Complete credentials with username of its counterpart
        RBuf8 newUserName;
        CleanupClosePushL( newUserName );
        newUserName.CreateL( aCredentials.Username().Size() 
            + counterpart->Username().Size()
            + KColon().Size() );
        
        TInt ind = aCredentials.Username().Find( KColon );
        KErrNotFound == ind 
            ? ind = aCredentials.Username().Size() : ind = ind;
        newUserName.Append( aCredentials.Username().Left( ind ) );
        newUserName.Append( KColon() );
        
        ind = counterpart->Username().Find( KColon() );
        KErrNotFound == ind 
            ? ind = counterpart->Username().Size() : ind = ind;
        newUserName.Append( counterpart->Username().Left( ind ) );
        aCredentials.SetUsernameL( newUserName );
        
        // Update also username of counterpart
        newUserName.Zero();
        ind = counterpart->Username().Find( KColon );
        KErrNotFound == ind 
            ? ind = counterpart->Username().Size() : ind = ind;
        newUserName.Append( counterpart->Username().Left( ind ) );
        newUserName.Append( KColon() );

        ind = aCredentials.Username().Find( KColon() );
        KErrNotFound == ind 
            ? ind = aCredentials.Username().Size() : ind = ind;
        newUserName.Append( aCredentials.Username().Left( ind ) );
        const_cast<CNATFWCredentials*>( counterpart )
            ->SetUsernameL( newUserName );
        
        CleanupStack::PopAndDestroy( &newUserName );
        }
    }
