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
#include "cnatfwunsaftlssender.h"
#include "mnatfwunsaftlstransportobserver.h"
#include "cnatfwunsafencodeditem.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsSender* CNATFWUNSAFTlsSender::NewL(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsSender* self =
        CNATFWUNSAFTlsSender::NewLC( aSecureSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsSender* CNATFWUNSAFTlsSender::NewLC(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    {
    CNATFWUNSAFTlsSender* self =
        new ( ELeave ) CNATFWUNSAFTlsSender( aSecureSocket, aObserver );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::CNATFWUNSAFTlsSender
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsSender::CNATFWUNSAFTlsSender(
    CSecureSocket& aSecureSocket,
    MNATFWUNSAFTlsTransportObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iSecureSocket( aSecureSocket ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::~CNATFWUNSAFTlsSender
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTlsSender::~CNATFWUNSAFTlsSender()
    {
    Cancel();
    delete iEncoded;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::SendL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::SendL( const CNATFWUNSAFMessage& aMessage,
                                  const TDesC8& aCredentials,
                                  const TUint aUseFingerprint )
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );

    EncodeL( aMessage, aCredentials, aUseFingerprint );
    SendToSocketL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::DoCancel()
    {
    iSecureSocket.CancelSend();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::RunL()
    {
    RemoveEncoded();

    TInt status( iStatus.Int() );

    NATFWUNSAF_INTLOG( "CNATFWUNSAFTlsSender::RunL status", status )

    if ( status != KErrNone )
        {
        iObserver.ErrorOccured( status );
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::RunError
// From CActive
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTlsSender::RunError( TInt aError )
    {
    iObserver.ErrorOccured( aError );

    if ( aError == KErrNoMemory )
        {
        return aError;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::EncodeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::EncodeL( const CNATFWUNSAFMessage& aMessage,
                                    const TDesC8& aCredentials,
                                    const TUint aUseFingerprint )
    {
    RemoveEncoded();
    if ( 0 < aCredentials.Length() )
        {
        iEncoded = CNATFWUNSAFEncodedItem::NewL( aMessage, &aCredentials, 0, 0, 0,
                   aUseFingerprint );
        }
    else
        {
        iEncoded = CNATFWUNSAFEncodedItem::NewL( aMessage, 0, 0, 0, 0,
                   aUseFingerprint );
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::RemoveEncoded
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::RemoveEncoded()
    {
    delete iEncoded;
    iEncoded = 0;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTlsSender::SendToSocketL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTlsSender::SendToSocketL()
    {
    __ASSERT_ALWAYS( iEncoded, User::Leave( KErrNotReady ) );

    iCurrent.Set( iEncoded->Ptr() );

    NATFWUNSAF_BYTESTREAMLOG( "CNATFWUNSAFTlsSender::SendToSocketL data",
        iCurrent )

    iSecureSocket.Send( iCurrent, iStatus );
    SetActive();
    }

// End of File

