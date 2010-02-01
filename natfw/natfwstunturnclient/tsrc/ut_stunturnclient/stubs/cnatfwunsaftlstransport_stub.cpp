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
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::~CNATFWUNSAFTlsTransport
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTlsTransport::~CNATFWUNSAFTlsTransport()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::IsConnected
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFTlsTransport::IsConnected()
    {
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
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ContinueListeningL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFTlsTransport::ContinueListeningL() const
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::EnterStateL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::EnterStateL(
    TNATFWUNSAFTlsTransportStateBase::TState aState )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConnectTcpL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConnectTcpL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::ConnectTlsL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::ConnectTlsL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::TlsConnectedL
// From MNATFWUNSAFTlsTransportStateOwner
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::TlsConnectedL()
    {
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
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::RunL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::RunError
// From CActive
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTlsTransport::RunError( TInt aError )
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::CheckCipherSuiteAvailabilityL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::CheckCipherSuiteAvailabilityL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::InitializeStatesL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsTransport::InitializeStatesL()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsTransport::CurrentState
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase& CNATFWUNSAFTlsTransport::CurrentState()
    {
    }

// End of File

