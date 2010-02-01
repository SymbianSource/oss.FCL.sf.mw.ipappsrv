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




#include "natfwunsafsharedsecretresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretResponse* CNATFWUNSAFSharedSecretResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretResponse* self =
        CNATFWUNSAFSharedSecretResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretResponse* CNATFWUNSAFSharedSecretResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretResponse* self =
        new (ELeave) CNATFWUNSAFSharedSecretResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFSharedSecretResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::CNATFWUNSAFSharedSecretResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretResponse::CNATFWUNSAFSharedSecretResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::~CNATFWUNSAFSharedSecretResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretResponse::~CNATFWUNSAFSharedSecretResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSharedSecretResponse::Type() const
    {
    return CNATFWUNSAFMessage::ESharedSecretResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretResponse::Validate() const
    {
    return (HasAttribute(CNATFWUNSAFAttribute::EUsername) &&
            HasAttribute(CNATFWUNSAFAttribute::EPassword));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EUsername ||
            aAttributeType == CNATFWUNSAFAttribute::EPassword ||
            aAttributeType == CNATFWUNSAFAttribute::ERealm ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
