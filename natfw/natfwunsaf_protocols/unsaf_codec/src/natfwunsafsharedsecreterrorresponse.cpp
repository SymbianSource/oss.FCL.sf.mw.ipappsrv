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




#include "natfwunsafsharedsecreterrorresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretErrorResponse*
CNATFWUNSAFSharedSecretErrorResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretErrorResponse* self =
        CNATFWUNSAFSharedSecretErrorResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretErrorResponse*
CNATFWUNSAFSharedSecretErrorResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSharedSecretErrorResponse* self =
        new (ELeave) CNATFWUNSAFSharedSecretErrorResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFSharedSecretErrorResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::CNATFWUNSAFSharedSecretErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretErrorResponse::CNATFWUNSAFSharedSecretErrorResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::~CNATFWUNSAFSharedSecretErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSharedSecretErrorResponse::~CNATFWUNSAFSharedSecretErrorResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSharedSecretErrorResponse::Type() const
    {
    return CNATFWUNSAFMessage::ESharedSecretErrorResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretErrorResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EErrorCode);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSharedSecretErrorResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSharedSecretErrorResponse::IsAllowed(
    TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EErrorCode ||
            aAttributeType == CNATFWUNSAFAttribute::EUnknownAttributes ||
            aAttributeType == CNATFWUNSAFAttribute::ENonce ||
            aAttributeType == CNATFWUNSAFAttribute::ERealm ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
