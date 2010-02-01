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




#include "natfwunsafbindingindication.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingIndication*
CNATFWUNSAFBindingIndication::NewL(const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingIndication* self = CNATFWUNSAFBindingIndication::NewLC(
        aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBindingIndication*
CNATFWUNSAFBindingIndication::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFBindingIndication* self =
        new (ELeave) CNATFWUNSAFBindingIndication(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFBindingIndication::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::CNATFWUNSAFBindingIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingIndication::CNATFWUNSAFBindingIndication(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::~CNATFWUNSAFBindingIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBindingIndication::~CNATFWUNSAFBindingIndication()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFBindingIndication::Type() const
    {
    return CNATFWUNSAFMessage::EBindingIndication;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::Validate
// No mandatory attributes
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingIndication::Validate() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBindingIndication::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFBindingIndication::IsAllowed(TUint16 /*aAttributeType*/) const
    {
    //SHOULD NOT contain any attributes (ICE-17 draft)
    return EFalse;
    }
