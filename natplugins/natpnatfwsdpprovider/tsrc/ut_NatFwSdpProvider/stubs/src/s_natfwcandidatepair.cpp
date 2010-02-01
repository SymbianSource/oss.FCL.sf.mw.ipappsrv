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
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNATFWCandidatePair::CNATFWCandidatePair
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair::CNATFWCandidatePair() 
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::CNATFWCandidatePair" )
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::ConstructL()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::ConstructL" )
    
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
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::ConstructL (copy)" )
    
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
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::ConstructL (parametrized)" )
    
    iLocalCand = CNATFWCandidate::NewL( aLocalCand );
    iRemoteCand = CNATFWCandidate::NewL( aRemoteCand );
    
    UpdateFoundationL();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair* CNATFWCandidatePair::NewL()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewL" )
    
    CNATFWCandidatePair* self = CNATFWCandidatePair::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair* CNATFWCandidatePair::NewLC()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewLC" )
    
    CNATFWCandidatePair* self = new( ELeave ) CNATFWCandidatePair();
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair* CNATFWCandidatePair::NewL( 
        const CNATFWCandidatePair& aPair )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewL (copy)" )
    
    CNATFWCandidatePair* self = CNATFWCandidatePair::NewLC( aPair );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//    
CNATFWCandidatePair* CNATFWCandidatePair::NewLC( 
        const CNATFWCandidatePair& aPair )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewLC (copy)" )
    
    CNATFWCandidatePair* self = new( ELeave ) CNATFWCandidatePair();
    CleanupStack::PushL( self );
    self->ConstructL( aPair );
    
    return self;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewL
// ---------------------------------------------------------------------------
//    
CNATFWCandidatePair* CNATFWCandidatePair::NewL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewL (parametrized)" )
    
    CNATFWCandidatePair* self 
        = CNATFWCandidatePair::NewLC( aLocalCand, aRemoteCand );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::NewLC
// ---------------------------------------------------------------------------
//    
CNATFWCandidatePair* CNATFWCandidatePair::NewLC(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::NewLC (parametrized)" )
    
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
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::~CNATFWCandidatePair" )
    
    delete iRemoteCand;
    delete iLocalCand;
    iFoundation.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Foundation
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWCandidatePair::Foundation() const
    {
    NSPLOG8_STR2( "(STUB)CNATFWCandidatePair::Foundation", iFoundation )
    
    return iFoundation;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetLocalCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWCandidatePair::SetLocalCandidateL( 
        const CNATFWCandidate& aCandidate )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::SetLocalCandidateL" )
    
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
const CNATFWCandidate& CNATFWCandidatePair::LocalCandidate() const
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::LocalCandidate" )
    
    return *iLocalCand;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetRemoteCandidateL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::SetRemoteCandidateL( 
        const CNATFWCandidate& aCandidate )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::SetRemoteCandidateL" )
    
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
const CNATFWCandidate& CNATFWCandidatePair::RemoteCandidate() const
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::RemoteCandidate" )
    
    return *iRemoteCand;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetPriority
// ---------------------------------------------------------------------------
//    
void CNATFWCandidatePair::SetPriority( const TUint64 aPriority )
    {
    NSPLOG_INT( "(STUB)CNATFWCandidatePair::SetPriority", 
        static_cast<TUint>( aPriority ) )
    
    iPairPriority = aPriority;
    }

// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Priority
// ---------------------------------------------------------------------------
//    
TUint64 CNATFWCandidatePair::Priority() const
    {
    NSPLOG_INT( "(STUB)CNATFWCandidatePair::Priority", 
        static_cast<TUint>( iPairPriority ) )
    
    return iPairPriority;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::SetSelected
// ---------------------------------------------------------------------------
//
void CNATFWCandidatePair::SetSelected( TBool aSelected )
    {
    NSPLOG_INT( "(STUB)CNATFWCandidatePair::SetSelected", aSelected )
    
    iSelected = aSelected;
    }
   
    
// ---------------------------------------------------------------------------
// CNATFWCandidatePair::Selected
// ---------------------------------------------------------------------------
//    
TBool CNATFWCandidatePair::Selected() const
    {
    NSPLOG_INT( "(STUB)CNATFWCandidatePair::Selected", iSelected )
    
    return iSelected;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::PriorityOrder
// Implements TLinearOrder (descending order).
// ---------------------------------------------------------------------------
//
TInt CNATFWCandidatePair::PriorityOrder( 
        const CNATFWCandidatePair& aA,
        const CNATFWCandidatePair& aB  )     
    {
    return static_cast<TInt>( aB.Priority() - aA.Priority() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidatePair::CopyL
// ---------------------------------------------------------------------------
//
void CNATFWCandidatePair::CopyL( const CNATFWCandidatePair& aPair )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::CopyL" )
    
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
TBool CNATFWCandidatePair::operator==( 
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
TBool CNATFWCandidatePair::operator!=(
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
TBool CNATFWCandidatePair::Compare(
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
TBool CNATFWCandidatePair::MatchAddresses( 
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
    NSPLOG_STR( "(STUB)CNATFWCandidatePair::UpdateFoundationL" )
    
    if ( iLocalCand && iRemoteCand )
        {
        iFoundation.Close();
        iFoundation.CreateL( iLocalCand->Foundation().Size() 
            + iRemoteCand->Foundation().Size() );
        iFoundation.Append( iLocalCand->Foundation() );
        iFoundation.Append( iRemoteCand->Foundation() );
        }
    }

