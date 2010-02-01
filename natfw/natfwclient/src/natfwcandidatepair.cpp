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
* Description:    
*
*/




#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "natfwcredentials.h"
#include "natfwclientlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWCandidatePair::CNATFWCandidatePair
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair::CNATFWCandidatePair() 
    {
    __NATFWCLIENT( "CNATFWCandidatePair::CNATFWCandidatePair" )
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::ConstructL()
    {
    __NATFWCLIENT( "CNATFWCandidatePair::ConstructL" )
    
    iFoundation.Assign( KNullDesC8().AllocL() );
    iLocalCand = CNATFWCandidate::NewL();
    iRemoteCand = CNATFWCandidate::NewL();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::ConstructL( const CNATFWCandidatePair& aPair )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::ConstructL (copy)" )
    
    iFoundation.Assign( aPair.Foundation().AllocL() );
    iLocalCand = CNATFWCandidate::NewL( aPair.LocalCandidate() );
    iRemoteCand = CNATFWCandidate::NewL( aPair.RemoteCandidate() );
    iPairPriority = aPair.Priority();
    iSelected = aPair.Selected();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::ConstructL( 
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::ConstructL (parametrized)" )
    
    iLocalCand = CNATFWCandidate::NewL( aLocalCand );
    iRemoteCand = CNATFWCandidate::NewL( aRemoteCand );
    
    UpdateFoundationL();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewL()
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewL" )
    
    CNATFWCandidatePair* self = CNATFWCandidatePair::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewLC()
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewLC" )
    
    CNATFWCandidatePair* self = new( ELeave ) CNATFWCandidatePair();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewL( 
        const CNATFWCandidatePair& aPair )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewL (copy)" )
    
    CNATFWCandidatePair* self = CNATFWCandidatePair::NewLC( aPair );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//    
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewLC( 
        const CNATFWCandidatePair& aPair )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewLC (copy)" )
    
    CNATFWCandidatePair* self = new( ELeave ) CNATFWCandidatePair();
    CleanupStack::PushL( self );
    self->ConstructL( aPair );
    
    return self;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//    
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewL (parametrized)" )
    
    CNATFWCandidatePair* self 
        = CNATFWCandidatePair::NewLC( aLocalCand, aRemoteCand );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//    
EXPORT_C CNATFWCandidatePair* CNATFWCandidatePair::NewLC(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::NewLC (parametrized)" )
    
    CNATFWCandidatePair* self = new( ELeave ) CNATFWCandidatePair();
    CleanupStack::PushL( self );
    self->ConstructL( aLocalCand, aRemoteCand );
    
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::~CNATFWCandidatePair
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair::~CNATFWCandidatePair()
    {
    __NATFWCLIENT( "CNATFWCandidatePair::~CNATFWCandidatePair" )
    
    delete iRemoteCand;
    delete iLocalCand;
    iFoundation.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Foundation
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWCandidatePair::Foundation() const
    {
    __NATFWCLIENT_STR8( "CNATFWCandidatePair::Foundation", iFoundation )
    
    return iFoundation;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetLocalCandidateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidatePair::SetLocalCandidateL( 
        const CNATFWCandidate& aCandidate )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::SetLocalCandidateL" )
    
    if ( NULL != iLocalCand )
        {
        iLocalCand->CopyL( aCandidate );
        }
    else
        {
        iLocalCand = CNATFWCandidate::NewL( aCandidate );
        }
    
    UpdateFoundationL();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::LocalCandidate
// ---------------------------------------------------------------------------
//    
EXPORT_C const CNATFWCandidate& CNATFWCandidatePair::LocalCandidate() const
    {
    __NATFWCLIENT( "CNATFWCandidatePair::LocalCandidate" )
    
    return *iLocalCand;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetRemoteCandidateL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWCandidatePair::SetRemoteCandidateL( 
        const CNATFWCandidate& aCandidate )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::SetRemoteCandidateL" )
    
    if ( NULL != iRemoteCand )
        {
        iRemoteCand->CopyL( aCandidate );
        }
    else
        {
        iRemoteCand = CNATFWCandidate::NewL( aCandidate );
        }
    
    UpdateFoundationL();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::RemoteCandidate
// ---------------------------------------------------------------------------
//    
EXPORT_C const CNATFWCandidate& CNATFWCandidatePair::RemoteCandidate() const
    {
    __NATFWCLIENT( "CNATFWCandidatePair::RemoteCandidate" )
    
    return *iRemoteCand;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetPriority
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWCandidatePair::SetPriority( const TUint64 aPriority )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidatePair::SetPriority", 
        static_cast<TUint>( aPriority ) )
    
    iPairPriority = aPriority;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Priority
// ---------------------------------------------------------------------------
//    
EXPORT_C TUint64 CNATFWCandidatePair::Priority() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidatePair::Priority", 
        static_cast<TUint>( iPairPriority ) )
    
    return iPairPriority;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetSelected
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidatePair::SetSelected( TBool aSelected )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidatePair::SetSelected", aSelected )
    
    iSelected = aSelected;
    }
   
    
// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Selected
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CNATFWCandidatePair::Selected() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidatePair::Selected", iSelected )
    
    return iSelected;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::PriorityOrder
// Implements TLinearOrder (descending order).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWCandidatePair::PriorityOrder( 
        const CNATFWCandidatePair& aA,
        const CNATFWCandidatePair& aB  )     
    {
    return static_cast<TInt>( aB.Priority() - aA.Priority() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::CopyL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidatePair::CopyL( const CNATFWCandidatePair& aPair )
    {
    __NATFWCLIENT( "CNATFWCandidatePair::CopyL" )
    
    if ( iLocalCand )
        {
        iLocalCand->CopyL( aPair.LocalCandidate() );
        }
    else
        {
        iLocalCand = CNATFWCandidate::NewL( aPair.LocalCandidate() );
        }
    
    if ( iRemoteCand )
        {
        iRemoteCand->CopyL( aPair.RemoteCandidate() );
        }
    else
        {
        iRemoteCand = CNATFWCandidate::NewL( aPair.RemoteCandidate() );
        }
    
    iPairPriority = aPair.Priority();
    iSelected = aPair.Selected();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::operator==
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCandidatePair::operator==( 
        const CNATFWCandidatePair& aPair ) const
    {
    return ( 
        iPairPriority == aPair.Priority()
        && *iLocalCand == aPair.LocalCandidate()
        && *iRemoteCand == aPair.RemoteCandidate()
        && iSelected == aPair.Selected() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::operator!=
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCandidatePair::operator!=(
        const CNATFWCandidatePair& aPair ) const
    {
    return !( 
        iPairPriority == aPair.Priority()
        && *iLocalCand == aPair.LocalCandidate()
        && *iRemoteCand == aPair.RemoteCandidate()
        && iSelected == aPair.Selected() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Compare
// Implements TIdentityRelation. Foundation is compared by comparing
// foundations of local and remote candidates.
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CNATFWCandidatePair::Compare(
        const CNATFWCandidatePair& aItem1,
        const CNATFWCandidatePair& aItem2 )
    {
    return (
        aItem1.Priority() == aItem2.Priority()
        && aItem1.LocalCandidate() == aItem2.LocalCandidate()
        && aItem1.RemoteCandidate() == aItem2.RemoteCandidate()
        && aItem1.Selected() == aItem2.Selected() );
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidatePair::MatchAddresses
// Implements TIdentityRelation.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCandidatePair::MatchAddresses( 
        const CNATFWCandidatePair& aItem1, 
        const CNATFWCandidatePair& aItem2 )
    {
    const TInetAddr& localAddrA( aItem1.LocalCandidate().TransportAddr() );
    const TInetAddr& localAddrB( aItem2.LocalCandidate().TransportAddr() );
    const TInetAddr& remoteAddrA( aItem1.RemoteCandidate().TransportAddr() );
    const TInetAddr& remoteAddrB( aItem2.RemoteCandidate().TransportAddr() );
    
    return ( ( localAddrA.CmpAddr( localAddrB ) ) 
        && ( remoteAddrA.CmpAddr( remoteAddrB ) ) );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::UpdateFoundationL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::UpdateFoundationL()
    {
    __NATFWCLIENT( "CNATFWCandidatePair::UpdateFoundationL" )
    
    if ( iLocalCand && iRemoteCand )
        {
        iFoundation.Close();
        iFoundation.CreateL( iLocalCand->Foundation().Size() 
            + iRemoteCand->Foundation().Size() );
        iFoundation.Append( iLocalCand->Foundation() );
        iFoundation.Append( iRemoteCand->Foundation() );
        }
    }
