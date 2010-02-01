/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Wrapper class to convert multiplexer connection into object.
*
*/



#include "mncmconnectionmultiplexer.h"
#include "natfwcandidate.h"
#include "ciceconnection.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIceConnection::CIceConnection( TUint aStreamId, 
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver )
        :
        iStreamId( aStreamId ),
        iMultiplexer( aMultiplexer ),
        iObserver( aObserver )
    {
    
    }


void CIceConnection::ConstructL( TUint aAddrFamily )
    {
    iMultiplexer.RegisterConnectionObserverL( iStreamId, *this );
    iConnectionId 
        = iMultiplexer.CreateConnectionL( iStreamId, aAddrFamily );
    
    TUint32 dummyIap;
    TInt dummyQos;
    iMultiplexer.GetStreamInfoL( iStreamId, dummyIap, dummyQos, iProtocol );
    
    iLocalAddress = iMultiplexer.LocalIPAddressL( iStreamId, iConnectionId );
    }


CIceConnection* CIceConnection::NewL( TUint aStreamId, 
        TUint aAddrFamily,
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver )
    {
    CIceConnection* self = CIceConnection::NewLC( 
        aStreamId, aAddrFamily, aMultiplexer, aObserver );
    CleanupStack::Pop( self );
    return self;
    }


CIceConnection* CIceConnection::NewLC( TUint aStreamId, 
        TUint aAddrFamily,
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver )
    {
    CIceConnection* self = new( ELeave ) CIceConnection( 
        aStreamId, aMultiplexer, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aAddrFamily );
    return self;
    }


CIceConnection::~CIceConnection()
    {
    TRAP_IGNORE( iMultiplexer.UnregisterConnectionObserverL( 
        iStreamId, *this ) )
    
    TRAP_IGNORE( iMultiplexer.RemoveConnectionL( iStreamId, iConnectionId ) )
    iBoundCandidates.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CIceConnection::StreamId
// ---------------------------------------------------------------------------
//
TUint CIceConnection::StreamId() const
    {
    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CIceConnection::State
// ---------------------------------------------------------------------------
//
CIceConnection::TConnectionState CIceConnection::State() const
    {
    return iState;
    }


// ---------------------------------------------------------------------------
// CIceConnection::LocalAddress
// ---------------------------------------------------------------------------
//
const TInetAddr& CIceConnection::LocalAddress() const
    {
    return iLocalAddress;
    }


// ---------------------------------------------------------------------------
// CIceConnection::TransportProtocol
// ---------------------------------------------------------------------------
//
TUint CIceConnection::TransportProtocol() const
    {
    return iProtocol;
    }


// ---------------------------------------------------------------------------
// CIceConnection::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CIceConnection::ConnectionId() const
    {
    return iConnectionId;
    }    


// ---------------------------------------------------------------------------
// CIceConnection::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CIceConnection::SetReceivingStateL( TNATFWStreamingState aState,
        TConnectionNotifyState aNotifyState )
    {
    __ICEDP( "CIceConnection::SetReceivingStateL" )
    
    iNotifyState = aNotifyState;
    iMultiplexer.SetReceivingStateL( iStreamId, iConnectionId, aState );
    }


// ---------------------------------------------------------------------------
// CIceConnection::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CIceConnection::SetSendingStateL( TNATFWStreamingState aState,
        TConnectionNotifyState aNotifyState,
        const TInetAddr& aDestAddr )
    {
    __ICEDP( "CIceConnection::SetSendingStateL" )
    
    iNotifyState = aNotifyState;
    iMultiplexer.SetSendingStateL( 
        iStreamId, iConnectionId, aDestAddr, aState );
    }


// ---------------------------------------------------------------------------
// CIceConnection::BindWithCandidateL
// ---------------------------------------------------------------------------
//
void CIceConnection::BindWithCandidateL( const CNATFWCandidate& aCandidate )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC( aCandidate );
    iBoundCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    }


// ---------------------------------------------------------------------------
// CIceConnection::IsBoundToCandidate
// ---------------------------------------------------------------------------
//
TBool CIceConnection::IsBoundToCandidate( 
        const CNATFWCandidate& aCandidate ) const
    {
    TInt index( iBoundCandidates.Count() - 1 );
    TBool matchFound( EFalse );
    while ( !matchFound && 0 <= index )
        {
        CNATFWCandidate& item( *iBoundCandidates[index] );
        if ( aCandidate.Type() == item.Type() &&
             TIceUtils::MatchAddresses( aCandidate.TransportAddr(),
                 item.TransportAddr() ) &&
             TIceUtils::MatchAddresses( aCandidate.Base(), item.Base() ) )
             {
             matchFound = ETrue;
             }
        
        index--;
        }
    
    return matchFound;
    }


// ---------------------------------------------------------------------------
// From class MNcmConnectionObserver.
// ---------------------------------------------------------------------------
//
void CIceConnection::ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TConnectionNotifyType aType, TInt aError )
    {
    if ( iStreamId == aStreamId && iConnectionId == aConnectionId )
        {
        __ICEDP( "CIceConnection::ConnectionNotify, CONSUMING" )
        
        if ( ENotifyStateEnabled == iNotifyState )
            {
            MIceConnectionObserver::TNotifyType type 
            = ConvertToIceConnectionEvent( aType );
            
            if ( MIceConnectionObserver::ENotifyFirstMediaPacketSent != type )
                {
                iObserver.ConnectionNotify( *this, type, aError );
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceConnection::ConvertToIceConnectionEvent
// ---------------------------------------------------------------------------
//
MIceConnectionObserver::TNotifyType 
    CIceConnection::ConvertToIceConnectionEvent( 
        MNcmConnectionObserver::TConnectionNotifyType aType ) const
    {
    switch ( aType )
        {
        case MNcmConnectionObserver::EConnectionRemoved:
            return MIceConnectionObserver::ENotifyConnectionRemoved;
        
        case MNcmConnectionObserver::EConnectionError:
            return MIceConnectionObserver::ENotifyConnectionError;
        
        case MNcmConnectionObserver::EFirstMediaPacketSent:
            return MIceConnectionObserver::ENotifyFirstMediaPacketSent;
        
        case MNcmConnectionObserver::EReceivingActivated:
            return MIceConnectionObserver::ENotifyRecvActivated;
        
        case MNcmConnectionObserver::EReceivingDeactivated:
            return MIceConnectionObserver::ENotifyRecvDeactivated;
       
        case MNcmConnectionObserver::ESendingActivated:
            return MIceConnectionObserver::ENotifySendActivated;
        
        case MNcmConnectionObserver::ESendingDeactivated:
            return MIceConnectionObserver::ENotifySendDeactivated;
        
        default:
            ASSERT( EFalse );
            return MIceConnectionObserver::ENotifyUnknown; // lint#527
        }
    }
