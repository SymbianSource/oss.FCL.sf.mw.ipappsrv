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




#include "natfwunsafallocateresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateResponse*
CNATFWUNSAFAllocateResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateResponse* self = CNATFWUNSAFAllocateResponse::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateResponse*
CNATFWUNSAFAllocateResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFAllocateResponse* self =
        new (ELeave) CNATFWUNSAFAllocateResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFAllocateResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::CNATFWUNSAFAllocateResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateResponse::CNATFWUNSAFAllocateResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::~CNATFWUNSAFAllocateResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAllocateResponse::~CNATFWUNSAFAllocateResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFAllocateResponse::Type() const
    {
    return CNATFWUNSAFMessage::EAllocateResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateResponse::Validate() const
    {
    return (HasAttribute(CNATFWUNSAFAttribute::ERelayAddress) &&
            HasAttribute(CNATFWUNSAFAttribute::EXorMappedAddress) &&
            HasAttribute(CNATFWUNSAFAttribute::ELifetime) &&
            HasAttribute(CNATFWUNSAFAttribute::EBandwidth) &&
            HasAttribute(CNATFWUNSAFAttribute::EMessageIntegrity));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAllocateResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAllocateResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::ERelayAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EXorMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::ELifetime ||
            aAttributeType == CNATFWUNSAFAttribute::EBandwidth ||
            aAttributeType == CNATFWUNSAFAttribute::EMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
