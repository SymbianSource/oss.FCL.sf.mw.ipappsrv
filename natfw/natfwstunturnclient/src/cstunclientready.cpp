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




#include "cstunclientready.h"
#include "cstunclientimplementation.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientReady::CSTUNClientReady
// ---------------------------------------------------------------------------
//
CSTUNClientReady::CSTUNClientReady(
    const CSTUNClientState& aRenewSharedSecret ) :
    iRenewSharedSecret( aRenewSharedSecret )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientReady::CSTUNClientReady
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientReady::CSTUNClientReady() :
    iRenewSharedSecret( *( CSTUNClientState* )0x1 )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientReady::CSTUNClientReady
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientReady::CSTUNClientReady( const CSTUNClientReady& aReady ) :
    CSTUNClientState(),
    iRenewSharedSecret( aReady.iRenewSharedSecret )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientReady::~CSTUNClientReady
// ---------------------------------------------------------------------------
//
CSTUNClientReady::~CSTUNClientReady()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientReady::ObtainSharedSecretL
// Username and password are available.
// ---------------------------------------------------------------------------
//
void
CSTUNClientReady::ObtainSharedSecretL( CSTUNClientImplementation& aContext,
                                       CBinding& aBinding ) const
    {
    aContext.PassSharedSecretToBindingL( aBinding );
    }

// ---------------------------------------------------------------------------
// CSTUNClientReady::SharedSecretRejectedL
// If the current shared secret was rejected, get a new one.
// ---------------------------------------------------------------------------
//
TBool
CSTUNClientReady::SharedSecretRejectedL( CSTUNClientImplementation& aContext,
                                         const CBinding& aBinding,
                                         const TDesC8& aUsername,
                                         const TDesC8& aPassword ) const
    {        
    if ( aContext.DoesSharedSecretMatch( aUsername, aPassword ) )
        {
        if ( aContext.HasPresetCredentials() )
            {
            aContext.PassCredentialsRejectedL( aBinding );

            //TODO: should the preset credentials be cleared, as they've been
            //rejected?
            return EFalse;
            }
        else
            {
            aContext.GetSharedSecretFromServerL();
            aContext.ChangeState( &iRenewSharedSecret );
            }
        }
    return ETrue;
    }
