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




#include "natfwunsafsetactivedestinationresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationResponse*
CNATFWUNSAFSetActiveDestinationResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationResponse* self =
        CNATFWUNSAFSetActiveDestinationResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationResponse*
CNATFWUNSAFSetActiveDestinationResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationResponse* self =
        new (ELeave) CNATFWUNSAFSetActiveDestinationResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFSetActiveDestinationResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::
// CNATFWUNSAFSetActiveDestinationResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationResponse::
CNATFWUNSAFSetActiveDestinationResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::
// ~CNATFWUNSAFSetActiveDestinationResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationResponse::
~CNATFWUNSAFSetActiveDestinationResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSetActiveDestinationResponse::Type() const
    {
    return CNATFWUNSAFMessage::ESetActiveDestinationResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationResponse::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationResponse::IsAllowed(
    TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::ETimerVal ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
