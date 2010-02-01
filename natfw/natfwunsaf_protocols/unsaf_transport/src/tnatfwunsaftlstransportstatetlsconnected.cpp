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
* Description:    implementation
*
*/



#include "tnatfwunsaftlstransportstatetlsconnected.h"
#include "mnatfwunsaftlstransportstateowner.h"


// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTlsConnected::
//     TNATFWUNSAFTlsTransportStateTlsConnected
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateTlsConnected::
TNATFWUNSAFTlsTransportStateTlsConnected(
    MNATFWUNSAFTlsTransportStateOwner& aOwner )
    : TNATFWUNSAFTlsTransportStateBase( aOwner )
    {
    }

// ----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTlsConnected::State
// From TNATFWUNSAFTlsTransportStateBase
// ----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase::TState
    TNATFWUNSAFTlsTransportStateTlsConnected::State() const
    {
    return ETlsConnected;
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTlsConnected::EnterL
// From TNATFWUNSAFTlsTransportStateBase
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateTlsConnected::EnterL()
    {
    iOwner.TlsConnectedL();
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTlsConnected::SendL
// From TNATFWUNSAFTlsTransportStateBase
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateTlsConnected::SendL(
    const CNATFWUNSAFMessage& aMessage,
    const TDesC8& aCredentials,
    const TUint aUseFingerprint )
    {
    iOwner.SendOverTlsL( aMessage, aCredentials, aUseFingerprint );
    }

// End of File

