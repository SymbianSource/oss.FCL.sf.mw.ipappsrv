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




#include "natfwunsafbindingresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingResponse*
CNATFWUNSAFBindingResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingResponse* self = CNATFWUNSAFBindingResponse::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingResponse*
CNATFWUNSAFBindingResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingResponse* self =
        new (ELeave) CNATFWUNSAFBindingResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFBindingResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::CNATFWUNSAFBindingResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingResponse::CNATFWUNSAFBindingResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::~CNATFWUNSAFBindingResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingResponse::~CNATFWUNSAFBindingResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFBindingResponse::Type() const
    {
    return CNATFWUNSAFMessage::EBindingResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::Validate
// If the Binding Request had XOR-ONLY, server must not put the MAPPED-ADDRESS
// in the Binding Response, but must put the XOR-MAPPED-ADDRESS.
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EMappedAddress) ||
           HasAttribute(CNATFWUNSAFAttribute::EXorMappedAddress);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::ESourceAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EChangedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EReflectedFrom ||
            aAttributeType == CNATFWUNSAFAttribute::EXorMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EServer ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
