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




#include "cnatfwunsaftcprelaypacketsender.h"
#include "cnatfwunsafencodeditem.h"
#include "natfwunsaflog.h"
#include "mncmsenderobserver.h"
#include "mncmconnectionmultiplexer.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketSender*
CNATFWUNSAFTcpRelayPacketSender::NewL(
    MNcmConnectionMultiplexer& aMediaConnection )
    {
    CNATFWUNSAFTcpRelayPacketSender* self =
        CNATFWUNSAFTcpRelayPacketSender::NewLC( aMediaConnection );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketSender*
CNATFWUNSAFTcpRelayPacketSender::NewLC(
    MNcmConnectionMultiplexer& aMediaConnection )
    {
    CNATFWUNSAFTcpRelayPacketSender* self =
        new ( ELeave ) CNATFWUNSAFTcpRelayPacketSender( aMediaConnection );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::CNATFWUNSAFTcpRelayPacketSender
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacketSender::CNATFWUNSAFTcpRelayPacketSender(
    MNcmConnectionMultiplexer& aMediaConnection ) :
    iMediaConnection( aMediaConnection )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::~CNATFWUNSAFTcpRelayPacketSender
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketSender::~CNATFWUNSAFTcpRelayPacketSender()
    {
    delete iEncoded;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::SendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFTcpRelayPacketSender::SendL(
    const TUint aStreamId,
    const TUint aSubstreamId,
    const CNATFWUNSAFTcpRelayPacket& aMessage,
    MNcmSenderObserver* aObserver )
    {
    EncodeL( aMessage, aStreamId, aSubstreamId );
    SendToMediaConnectionL( aObserver );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::EncodeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::EncodeL(
    const CNATFWUNSAFTcpRelayPacket& aMessage,
    const TUint aStreamId,
    const TUint aSubstreamId )
    {
    RemoveEncoded();
    iEncoded = CNATFWUNSAFEncodedItem::NewL(
        aMessage, aStreamId, aSubstreamId );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::RemoveEncoded
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::RemoveEncoded()
    {
    delete iEncoded;
    iEncoded = 0;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL(
    MNcmSenderObserver* aObserver )
    {
    __ASSERT_ALWAYS( iEncoded, User::Leave( KErrNotReady ) );

    iCurrent.Set( iEncoded->Ptr() );

    NATFWUNSAF_INTLOG(
        "CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL stream ID",
            iEncoded->StreamId() )
    NATFWUNSAF_INTLOG(
        "CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL substream ID",
            iEncoded->SubstreamId() )
    NATFWUNSAF_BYTESTREAMLOG(
        "CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL data",
            iCurrent )

    iMediaConnection.SendL(
        iEncoded->StreamId(), iEncoded->SubstreamId(), iCurrent, aObserver );
    }

// End of File

