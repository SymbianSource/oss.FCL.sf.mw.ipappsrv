/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "stunassert.h"
#include "cstunsharedsecretconnecting.h"
#include "cstunsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting(
    const CSTUNSharedSecretState& aWaitResponse ) :
    iWaitResponse( aWaitResponse )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting() :
    iWaitResponse( *( CSTUNSharedSecretState* )0x1 )
    {
    }
// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretConnecting::CSTUNSharedSecretConnecting(
    const CSTUNSharedSecretConnecting& aConnecting ) :
    CSTUNSharedSecretState(),
    iWaitResponse( aConnecting.iWaitResponse )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::~CSTUNSharedSecretConnecting
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretConnecting::~CSTUNSharedSecretConnecting()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::TlsConnectedL
// ---------------------------------------------------------------------------
//
void
CSTUNSharedSecretConnecting::TlsConnectedL( CSTUNSharedSecret& aContext ) const
    {    
    aContext.SendRequestL();    
    aContext.ChangeState( &iWaitResponse );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::ErrorOccured
// Treat all errors as KErrCouldNotConnect
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretConnecting::ErrorOccured( CSTUNSharedSecret& aContext,
                                                TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    
    aContext.Terminate( KErrCouldNotConnect );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretConnecting::TimerExpiredL
// Timeout while creating a TLS connection.
// ---------------------------------------------------------------------------
//
void
CSTUNSharedSecretConnecting::TimerExpiredL( CSTUNSharedSecret& aContext ) const
    {
    aContext.Terminate( KErrCouldNotConnect );
    }
