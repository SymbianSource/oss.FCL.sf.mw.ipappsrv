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



#include "tnatfwunsaftlstransportstatebase.h"


// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateBase::EnterL
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateBase::EnterL()
    {
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateBase::StateChangedL
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateBase::StateChangedL( TInt aStatus )
    {
    User::LeaveIfError( aStatus );
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateBase::SendL
// -----------------------------------------------------------------------------
//
void TNATFWUNSAFTlsTransportStateBase::SendL(
    const CNATFWUNSAFMessage& /*aMessage*/,
    const TDesC8& /*aCredentials*/,
    const TUint /*aUseFingerprint*/ )
    {
    User::Leave( KErrNotReady );
    }

// -----------------------------------------------------------------------------
// TNATFWUNSAFTlsTransportStateBase::TNATFWUNSAFTlsTransportStateBase
// -----------------------------------------------------------------------------
//
TNATFWUNSAFTlsTransportStateBase::TNATFWUNSAFTlsTransportStateBase(
    MNATFWUNSAFTlsTransportStateOwner& aOwner ) : iOwner( aOwner )
    {
    }

// End of File

