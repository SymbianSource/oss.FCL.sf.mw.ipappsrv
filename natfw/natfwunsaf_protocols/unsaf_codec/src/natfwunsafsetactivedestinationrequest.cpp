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




#include "natfwunsafsetactivedestinationrequest.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSetActiveDestinationRequest*
CNATFWUNSAFSetActiveDestinationRequest::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationRequest* self =
        CNATFWUNSAFSetActiveDestinationRequest::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSetActiveDestinationRequest*
CNATFWUNSAFSetActiveDestinationRequest::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSetActiveDestinationRequest* self =
        new (ELeave) CNATFWUNSAFSetActiveDestinationRequest(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFSetActiveDestinationRequest::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::
// CNATFWUNSAFSetActiveDestinationRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationRequest::CNATFWUNSAFSetActiveDestinationRequest(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::
// ~CNATFWUNSAFSetActiveDestinationRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSetActiveDestinationRequest::
~CNATFWUNSAFSetActiveDestinationRequest()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSetActiveDestinationRequest::Type() const
    {
    return CNATFWUNSAFMessage::ESetActiveDestinationRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationRequest::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSetActiveDestinationRequest::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSetActiveDestinationRequest::
IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::ERemoteAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
