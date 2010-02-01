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
 
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "cncmportstore.h"
#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
#include "natfwmediawrapper.h"

const TUint KFirstSessionId = 1;


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
EXPORT_C CNcmConnectionMultiplexer* CNcmConnectionMultiplexer::NewL(
        MNcmConnectionMultiplexerObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::NewL" )

    CNcmConnectionMultiplexer* self =
        new (ELeave) CNcmConnectionMultiplexer( aObserver );

    return self;
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
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateSessionL( TUint32 aIapId,
    TUint aPortRangeStart, TUint aPortRangeStop )
    {
    CNcmSession* session = 
        CNcmSession::NewL( iNextSessionId, aIapId,
            aPortRangeStart, aPortRangeStop, iObserver, *this );
     
    CleanupStack::PushL( session ); 
    iSessions.AppendL( session );
    CleanupStack::Pop( session );
    
    iNextSessionId++;
    return session->SessionId();
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateStreamL( TUint aSessionId,
    TInt aQos, TUint aProtocol )
    {
     __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CreateStreamL" )
    
    CNcmSession* session( SessionByIdL( aSessionId ) );
    
    TUint streamId( session->CreateStreamL( aQos, aProtocol ) );;

    return streamId;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetStreamInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::GetStreamInfoL( 
    TUint /*aStreamId*/, TUint32& /*aIapId*/, TInt& /*aQos*/,
    TUint& /*aProtocol*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::LocalIPAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CNcmConnectionMultiplexer::LocalIPAddressL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    TInetAddr add;
    return add;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ResolveDestinationAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::ResolveDestinationAddressL(
    TUint /*aStreamId*/, const TDesC8& /*aAddress*/, TUint /*aPort*/,
    TInetAddr& /*aResult*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, TUint /*aAddrFamily*/ )
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL - overloaded
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, const TInetAddr& /*aLocalAddr*/ )
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::OpenTcpConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TNATFWTcpConnectionSetup /*aConfig*/, const TInetAddr& /*aDestAddr*/ )
    {
    
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CloseTcpConnection
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CloseTcpConnection(
        TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    
    }     
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveSessionL(
    TUint aSessionId )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveSessionL" )

    TInt sessionCount( iSessions.Count() );
    
    for ( TInt i = 0; i < sessionCount; i++ )
        {
        if ( iSessions[i]->SessionId() == aSessionId )
            {
            delete iSessions[i];
            iSessions.Remove( i );
            return;
            }
        }
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveStreamL( TUint aStreamId )
    {
        __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveStreamL" )

    CNcmSession* session( SessionByStreamIdL( aStreamId ) );    

    session->RemoveStreamL( aStreamId );
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterIncomingConnectionObserverL( TUint /*aStreamId*/,
        MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterIncomingConnectionObserverL(
        TUint /*aStreamId*/,MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMediaWrapperL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMediaWrapperL(
    MNATFWMediaWrapper* aMediaWrapper )
    {
    TUint streamId( aMediaWrapper->StreamId() );
    
    CNcmStream* stream( StreamByIdL( streamId ) );
    
    stream->RegisterWrapperL( aMediaWrapper );
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    const TInetAddr& /*aDestinationAddress*/,
    MNcmSenderObserver* /*aSenderObserver*/  )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    const TDesC8& /*aMessage*/, const TInetAddr& /*aNextHopAddress*/,
    const TInetAddr& /*aDestinationAddress*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/, TBool /*aSendDirectly*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSend
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CancelMessageSend( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CancelMessageSend " )
        
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetSessionInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C RSocketServ* CNcmConnectionMultiplexer::GetSessionInfoL( TUint /*aSessionId*/,
    TName& /*aConnectionName*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::PortStore
// ---------------------------------------------------------------------------
//
EXPORT_C CNcmPortStore& CNcmConnectionMultiplexer::PortStoreL(
    TUint aSessionId )
    {
    return SessionByIdL( aSessionId )->PortStore();
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateL(
        TUint aStreamId, TUint /*aConnectionId*/, const TInetAddr& /*aDestAddr*/,
        TNATFWStreamingState aState )
    {
    switch( aState )
        {
        case EStreamingStatePassive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        ESendingDeactivated, KErrNone );
            break;
        case EStreamingStateActive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        ESendingActivated, KErrNone );
            break;
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateL(
    TUint aStreamId, TUint /*aConnectionId*/, TNATFWStreamingState aState )
    {
    switch( aState )
        {
        case EStreamingStatePassive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        EReceivingDeactivated, KErrNone );
            break;
        case EStreamingStateActive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        EReceivingActivated, KErrNone );
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetAcceptedFromAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetAcceptedFromAddressL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/,
    const TInetAddr& /*aAddress*/ )
    {
    
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateForMediaL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateForMediaL(
    TUint aStreamId, TUint /*aConnectionId*/, TNATFWStreamingState aState )
    {
    switch( aState )
        {
        case EStreamingStatePassive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        ESendingDeactivated, KErrNone );
            break;
        case EStreamingStateActive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        ESendingActivated, KErrNone );
        default:
            break;
        }
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateForMediaL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateForMediaL(
    TUint aStreamId, TUint /*aConnectionId*/, TNATFWStreamingState aState )
    {
    switch( aState )
        {
        case EStreamingStatePassive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        EReceivingDeactivated, KErrNone );
            break;
        case EStreamingStateActive:
            iObserver.Notify( SessionByStreamIdL( aStreamId )->SessionId(),
                        aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        EReceivingActivated, KErrNone );
        default:
            break;
        }
    }

  
// ---------------------------------------------------------------------------
// From class MMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionError(
    TUint /*aSessionId*/, TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TMultiplexerConnectionNotifyType /*aNotifyType*/, TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MMultiplexerSocketDestinationObserver
//
// CNcmConnectionMultiplexer::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotify( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MMultiplexerSocketDestinationObserver
//
// CNcmConnectionMultiplexer::GetNewFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePort( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& /*aPort*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ConnectionNotifyL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotifyL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetNewFreePortL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePortL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& /*aPort*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// From class MMultiplexerSessionObserver
//
// Called as a result for CreateSessionL() when wrapper is connected
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SessionCreationFailed( TUint /*aSessionId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SessionByIdL()
// ---------------------------------------------------------------------------
//       
CNcmSession* CNcmConnectionMultiplexer::SessionByIdL(
    TUint aSessionId )
    {
    TInt ind( iSessions.Count() );
    
    while ( ind-- )
        {
        if ( iSessions[ind]->SessionId() == aSessionId )
            {
            return iSessions[ind];
            }
        }
   
    User::Leave( KErrNotFound );
#ifndef _DEBUG_EUNIT 
    return NULL;
#endif    
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::StreamByIdL()
// ---------------------------------------------------------------------------
//       
CNcmStream* CNcmConnectionMultiplexer::StreamByIdL(
    TUint aStreamId )
    {
    TInt sessInd( iSessions.Count() );
    
    while ( sessInd-- )
        {
        CNcmStream* stream = iSessions[sessInd]->
            StreamByIdL( aStreamId );
            
        if ( stream )
            {
            return stream;
            }
        }
   
    User::Leave( KErrNotFound ); 
#ifndef _DEBUG_EUNIT 
    return NULL;
#endif
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::StreamById()
// ---------------------------------------------------------------------------
//       
CNcmStream* CNcmConnectionMultiplexer::StreamById(
    TUint /*aStreamId*/ )
    {
    return NULL;
    }
 
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SessionByStreamIdL()
// ---------------------------------------------------------------------------
//       
CNcmSession* CNcmConnectionMultiplexer::SessionByStreamIdL(
    TUint aStreamId )
    {   
    TInt sessInd( iSessions.Count() );
    
    while ( sessInd-- )
        {
        CNcmStream* stream = iSessions[sessInd]->
            StreamByIdL( aStreamId );
            
        if ( stream )
            {
            return iSessions[sessInd];
            }
        }
    
    User::Leave( KErrNotFound );
#ifndef _DEBUG_EUNIT 
    return NULL;
#endif
    }   
  

// ---------------------------------------------------------------------------
// From class MMultiplexerMediaSourceObserver
//
// CNcmConnectionMultiplexer::WrapperSenderError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::WrapperSenderError( TUint /*aStreamId*/,
    TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1( 
        "CNcmConnectionMultiplexer::WrapperSenderObserver - Error: ", aError )
    }

// ---------------------------------------------------------------------------
// From class MMultiplexerMediaSourceObserver
//
// CNcmConnectionMultiplexer::GetFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetFreePort( TUint /*aStreamId*/, TUint& /*aPort*/ )
    {
    }


// ---------------------------------------------------------------------------
// From MNcmMediaSourceObserver
// CNcmConnectionMultiplexer::FirstMediaPacketSent
// ---------------------------------------------------------------------------
//  
void CNcmConnectionMultiplexer::FirstMediaPacketSent( TUint /*aStreamId*/ )
    {
    
    }
