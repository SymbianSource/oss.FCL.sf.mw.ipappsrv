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




#include "natfwunsafsetactivedestinationerrorresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationErrorResponse*
CNATFWUNSAFSetActiveDestinationErrorResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationErrorResponse* self =
        CNATFWUNSAFSetActiveDestinationErrorResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationErrorResponse*
CNATFWUNSAFSetActiveDestinationErrorResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationErrorResponse* self =
        new (ELeave) CNATFWUNSAFSetActiveDestinationErrorResponse(
            aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFSetActiveDestinationErrorResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::
// CNATFWUNSAFSetActiveDestinationErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationErrorResponse::
CNATFWUNSAFSetActiveDestinationErrorResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::
// ~CNATFWUNSAFSetActiveDestinationErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationErrorResponse::
~CNATFWUNSAFSetActiveDestinationErrorResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType
CNATFWUNSAFSetActiveDestinationErrorResponse::Type() const
    {
    return CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationErrorResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EErrorCode);
    }        

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationErrorResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationErrorResponse::
IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EErrorCode ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EUnknownAttributes ||
            aAttributeType == CNATFWUNSAFAttribute::EAlternateServer ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
