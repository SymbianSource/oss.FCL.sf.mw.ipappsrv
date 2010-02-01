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



#include "natfwbindingimplementationbase.h"
#include "mstunbindingobserver.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CBindingImplementation::CBindingImplementation( MSTUNBindingObserver& aClient,
                                                CBinding& aBinding, 
                                                RSocket& aSocket ) :
    CTimer( EPriorityStandard ),
    iClient( &aClient ),    
    iBinding( aBinding ),
    iSocket( &aSocket )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Default constructor - overloaded
// ---------------------------------------------------------------------------
//
CBindingImplementation::CBindingImplementation( MSTUNBindingObserver& aClient,
                                                CBinding& aBinding,
                                                TUint aStreamId,
                                                TUint aSubstreamId ) :
    CTimer( EPriorityStandard ),
    iClient( &aClient ),
    iBinding( aBinding ),
    iStreamId( aStreamId ),
    iSubstreamId( aSubstreamId )
    {
    CActiveScheduler::Add( this );
    }
    
// ---------------------------------------------------------------------------
// CBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SendRequestL()
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::AllocateRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::AllocateRequestL()
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::StoreAddressL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::StoreAddressL( 
    const TInetAddr& /*ReflexiveAddr*/, const TInetAddr& /*aRelayAddr*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::FetchCandidateL( TInetAddr& /*aReflexiveAddr*/, 
                                              TInetAddr& /*aRelayAddr*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SetActiveDestinationRequestL( 
    const TInetAddr& /*aRemoteAddr*/ )
    {
    User::Leave( KErrNotFound );
    }
    
// ---------------------------------------------------------------------------
// CBindingImplementation::SendIndicationL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SendIndicationL( const TInetAddr& /*aRemoteAddr*/, 
                                              const TDesC8& /*aData*/ )
    {
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::ConnectRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::ConnectRequestL( const TInetAddr& /*aRemoteAddr*/ )
    {
    User::Leave( KErrNotFound );
    }
    
void CBindingImplementation::RunL()
    {
    
    }


// End of file
   
