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




#include "cstunclientimplementation.h"
#include "cstunclientgetsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret(
    const CSTUNClientState& aReady ) :
    iReady( aReady )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret() :
    iReady( *( CSTUNClientState* )0x1 )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientGetSharedSecret::CSTUNClientGetSharedSecret(
    const CSTUNClientGetSharedSecret& aGetSharedSecret ) :    
    CSTUNClientState(),  
    iReady( aGetSharedSecret.iReady )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::~CSTUNClientGetSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNClientGetSharedSecret::~CSTUNClientGetSharedSecret()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::SharedSecretReceivedL
// ---------------------------------------------------------------------------
//
void CSTUNClientGetSharedSecret::SharedSecretReceivedL(
    CSTUNClientImplementation& aContext ) const
    {    
    aContext.StartSharedSecretTimer();
    aContext.PassInitCompletedL( KErrNone );
    aContext.ChangeState( &iReady );
    }

// ---------------------------------------------------------------------------
// CSTUNClientGetSharedSecret::SharedSecretErrorL
// ---------------------------------------------------------------------------
//
void CSTUNClientGetSharedSecret::SharedSecretErrorL(
    CSTUNClientImplementation& aContext,
    TInt aError ) const
     {
     aContext.HandleSharedSecretErrorL( aError );
     }

