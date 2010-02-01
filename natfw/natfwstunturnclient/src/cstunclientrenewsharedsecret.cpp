/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cstunclientrenewsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::CSTUNClientRenewSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNClientRenewSharedSecret::CSTUNClientRenewSharedSecret()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::CSTUNClientRenewSharedSecret
// Dummy implementation. Copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientRenewSharedSecret::CSTUNClientRenewSharedSecret(
    const CSTUNClientRenewSharedSecret& /*aRenewSharedSecret*/ ) :
    CSTUNClientState()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::~CSTUNClientRenewSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNClientRenewSharedSecret::~CSTUNClientRenewSharedSecret()
    {    
    iReady = NULL;  
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::SharedSecretReceivedL
// Don't inform upper layer, as STUN client isn't being initialized now.
// ---------------------------------------------------------------------------
//
void CSTUNClientRenewSharedSecret::SharedSecretReceivedL(
    CSTUNClientImplementation& aContext ) const
    {
    aContext.StartSharedSecretTimer();
    aContext.InformWaitingBindingsL();
    aContext.ChangeState( iReady );
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::SharedSecretErrorL
// ---------------------------------------------------------------------------
//
void CSTUNClientRenewSharedSecret::SharedSecretErrorL(
    CSTUNClientImplementation& aContext,
    TInt aError ) const
     {
     aContext.HandleSharedSecretErrorL( aError );
     }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::ObtainSharedSecretL
// A new shared secret is being obtained. When it has been got or error occurs,
// the binding is informed. No action.
// ---------------------------------------------------------------------------
//
void CSTUNClientRenewSharedSecret::ObtainSharedSecretL(
    CSTUNClientImplementation& /*aContext*/,
    CBinding& /*aBinding*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::SharedSecretRejectedL
// As a new shared secret is being obtained, it doesn't matter the old username
// and password were rejected.
// ---------------------------------------------------------------------------
//
TBool CSTUNClientRenewSharedSecret::SharedSecretRejectedL(
    CSTUNClientImplementation& /*aContext*/,
    const CBinding& /*aBinding*/,
    const TDesC8& /*aUsername*/,
    const TDesC8& /*aPassword*/ ) const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSTUNClientRenewSharedSecret::SetNeighbourStates
// ---------------------------------------------------------------------------
//
void CSTUNClientRenewSharedSecret::SetNeighbourStates(
    const CSTUNClientState& aReady )
    {
    iReady = &aReady;
    }

