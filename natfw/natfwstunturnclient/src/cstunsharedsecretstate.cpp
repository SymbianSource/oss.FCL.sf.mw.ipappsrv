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
#include "cstunsharedsecretstate.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::CSTUNSharedSecretState
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretState::CSTUNSharedSecretState()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::CSTUNSharedSecretState
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretState::CSTUNSharedSecretState(
    const CSTUNSharedSecretState& /*aState*/ ) :
    CBase()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::~CSTUNSharedSecretState
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretState::~CSTUNSharedSecretState()
    {    
    }
    
// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::TlsConnectedL
// ---------------------------------------------------------------------------
//
void
CSTUNSharedSecretState::TlsConnectedL( CSTUNSharedSecret& /*aContext*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::IncomingMessageL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretState::IncomingMessageL( CSTUNSharedSecret& /*aContext*/,
                                               CNATFWUNSAFMessage* aMessage ) const
    {
    __STUN_ASSERT_L( aMessage != NULL, KErrArgument );
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::ErrorOccured
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretState::ErrorOccured( CSTUNSharedSecret& /*aContext*/,
                                           TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    __STUN_FAILURE_RETURN( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretState::TimerExpiredL
// ---------------------------------------------------------------------------
//
void
CSTUNSharedSecretState::TimerExpiredL( CSTUNSharedSecret& /*aContext*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    }
