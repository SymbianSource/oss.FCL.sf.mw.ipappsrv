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



#include "tnatfwunsaftlstransportstatetcpconnecting.h"
#include "mnatfwunsaftlstransportstateowner.h"


// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTcpConnecting::
//     TNATFWUNSAFTlsTransportStateTcpConnecting
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateTcpConnecting::
TNATFWUNSAFTlsTransportStateTcpConnecting(
    MNATFWUNSAFTlsTransportStateOwner& aOwner )
    : TNATFWUNSAFTlsTransportStateBase( aOwner )
    {
    }

// ----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTcpConnecting::State
// From TNATFWUNSAFTlsTransportStateBase
// ----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase::TState
    TNATFWUNSAFTlsTransportStateTcpConnecting::State() const
    {
    return ETcpConnecting;
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateTcpConnecting::StateChangedL
// From TNATFWUNSAFTlsTransportStateBase
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateTcpConnecting::StateChangedL( TInt aStatus )
    {
    User::LeaveIfError( aStatus );

    iOwner.EnterStateL( TNATFWUNSAFTlsTransportStateBase::ETlsConnecting );
    iOwner.ConnectTlsL();
    }

// End of File

