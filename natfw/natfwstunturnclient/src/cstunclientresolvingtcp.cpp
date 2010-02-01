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




#include "stunassert.h"
#include "cstunclientimplementation.h"
#include "cstunclientresolvingtcp.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::CSTUNClientResolvingTCP
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTCP::CSTUNClientResolvingTCP(
    const CSTUNClientState& aResolvingTLS, 
    const CSTUNClientState& aResolvingUDP ) :
    iResolvingTLS( aResolvingTLS ), iResolvingUDP( aResolvingUDP ) 
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::CSTUNClientResolvingTCP
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTCP::CSTUNClientResolvingTCP(
    const CSTUNClientState& aResolvingUDP ) :
    iResolvingTLS( *( CSTUNClientState* )0x1 ),
    iResolvingUDP( aResolvingUDP )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::CSTUNClientResolvingTCP
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTCP::CSTUNClientResolvingTCP() :
    iResolvingTLS( *( CSTUNClientState* )0x1 ),
    iResolvingUDP( *( CSTUNClientState* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::~CSTUNClientResolvingTCP
// ---------------------------------------------------------------------------
//
CSTUNClientResolvingTCP::~CSTUNClientResolvingTCP()
    {    
    }
    
// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::ResolvingCompletedL        
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingTCP::ResolvingCompletedL(
    CSTUNClientImplementation& aContext,
    TBool aObtainSharedSecret ) const
    {
    aContext.TcpResolvedL( aObtainSharedSecret );
    if ( aObtainSharedSecret )
        {
        aContext.ChangeState( &iResolvingTLS );
        }
    else
        {
        aContext.ChangeState( &iResolvingUDP );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNClientResolvingTCP::ResolvingFailed
// Continue without using shared secret as TCP address couldn't be obtained.
// ---------------------------------------------------------------------------
//
void CSTUNClientResolvingTCP::ResolvingFailed(
    CSTUNClientImplementation& aContext,
    TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    TRAPD( err, ResolvingCompletedL( aContext, EFalse ) );
    if ( err != KErrNone )
        {
        aContext.Terminate( err );
        }
    }
