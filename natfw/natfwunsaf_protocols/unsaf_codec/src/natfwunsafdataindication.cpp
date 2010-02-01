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




#include "natfwunsafdataindication.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDataIndication*
CNATFWUNSAFDataIndication::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFDataIndication* self = CNATFWUNSAFDataIndication::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDataIndication*
CNATFWUNSAFDataIndication::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFDataIndication* self =
        new (ELeave) CNATFWUNSAFDataIndication(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFDataIndication::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::CNATFWUNSAFDataIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDataIndication::CNATFWUNSAFDataIndication(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::~CNATFWUNSAFDataIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDataIndication::~CNATFWUNSAFDataIndication()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFDataIndication::Type() const
    {
    return CNATFWUNSAFMessage::EDataIndication;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFDataIndication::Validate() const
    {
    return (HasAttribute(CNATFWUNSAFAttribute::EData) &&
            HasAttribute(CNATFWUNSAFAttribute::ERemoteAddress));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataIndication::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFDataIndication::IsAllowed(TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EData ||
            aAttributeType == CNATFWUNSAFAttribute::ERemoteAddress ||
            aAttributeType == CNATFWUNSAFAttribute::EFingerprint);
    }
