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
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/,
    const CNATFWUNSAFTcpRelayPacket& /*aMessage*/,
    MNcmSenderObserver* /*aObserver*/ )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::EncodeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::EncodeL(
    const CNATFWUNSAFTcpRelayPacket& /*aMessage*/,
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::RemoveEncoded
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::RemoveEncoded()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacketSender::SendToMediaConnectionL(
    MNcmSenderObserver* /*aObserver*/ )
    {
    }

// End of File

