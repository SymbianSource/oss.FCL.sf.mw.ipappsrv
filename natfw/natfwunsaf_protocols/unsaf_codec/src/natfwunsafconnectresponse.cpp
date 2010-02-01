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




#include "natfwunsafconnectresponse.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectResponse*
CNATFWUNSAFConnectResponse::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectResponse* self = CNATFWUNSAFConnectResponse::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectResponse*
CNATFWUNSAFConnectResponse::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectResponse* self =
        new (ELeave) CNATFWUNSAFConnectResponse(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFConnectResponse::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::CNATFWUNSAFConnectResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectResponse::CNATFWUNSAFConnectResponse(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::~CNATFWUNSAFConnectResponse
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectResponse::~CNATFWUNSAFConnectResponse()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFConnectResponse::Type() const
    {
    return CNATFWUNSAFMessage::EConnectResponse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::Validate
// No mandatory attributes for connect response
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectResponse::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectResponse::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectResponse::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::ERelayAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EXorMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::ELifetime ||
            aAttributeType == CNATFWUNSAFAttribute::EBandwidth ||
            aAttributeType == CNATFWUNSAFAttribute::EMappedAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EMessageIntegrity ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
