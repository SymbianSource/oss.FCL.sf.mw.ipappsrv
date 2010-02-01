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




#include "natfwunsafattributelookuptableitem.h"

// -----------------------------------------------------------------------------
// TAttributeLookupTableItem::TAttributeLookupTableItem
// -----------------------------------------------------------------------------
//
TAttributeLookupTableItem::TAttributeLookupTableItem(
    CNATFWUNSAFAttribute::TType aAttributeType,
    TDecodeFnPtr aDecodeFn) :
    iAttributeType(aAttributeType),
    iDecodeFn(aDecodeFn)
    {
    }

// -----------------------------------------------------------------------------
// TAttributeLookupTableItem::AttributeType
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType TAttributeLookupTableItem::AttributeType() const
    {
    return iAttributeType;
    }

// -----------------------------------------------------------------------------
// TAttributeLookupTableItem::DecodeFunction
// -----------------------------------------------------------------------------
//
TDecodeFnPtr TAttributeLookupTableItem::DecodeFunction() const
    {
    return iDecodeFn;
    }
