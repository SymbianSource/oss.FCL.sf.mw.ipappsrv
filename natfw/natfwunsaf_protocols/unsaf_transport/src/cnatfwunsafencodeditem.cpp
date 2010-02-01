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
* Description:    implementation
*
*/




// INCLUDE FILES
#include "cnatfwunsafencodeditem.h"
#include "natfwunsafmessage.h"
#include "natfwunsaftcprelaypacket.h"
#include <in_sock.h>


// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFEncodedItem* CNATFWUNSAFEncodedItem::NewL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8* aSharedSecret,
    const TInetAddr* aAddr,
    const TUint aStreamId,
    const TUint aSubstreamId,
    const TUint aUseFingerprint )
    {
    CNATFWUNSAFEncodedItem* self =
        CNATFWUNSAFEncodedItem::NewLC(
            aMessage, aSharedSecret, aAddr, aStreamId, aSubstreamId,
                aUseFingerprint );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFEncodedItem* CNATFWUNSAFEncodedItem::NewLC(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8* aSharedSecret,
    const TInetAddr* aAddr,
    const TUint aStreamId,
    const TUint aSubstreamId,
    const TUint aUseFingerprint )
    {
    CNATFWUNSAFEncodedItem* self = new ( ELeave ) CNATFWUNSAFEncodedItem();
    CleanupStack::PushL( self );
    self->ConstructL( aMessage, aSharedSecret, aAddr, aStreamId, aSubstreamId,
        aUseFingerprint );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFEncodedItem* CNATFWUNSAFEncodedItem::NewL(
    const CNATFWUNSAFTcpRelayPacket& aMessage,
    const TUint aStreamId,
    const TUint aSubstreamId )
    {
    CNATFWUNSAFEncodedItem* self =
        CNATFWUNSAFEncodedItem::NewLC( aMessage, aStreamId, aSubstreamId );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFEncodedItem* CNATFWUNSAFEncodedItem::NewLC(
    const CNATFWUNSAFTcpRelayPacket& aMessage,
    const TUint aStreamId,
    const TUint aSubstreamId )
    {
    __ASSERT_ALWAYS( aStreamId && aSubstreamId, User::Leave( KErrArgument ) );
    CNATFWUNSAFEncodedItem* self = new ( ELeave ) CNATFWUNSAFEncodedItem();
    CleanupStack::PushL( self );
    self->ConstructL( aMessage, aStreamId, aSubstreamId );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::~CNATFWUNSAFEncodedItem
// -----------------------------------------------------------------------------
//
CNATFWUNSAFEncodedItem::~CNATFWUNSAFEncodedItem()
    {
    delete iAddr;
    delete iEncoded;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::Ptr
// -----------------------------------------------------------------------------
//
TPtr8 CNATFWUNSAFEncodedItem::Ptr()
    {
    return iEncoded->Ptr( 0 );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::StreamIdL
// -----------------------------------------------------------------------------
//
TUint CNATFWUNSAFEncodedItem::StreamId() const
    {
    return iStreamId;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::SubstreamIdL
// -----------------------------------------------------------------------------
//
TUint CNATFWUNSAFEncodedItem::SubstreamId() const
    {
    return iSubstreamId;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::AddressL
// -----------------------------------------------------------------------------
//
TInetAddr& CNATFWUNSAFEncodedItem::AddressL()
    {
    __ASSERT_ALWAYS( iAddr, User::Leave( KErrNotFound ) );

    return *iAddr;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFEncodedItem::ConstructL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8* aSharedSecret,
    const TInetAddr* aAddr,
    const TUint aStreamId,
    const TUint aSubstreamId,
    const TUint aUseFingerprint )
    {
    if ( aAddr )
        {
        iAddr = new ( ELeave ) TInetAddr( *aAddr );
        }
    else if ( aStreamId && aSubstreamId )
        {
        iStreamId = aStreamId;
        iSubstreamId = aSubstreamId;
        }
    if ( aSharedSecret )
        {
        iEncoded = aMessage.EncodeL( *aSharedSecret, aUseFingerprint );
        }
    else
        {
        iEncoded = aMessage.EncodeL(KNullDesC8, aUseFingerprint );
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFEncodedItem::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFEncodedItem::ConstructL(
    const CNATFWUNSAFTcpRelayPacket& aMessage,
    const TUint aStreamId,
    const TUint aSubstreamId )
    {
    iStreamId = aStreamId;
    iSubstreamId = aSubstreamId;
    iEncoded = aMessage.EncodeL();
    }

// End of File
