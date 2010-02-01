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




#include "natfwunsafbindingerrorresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingErrorResponse*
CNATFWUNSAFBindingErrorResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingErrorResponse* self =
        CNATFWUNSAFBindingErrorResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingErrorResponse*
CNATFWUNSAFBindingErrorResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingErrorResponse* self =
        new (ELeave) CNATFWUNSAFBindingErrorResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFBindingErrorResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::CNATFWUNSAFBindingErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingErrorResponse::CNATFWUNSAFBindingErrorResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::~CNATFWUNSAFBindingErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingErrorResponse::~CNATFWUNSAFBindingErrorResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFBindingErrorResponse::Type() const
    {
    return CNATFWUNSAFMessage::EBindingErrorResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingErrorResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EErrorCode);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingErrorResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingErrorResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EErrorCode ||
            aAttributeType == CNATFWUNSAFAttribute::EAlternateServer ||
            aAttributeType == CNATFWUNSAFAttribute::ERealm ||
            aAttributeType == CNATFWUNSAFAttribute::ENonce ||
            aAttributeType == CNATFWUNSAFAttribute::EUnknownAttributes ||
            aAttributeType == CNATFWUNSAFAttribute::EServer ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
