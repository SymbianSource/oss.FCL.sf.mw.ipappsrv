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




#include "natfwunsafbindingrequest.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingRequest*
CNATFWUNSAFBindingRequest::NewL(const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingRequest* self = CNATFWUNSAFBindingRequest::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingRequest*
CNATFWUNSAFBindingRequest::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingRequest* self =
        new (ELeave) CNATFWUNSAFBindingRequest(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFBindingRequest::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::CNATFWUNSAFBindingRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingRequest::CNATFWUNSAFBindingRequest(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::~CNATFWUNSAFBindingRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingRequest::~CNATFWUNSAFBindingRequest()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFBindingRequest::Type() const
    {
    return CNATFWUNSAFMessage::EBindingRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingRequest::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingRequest::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingRequest::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EUsername ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::ERealm ||
            aAttributeType == CNATFWUNSAFAttribute::ENonce ||
            aAttributeType == CNATFWUNSAFAttribute::EResponseAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EChangeRequest ||
            aAttributeType == CNATFWUNSAFAttribute::EXorOnly ||
            aAttributeType == CNATFWUNSAFAttribute::EPriority ||
            aAttributeType == CNATFWUNSAFAttribute::EUseCandidate ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint ||
            aAttributeType == CNATFWUNSAFAttribute::EIceControlled ||
            aAttributeType == CNATFWUNSAFAttribute::EIceControlling);
    }
