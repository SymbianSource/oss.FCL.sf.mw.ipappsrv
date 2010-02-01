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
#include "stunturnclientlogs.h"
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
    __STUNTURNCLIENT( "STUNUtils::GetMappedAddress" )
    CNATFWUNSAFMappedAddressAttribute* mappedAddress =
        static_cast<CNATFWUNSAFMappedAddressAttribute*>
            ( aResponse.Attribute( CNATFWUNSAFAttribute::EMappedAddress ) );
    if ( mappedAddress )
        {
        aAddr = mappedAddress->Address();
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// STUNUtils::GetXorMappedAddressL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetXorMappedAddressL( const CNATFWUNSAFMessage& aResponse,
                                       TInetAddr& aAddr )
    {
    __STUNTURNCLIENT( "STUNUtils::GetXorMappedAddressL" )
    CNATFWUNSAFXorMappedAddressAttribute* xorMappedAddress =
        static_cast<CNATFWUNSAFXorMappedAddressAttribute*>
            ( aResponse.Attribute( CNATFWUNSAFAttribute::EXorMappedAddress ) );
    if ( xorMappedAddress )
        {
        xorMappedAddress->SetTransactionIDAndXorL( aResponse.TransactionID() );
        aAddr = xorMappedAddress->Address();
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// STUNUtils::GetRelayAddressL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetRelayAddressL( const CNATFWUNSAFMessage& aResponse,
                                   TInetAddr& aAddr )
    {
    __STUNTURNCLIENT( "STUNUtils::GetRelayAddressL" )
    CNATFWUNSAFRelayAddressAttribute* relayAddress =
    static_cast<CNATFWUNSAFRelayAddressAttribute*>
            ( aResponse.Attribute( CNATFWUNSAFAttribute::ERelayAddress ) );
    
    if ( relayAddress )
        {
        aAddr = relayAddress->Address();
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// STUNUtils::GetTimerValueL
// -----------------------------------------------------------------------------
//
TBool STUNUtils::GetTimerValueL( const CNATFWUNSAFMessage& aResponse,
                                 TUint32& aTimerVal )
    {
    __STUNTURNCLIENT( "STUNUtils::GetTimerValueL" )
    CNATFWUNSAFTimerValAttribute* timerval =
    static_cast<CNATFWUNSAFTimerValAttribute*>
            ( aResponse.Attribute( CNATFWUNSAFAttribute::ETimerVal ) );
    
    if ( timerval )
        {
        aTimerVal = timerval->TimerVal();
        return ETrue;
        }
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// STUNUtils::Is5xxResponse
// -----------------------------------------------------------------------------
//
TBool STUNUtils::Is5xxResponse( TInt aResponseCode )
    {
    return ( aResponseCode >= 500 && aResponseCode < 600 );
    }

// -----------------------------------------------------------------------------
// STUNUtils::RemoveUnknownAttributes
// -----------------------------------------------------------------------------
//
void STUNUtils::RemoveUnknownAttributes( CNATFWUNSAFMessage& aMessage,
    const CNATFWUNSAFUnknownAttributesAttribute& aUnknownAttr )
    {
    __STUNTURNCLIENT( "STUNUtils::RemoveUnknownAttributes" )
    const RArray<TUint32>& unknown = aUnknownAttr.ContainedAttributes();

    for ( TInt i = 0; i < unknown.Count(); ++i )
        {        
        aMessage.DeleteAttribute( static_cast<TUint16>( unknown[i] ) );
        }
    __STUNTURNCLIENT( "STUNUtils::RemoveUnknownAttributes end" )
    }
