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
#include "cnatfwunsafudpsender.h"
#include "mnatfwunsafudpsenderobserver.h"
#include "cnatfwunsafencodeditem.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUdpSender* CNATFWUNSAFUdpSender::NewL(
    RSocket& aSocket,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    {
    CNATFWUNSAFUdpSender* self = CNATFWUNSAFUdpSender::NewLC(
        aSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUdpSender* CNATFWUNSAFUdpSender::NewLC(
    RSocket& aSocket,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    {
    CNATFWUNSAFUdpSender* self =
        new ( ELeave ) CNATFWUNSAFUdpSender( aSocket, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::NewL - overloaded
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUdpSender* CNATFWUNSAFUdpSender::NewL(
    TUint aStreamId,
    TUint aConnectionId,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    {
    CNATFWUNSAFUdpSender* self = CNATFWUNSAFUdpSender::NewLC(
        aStreamId, aConnectionId, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::NewLC - overloaded
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUdpSender* CNATFWUNSAFUdpSender::NewLC(
    TUint aStreamId,
    TUint aConnectionId,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    {
    CNATFWUNSAFUdpSender* self =
        new ( ELeave ) CNATFWUNSAFUdpSender( aStreamId,
                                             aConnectionId,
                                             aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::CNATFWUNSAFUdpSender
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUdpSender::CNATFWUNSAFUdpSender(
    RSocket& aSocket,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iSocket( aSocket ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::CNATFWUNSAFUdpSender - overloaded
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUdpSender::CNATFWUNSAFUdpSender(
    TUint aStreamId,
    TUint aConnectionId,
    MNATFWUNSAFUdpSenderObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iStreamId( aStreamId ),
      iConnectionId( aConnectionId ),
      iSocket( *( reinterpret_cast<RSocket*>( 0x1 ) ) ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::ConstructL() const
    {
    __ASSERT_ALWAYS( iSocket.SubSessionHandle(), User::Leave( KErrArgument ) );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::~CNATFWUNSAFUdpSender
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUdpSender::~CNATFWUNSAFUdpSender()
    {
    Cancel();
    delete iEncoded;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::SendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFUdpSender::SendL(
    const TInetAddr& aAddr,
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8& aSharedSecret )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::DoCancel()
    {
    iSocket.CancelSend();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::RunL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::RunError
// From CActive
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFUdpSender::RunError( TInt aError )
    {
    iObserver.UNSAFUdpMessageFailure( aError );

    if ( aError == KErrNoMemory )
        {
        return aError;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::EncodeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::EncodeL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8& aSharedSecret,
    const TInetAddr& aAddr )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::RemoveEncoded
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::RemoveEncoded()
    {
    delete iEncoded;
    iEncoded = 0;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUdpSender::SendToSocketL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUdpSender::SendToSocketL()
    {
    }

// End of File

