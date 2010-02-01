/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "stunutils.h"
#include "natfwunsafmessage.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafrelayaddressattribute.h"
#include "natfwunsaftimervalattribute.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// STUNUtils::GetMappedAddress
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetMappedAddress( const CNATFWUNSAFMessage& aResponse,
                                   TInetAddr& aAddr )
    {

    }

// -----------------------------------------------------------------------------
// STUNUtils::GetXorMappedAddressL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetXorMappedAddressL( const CNATFWUNSAFMessage& aResponse,
                                       TInetAddr& aAddr )
    {

    }

// -----------------------------------------------------------------------------
// STUNUtils::GetRelayAddressL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetRelayAddressL( const CNATFWUNSAFMessage& aResponse,
                                   TInetAddr& aAddr )
    {
    }

// -----------------------------------------------------------------------------
// STUNUtils::GetTimerValueL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetTimerValueL( const CNATFWUNSAFMessage& aResponse,
                                 TUint32& aTimerVal )
    {
    }
    
// -----------------------------------------------------------------------------
// STUNUtils::Is5xxResponse
// -----------------------------------------------------------------------------
//
TBool STUNUtils::Is5xxResponse( TInt aResponseCode )
    {
    }

// -----------------------------------------------------------------------------
// STUNUtils::RemoveUnknownAttributes
// -----------------------------------------------------------------------------
//
void STUNUtils::RemoveUnknownAttributes( CNATFWUNSAFMessage& aMessage,
    const CNATFWUNSAFUnknownAttributesAttribute& aUnknownAttr )
    {
    }
