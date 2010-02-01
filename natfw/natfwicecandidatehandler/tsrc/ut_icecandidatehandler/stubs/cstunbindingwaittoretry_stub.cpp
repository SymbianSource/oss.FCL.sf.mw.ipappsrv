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




#include "cstunbindingwaittoretry.h"
#include "natfwbindingimplementationbase.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry
// ---------------------------------------------------------------------------
//
CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry(
    CSTUNBindingState& aGetSharedSecret ) :
    iGetSharedSecret( aGetSharedSecret )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry() :
    iGetSharedSecret( *(CSTUNBindingState*)0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingWaitToRetry::CSTUNBindingWaitToRetry(
    const CSTUNBindingWaitToRetry& aWaitToRetry ) :
    CSTUNBindingState(),
    iGetSharedSecret( aWaitToRetry.iGetSharedSecret )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::~CSTUNBindingWaitToRetry
// ---------------------------------------------------------------------------
//
CSTUNBindingWaitToRetry::~CSTUNBindingWaitToRetry()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::TimerExpiredL
// ---------------------------------------------------------------------------
//
void CSTUNBindingWaitToRetry::TimerExpiredL(
    CBindingImplementation& aContext ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingWaitToRetry::SendRequestL
// No action, since obtaining a binding will soon be started.
// ---------------------------------------------------------------------------
//
void CSTUNBindingWaitToRetry::SendRequestL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }


