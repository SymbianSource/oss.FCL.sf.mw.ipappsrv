/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "natfwbindingimplementationbase.h"
#include "cstunbindinggetsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret(
    CSTUNBindingState& aGetPublicAddress ) :
    iGetPublicAddress( aGetPublicAddress )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret() :
    iGetPublicAddress( *( CSTUNBindingState* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingGetSharedSecret::CSTUNBindingGetSharedSecret(
    const CSTUNBindingGetSharedSecret& aGetSharedSecret ) :    
    CSTUNBindingState(),    
    iGetPublicAddress( aGetSharedSecret.iGetPublicAddress )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::~CSTUNBindingGetSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNBindingGetSharedSecret::~CSTUNBindingGetSharedSecret()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::SharedSecretObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetSharedSecret::SharedSecretObtainedL(
    CBindingImplementation& aContext,
    const TDesC8& aUsername,
    const TDesC8& aPassword ) const
    {
    // CreateBindingRequestL and SendBindingRequestL are valid method calls
    // for both stun and stun-relay functionality. State machine should
    // work fine.
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetSharedSecret::SendRequestL
// No action, since obtaining a binding is already in progress.
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetSharedSecret::SendRequestL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }


// End of file
    
