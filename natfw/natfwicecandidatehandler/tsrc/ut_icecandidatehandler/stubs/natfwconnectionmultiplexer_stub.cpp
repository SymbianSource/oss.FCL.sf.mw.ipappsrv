/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connection multiplexer is responsible for handling network
*                connections, send and receive data from the
*                network and allow client to start media flow between
*                connection multiplexer and socket media connection wrapper.
*
*/




#include <in_sock.h>
 
#include "cncmconnectionmultiplexer_stub.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "cncmportstore.h"
#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
#include "natfwmediawrapper.h"
#include "cncmcallbackexecuter.h"
#include "ncmconnectionmultiplexerassert.h"
#include "cncmconnectionobserverhandler.h"

const TUint KFirstSessionId = 1;

const TUint KDefaultProtocolSource = 0;

TInt CNcmConnectionMultiplexer::HandleCallback( TAny* aObject )
    {
    if ( aObject )
        {
        static_cast<CNcmConnectionMultiplexer*>( aObject )
            ->DoHandleCallback();
        }
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CNcmConnectionMultiplexer
// ---------------------------------------------------------------------------
//
CNcmConnectionMultiplexer::CNcmConnectionMultiplexer(
    MNcmConnectionMultiplexerObserver& aObserver ) :
    iObserver( aObserver ), iNextSessionId( KFirstSessionId )
    {
    
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::NewL
// ---------------------------------------------------------------------------
//
CNcmConnectionMultiplexer* CNcmConnectionMultiplexer::NewL(
        MNcmConnectionMultiplexerObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::NewL" )

    CNcmConnectionMultiplexer* self =
        new (ELeave) CNcmConnectionMultiplexer( aObserver );
    CleanupStack::PushL( self );        
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ConstructL
// -----------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer
// ---------------------------------------------------------------------------
//
CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer()
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer" )
    
    iSessions.ResetAndDestroy();
    iIncomingObservers.Close();
    iRequestDatas.Close();
    iCallbacks.ResetAndDestroy();
    iStreamObserverAssocs.Close();
    iConnectionDatas.Close();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateSessionL
// ---------------------------------------------------------------------------
//
TUint CNcmConnectionMultiplexer::CreateSessionL( 
        TUint32 aIapId,
        TUint /*aPortRangeStart*/,
        TUint /*aPortRangeStop*/ )
    {
    iIapId = aIapId;    
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNcmConnectionMultiplexer::CreateStreamL( TUint /*aSessionId*/,
        TInt aQos, TUint aProtocol )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CreateStreamL" )
    
    iQos = aQos;
    iConnectionProtocol = aProtocol;
    return iStreamId++;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetStreamInfoL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetStreamInfoL( TUint /*aStreamId*/,
    TUint32& aIapId, TInt& aQos, TUint& aProtocol )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetStreamInfoL" )
        
    aIapId = iIapId;
    aQos = iQos;
    iConnectionProtocol = KProtocolInetUdp;
    aProtocol = iConnectionProtocol;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::LocalIPAddressL
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmConnectionMultiplexer::LocalIPAddressL( 
    TUint aStreamId, TUint aConnectionId )
    {
    for ( TInt i = 0; i < iConnectionDatas.Count(); i++ )
        {
        if ( iConnectionDatas[i].iStreamId == aStreamId && 
             iConnectionDatas[i].iConnectionId == aConnectionId )
            {
            return iConnectionDatas[i].iAddress;
            }
        }
    
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::LocalIPAddressL ASSERT!" )
    ASSERT( EFalse );
    return iLocalAddr;
    }
 
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ResolveDestinationAddressL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ResolveDestinationAddressL(
    TUint /*aStreamId*/, const TDesC8& /*aAddress*/, TUint /*aPort*/,
    TInetAddr& /*aResult*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint aStreamId, TUint aAddrFamily )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::CreateConnectionL" )
    
    TInetAddr addr( INET_ADDR( 192, 168, 1, 2 ), 16384 + iConnectionId );
    addr.SetFamily( aAddrFamily );
    TConnectionData data( aStreamId, iConnectionId, addr );
    iConnectionDatas.AppendL( data );
    
    return iConnectionId++;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL - overloaded
// ---------------------------------------------------------------------------
//
TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint aStreamId, const TInetAddr& aLocalAddr )
    {
    TConnectionData data( aStreamId, iConnectionId, aLocalAddr );
    iConnectionDatas.AppendL( data );
    
    return iConnectionId++;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::OpenTcpConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWTcpConnectionSetup /*aConfig*/,
    const TInetAddr& /*aDestAddr*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::CloseTcpConnection
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::CloseTcpConnection(
    TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveSessionL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RemoveSessionL( TUint /*aSessionId*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveSessionL" )
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveStreamL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RemoveStreamL( TUint /*aStreamId*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveStreamL" )
    
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveConnectionL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RemoveConnectionL(
        TUint aStreamId, TUint aConnectionId )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveConnectionL" )
    for ( TInt i = 0; i < iConnectionDatas.Count(); i++ )
        {
        if ( iConnectionDatas[i].iStreamId == aStreamId && 
             iConnectionDatas[i].iConnectionId == aConnectionId )
            {
            iConnectionDatas.Remove(i);
            }
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL(
    TUint /*aStreamId*/,
    MNcmIncomingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL" )

    iIncomingObservers.Append( &aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL(
    TUint /*aStreamId*/,
    MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL" )
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL(
        TUint /*aStreamId*/,
        MNcmIncomingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserver" )
    
    TInt index = iIncomingObservers.Find( &aObserver );
    if ( KErrNotFound != index )
        {
        iIncomingObservers.Remove( index );
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::
    UnregisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/,
        MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserver" )
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RegisterConnectionObserverL( TUint aStreamId,
    MNcmConnectionObserver& aObserver )
    {
    TStreamObserverAssoc assoc( aStreamId, &aObserver );
    iStreamObserverAssocs.AppendL( assoc );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::UnregisterConnectionObserverL( TUint aStreamId,
    MNcmConnectionObserver& aObserver )
    {
    TStreamObserverAssoc assoc( aStreamId, &aObserver );
    TInt ind = iStreamObserverAssocs.FindL( assoc );
    iStreamObserverAssocs.Remove( ind );
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMessageObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RegisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterMessageObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::UnregisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {

    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMediaWrapperL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::RegisterMediaWrapperL(
        MNATFWMediaWrapper* /*aMediaWrapper*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterMediaWrapperL" )
        
    
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )

    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    const TInetAddr& /*aDestinationAddress*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )

    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    const TDesC8& /*aMessage*/, const TInetAddr& /*aNextHopAddress*/,
    const TInetAddr& /*aDestinationAddress*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/, TBool /*aSendDirectly*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )

    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSend
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::CancelMessageSend( 
        TUint /*aStreamId*/, TUint /*aConnectionId*/, 
        const MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CancelMessageSend" )
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::PortStoreL
// ---------------------------------------------------------------------------
//
CNcmPortStore& CNcmConnectionMultiplexer::PortStoreL( TUint /*aSessionId*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::PortStoreL" );
    return *iPortStore;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetSessionInfoL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetSessionInfoL(
    TUint /*aSessionId*/, RSocketServ& /*aSocketServ*/, TName& /*aConnectionName*/ )
    {
    }
    

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateForMediaL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SetSendingStateForMediaL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWStreamingState /*aState*/ )
    {
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateForMediaL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SetReceivingStateForMediaL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWStreamingState /*aState*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SetSendingStateL(
        TUint aStreamId, TUint aConnectionId, 
        const TInetAddr& /*aDestAddr*/,
        TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SetSendingStateL" )

    if ( iConnectionNotifyEnabled )
        {
        MNcmConnectionObserver::TConnectionNotifyType notifyType 
            = EStreamingStateActive == aState 
                ? MNcmConnectionObserver::ESendingActivated
                : MNcmConnectionObserver::ESendingDeactivated;        
        
        TRequestData data( aStreamId, aConnectionId, notifyType );
        iRequestDatas.AppendL( data );
        
        TCallBack callback( CNcmConnectionMultiplexer::HandleCallback, this );
        CAsyncCallBack* asyncCallback = new (ELeave) CAsyncCallBack( 
            callback, CActive::EPriorityStandard );
        CleanupStack::PushL( asyncCallback );
        iCallbacks.AppendL( asyncCallback );
        CleanupStack::Pop( asyncCallback );
        asyncCallback->CallBack();
        }
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SetReceivingStateL(
    TUint aStreamId, TUint aConnectionId, TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::SetReceivingStateL" )

    if ( iConnectionNotifyEnabled )
        {
        MNcmConnectionObserver::TConnectionNotifyType notifyType 
            = EStreamingStateActive == aState 
                ? MNcmConnectionObserver::EReceivingActivated
                : MNcmConnectionObserver::EReceivingDeactivated;
        
        TRequestData data( aStreamId, aConnectionId, notifyType );
        iRequestDatas.AppendL( data );
        
        TCallBack callback( CNcmConnectionMultiplexer::HandleCallback, this );
        CAsyncCallBack* asyncCallback = new (ELeave) CAsyncCallBack( 
            callback, CActive::EPriorityStandard );
        CleanupStack::PushL( asyncCallback );
        iCallbacks.AppendL( asyncCallback );
        CleanupStack::Pop( asyncCallback );
        asyncCallback->CallBack();
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetAcceptedFromAddressL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SetAcceptedFromAddressL( 
    TUint /*aStreamId*/, TUint /*aConnectionId*/, const TInetAddr& /*aAddress*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::SetAcceptedFromAddressL" )
    }


void CNcmConnectionMultiplexer::DoHandleCallback()
    {
    if ( iRequestDatas.Count() )
        {
        TRequestData& data( iRequestDatas[0] );
        TInt index( iStreamObserverAssocs.Count() - 1 );
        for ( TInt i( index ); 0 <= i; --i )
            {
            if ( iStreamObserverAssocs[i].iStreamId == data.iStreamId )
                {
                iStreamObserverAssocs[i].iObserver->ConnectionNotify( 
                    data.iStreamId, data.iConnectionId, data.iNotifyType, 
                    KErrNone );
                }
            }
        
        iRequestDatas.Remove( 0 );
        }
    }
