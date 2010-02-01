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




#include "mncmconnectionmultiplexer.h"
#include "cnatfwunsafmediaconnsender.h"
#include "cnatfwunsafencodeditem.h"
#include "natfwunsaflog.h"
#include "mncmsenderobserver.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMediaConnSender* CNATFWUNSAFMediaConnSender::NewL(
    MNcmConnectionMultiplexer& aMediaConnection )
    {
    CNATFWUNSAFMediaConnSender* self = CNATFWUNSAFMediaConnSender::NewLC(
        aMediaConnection );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMediaConnSender* CNATFWUNSAFMediaConnSender::NewLC(
    MNcmConnectionMultiplexer& aMediaConnection )
    {
    CNATFWUNSAFMediaConnSender* self =
        new ( ELeave ) CNATFWUNSAFMediaConnSender(aMediaConnection);
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::CNATFWUNSAFMediaConnSender
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMediaConnSender::CNATFWUNSAFMediaConnSender(
    MNcmConnectionMultiplexer& aMediaConnection ) :
    iMediaConnection( aMediaConnection )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::~CNATFWUNSAFMediaConnSender
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMediaConnSender::~CNATFWUNSAFMediaConnSender()
    {
    delete iEncoded;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::SendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMediaConnSender::SendL(
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/,
    const CNATFWUNSAFMessage& /*aMessage*/,
    const TDesC8& /*aSharedSecret*/,
    MNcmSenderObserver* /*aObserver*/,
    const TUint /*aUseFingerprint*/ )
    {
    }

// -----------------------------------------------------------------------------
// Overloaded
// CNATFWUNSAFMediaConnSender::SendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMediaConnSender::SendL(
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/,
    const CNATFWUNSAFMessage& /*aMessage*/,
    const TDesC8& /*aSharedSecret*/,
    const TInetAddr& /*aDestinationAddress*/,
    MNcmSenderObserver* /*aObserver*/,
    const TUint /*aUseFingerprint*/ )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::CancelL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMediaConnSender::Cancel(
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/,
    MNcmSenderObserver* /*aObserver*/ ) const
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::EncodeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMediaConnSender::EncodeL(
    const CNATFWUNSAFMessage& /*aMessage*/,
    const TDesC8& /*aSharedSecret*/,
    const TUint /*aStreamId*/,
    const TUint /*aSubstreamId*/,
    const TUint /*aUseFingerprint*/ )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::RemoveEncoded
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMediaConnSender::RemoveEncoded()
    {
    delete iEncoded;
    iEncoded = 0;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMediaConnSender::SendToMediaConnectionL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMediaConnSender::SendToMediaConnectionL(
    MNcmSenderObserver* /*aObserver*/,
    const TInetAddr& /*aDestinationAddress*/ )
    {
    }

// End of File

