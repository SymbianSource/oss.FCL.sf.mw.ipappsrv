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
#include "natfwunsafmessage.h"
#include "cstunsharedsecretwaitresponse.h"
#include "cstunsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse(
    const CSTUNSharedSecretState& aActive,
    const CSTUNSharedSecretState& aWaitToRetry ) :
    iActive( aActive ),
    iWaitToRetry( aWaitToRetry )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse() :
    iActive( *( CSTUNSharedSecretState* )0x1 ),
    iWaitToRetry( *( CSTUNSharedSecretState* )0x1 )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitResponse::CSTUNSharedSecretWaitResponse(
    const CSTUNSharedSecretWaitResponse& aWaitResponse ) :    
    CSTUNSharedSecretState(), 
    iActive( aWaitResponse.iActive ),
    iWaitToRetry( aWaitResponse.iWaitToRetry )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::~CSTUNSharedSecretWaitResponse
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitResponse::~CSTUNSharedSecretWaitResponse()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::IncomingMessageL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretWaitResponse::IncomingMessageL( 
    CSTUNSharedSecret& aContext, CNATFWUNSAFMessage* aMessage ) const
    {
    __STUN_ASSERT_L( aMessage != NULL, KErrArgument );

    if ( aContext.CheckMessage( *aMessage ) )
        {
        if ( aMessage->Type() == CNATFWUNSAFMessage::ESharedSecretResponse )
            {
            //This leads to iObserver.SharedSecretObtainedL
            aContext.ResponseReceivedL( *aMessage );        
            aContext.ChangeState( &iActive );
            }
        else
            {
            if ( aContext.ErrorResponseReceivedL( *aMessage ) )
                {
                aContext.ChangeState( &iWaitToRetry );
                }
            else
                {
                aContext.Terminate( KErrGeneral );
                }
            }
        }
    else
        {
        //Invalid response from server
        aContext.Terminate( KErrGeneral );
        }

    delete aMessage;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::ErrorOccured
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretWaitResponse::ErrorOccured( CSTUNSharedSecret& aContext,
                                                  TInt aError ) const
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    aContext.Terminate( aError );
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitResponse::TimerExpiredL
// No response from server
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretWaitResponse::TimerExpiredL(
    CSTUNSharedSecret& aContext ) const
    {
    aContext.Terminate( KErrTimedOut );
    }
