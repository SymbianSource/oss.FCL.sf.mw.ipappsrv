/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cstunclientresolvingtls.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTLS::CSTUNClientResolvingTLS
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTLS::CSTUNClientResolvingTLS(
    const CSTUNClientState& aResolvingUDP ) :
    iResolvingUDP( aResolvingUDP )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTLS::CSTUNClientResolvingTLS
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTLS::CSTUNClientResolvingTLS() :
    iResolvingUDP( *( CSTUNClientState* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTLS::~CSTUNClientResolvingTLS
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTLS::~CSTUNClientResolvingTLS()
    {    
    }
    
// ---------------------------------------------------------------------------
// CSTUNClientResolvingTLS::ResolvingCompletedL        
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingTLS::ResolvingCompletedL(
    CSTUNClientImplementation& aContext,
    TBool /*aObtainSharedSecret*/ ) const
    {
    aContext.TlsResolvedL( );
    aContext.ChangeState( &iResolvingUDP );
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTLS::ResolvingFailed
// Continue without using shared secret as TLS address couldn't be obtained.
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingTLS::ResolvingFailed(
    CSTUNClientImplementation& aContext,
    TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    
    // ignore error, continue with udp resolver
    TRAPD( err, ResolvingCompletedL( aContext, EFalse ) );
    if ( err != KErrNone )
        {
        aContext.Terminate( err );
        }
    }
