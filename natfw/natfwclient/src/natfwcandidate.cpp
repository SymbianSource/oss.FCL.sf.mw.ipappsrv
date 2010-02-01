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
#include <utf.h>
#include "natfwcandidate.h"
#include "natfwclientlogs.h"

// CONSTANTS
const TInt KMaxLengthOfFQDN = 255;
_LIT8(KValidFQDNChars,
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-");

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
    __NATFWCLIENT( "CNATFWCandidate::ConstructL" )

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
EXPORT_C CNATFWCandidate* CNATFWCandidate::NewL()
    {
    __NATFWCLIENT("CNATFWCandidate::NewL")

    CNATFWCandidate* self = CNATFWCandidate::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidate* CNATFWCandidate::NewLC()
    {
    __NATFWCLIENT("CNATFWCandidate::NewLC")

    CNATFWCandidate* self = new( ELeave ) CNATFWCandidate();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidate* CNATFWCandidate::NewL(
        const CNATFWCandidate& aCandidate )
    {
    __NATFWCLIENT("CNATFWCandidate::NewL (copy)")

    CNATFWCandidate* self = CNATFWCandidate::NewLC( aCandidate );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidate* CNATFWCandidate::NewLC(
        const CNATFWCandidate& aCandidate )
    {
    __NATFWCLIENT("CNATFWCandidate::NewLC(copy)")

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
    __NATFWCLIENT("CNATFWCandidate::~CNATFWCandidate")

    iTransportDomainAddr.Close();
    iFoundation.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SessionId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::SessionId() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SessionId", iSessionId )

    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetSessionId
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetSessionId( TUint aId )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetSessionId", aId )

    iSessionId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::StreamCollectionId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::StreamCollectionId() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::StreamCollectionId",
        iStreamCollectionId )

    return iStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// SetStreamCollectionId
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetStreamCollectionId( TUint aId )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetStreamCollectionId", aId )

    iStreamCollectionId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::StreamId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::StreamId() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::StreamId", iStreamId )

    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetStreamId
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetStreamId( TUint aId )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetStreamId", aId )

    iStreamId = aId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Priority
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::Priority() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::Priority", iPriority )

    return iPriority;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetPriority
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetPriority( TUint aPriority )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetPriority", aPriority )

    iPriority = aPriority;
    }


// ---------------------------------------------------------------------------
//  CNATFWCandidate::TransportAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CNATFWCandidate::TransportAddr() const
    {
    __NATFWCLIENT_ADDR( "CNATFWCandidate::TransportAddr", iTransportAddr )

    return iTransportAddr;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportAddrL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetTransportAddrL( const TInetAddr& aAddr )
    {
    __NATFWCLIENT_ADDR( "CNATFWCandidate::SetTransportAddrL", aAddr )

    iTransportAddr = aAddr;

    if ( !iTransportDomainAddr.Length() )
        {
        // Update domain address as well
        THostName name;
        aAddr.Output( name );

        HBufC8* addrBuf = HBufC8::NewLC( name.Length() );
        TPtr8 addrBufPtr = addrBuf->Des();
        User::LeaveIfError(
            CnvUtfConverter::ConvertFromUnicodeToUtf8( addrBufPtr, name ) );

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
EXPORT_C const TDesC8& CNATFWCandidate::TransportDomainAddr() const
    {
    __NATFWCLIENT_STR8( "CNATFWCandidate::TransportDomainAddr",
        iTransportDomainAddr )

    return iTransportDomainAddr;
    }

// ---------------------------------------------------------------------------
// CNATFWCandidate::TransportDomainPort
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::TransportDomainPort() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::TransportDomainAddrPort",
        iTransportDomainPort)

    return iTransportDomainPort;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportDomainAddrL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetTransportDomainAddrL( const TDesC8& aAddr,
    TUint aPort )
    {
    __NATFWCLIENT_STR( "CNATFWCandidate::SetTransportDomainAddrL - domain",
        aAddr )
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetTransportDomainAddrL - port",
        aPort )

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
    User::LeaveIfError(
        CnvUtfConverter::ConvertToUnicodeFromUtf8( addrPtr, aAddr ) );

    if ( !addr.Input( addrPtr ) )
        {
        // This is a valid IP address so set the transport address as well
        iTransportAddr = addr;
        iTransportAddr.SetPort( aPort );
        __NATFWCLIENT_ADDR(
            "CNATFWCandidate::SetTransportDomainAddrL: IP now",
                iTransportAddr )
        }

    CleanupStack::PopAndDestroy( addrBuf );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::TransportProtocol
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::TransportProtocol() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::TransportProtocol",
        iTransportProtocol )

    return iTransportProtocol;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetTransportProtocol
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetTransportProtocol(
        TUint aTransportProtocol )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetTransportProtocol",
        aTransportProtocol )

    iTransportProtocol = aTransportProtocol;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Base
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CNATFWCandidate::Base() const
    {
    __NATFWCLIENT_ADDR( "CNATFWCandidate::Base", iBase )

    return iBase;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetBase
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetBase( const TInetAddr& aBase )
    {
    __NATFWCLIENT_ADDR( "CNATFWCandidate::SetBase", aBase )

    iBase = aBase;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Foundation
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWCandidate::Foundation() const
    {
    __NATFWCLIENT_STR8( "CNATFWCandidate::Foundation", iFoundation )

    return iFoundation;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetFoundationL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetFoundationL( const TDesC8& aFoundation )
    {
    __NATFWCLIENT_STR8( "CNATFWCandidate::SetFoundationL", aFoundation )

    iFoundation.Close();
    iFoundation.Assign( aFoundation.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::Type
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCandidate::TCandidateType CNATFWCandidate::Type() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::Type", iType )

    return iType;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetType
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetType(
        CNATFWCandidate::TCandidateType aType )
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetType", aType )

    iType = aType;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::ComponentId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWCandidate::ComponentId() const
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::ComponentId", iComponentId )

    return iComponentId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::SetComponentId
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::SetComponentId( TUint aComponentId)
    {
    __NATFWCLIENT_INT1( "CNATFWCandidate::SetComponentId", aComponentId )

    iComponentId = aComponentId;
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::CompareFoundations
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCandidate::CompareFoundations(
    const CNATFWCandidate& aSource, const CNATFWCandidate& aTarget )
    {
    __NATFWCLIENT( "CNATFWCandidate::CompareFoundations" )

    return ( aSource.iFoundation == aTarget.iFoundation );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::PriorityOrder
// Implements TLinearOrder.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWCandidate::PriorityOrder( const CNATFWCandidate& aC1,
        const CNATFWCandidate& aC2 )
    {
    return ( aC2.iPriority - aC1.iPriority );
    }


// ---------------------------------------------------------------------------
// CNATFWCandidate::CopyL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCandidate::CopyL( const CNATFWCandidate& aCandidate )
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
EXPORT_C TBool CNATFWCandidate::operator==(
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
EXPORT_C TBool CNATFWCandidate::operator!=(
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
