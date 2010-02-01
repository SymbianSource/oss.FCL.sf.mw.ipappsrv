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
#include "cstunclientstate.h"


// ======== MEMBER FUNCTIONS ========
 
// ---------------------------------------------------------------------------
// CSTUNClientState::CSTUNClientState
// ---------------------------------------------------------------------------
//
CSTUNClientState::CSTUNClientState()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::CSTUNClientState
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientState::CSTUNClientState( const CSTUNClientState& /*aState*/ ) :
    CBase()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::~CSTUNClientState
// ---------------------------------------------------------------------------
//
CSTUNClientState::~CSTUNClientState()
    {
    }
    
// ---------------------------------------------------------------------------
// CSTUNClientState::ResolvingCompletedL
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
void CSTUNClientState::ResolvingCompletedL(
    CSTUNClientImplementation& /*aContext*/,
    TBool /*aObtainSharedSecret*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::ResolvingFailed
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
void
CSTUNClientState::ResolvingFailed( CSTUNClientImplementation& /*aContext*/,
                                      TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    __STUN_FAILURE_RETURN( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::SharedSecretReceivedL
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
void CSTUNClientState::SharedSecretReceivedL(
    CSTUNClientImplementation& /*aContext*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::SharedSecretErrorL
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
void CSTUNClientState::SharedSecretErrorL(
    CSTUNClientImplementation& /*aContext*/,
    TInt aError ) const
    {
    __STUN_ASSERT_L( aError != KErrNone, KErrArgument );
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::ObtainSharedSecretL
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
void
CSTUNClientState::ObtainSharedSecretL(    CSTUNClientImplementation& /*aContext*/,
                                        CBinding& /*aBinding*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    }

// ---------------------------------------------------------------------------
// CSTUNClientState::SharedSecretRejectedL
// This function should never be called. If it is called, then the actual
// state is missing the event handler.
// ---------------------------------------------------------------------------
//
TBool CSTUNClientState::SharedSecretRejectedL(
    CSTUNClientImplementation& /*aContext*/,
    const CBinding& /*aBinding*/,
    const TDesC8& /*aUsername*/,
    const TDesC8& /*aPassword*/ ) const
    {
    __STUN_FAILURE_L( KErrGeneral );
    return EFalse;
    }
