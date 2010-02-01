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




#include "natfwunsafconnectrequest.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFConnectRequest*
CNATFWUNSAFConnectRequest::NewL(const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectRequest* self = CNATFWUNSAFConnectRequest::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFConnectRequest*
CNATFWUNSAFConnectRequest::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectRequest* self =
        new (ELeave) CNATFWUNSAFConnectRequest(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFConnectRequest::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::CNATFWUNSAFConnectRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectRequest::CNATFWUNSAFConnectRequest(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::~CNATFWUNSAFConnectRequest
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectRequest::~CNATFWUNSAFConnectRequest()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFConnectRequest::Type() const
    {
    return CNATFWUNSAFMessage::EConnectRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectRequest::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectRequest::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectRequest::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::ERemoteAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EDestinationAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
