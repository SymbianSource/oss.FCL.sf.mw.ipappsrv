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




#include "natfwunsafallocaterequest.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFAllocateRequest*
CNATFWUNSAFAllocateRequest::NewL(const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateRequest* self = CNATFWUNSAFAllocateRequest::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFAllocateRequest*
CNATFWUNSAFAllocateRequest::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateRequest* self =
        new (ELeave) CNATFWUNSAFAllocateRequest(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFAllocateRequest::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::CNATFWUNSAFAllocateRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateRequest::CNATFWUNSAFAllocateRequest(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::~CNATFWUNSAFAllocateRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateRequest::~CNATFWUNSAFAllocateRequest()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFAllocateRequest::Type() const
    {
    return CNATFWUNSAFMessage::EAllocateRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateRequest::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateRequest::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateRequest::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::ERequestedTransport ||
            aAttributeType == CNATFWUNSAFAttribute::ERequestedIp ||
            aAttributeType == CNATFWUNSAFAttribute::ERequestedPortProps ||
            aAttributeType == CNATFWUNSAFAttribute::EBandwidth ||
            aAttributeType == CNATFWUNSAFAttribute::ELifetime ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
