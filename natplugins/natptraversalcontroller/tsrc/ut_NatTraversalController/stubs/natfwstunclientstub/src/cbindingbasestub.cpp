/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include "natfwbindingbase.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBinding::~CBinding
// ---------------------------------------------------------------------------
//
CBinding::~CBinding()
    {
    }

// ---------------------------------------------------------------------------
// CBinding::SendRequestL
// ---------------------------------------------------------------------------
//
void CBinding::SendRequestL()
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::SendRequestL - overloaded
// ---------------------------------------------------------------------------
//
void CBinding::SendRequestL( const TInetAddr& /*aDestAddr*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::AllocateRequestL
// ---------------------------------------------------------------------------
//
void CBinding::AllocateRequestL()
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CBinding::FetchCandidateL( TInetAddr& /*aReflexiveAddr*/,
                                TInetAddr& /*aRelayAddr*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CBinding::SetActiveDestinationRequestL( const TInetAddr& /*aRemoteAddr*/,
                                             TUint32& /*aTimerValue*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::SendIndicationL
// ---------------------------------------------------------------------------
//
void CBinding::SendIndicationL( const TInetAddr& /*aRemoteAddr*/,
                                const TDesC8& /*aData*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBinding::ConnectRequestL
// ---------------------------------------------------------------------------
//
void CBinding::ConnectRequestL( const TInetAddr& /*aRemoteAddr*/ )
    {
    User::Leave( KErrNotFound );
    }


// End of file
