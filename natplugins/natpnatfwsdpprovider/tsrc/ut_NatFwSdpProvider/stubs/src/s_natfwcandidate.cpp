/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



#include <es_sock.h>
#include "natfwcandidate.h"
#include "nspdefs.h"

// CONSTANTS
const TInt KMaxLengthOfFQDN = 255;
_LIT8( KValidFQDNChars,
       "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-" );

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNATFWCandidate::CNATFWCandidate
// ---------------------------------------------------------------------------
//
CNATFWCandidate::CNATFWCandidate( )
    :
    iTransportAddr( KInetAddrNone, 0 ),
    iBase( KInetAddrNone, 0 )
    {
    }


// ---------------------------------------------------------------------------
//  CNATFWCandidate::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::ConstructL()
    {
    iFoundation.Assign( KNullDesC8().AllocL() );
    iTransportDomainAddr.Assign( KNullDesC8().AllocL() );
    iTransportAddr = TInetAddr();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidate::ConstructL( const CNATFWCandidate& aCandidate )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::ConstructL" )
    
    iPriority = aCandidate.Priority();
    iTransportAddr = aCandidate.TransportAddr();
    iTransportProtocol = aCandidate.TransportProtocol();
    iTransportDomainAddr.Assign( aCandidate.TransportDomainAddr().AllocL() );
    iTransportDomainPort = aCandidate.TransportDomainPort();
    iBase = aCandidate.Base();
    iFoundation.Assign( aCandidate.Foundation().AllocL() );
    iType = aCandidate.Type();
    iComponentId = aCandidate.ComponentId();
    iSessionId = aCandidate.SessionId();
    iStreamId = aCandidate.StreamId();
    iStreamCollectionId = aCandidate.StreamCollectionId();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::NewL
// ---------------------------------------------------------------------------
//
CNATFWCandidate* CNATFWCandidate::NewL()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::NewL" )
    
    CNATFWCandidate* self = CNATFWCandidate::NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWCandidate::NewLC
// ---------------------------------------------------------------------------
//
CNATFWCandidate* CNATFWCandidate::NewLC()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::NewLC" )
    
    CNATFWCandidate* self = new( ELeave ) CNATFWCandidate();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::NewL
// ---------------------------------------------------------------------------
//
CNATFWCandidate* CNATFWCandidate::NewL( 
        const CNATFWCandidate& aCandidate )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::NewL(copy)" )
    
    CNATFWCandidate* self = CNATFWCandidate::NewLC( aCandidate );
    CleanupStack::Pop( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// CNATFWCandidate::NewLC
// ---------------------------------------------------------------------------
//    
CNATFWCandidate* CNATFWCandidate::NewLC( 
        const CNATFWCandidate& aCandidate )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::NewLC(copy)" )
    
    CNATFWCandidate* self = new ( ELeave ) CNATFWCandidate( );
    CleanupStack::PushL( self );
    self->ConstructL( aCandidate );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::~CNATFWCandidate
// ---------------------------------------------------------------------------
//    
CNATFWCandidate::~CNATFWCandidate()
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::~CNATFWCandidate" )
    
    iTransportDomainAddr.Close();
    iFoundation.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SessionId
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::SessionId() const
    {
    NSPLOG_INT( "(STUB)CNATFWCandidate::SessionId", iSessionId )
    
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetSessionId
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetSessionId( TUint aId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetSessionId", aId )
        
    iSessionId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::StreamCollectionId
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::StreamCollectionId() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::StreamCollectionId",
        iStreamCollectionId )
    
    return iStreamCollectionId;
    }

    
// ---------------------------------------------------------------------------
// SetStreamCollectionId
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetStreamCollectionId( TUint aId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetStreamCollectionId", aId )
    
    iStreamCollectionId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::StreamId
// ---------------------------------------------------------------------------
//     
TUint CNATFWCandidate::StreamId() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::StreamId", iStreamId )
    
    return iStreamId;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidate::SetStreamId
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetStreamId( TUint aId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetStreamId", aId )
    
    iStreamId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Priority
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::Priority() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::Priority", iPriority )
    
    return iPriority;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetPriority
// ---------------------------------------------------------------------------
//    
void CNATFWCandidate::SetPriority( TUint aPriority )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetPriority", aPriority )
    
    iPriority = aPriority;
    }


// ---------------------------------------------------------------------------
//  CNATFWCandidate::TransportAddr
// ---------------------------------------------------------------------------
//     
const TInetAddr& CNATFWCandidate::TransportAddr() const
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::TransportAddr" )
    
    return iTransportAddr;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportAddrL
// ---------------------------------------------------------------------------
//    
void CNATFWCandidate::SetTransportAddrL( const TInetAddr& aAddr )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::SetTransportAddr" )
    
    iTransportAddr = aAddr;

    if ( !iTransportDomainAddr.Length() )
        {
        // Update domain address as well
        THostName name;
        aAddr.Output( name );

        HBufC8* addrBuf = HBufC8::NewLC( name.Length() );
        TPtr8 addrBufPtr = addrBuf->Des();
        
        iTransportDomainAddr.Close();
        iTransportDomainAddr.Assign( addrBuf ); // takes ownership
        iTransportDomainPort = aAddr.Port();
        CleanupStack::Pop( addrBuf );
        }
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::TransportDomainAddr
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWCandidate::TransportDomainAddr() const
    {
    NSPLOG8_STR2( "(STUB)CNATFWCandidate::TransportDomainAddr",
        iTransportDomainAddr )
    
    return iTransportDomainAddr;
    }

// ---------------------------------------------------------------------------
// CNATFWCandidate::TransportDomainPort
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::TransportDomainPort() const
    {
    NSPLOG_INT( "(STUB)CNATFWCandidate::TransportDomainAddrPort",
        iTransportDomainPort )
    
    return iTransportDomainPort;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportDomainAddrL
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetTransportDomainAddrL( const TDesC8& aAddr,
    TUint aPort )
    {
    NSPLOG8_STR2( "(STUB)CNATFWCandidate::SetTransportDomainAddr - domain:", aAddr )
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetTransportDomainAddr - port:",aPort )

    // Verify that the domain name doesn't exceed maximum length for FQDNs
    if ( KMaxLengthOfFQDN < aAddr.Length() )
        {
        User::Leave( KErrOverflow );
        }

    // Verify that the domain name contains only valid FQDN characters
    for ( TInt i( 0 ); i < aAddr.Length(); i++ )
        {
        if ( KErrNotFound == KValidFQDNChars().Locate( aAddr[ i ] ) )
            {
            User::Leave( KErrArgument );
            }
        }

    iTransportDomainAddr.Close();
    iTransportDomainAddr.Assign( aAddr.AllocL() );
    iTransportDomainPort = aPort;

    TInetAddr addr;
    HBufC* addrBuf = HBufC::NewLC( aAddr.Length() );
    TPtr addrPtr( addrBuf->Des() );
    
    if ( !addr.Input( addrPtr ) )
        {
        // This is a valid IP address so set the transport address as well
        iTransportAddr = addr;
        iTransportAddr.SetPort( aPort );
        }

    CleanupStack::PopAndDestroy( addrBuf );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::TransportProtocol
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::TransportProtocol() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::TransportProtocol", iTransportProtocol )
    
    return iTransportProtocol;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportProtocol
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetTransportProtocol(
        TUint aTransportProtocol )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetTransportProtocol", aTransportProtocol )
    
    iTransportProtocol = aTransportProtocol;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Base
// ---------------------------------------------------------------------------
//     
const TInetAddr& CNATFWCandidate::Base() const
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::Base" )
    
    return iBase;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidate::SetBase
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetBase( const TInetAddr& aBase )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::SetBase" )
    
    iBase = aBase;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Foundation
// ---------------------------------------------------------------------------
//     
const TDesC8& CNATFWCandidate::Foundation() const
    {
    NSPLOG8_STR2( "(STUB)CNATFWCandidate::Foundation", iFoundation )
    
    return iFoundation;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidate::SetFoundationL
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetFoundationL( const TDesC8& aFoundation )
    {
    NSPLOG8_STR2( "(STUB)CNATFWCandidate::SetFoundationL", aFoundation )
    
    iFoundation.Close();
    iFoundation.Assign( aFoundation.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Type
// ---------------------------------------------------------------------------
//     
CNATFWCandidate::TCandidateType CNATFWCandidate::Type() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::Type", iType )
    
    return iType;
    }
    

// ---------------------------------------------------------------------------
// CNATFWCandidate::SetType
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetType( 
        CNATFWCandidate::TCandidateType aType )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetType", aType )
    
    iType = aType;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::ComponentId
// ---------------------------------------------------------------------------
//
TUint CNATFWCandidate::ComponentId() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::ComponentId", iComponentId )
    
    return iComponentId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetComponentId
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::SetComponentId( TUint aComponentId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCandidate::SetComponentId", aComponentId )
    
    iComponentId = aComponentId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::CompareFoundations
// ---------------------------------------------------------------------------
//
TBool CNATFWCandidate::CompareFoundations( 
    const CNATFWCandidate& aSource, const CNATFWCandidate& aTarget )
    {
    NSPLOG_STR( "(STUB)CNATFWCandidate::CompareFoundations" )
    
    return ( aSource.iFoundation == aTarget.iFoundation );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::PriorityOrder
// Implements TLinearOrder.
// ---------------------------------------------------------------------------
//
TInt CNATFWCandidate::PriorityOrder( const CNATFWCandidate& aC1,
        const CNATFWCandidate& aC2 )
    {
    return ( aC2.iPriority - aC1.iPriority );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::CopyL
// ---------------------------------------------------------------------------
//
void CNATFWCandidate::CopyL( const CNATFWCandidate& aCandidate )
    {
    iPriority = aCandidate.Priority();
    iTransportAddr = aCandidate.TransportAddr();
    iTransportProtocol = aCandidate.TransportProtocol();
    iTransportDomainAddr.Close();
    iTransportDomainAddr.Assign( aCandidate.TransportDomainAddr().AllocL() );
    iTransportDomainPort = aCandidate.TransportDomainPort();
    iBase = aCandidate.Base();
    iFoundation.Close();
    iFoundation.Assign( aCandidate.Foundation().AllocL() );
    iType = aCandidate.Type();
    iComponentId = aCandidate.ComponentId();
    iSessionId = aCandidate.SessionId();
    iStreamId = aCandidate.StreamId();
    iStreamCollectionId = aCandidate.StreamCollectionId();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::operator==
// ---------------------------------------------------------------------------
//
TBool CNATFWCandidate::operator==( 
        const CNATFWCandidate& aCandidate ) const
    {
    return (
        iPriority == aCandidate.Priority()
        && iTransportAddr.CmpAddr( aCandidate.TransportAddr() )
        && iTransportDomainAddr == aCandidate.TransportDomainAddr()
        && iTransportDomainPort == aCandidate.TransportDomainPort()
        && iTransportProtocol == aCandidate.TransportProtocol()
        && iBase.CmpAddr( aCandidate.Base() )
        && iFoundation == aCandidate.Foundation()
        && iType == aCandidate.Type()
        && iComponentId == aCandidate.ComponentId()
        && iSessionId == aCandidate.SessionId()
        && iStreamId == aCandidate.StreamId()
        && iStreamCollectionId == aCandidate.StreamCollectionId() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::operator!=
// ---------------------------------------------------------------------------
//
TBool CNATFWCandidate::operator!=(
        const CNATFWCandidate& aCandidate ) const
    {
    return (
        iPriority != aCandidate.Priority()
        || !iTransportAddr.CmpAddr( aCandidate.TransportAddr() )
        || iTransportDomainAddr != aCandidate.TransportDomainAddr()
        || iTransportDomainPort != aCandidate.TransportDomainPort()
        || iTransportProtocol != aCandidate.TransportProtocol()
        || !iBase.CmpAddr( aCandidate.Base() )
        || iFoundation != aCandidate.Foundation()
        || iType != aCandidate.Type()
        || iComponentId != aCandidate.ComponentId()
        || iSessionId != aCandidate.SessionId()
        || iStreamId != aCandidate.StreamId()
        || iStreamCollectionId != aCandidate.StreamCollectionId() );
    }
