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
#include "cnatfwunsaftlstransport.h"
#include "mnatfwunsaftlstransportobserver.h"
#include "cnatfwunsaftlssender.h"
#include "cnatfwunsaftlsreceiver.h"
#include "tnatfwunsaftlstransportstateinit.h"
#include "tnatfwunsaftlstransportstatetcpconnecting.h"
#include "tnatfwunsaftlstransportstatetlsconnecting.h"
#include "tnatfwunsaftlstransportstatetlsconnected.h"
#include "natfwunsaflog.h"


// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTlsTransport* CNATFWUNSAFTlsTransport::NewL(
    RSocketServ& aSocketServer,
    RConnection& aConnection,
    const TInetAddr& aAddr,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsTransport* self =
        CNATFWUNSAFTlsTransport::NewLC( aSocketServer,
                                   aConnection,
                                   aAddr,
                                   aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTlsTransport* CNATFWUNSAFTlsTransport::NewLC(
    RSocketServ& aSocketServer,
    RConnection& aConnection,
    const TInetAddr& aAddr,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsTransport* self =
        new ( ELeave ) CNATFWUNSAFTlsTransport( aSocketServer,
                                           aConnection,
                                           aAddr,
                                           aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::CNATFWUNSAFTlsTransport
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsTransport::CNATFWUNSAFTlsTransport(
    RSocketServ& aSocketServer,
    RConnection& aConnection,
    const TInetAddr& aAddr,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iSocketServer( aSocketServer ),
      iConnection( aConnection ),
      iObserver( aObserver ),
      iStates( TNATFWUNSAFTlsTransportStateBase::EMaxStates ),
      iStateValue( TNATFWUNSAFTlsTransportStateBase::EInit ),
      iAddr( aAddr )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConstructL()
    {
    InitializeStatesL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::~CNATFWUNSAFTlsTransport
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTlsTransport::~CNATFWUNSAFTlsTransport()
    {
    Cancel();
    delete iReceiver;
    delete iSender;
    delete iSecureSocket;
    iSocket.Close();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::IsConnected
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFTlsTransport::IsConnected()
    {
    return ( CurrentState().State()
                == TNATFWUNSAFTlsTransportStateBase::ETlsConnected );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::SendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFTlsTransport::SendL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8& aCredentials,
    const TUint aUseFingerprint )
    {
    CurrentState().SendL( aMessage, aCredentials, aUseFingerprint );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ContinueListeningL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFTlsTransport::ContinueListeningL() const
    {
    __ASSERT_ALWAYS( iReceiver, User::Leave( KErrNotReady ) );
    iReceiver->ContinueRecvL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::EnterStateL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::EnterStateL(
    TNATFWUNSAFTlsTransportStateBase::TState aState )
    {
    __ASSERT_ALWAYS( aState < iStates.Count(), User::Leave( KErrArgument ) );

    iStateValue = aState;

    NATFWUNSAF_INTLOG( "CNATFWUNSAFTlsTransport::EnterStateL state",
        iStateValue )

    CurrentState().EnterL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConnectTcpL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConnectTcpL()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );

    NATFWUNSAF_LOG( "CNATFWUNSAFTlsTransport::ConnectTcpL" )

    User::LeaveIfError( iSocket.Open( iSocketServer,
                                      KAfInet,
                                      KSockStream,
                                      KProtocolInetTcp,
                                      iConnection ) );
    iSocket.Connect( iAddr, iStatus );

    SetActive();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConnectTlsL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConnectTlsL()
    {
    __ASSERT_ALWAYS( !IsActive() && !iSecureSocket, User::Leave( KErrInUse ) );

    NATFWUNSAF_LOG( "CNATFWUNSAFTlsTransport::ConnectTlsL" )

    _LIT( KTLS1,"TLS1.0" );
    iSecureSocket = CSecureSocket::NewL( iSocket, KTLS1 );

    CheckCipherSuiteAvailabilityL();

    // Don't show any pop-up queries if matching certificate is not found
    // while establishing the secure connection.
    User::LeaveIfError(
        iSecureSocket->SetDialogMode( EDialogModeUnattended ) );

    // start the handshake
    iSecureSocket->StartClientHandshake( iStatus );

    SetActive();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::TlsConnectedL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::TlsConnectedL()
    {
    __ASSERT_ALWAYS( iSecureSocket, User::Leave( KErrNotReady ) );

    if ( !iReceiver )
        {
        iReceiver = CNATFWUNSAFTlsReceiver::NewL( *iSecureSocket, iObserver );
        }
    if ( !iSender )
        {
        iSender = CNATFWUNSAFTlsSender::NewL( *iSecureSocket, iObserver );
        }

    iObserver.TlsConnectedL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::SendOverTlsL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::SendOverTlsL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8& aCredentials,
    const TUint aUseFingerprint )
    {
    __ASSERT_ALWAYS( iSender, User::Leave( KErrNotReady ) );

    iSender->SendL( aMessage, aCredentials, aUseFingerprint );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::DoCancel()
    {
    if ( iSecureSocket )
        {
        iSecureSocket->CancelHandshake();
        }
    else
        {
        iSocket.CancelConnect();
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::RunL()
    {
    TInt status( iStatus.Int() );

    NATFWUNSAF_INTLOG( "CNATFWUNSAFTlsTransport::RunL status", status )

    CurrentState().StateChangedL( status );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::RunError
// From CActive
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTlsTransport::RunError( TInt aError )
    {
    NATFWUNSAF_INTLOG( "CNATFWUNSAFTlsTransport::RunError error", aError )

    iObserver.ErrorOccured( aError );

    if ( aError == KErrNoMemory )
        {
        return aError;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::CheckCipherSuiteAvailabilityL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::CheckCipherSuiteAvailabilityL()
    {
    __ASSERT_ALWAYS( iSecureSocket, User::Leave( KErrNotReady ) );

    const TInt KCipherSuitesDesLen = 64;
    TBuf8< KCipherSuitesDesLen > ciphers;
    User::LeaveIfError( iSecureSocket->AvailableCipherSuites( ciphers ) );

    // Each cipher should consist from two bytes
    TInt len( ciphers.Length() );
    const TInt KCipherSuiteInBytes = 2;
    __ASSERT_ALWAYS( !( len % KCipherSuiteInBytes ),
                     User::Leave( KErrGeneral ) );

    // At least this cipher suite should be supported
    // (value defined in RFC 3268)
    const TUint TLS_RSA_WITH_AES_128_CBC_SHA_Value = 0x2F;

    TBool found( EFalse );
    for ( TInt i = 0; i < len && !found; i += KCipherSuiteInBytes )
        {
        TUint cipherVal = BigEndian::Get16( ciphers.Mid( i ).Ptr() );
        found = ( cipherVal == TLS_RSA_WITH_AES_128_CBC_SHA_Value );
        }

    __ASSERT_ALWAYS( found, User::Leave( KErrNotSupported ) );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::InitializeStatesL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::InitializeStatesL()
    {
    iStates.AppendL( TNATFWUNSAFTlsTransportStateInit( *this ),
                     sizeof( TNATFWUNSAFTlsTransportStateInit ) );

    iStates.AppendL( TNATFWUNSAFTlsTransportStateTcpConnecting( *this ),
                     sizeof( TNATFWUNSAFTlsTransportStateTcpConnecting ) );

    iStates.AppendL( TNATFWUNSAFTlsTransportStateTlsConnecting( *this ),
                     sizeof( TNATFWUNSAFTlsTransportStateTlsConnecting ) );

    iStates.AppendL( TNATFWUNSAFTlsTransportStateTlsConnected( *this ),
                     sizeof( TNATFWUNSAFTlsTransportStateTlsConnected ) );

    EnterStateL( TNATFWUNSAFTlsTransportStateBase::EInit );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::CurrentState
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase& CNATFWUNSAFTlsTransport::CurrentState()
    {
    _LIT(KPanicCategory,"CNATFWUNSAFTlsTransport::CurrentState");
    __ASSERT_ALWAYS( iStateValue < iStates.Count(),
        User::Panic( KPanicCategory, KErrTotalLossOfPrecision ) );

    return iStates.At( iStateValue );
    }

// End of File

