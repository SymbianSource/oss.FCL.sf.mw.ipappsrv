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




#include "natfwunsaflog.h"
#include "stunassert.h"
#include "cstunbindinggetaddress.h"
#include "natfwbindingimplementationbase.h"
#include "stunutils.h"
#include "stunturnclientlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::CSTUNBindingGetAddress
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::CSTUNBindingGetAddress()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::CSTUNBindingGetAddress
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::CSTUNBindingGetAddress(
    const CSTUNBindingGetAddress& /*aGetAddress*/ ) :
    CSTUNBindingState()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::~CSTUNBindingGetAddress
// ---------------------------------------------------------------------------
//
CSTUNBindingGetAddress::~CSTUNBindingGetAddress()
    {
    iErrorBuffer.Close();    
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::PublicAddressReceivedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::PublicAddressReceivedL(
    CBindingImplementation& aContext,
    const TInetAddr& aPublicAddress ) const
    {
    NATFWUNSAF_ADDRLOG( "STUNBinding public addr", aPublicAddress )

    aContext.StoreAddressL( aPublicAddress );
    aContext.ChangeState( *iActive );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::PublicAddressReceivedL - overloaded for stun relay
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::PublicAddressReceivedL(
    CBindingImplementation& aContext,
    const TInetAddr& aReflexiveAddr,
    const TInetAddr& aRelayAddr ) const
    {
    NATFWUNSAF_ADDRLOG( "STUNRelayBinding public addr", aReflexiveAddr )
    NATFWUNSAF_ADDRLOG( "STUNRelayBinding relay addr", aRelayAddr )

    aContext.StoreAddressL( aReflexiveAddr, aRelayAddr );
    aContext.ChangeState( *iActive );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::TransactionError
// ---------------------------------------------------------------------------
//
void
CSTUNBindingGetAddress::TransactionError( CBindingImplementation& aContext,
                                          TInt aError )
    {
    __STUNTURNCLIENT_INT1( 
        "CSTUNBindingGetAddress::TransactionError Error: ", aError )
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    
    TInt error( KErrNone );
    TInt found( KErrNotFound );
    if ( iErrorBuffer.Count() > 0 )
        {
        found = iErrorBuffer.Find( aError );
        if ( KErrNotFound != found )
            {
            // Same error happened again, stop error handling
            __STUNTURNCLIENT( 
                "Same error happened again, end processing" )
            aContext.Terminate( aError );
            return;
            }
        }
    
    if ( iErrorBuffer.Count() == 0 || KErrNotFound == found  )
        {
        iErrorBuffer.Append( aError );
        }
    
    if ( aContext.HandleTransactionError( aError ) )
        {
        if ( STUNUtils::Is5xxResponse( aError ) )
            {
            __STUNTURNCLIENT( "Is5xxResponse" )
            aContext.WaitBeforeRetrying();
            aContext.ChangeState( *iWaitToRetry );
            }
        
        if ( E430StaleCredentials == aError || E436UnknownUsername == aError ||
             E432MissingUsername == aError || E434MissingRealm == aError )
            {
            aContext.ChangeState( *iGetSharedSecret );
            TRAP( error, aContext.ClientL().BindingErrorL( 
                aContext.Binding(), aError, EFalse ) );
            
            if ( KErrNone != error )
                {
                __STUNTURNCLIENT_INT1( "TransactionError leave ", error )
                aContext.Terminate( error );
                }
            }
        
        if ( E401Unauthorized == aError )
            {
            aContext.ChangeState( *iGetSharedSecret );
            
            TRAP( error, aContext.ClientL().BindingErrorL( 
                aContext.Binding(), aError, EFalse ) );
            if ( error != KErrNone )
                {
                __STUNTURNCLIENT_INT1( "TransactionError 401 LEAVED: ", error )
                aContext.Terminate( error );
                }
            }
        
        if ( E300TryAlternate == aError )
            {
            // last argument is true, because binding cannot be destroyed
            // before alternate server is read from it.
            TRAP_IGNORE( aContext.ClientL().BindingErrorL( 
                aContext.Binding(), aError, ETrue ) );
            }
            
        else
            {
            // this branch will send new request
            aContext.ChangeState( *iGetSharedSecret );
            TRAP( error, aContext.GetSharedSecretL() );
            if ( error != KErrNone )
                {
                aContext.Terminate( error );
                }
            }
        }
    
    else
        {
        aContext.Terminate( aError );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::SendRequestL
// No action, since obtaining a binding is already in progress.
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::SendRequestL(
    CBindingImplementation& /*aContext*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingGetAddress::SetNeighbourStates
// ---------------------------------------------------------------------------
//
void CSTUNBindingGetAddress::SetNeighbourStates(
    CSTUNBindingState& aGetSharedSecret,
    CSTUNBindingState& aWaitToRetry,
    CSTUNBindingState& aActive )
    {
    iGetSharedSecret = &aGetSharedSecret;
    iWaitToRetry = &aWaitToRetry;
    iActive = &aActive;
    }
