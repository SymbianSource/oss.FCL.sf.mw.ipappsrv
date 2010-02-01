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
#include "cstunclientimplementation.h"
#include "cstunclientresolvingudp.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::CSTUNClientResolvingUDP
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingUDP::CSTUNClientResolvingUDP(
    const CSTUNClientState& aGetSharedSecret,
    const CSTUNClientState& aReady ) :
    iGetSharedSecret( aGetSharedSecret ),
    iReady( aReady )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::CSTUNClientResolvingUDP
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingUDP::CSTUNClientResolvingUDP() :
    iGetSharedSecret( *( CSTUNClientState* )0x1 ),
    iReady( *( CSTUNClientState* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::CSTUNClientResolvingUDP
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingUDP::CSTUNClientResolvingUDP(
    const CSTUNClientResolvingUDP& aResolvingUDP ) :    
    CSTUNClientState(),
    iGetSharedSecret( aResolvingUDP.iGetSharedSecret ),
    iReady( aResolvingUDP.iReady )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::~CSTUNClientResolvingUDP
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingUDP::~CSTUNClientResolvingUDP()
    {    
    }
    
// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::ResolvingCompletedL
// Release resolver resources after changing state, as invariant expects
// resolver to exist in ResolvingUDP state.
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingUDP::ResolvingCompletedL(
    CSTUNClientImplementation& aContext,
    TBool aObtainSharedSecret ) const
    {
    if ( aObtainSharedSecret )
        {
        aContext.GetSharedSecretFromServerL();
        aContext.ChangeState( &iGetSharedSecret );
        }
    else
        {
        aContext.PassInitCompletedL( KErrNone );
        aContext.ChangeState( &iReady );
        }
    aContext.FreeResolverResources();
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingUDP::ResolvingFailed
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingUDP::ResolvingFailed(
    CSTUNClientImplementation& aContext,
    TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    aContext.Terminate( aError );
    }
