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




#include "natfwunsafsharedsecretrequest.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSharedSecretRequest* CNATFWUNSAFSharedSecretRequest::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretRequest* self =
        CNATFWUNSAFSharedSecretRequest::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSharedSecretRequest* CNATFWUNSAFSharedSecretRequest::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretRequest* self =
        new (ELeave) CNATFWUNSAFSharedSecretRequest(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFSharedSecretRequest::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::CNATFWUNSAFSharedSecretRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretRequest::CNATFWUNSAFSharedSecretRequest(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::~CNATFWUNSAFSharedSecretRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretRequest::~CNATFWUNSAFSharedSecretRequest()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSharedSecretRequest::Type() const
    {
    return CNATFWUNSAFMessage::ESharedSecretRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretRequest::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretRequest::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretRequest::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::ENonce ||
            aAttributeType == CNATFWUNSAFAttribute::ERealm ||
            aAttributeType == CNATFWUNSAFAttribute::EUsername ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
