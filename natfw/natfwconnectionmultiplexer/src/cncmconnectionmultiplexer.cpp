/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cncmcallbackexecuter.h"
#include "ncmconnectionmultiplexerassert.h"
#include "cncmconnectionobserverhandler.h"

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
    iAsyncCallback = CNcmCallBackExecuter::NewL();
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
    delete iAsyncCallback;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateSessionL( TUint32 aIapId,
    TUint aPortRangeStart, TUint aPortRangeStop )
    {
    __CONNECTIONMULTIPLEXER_INT3(
        "CNcmConnectionMultiplexer::CreateSessionL - aIapId:", aIapId,
            "aPortRangeStart: ", aPortRangeStart, "aPortRangeStop: ", aPortRangeStop )
        
    CNcmSession* session = 
        CNcmSession::NewLC( iNextSessionId, aIapId,
            aPortRangeStart, aPortRangeStop, iObserver, *this );
        
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
    __CONNECTIONMULTIPLEXER_INT3(
        "CNcmConnectionMultiplexer::CreateStreamL - aSessionId:", aSessionId,
            "aQos: ", aQos, "aProtocol: ", aProtocol )
    
    CNcmSession* session( SessionByIdL( aSessionId ) );
    
    __ASSERT_ALWAYS( session->Initialized(), User::Leave( KErrNotReady ) ); 
    
    return ( session->CreateStreamL( aQos, aProtocol ) );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetStreamInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::GetStreamInfoL( 
    TUint aStreamId, TUint32& aIapId, TInt& aQos,
    TUint& aProtocol )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetStreamInfoL" )
    
    CNcmStream* stream( StreamByIdL( aStreamId ) );
       
    aIapId = SessionByStreamIdL( aStreamId )->IapId();
    aQos = stream->Qos();
    aProtocol = stream->StreamProtocol();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::LocalIPAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CNcmConnectionMultiplexer::LocalIPAddressL(
    TUint aStreamId, TUint aConnectionId )
    {
    return StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->LocalAddress();
    }
 
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ResolveDestinationAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::ResolveDestinationAddressL(
    TUint aStreamId, const TDesC8& aAddress, TUint aPort,
    TInetAddr& aResult )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::ResolveDestinationAddressL, FQDN" )

    SessionByStreamIdL( aStreamId )->ResolveDestinationAddressL(
            aAddress, aPort, aResult );
    }
 
   
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint aStreamId, TUint aAddrFamily )
    {              
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::CreateConnectionL - Entry, aStreamId:", aStreamId,
            "aAddrFamily: ", aAddrFamily )

    __CONNECTIONMULTIPLEXER_ASSERT_L( ( KAfInet == aAddrFamily || KAfInet6 == aAddrFamily ),
        KErrArgument );
        
    CNcmStream* stream( StreamByIdL( aStreamId ) );
    CNcmSession* session( SessionByStreamIdL( aStreamId ) );

    TInetAddr addr( KInetAddrNone, 0 );
    
    addr = ( KAfInet == aAddrFamily )
        ? session->LocalIPv4Address()
        : session->LocalIPv6Address();         

    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnectionMultiplexer::CreateConnectionL - ADDRESS: ", addr )

    __ASSERT_ALWAYS( !addr.IsUnspecified(), User::Leave( KErrNotFound ) );
   
    addr.SetPort( session->PortStore().Port() );
    
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnectionMultiplexer::CreateConnectionL - PORT: ", addr.Port() )
                    
    // Create Connection
    return stream->CreateConnectionL(
            *this, stream->ConnectionObserverHandler(),
            session->SocketServer(),
            session->Connection(), addr, EFalse );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL - overloaded
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint aStreamId, const TInetAddr& aLocalAddr )
    {
    __CONNECTIONMULTIPLEXER_ADDRLOG( 
        "CNcmConnectionMultiplexer::CreateConnectionL - overloaded - \
             Local address:", aLocalAddr )
               
    __CONNECTIONMULTIPLEXER_ASSERT_L( ( !aLocalAddr.IsUnspecified() ),
        KErrNotSupported );

    CNcmStream* stream( StreamByIdL( aStreamId ) );
    CNcmSession* session( SessionByStreamIdL( aStreamId ) );

    TInetAddr addr( KInetAddrNone, 0 );
    addr = aLocalAddr;            
                        
    // Create Connection
    return stream->CreateConnectionL(
            *this, stream->ConnectionObserverHandler(),
            session->SocketServer(),
            session->Connection(), addr, ETrue );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::OpenTcpConnectionL(
    TUint aStreamId, TUint aConnectionId, TNATFWTcpConnectionSetup aConfig,
    const TInetAddr& aDestAddr )
    { 
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::OpenTcpConnectionL - aStreamId:", aStreamId,
            "aConnectionId: ", aConnectionId )
    __CONNECTIONMULTIPLEXER_ADDRLOG( 
        "CNcmConnectionMultiplexer::OpenTcpConnectionL - aDestAddr: ", aDestAddr )
            
    StreamByIdL( aStreamId )->ConnectionL( aConnectionId )->
        OpenTcpConnectionL( aConfig, aDestAddr );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::CloseTcpConnection
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CloseTcpConnection(
    TUint aStreamId, TUint aConnectionId )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::CloseTcpConnection - aStreamId:", aStreamId,
            "aConnectionId: ", aConnectionId )
    
    TRAP_IGNORE( this->CloseTcpConnectionL( aStreamId, aConnectionId ) )
    }

    
// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::CloseTcpConnectionL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::CloseTcpConnectionL(
    TUint aStreamId, TUint aConnectionId )
    {
    __CONNECTIONMULTIPLEXER(
        "CNATFWConnectionMultiplexer::CloseTcpConnectionL" )
        
     StreamByIdL( aStreamId )->ConnectionL( aConnectionId )->
        CloseTcpConnectionL();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveSessionL( TUint aSessionId )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::RemoveSessionL - aSessionId:", aSessionId )

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
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::RemoveStreamL - aStreamId:", aStreamId )

    CNcmSession* session( SessionByStreamIdL( aStreamId ) );    
    session->RemoveStreamL( aStreamId );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveConnectionL(
    TUint aStreamId, TUint aConnectionId )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::RemoveConnectionL - aStreamId:", aStreamId,
            "aConnectionId: ", aConnectionId )
    
    CNcmStream* stream( StreamByIdL( aStreamId ) );
    
    if ( stream->MediaConnectionId() == aConnectionId )
        {
        // media connection can't be deleted.        
        User::Leave( KErrPermissionDenied );
        }
        
    stream->RemoveConnectionL( aConnectionId );     
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterIncomingConnectionObserverL( TUint aStreamId,
        MNcmIncomingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterIncomingConnectionObserver" )
        
    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterIncomingConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterOutgoingConnectionObserverL(
        TUint aStreamId, MNcmOutgoingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserver" )

    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterOutgoingConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterIncomingConnectionObserverL(
        TUint aStreamId,MNcmIncomingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserver" )

    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterIncomingConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterOutgoingConnectionObserverL(
        TUint aStreamId, MNcmOutgoingConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserver" )

    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterOutgoingConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterConnectionObserverL(
    TUint aStreamId, MNcmConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterConnectionObserverL" )

    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterConnectionObserverL(
    TUint aStreamId, MNcmConnectionObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterConnectionObserverL" )

    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterConnectionObserverL( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMessageObserverL(
    TUint aStreamId, MNcmMessageObserver& aObserver )
    {
    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        RegisterMessageObserver( aObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterMessageObserverL(
    TUint aStreamId, MNcmMessageObserver& aObserver )
    {
    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterMessageObserver( aObserver );   
    }
        

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMediaWrapperL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMediaWrapperL(
    MNATFWMediaWrapper* aMediaWrapper )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterMediaWrapperL" )
        
    __ASSERT_ALWAYS( aMediaWrapper, User::Leave( KErrNotReady ) );
    
    TUint streamId( aMediaWrapper->StreamId() );
    
    CNcmStream* stream( StreamByIdL( streamId ) );
    CNcmSession* session( SessionByStreamIdL( streamId ) );
      
    stream->RegisterWrapperL( aMediaWrapper );

    TInetAddr addr( session->LocalIPv4Address() );
    
    if ( addr.IsUnspecified() )
        {
        addr = session->LocalIPv6Address();
        
        if ( addr.IsUnspecified() )
            {
            User::Leave( KErrNotReady );
            }
        }

    // Register media source for stream    
    stream->RegisterMediaSourceL( CNcmMediaSource::NewLC( *this,
        *stream->WrapperL(), SessionByStreamIdL( streamId )->SocketServer(),
        addr ) );
         
    CleanupStack::Pop();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint aStreamId,
    TUint aConnectionId, const TDesC8& aMessage,
    MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SendL - aStreamId:", aStreamId,
            "aConnectionId: ", aConnectionId )
    
    CNcmConnection* connection = StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId );

    connection->Send( aMessage, KAFUnspec, aSenderObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint aStreamId,
    TUint aConnectionId, const TDesC8& aMessage,
    const TInetAddr& aDestinationAddress, 
    MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SendL - aStreamId:", aStreamId,
            "aConnectionId: ", aConnectionId )

    CNcmConnection* connection(StreamByIdL( aStreamId )->ConnectionL(
        aConnectionId ) ); 

    connection->Send( aMessage, aDestinationAddress, aSenderObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint aStreamId,
    const TDesC8& aMessage, const TInetAddr& aNextHopAddress,
    const TInetAddr& aDestinationAddress )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::SendL - aStreamId:", aStreamId )
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnectionMultiplexer::SendL - aDestinationAddress: ", aDestinationAddress )
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnectionMultiplexer::SendL - aNextHopAddress: ", aNextHopAddress )
    
    CNcmConnection* connection( NULL );
    CNcmStream* stream = StreamByIdL( aStreamId );
   
    connection = stream->ConnectionByDestinationAddressL( aNextHopAddress );  

    connection->Send( aMessage, aDestinationAddress, NULL );
    }

     
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint aStreamId,
    TUint aConnectionId, const TDesC8& aMessage, TBool aSendDirectly,
    MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL - \
         overloaded, send message directly from connection" )
    __CONNECTIONMULTIPLEXER_INT2( "CNcmConnectionMultiplexer::SendL - \
        aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )
        
    if ( aSendDirectly )
        {  
        StreamByIdL( aStreamId )->ConnectionL( aConnectionId )->
            SendDirectlyToNetwork( aMessage, aSenderObserver );
        }
    else
        {
        this->SendL( aStreamId, aConnectionId, aMessage, aSenderObserver );
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSend
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CancelMessageSend( TUint aStreamId,
    TUint aConnectionId, const MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CancelMessageSend " )

    TRAP_IGNORE( this->CancelMessageSendL( aStreamId, aConnectionId,
        aSenderObserver ) )
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSendL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::CancelMessageSendL( TUint aStreamId,
    TUint aConnectionId, const MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CancelMessageSendL " )
        
    StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->CancelMessageSendL( aSenderObserver ); 
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::PortStoreL
// ---------------------------------------------------------------------------
//
EXPORT_C CNcmPortStore& CNcmConnectionMultiplexer::PortStoreL(
    TUint aSessionId )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::PortStoreL" )

    return SessionByIdL( aSessionId )->PortStore();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetSessionInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C RSocketServ* CNcmConnectionMultiplexer::GetSessionInfoL(
    TUint aSessionId, TName& aConnectionName )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::GetSessionInfoL - aSessionId:", aSessionId )

    CNcmSession* session( SessionByIdL( aSessionId ) );
            
    User::LeaveIfError( session->GetConnectionName( aConnectionName ) );
    return &session->SocketServer();
    }
    

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateForMediaL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateForMediaL(
    TUint aStreamId, TUint aConnectionId, TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SetSendingStateForMediaL - \
         aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )
    
    CNcmStream* stream( StreamByIdL( aStreamId ) );
    MNATFWMediaWrapper* wrapper = stream->WrapperL();
    TMultiplexerConnectionNotifyType type( EMultiplexerConnectionError );
    
    CNcmConnection* connection( stream->ConnectionL( aConnectionId ) );
    stream->SetMediaConnectionId( aConnectionId );
    
    if ( aState == EStreamingStateActive )
        {
        stream->ConnectionObserverHandler().
            RegisterMediaSourceObserver( *connection ); 
        wrapper->SetReceivingStateForMuxWrapper( aState );   
        type = EMultiplexerMediaSendingActivated;
        }
    else if ( aState == EStreamingStatePassive )
        {          
        stream->ConnectionObserverHandler().
            UnregisterMediaSourceObserver( *connection );
        wrapper->SetReceivingStateForMuxWrapper( aState );
        type = EMultiplexerMediaSendingDeactivated;
        }
    else
        {
        __CONNECTIONMULTIPLEXER_ASSERT_L( EFalse, KErrNotSupported );
        }

    TNcmCallBack callback( HandleCallBack, *this, aStreamId, aConnectionId,
                            type, KErrNone );
    iAsyncCallback->AddCallBackL( callback );
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateForMediaL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateForMediaL(
    TUint aStreamId, TUint aConnectionId, TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SetReceivingStateForMediaL - \
         aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )
    
    CNcmStream* stream( StreamByIdL( aStreamId ) );
    TMultiplexerConnectionNotifyType type(
        EMultiplexerConnectionError );

    // Is connection real
    stream->ConnectionL( aConnectionId );
    stream->SetMediaConnectionId( aConnectionId );
          
    if ( EStreamingStateActive == aState )
        {
        type = EMultiplexerMediaReceivingActivated;
        }
    else if( EStreamingStatePassive == aState )
        {          
        type = EMultiplexerMediaReceivingDeactivated;
        }
    else
        {
        __CONNECTIONMULTIPLEXER_ASSERT_L( EFalse, KErrNotSupported );        
        }
                  
    stream->ConnectionObserverHandler().SetReceivingState( aState );
        
    TNcmCallBack callback( HandleCallBack, *this, aStreamId, aConnectionId,
                            type, KErrNone );
    iAsyncCallback->AddCallBackL( callback );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateL(
        TUint aStreamId, TUint aConnectionId, const TInetAddr& aDestAddr,
        TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SetSendingStateL - \
            aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )
        
    StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->SetSendingStateL( aDestAddr, aState );  
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateL(
    TUint aStreamId, TUint aConnectionId, TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SetReceivingStateL - \
            aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )
            
    StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->SetReceivingStateL( aState );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetAcceptedFromAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetAcceptedFromAddressL( 
    TUint aStreamId, TUint aConnectionId, const TInetAddr& aAddress )
    {
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::SetAcceptedFromAddressL - \
            aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )  
    
    StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->SetAcceptedFromAddressL( aAddress );
    }


// ---------------------------------------------------------------------------
// From class MNcmMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionError(
    TUint aSessionId, TUint aStreamId, TUint aConnectionId,
    TMultiplexerConnectionNotifyType aNotifyType, TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1( 
        "CNcmConnectionMultiplexer::ConnectionError - ERROR: ", aError )
    
    CNcmStream* stream( StreamById( aStreamId ) ); 
    
    if ( stream )
        {
        // Notify connection observers
        stream->ConnectionObserverHandler().
            ConnectionNotify( aStreamId, aConnectionId, aNotifyType, aError );
                                                      
        // Lets notify stream user                  
        iObserver.Notify( aSessionId, aStreamId,
            MNcmConnectionMultiplexerObserver::EStreamError, aError );
        }
    }


// ---------------------------------------------------------------------------
// From class MNcmMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::GetNewFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePort( TUint aStreamId,
    TUint aConnectionId, TUint& aPort )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::GetNewFreePort" )
        
    TRAP_IGNORE( GetNewFreePortL( aStreamId, aConnectionId, aPort ) )
    }
    

// ---------------------------------------------------------------------------
// From class MNcmMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotify( TUint aStreamId,
    TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
    TInt aError )
    {    
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::ConnectionNotify - \
            aStreamId:", aStreamId, "aConnectionId: ", aConnectionId )      
    __CONNECTIONMULTIPLEXER_INT2(
        "CNcmConnectionMultiplexer::ConnectionNotify - \
            aType:", aType, "aError: ", aError )  
             
    TRAP_IGNORE( this->ConnectionNotifyL( aStreamId, aConnectionId, aType,
        aError ) )      
    } 


// ---------------------------------------------------------------------------
// From class MNcmMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionNotifyL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotifyL( TUint aStreamId,
    TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
    TInt aError )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::ConnectionNotifyL" )
        
    TNcmCallBack callback( HandleCallBack, *this, aStreamId, aConnectionId,
                            aType, aError );
    iAsyncCallback->AddCallBackL( callback );        
    } 
 
         
// ---------------------------------------------------------------------------
// From class MNcmMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::GetNewFreePortL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePortL( TUint aStreamId,
    TUint /*aConnectionId*/, TUint& aPort )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::GetNewFreePortL" )
        
    aPort = SessionByStreamIdL( aStreamId )->PortStore().Port();
    }
 
       
// ---------------------------------------------------------------------------
// From class MNcmSessionObserver
//
// Called as a result for CreateSessionL() when wrapper is connected
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SessionCreationFailed( TUint aSessionId )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::SessionCreationFailed" )
    
    TRAP_IGNORE( RemoveSessionL( aSessionId ) )
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
    TUint aStreamId )
    {
    CNcmStream* stream( NULL );
    
    TRAP_IGNORE( ( stream = StreamByIdL( aStreamId ) ) )
 
    return stream;
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
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::WrapperSenderError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::WrapperSenderError( TUint aStreamId,
    TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1( 
        "CNcmConnectionMultiplexer::WrapperSenderObserver - Error: ", aError )
    
    TUint connectionId( 0 );
    
    TRAP_IGNORE( ( connectionId = StreamByIdL( aStreamId )->
        MediaConnectionId() ) )
        
    this->ConnectionNotify( aStreamId, connectionId,
        EMultiplexerConnectionError, aError );
    }


// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::GetFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetFreePort( TUint aStreamId, TUint& aPort )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetFreePort" )
    
    TRAP_IGNORE( ( aPort = SessionByStreamIdL( 
        aStreamId )->PortStore().Port() ) )
    }


// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::FirstMediaPacketSent
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::FirstMediaPacketSent( TUint aStreamId )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::FirstMediaPacketSent- aStreamId: ", aStreamId )
 
    TInt connectionId( 0 );
    
    TRAP_IGNORE(
        ( connectionId = StreamByIdL( aStreamId )->MediaConnectionId() ) )
    
    TRAP_IGNORE( this->ConnectionNotifyL( aStreamId, connectionId,
        EMultiplexerFirstMediaPacketSent, KErrNone ) )
    }


// ---------------------------------------------------------------------------
// From class MNcmAsyncCallbackObserver
//
// CNcmConnectionMultiplexer::HandleCallBack
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::HandleCallBack( CBase& aObject,
    TUint aStreamId, TUint aConnectionId,
    TMultiplexerConnectionNotifyType aType, TInt aError )
    {
    CNcmConnectionMultiplexer& connMux(
        static_cast<CNcmConnectionMultiplexer&>( aObject ) );
    
    TRAP_IGNORE( connMux.HandleCallBackL( aStreamId,
                                        aConnectionId,
                                        aType,
                                        aError ) )
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::HandleCallBackL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::HandleCallBackL( TUint aStreamId,
    TUint aConnectionId, TMultiplexerConnectionNotifyType aType, TInt aError )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::HandleCallBackL" )      
                        
    CNcmStream* stream( StreamByIdL( aStreamId ) );       
    TUint sessionId( SessionByStreamIdL( aStreamId )->SessionId() );
      
    switch ( aType )
        {
        case EMultiplexerMediaReceivingActivated:
            {          
            iObserver.Notify( sessionId, aStreamId,
                MNcmConnectionMultiplexerObserver::
                EReceivingActivated, aError );                   
            break;
            }
        case EMultiplexerMediaReceivingDeactivated:
            {          
            MNATFWMediaWrapper* wrapper = stream->WrapperL();
            wrapper->SetReceivingStateForMuxWrapper( EStreamingStateActive );
            iObserver.Notify( sessionId, aStreamId,
                MNcmConnectionMultiplexerObserver::
                EReceivingDeactivated, aError );                   
            break;
            }            
        case EMultiplexerMediaSendingActivated:
            {          
            iObserver.Notify( sessionId, aStreamId,
                MNcmConnectionMultiplexerObserver::
                ESendingActivated, aError );                   
            break;
            }
        case EMultiplexerMediaSendingDeactivated:
            {          
            MNATFWMediaWrapper* wrapper = stream->WrapperL();
            wrapper->SetReceivingStateForMuxWrapper( EStreamingStatePassive );
            iObserver.Notify( sessionId, aStreamId,
                MNcmConnectionMultiplexerObserver::
                ESendingDeactivated, aError );                   
            break;
            }
                
        case EMultiplexerReceivingActivated:
        case EMultiplexerSendingActivated:
        case EMultiplexerReceivingDeactivated:     
        case EMultiplexerSendingDeactivated:           
        case EMultiplexerConnectionRemoved:
        case EMultiplexerTcpSetupCompleted:
        case EMultiplexerFirstMediaPacketSent:
            {
            stream->ConnectionObserverHandler().
                ConnectionNotify( aStreamId, aConnectionId, aType, aError );
            break;
            }             
            
        case EMultiplexerConnectionError:
            { 
            if( aConnectionId == stream->MediaConnectionId() )
                {
                iObserver.Notify( sessionId, aStreamId,
                    MNcmConnectionMultiplexerObserver::EStreamError,
                    aError );
                }
                
            stream->ConnectionObserverHandler().
                ConnectionNotify( aStreamId, aConnectionId, aType, aError );
            break;
            }
        }                   
    }
