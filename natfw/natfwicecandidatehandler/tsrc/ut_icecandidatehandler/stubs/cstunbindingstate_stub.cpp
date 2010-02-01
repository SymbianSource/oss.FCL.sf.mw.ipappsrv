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
#include "cstunbindingstate.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingState::CSTUNBindingState
// ---------------------------------------------------------------------------
//
CSTUNBindingState::CSTUNBindingState()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::CSTUNBindingState
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingState::CSTUNBindingState( const CSTUNBindingState& /*aState*/ ) :
    CBase()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::~CSTUNBindingState
// ---------------------------------------------------------------------------
//
CSTUNBindingState::~CSTUNBindingState()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::SharedSecretObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::SharedSecretObtainedL(
    CBindingImplementation& /*aContext*/,
    const TDesC8& /*aUsername*/,
    const TDesC8& /*aPassword*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::PublicAddressReceivedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::PublicAddressReceivedL(
    CBindingImplementation& /*aContext*/,
    const TInetAddr& /*aPublicAddress*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::PublicAddressReceivedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::PublicAddressReceivedL(
    CBindingImplementation& /*aContext*/,
    const TInetAddr& /*aReflexiveAddr*/,
    const TInetAddr& /*aRelayAddr*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::TransactionError
// ---------------------------------------------------------------------------
//
void
CSTUNBindingState::TransactionError( CBindingImplementation& /*aContext*/,
                                     TInt aError )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::TimerExpiredL
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::TimerExpiredL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingState::SendRequestL
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::SendRequestL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }


// ---------------------------------------------------------------------------
// CSTUNBindingState::CancelRequest
// ---------------------------------------------------------------------------
//
void CSTUNBindingState::CancelRequest(
    CBindingImplementation& /*aContext*/ ) const
    {
    }
