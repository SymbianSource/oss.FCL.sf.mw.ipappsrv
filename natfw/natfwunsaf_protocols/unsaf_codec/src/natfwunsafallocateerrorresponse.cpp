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




#include "natfwunsafallocateerrorresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateErrorResponse*
CNATFWUNSAFAllocateErrorResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateErrorResponse* self =
        CNATFWUNSAFAllocateErrorResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateErrorResponse*
CNATFWUNSAFAllocateErrorResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateErrorResponse* self =
        new (ELeave) CNATFWUNSAFAllocateErrorResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFAllocateErrorResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::CNATFWUNSAFAllocateErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateErrorResponse::CNATFWUNSAFAllocateErrorResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::~CNATFWUNSAFAllocateErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateErrorResponse::~CNATFWUNSAFAllocateErrorResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFAllocateErrorResponse::Type() const
    {
    return CNATFWUNSAFMessage::EAllocateErrorResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateErrorResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EErrorCode);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateErrorResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateErrorResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EErrorCode ||
            aAttributeType == CNATFWUNSAFAttribute::EUnknownAttributes ||
            aAttributeType == CNATFWUNSAFAttribute::EAlternateServer ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
