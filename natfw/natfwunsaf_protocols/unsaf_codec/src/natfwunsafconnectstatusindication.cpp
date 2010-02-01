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




#include "natfwunsafconnectstatusindication.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatusIndication*
CNATFWUNSAFConnectStatusIndication::NewL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectStatusIndication* self =
        CNATFWUNSAFConnectStatusIndication::NewLC(aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatusIndication*
CNATFWUNSAFConnectStatusIndication::NewLC(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFConnectStatusIndication* self =
        new (ELeave) CNATFWUNSAFConnectStatusIndication(aTransactionID);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
CNATFWUNSAFConnectStatusIndication::CreateL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    return NewL(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::CNATFWUNSAFConnectStatusIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatusIndication::CNATFWUNSAFConnectStatusIndication(
    const TNATFWUNSAFTransactionID& aTransactionID) :
    CNATFWUNSAFMessage(aTransactionID)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::~CNATFWUNSAFConnectStatusIndication
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatusIndication::~CNATFWUNSAFConnectStatusIndication()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::TType CNATFWUNSAFConnectStatusIndication::Type() const
    {
    return CNATFWUNSAFMessage::EConnectStatusIndication;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::Validate
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectStatusIndication::Validate() const
    {
    return HasAttribute(CNATFWUNSAFAttribute::EConnectStat);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatusIndication::IsAllowed
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFConnectStatusIndication::IsAllowed(
    TUint16 aAttributeType) const
    {
    return (aAttributeType == CNATFWUNSAFAttribute::EConnectStat);
    }
