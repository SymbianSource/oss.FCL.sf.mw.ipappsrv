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
    CSTUNTimerUser( aClient.TimerProvider() ),
    iClient( &aClient ),    
    iBinding( aBinding ),
    iSocket( &aSocket ),
    iDummyIP( KAFUnspec )
    {
    }

// ---------------------------------------------------------------------------
// Default constructor - overloaded
// ---------------------------------------------------------------------------
//
CBindingImplementation::CBindingImplementation( MSTUNBindingObserver& aClient,
                                        CBinding& aBinding,
                                        TUint aStreamId,
                                        TUint aConnectionId,
                                        MNcmConnectionMultiplexer* aMux ) :
    CSTUNTimerUser( aClient.TimerProvider() ),
    iClient( &aClient ),
    iBinding( aBinding ),
    iStreamId( aStreamId ),
    iConnectionId( aConnectionId ),
    iMux( aMux ),
    iDummyIP( KAFUnspec )
    {
    }
    
// ---------------------------------------------------------------------------
// CBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SendRequestL()
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SendRequestL( const TInetAddr& /*aDestAddr*/,
                                           TBool /*aAddFingerprint*/,
                                           TUint /*aRTvalue*/  )
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::AllocateRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::AllocateRequestL( TUint /*aRtoValue*/ )
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::StoreAddressL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::StoreAddressL( 
    const TInetAddr& /*ReflexiveAddr*/, const TInetAddr& /*aRelayAddr*/ )
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::RelayAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CBindingImplementation::RelayAddr() const
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SetActiveDestinationRequestL( 
    const TInetAddr& /*aRemoteAddr*/, TUint32& /*aTimerValue*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CBindingImplementation::ConnectRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::ConnectRequestL( const TInetAddr& /*aRemoteAddr*/ )
    {
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::NewRTTSampleMeasured
// ---------------------------------------------------------------------------
//
void CBindingImplementation::NewRTTSampleMeasured( TInt64& aRTTSample,
        TBool aRetransmitted )
    {
    }

// -----------------------------------------------------------------------------
// CBindingImplementation::CalculateNewRTO
// -----------------------------------------------------------------------------
//
void CBindingImplementation::CalculateNewRTO( TInt64& aRTTSample )
    {
    }


// -----------------------------------------------------------------------------
// CBindingImplementation::CancelRetransmission
// -----------------------------------------------------------------------------
//
void CBindingImplementation::CancelRetransmission()
    {
    }


// End of file
   
