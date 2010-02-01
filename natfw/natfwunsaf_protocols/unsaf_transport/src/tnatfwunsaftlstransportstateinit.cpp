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



#include "tnatfwunsaftlstransportstateinit.h"
#include "mnatfwunsaftlstransportstateowner.h"

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateInit::TNATFWUNSAFTlsTransportStateInit
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateInit::TNATFWUNSAFTlsTransportStateInit(
    MNATFWUNSAFTlsTransportStateOwner& aOwner )
    : TNATFWUNSAFTlsTransportStateBase( aOwner )
    {
    }

// ----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateInit::State
// From TNATFWUNSAFTlsTransportStateBase
// ----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase::TState
TNATFWUNSAFTlsTransportStateInit::State() const
    {
    return EInit;
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateInit::EnterL
// From TNATFWUNSAFTlsTransportStateBase
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateInit::EnterL()
    {
    iOwner.EnterStateL( TNATFWUNSAFTlsTransportStateBase::ETcpConnecting );
    iOwner.ConnectTcpL();
    }

// End of File
