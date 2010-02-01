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




#include "cstunbindingactive.h"
#include "natfwbindingimplementationbase.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingActive::CSTUNBindingActive
// ---------------------------------------------------------------------------
//
CSTUNBindingActive::CSTUNBindingActive(
    CSTUNBindingState& aGetSharedSecret ) :
    iGetSharedSecret( aGetSharedSecret )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingActive::CSTUNBindingActive
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingActive::CSTUNBindingActive() :
    iGetSharedSecret( *( CSTUNBindingState* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingActive::CSTUNBindingActive
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingActive::CSTUNBindingActive(
    const CSTUNBindingActive& aBindingActive ) :    
    CSTUNBindingState(),
    iGetSharedSecret( aBindingActive.iGetSharedSecret )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingActive::~CSTUNBindingActive
// ---------------------------------------------------------------------------
//
CSTUNBindingActive::~CSTUNBindingActive()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingActive::SendRequestL
// The existing public address is not cleared, as application may ask for it
// also during refresh.
// ---------------------------------------------------------------------------
//
void
CSTUNBindingActive::SendRequestL( CBindingImplementation& aContext ) const
    {
    }

