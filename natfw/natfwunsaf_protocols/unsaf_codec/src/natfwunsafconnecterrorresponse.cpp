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




#include "natfwunsafconnecterrorresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectErrorResponse*
CNATFWUNSAFConnectErrorResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectErrorResponse* self =
        CNATFWUNSAFConnectErrorResponse::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectErrorResponse*
CNATFWUNSAFConnectErrorResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectErrorResponse* self =
        new (ELeave) CNATFWUNSAFConnectErrorResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFConnectErrorResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::CNATFWUNSAFConnectErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectErrorResponse::CNATFWUNSAFConnectErrorResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::~CNATFWUNSAFConnectErrorResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectErrorResponse::~CNATFWUNSAFConnectErrorResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFConnectErrorResponse::Type() const
    {
    return CNATFWUNSAFMessage::EConnectErrorResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectErrorResponse::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EErrorCode);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectErrorResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectErrorResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EErrorCode ||
            aAttributeType == CNATFWUNSAFAttribute::EUnknownAttributes ||
            aAttributeType == CNATFWUNSAFAttribute::EAlternateServer ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
