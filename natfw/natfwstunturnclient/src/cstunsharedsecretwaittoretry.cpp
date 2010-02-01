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
#include "cstunsharedsecretwaittoretry.h"
#include "cstunsharedsecret.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitToRetry::CSTUNSharedSecretWaitToRetry
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitToRetry::CSTUNSharedSecretWaitToRetry()    
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitToRetry::CSTUNSharedSecretWaitToRetry
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitToRetry::CSTUNSharedSecretWaitToRetry(
    const CSTUNSharedSecretWaitToRetry& /*aWaitToRetry*/ ) :
    CSTUNSharedSecretState()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitToRetry::~CSTUNSharedSecretWaitToRetry
// ---------------------------------------------------------------------------
//
CSTUNSharedSecretWaitToRetry::~CSTUNSharedSecretWaitToRetry()
    {    
    iWaitResponse = NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitToRetry::SetNeighbourStates
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretWaitToRetry::SetNeighbourStates(
    const CSTUNSharedSecretState& aWaitResponse )
    {
    iWaitResponse = &aWaitResponse;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecretWaitToRetry::TimerExpiredL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecretWaitToRetry::TimerExpiredL(
    CSTUNSharedSecret& aContext ) const
    {    
    aContext.SendRequestL();    
    aContext.ChangeState( iWaitResponse );
    }
