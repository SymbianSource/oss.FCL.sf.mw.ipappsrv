/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation of STUN server client session
*
*/




#include "natfwstunsrvclientsession.h"
#include "natfwstunsrvimpl.h"

// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::CNATFWSTUNSrvClientSession
// ---------------------------------------------------------------------------
//
CNATFWSTUNSrvClientSession::CNATFWSTUNSrvClientSession()
    {    
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWSTUNSrvClientSession* CNATFWSTUNSrvClientSession::NewL( 
        MNATFWStunSrvObserver& aObserver,
        MNcmConnectionMultiplexer& aMultiplexer )
    {
    CNATFWSTUNSrvClientSession* self = 
                            new ( ELeave ) CNATFWSTUNSrvClientSession( );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aMultiplexer );
    CleanupStack::Pop( self );
    return self;
    }   


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::ConstructL 
// ---------------------------------------------------------------------------
//
void CNATFWSTUNSrvClientSession::ConstructL( MNATFWStunSrvObserver& aObserver,
                                   MNcmConnectionMultiplexer& aMultiplexer )
    {
    iImplementation = CNATFWSTUNSrvImpl::NewL( aObserver, aMultiplexer );
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::~CNATFWSTUNSrvClientSession 
// ---------------------------------------------------------------------------
//
 CNATFWSTUNSrvClientSession::~CNATFWSTUNSrvClientSession()
    {
    delete iImplementation;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::AddAuthenticationParamsL 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWSTUNSrvClientSession::AddAuthenticationParamsL(
    const RPointerArray<CNATFWCredentials>& aIdentifications )
    {
    iImplementation->AddAuthenticationParamsL( aIdentifications );
    }
     

// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::RemoveAuthenticationParamsL 
// ---------------------------------------------------------------------------
//     
EXPORT_C void CNATFWSTUNSrvClientSession::RemoveAuthenticationParamsL( 
    const RPointerArray<CNATFWCredentials>& aIdentifications )
    {
    iImplementation->RemoveAuthenticationParamsL( aIdentifications );
    }
   
    
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::SetRoleL
// ---------------------------------------------------------------------------
//     
EXPORT_C void CNATFWSTUNSrvClientSession::SetRoleL( 
    TNATFWIceRole aRole, TUint64 aTieBreaker ) 
    {
    iImplementation->SetRoleL( aRole, aTieBreaker );
    }
    
       

