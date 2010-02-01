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




#include "natfwunsaflog.h"
#include "stunassert.h"
#include "cstunbindinggetaddress.h"
#include "natfwbindingimplementationbase.h"
#include "stunutils.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::CSTUNBindingGetAddress
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::CSTUNBindingGetAddress()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::CSTUNBindingGetAddress
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::CSTUNBindingGetAddress(
    const CSTUNBindingGetAddress& /*aGetAddress*/ ) :
    CSTUNBindingState()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::~CSTUNBindingGetAddress
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::~CSTUNBindingGetAddress()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::PublicAddressReceivedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::PublicAddressReceivedL(
    CBindingImplementation& aContext,
    const TInetAddr& aPublicAddress ) const
    {

    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::PublicAddressReceivedL - overloaded for stun relay
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::PublicAddressReceivedL(
    CBindingImplementation& aContext,
    const TInetAddr& aReflexiveAddr,
    const TInetAddr& aRelayAddr ) const
    {

    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::TransactionError
// ---------------------------------------------------------------------------
//
void
CSTUNBindingGetAddress::TransactionError( CBindingImplementation& aContext,
                                          TInt aError )
    {

    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::SendRequestL
// No action, since obtaining a binding is already in progress.
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::SendRequestL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::SetNeighbourStates
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::SetNeighbourStates(
    CSTUNBindingState& aGetSharedSecret,
    CSTUNBindingState& aWaitToRetry,
    CSTUNBindingState& aActive )
    {
    }
