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
#include "cnatfwunsaftlsreceiver.h"
#include "mnatfwunsaftlstransportobserver.h"
#include "cnatfwunsafmsgassembler.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsReceiver* CNATFWUNSAFTlsReceiver::NewL(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsReceiver* self =
        CNATFWUNSAFTlsReceiver::NewLC( aSecureSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsReceiver* CNATFWUNSAFTlsReceiver::NewLC(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsReceiver* self =
        new ( ELeave ) CNATFWUNSAFTlsReceiver( aSecureSocket, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::CNATFWUNSAFTlsReceiver
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsReceiver::CNATFWUNSAFTlsReceiver(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iSecureSocket( aSecureSocket ),
      iObserver( aObserver ),
      iReceiveBufferPtr( 0, 0, 0 )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::ConstructL()
    {
    iMsgAssembler = CNATFWUNSAFMsgAssembler::NewL( *this );

    ReceiveL( CNATFWUNSAFMsgAssembler::EDefaultBufferSize );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::~CNATFWUNSAFTlsReceiver
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsReceiver::~CNATFWUNSAFTlsReceiver()
    {
    Cancel();
    delete iMsgAssembler;
    delete iReceiveBuffer;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::NATFWUNSAFMsgCompleteL
// From MNATFWUNSAFMsgAssemblerObserver
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::NATFWUNSAFMsgCompleteL(
    CNATFWUNSAFMessage* aMessage,
    TInt aStatus )
    {
    if ( aStatus )
        {
        iObserver.ErrorOccured( aStatus );
        }
    else
        {
        iObserver.IncomingMessageL( aMessage );
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::DoCancel()
    {
    NATFWUNSAF_LOG( "CNATFWUNSAFTlsReceiver::DoCancel" )
    iSecureSocket.CancelRecv();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::RunL()
    {
    TInt status( iStatus.Int() );

    NATFWUNSAF_INTLOG( "CNATFWUNSAFTlsReceiver::RunL status", status )

    switch ( status )
        {
        case KErrNone:
            {
            AssembleL();
            break;
            }
        case KErrEof:
            {
            // Connection closed, nothing to do
            break;
            }
        default:
            {
            iObserver.ErrorOccured( status );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::RunError
// From CActive
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTlsReceiver::RunError( TInt aError )
    {
    iObserver.ErrorOccured( aError );

    if ( aError == KErrNoMemory )
        {
        return aError;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::AssembleL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::AssembleL()
    {
    NATFWUNSAF_BYTESTREAMLOG( "CNATFWUNSAFTlsReceiver::AssembleL data",
        *iReceiveBuffer )

    // InputL call might lead to self deletion if assembling completed.
    // In that case it returns ETrue and member data should not be accessed.
    //
    HBufC8* tmpBuffer = iReceiveBuffer;
    iReceiveBuffer = 0;
    CleanupStack::PushL( tmpBuffer );

    TBool completed = iMsgAssembler->InputL( tmpBuffer, iNextLength );

    CleanupStack::Pop( tmpBuffer );

    if ( !completed )
        {
        // Receiving can be continued.
        ReceiveL( iNextLength );
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::ReceiveL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::ReceiveL( TUint aReceiveBufLength )
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );

    CreateReceiveBufferL( aReceiveBufLength );

    iSecureSocket.RecvOneOrMore( iReceiveBufferPtr,
                                 iStatus,
                                 iSockLength );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::ContinueRecvL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::ContinueRecvL()
    {
    iNextLength = 0;
    ReceiveL( CNATFWUNSAFMsgAssembler::EDefaultBufferSize );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsReceiver::CreateReceiveBufferL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsReceiver::CreateReceiveBufferL( TUint aReceiveBufLength )
    {
    delete iReceiveBuffer;
    iReceiveBuffer = 0;
    iReceiveBuffer = HBufC8::NewL( aReceiveBufLength );
    iReceiveBufferPtr.Set( iReceiveBuffer->Des() );
    }

// End of File

