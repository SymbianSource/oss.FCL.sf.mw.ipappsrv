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




#include "natfwunsafsendindication.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSendIndication*
CNATFWUNSAFSendIndication::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSendIndication* self = CNATFWUNSAFSendIndication::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSendIndication*
CNATFWUNSAFSendIndication::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFSendIndication* self =
        new (ELeave) CNATFWUNSAFSendIndication(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFSendIndication::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::CNATFWUNSAFSendIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSendIndication::CNATFWUNSAFSendIndication(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::~CNATFWUNSAFSendIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSendIndication::~CNATFWUNSAFSendIndication()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFSendIndication::Type() const
    {
    return CNATFWUNSAFMessage::ESendIndication;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSendIndication::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSendIndication::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFSendIndication::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EData ||
            aAttributeType == CNATFWUNSAFAttribute::ERemoteAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
