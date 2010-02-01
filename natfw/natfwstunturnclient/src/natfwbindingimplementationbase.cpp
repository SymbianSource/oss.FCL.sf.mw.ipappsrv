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
#include "stunturnclientlogs.h"

// ======== MEMBER FUNCTIONS ========

const TReal KAlpha = 0.125;   // alpha = 1/8   RFC 2988
const TReal KBeta = 0.25;     // beta = 1/4    RFC 2988

const TReal KAlphaRemainder = 1 - KAlpha;  // 1 - alpha = 7/8 (0.875)   RFC 2988 2.3
const TReal KBetaRemainder = 1 - KBeta;     // 1 - beta = 3/4 (0.75)  RFC 2988 2.3

const TInt KRTTVARFactor = 4;  // RFC 2988 2.3


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
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SendRequestL( const TInetAddr& /*aDestAddr*/,
                                           TBool /*aAddFingerprint*/,
                                           TUint /*aRTvalue*/  )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::AllocateRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::AllocateRequestL( TUint /*aRtoValue*/ )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::StoreAddressL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::StoreAddressL( 
    const TInetAddr& /*ReflexiveAddr*/, const TInetAddr& /*aRelayAddr*/ )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::RelayAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CBindingImplementation::RelayAddr() const
    {
    return iDummyIP;
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::SetActiveDestinationRequestL( 
    const TInetAddr& /*aRemoteAddr*/, TUint32& /*aTimerValue*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
// ---------------------------------------------------------------------------
// CBindingImplementation::ConnectRequestL
// ---------------------------------------------------------------------------
//
void CBindingImplementation::ConnectRequestL( const TInetAddr& /*aRemoteAddr*/ )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CBindingImplementation::NewRTTSampleMeasured
// ---------------------------------------------------------------------------
//
void CBindingImplementation::NewRTTSampleMeasured( TInt64& aRTTSample,
        TBool aRetransmitted )
    {
    __STUNTURNCLIENT( "CBindingImplementation::NewRTTSampleMeasured" )

    if ( aRetransmitted )
        {
        // If segment was retransmitted use backed off RTO for next segment
        iRetransmitInterval = I64INT( aRTTSample );
        }
    else
        {
        CalculateNewRTO( aRTTSample );
        iRetransmitInterval = I64INT( iRTO );
        }
        
    __STUNTURNCLIENT_INT1( "CBindingImplementation::NewRTTSampleMeasured - iRetransmitInterval:", iRetransmitInterval )                  
    }

// -----------------------------------------------------------------------------
// CBindingImplementation::CalculateNewRTO
// -----------------------------------------------------------------------------
//
void CBindingImplementation::CalculateNewRTO( TInt64& aRTTSample )
    {
    __STUNTURNCLIENT( "CBindingImplementation::RemoveAddress" )

    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - New RTT Sample:", aRTTSample )
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - current iRTTVAR:", iRTTVAR )
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - current iSRTT:", iSRTT )
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - current iRTO:", iRTO ) 
    
    if ( iFirstRTOCalculated )
        {
        /* 
        RTTVAR <- (1 - beta) * RTTVAR + beta * |SRTT - R'|
        SRTT <- (1 - alpha) * SRTT + alpha * R'
        RTO <- SRTT + max (G, K*RTTVAR) 
        */
              
        TInt64 diff = Abs( iSRTT - aRTTSample );
        
        iRTTVAR = TInt64( ( KBetaRemainder * iRTTVAR ) + ( KBeta * diff ) );
        
        iSRTT = TInt64( ( KAlphaRemainder * iSRTT ) + ( KAlpha * aRTTSample ) );
        
        // rfc2988 if the K*RTTVAR term in the RTO calculation equals zero,
        // the variance term MUST be rounded to G seconds
        TInt64 rttvar( 0 );
        rttvar = ( 0 == iRTTVAR ) ? 1 : iRTTVAR;
                
        iRTO = iSRTT + ( KRTTVARFactor * rttvar );        
        }
    else
        {       
        /*    
        (2.2) When the first RTT measurement R is made
        SRTT <- R
        RTTVAR <- R/2
        RTO <- SRTT + max (G, K*RTTVAR)     
        */
        
        iSRTT = aRTTSample;     
        iRTTVAR = aRTTSample / TInt64( 2 );      
        iRTO = iSRTT + ( TInt64( 4 ) * iRTTVAR );
        
        iFirstRTOCalculated = ETrue;
        }
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - new iRTTVAR:", iRTTVAR )
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - new iSRTT:", iSRTT )
    __STUNTURNCLIENT_INT1( "CBindingImplementation::CalculateNewRTO - new iRTO:", iRTO ) 
    }


// -----------------------------------------------------------------------------
// CBindingImplementation::CancelRetransmission
// -----------------------------------------------------------------------------
//
void CBindingImplementation::CancelRetransmission()
    {
    return;
    }


// End of file
   
