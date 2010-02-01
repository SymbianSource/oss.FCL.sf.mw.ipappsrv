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
#include <es_sock.h>
 
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
//#include "natfwmediawrapper.h"

const TUint KFirstSessionId = 1;

//Media connection is allways the first one
const TUint KMediaConnection = 1;

const TUint KDefaultProtocolSource = 0;

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
    
    //__ASSERT_ALWAYS( session->Initialized(), User::Leave( KErrNotReady ) ); 
    
    TUint streamId( session->CreateStreamL( aQos, aProtocol ) );
   
    TInetAddr localAddr( session->LocalIPv4Address() );
    localAddr.SetPort( session->LocalIPv4Address().Port() );
    
    //StreamByIdL( streamId )->SetLocalAddr( localAddr );

    //Create a default connection for stream
    //CreateDefaultConnectionL( streamId, aProtocol );

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
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetStreamInfoL" )
/*    
    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
       
    aIapId = SessionByStreamIdL( aStreamId )->IapId();
    aQos = stream->Qos();
    aProtocol = stream->StreamProtocol();*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::LocalIPAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CNcmConnectionMultiplexer::LocalIPAddressL(
    TUint aStreamId, TUint /*aConnectionId*/ )
    {
    return SessionByStreamIdL( aStreamId )->LocalIPv4Address();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetSessionInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C RSocketServ* CNcmConnectionMultiplexer::GetSessionInfoL(
    TUint aSessionId, TName& aConnectionName )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetSessionInfoL" )
    //RSocketServ* socketServ;
    CNcmSession* session( SessionByIdL( aSessionId ) );
    
    //aSocketServ = session->SocketServer();
    User::LeaveIfError( session->GetConnectionName( aConnectionName ) );
    return &session->SocketServer();
    }
 
   
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, TUint /*aAddrFamily*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::CreateConnectionL" )
    
/*    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
    CNATFWMultiplexerSession* session( SessionByStreamIdL( aStreamId ) );
    
    // Create Connection
    TUint connectionId = stream->
        CreateConnectionL( *this, stream->ConnectionObserverHandler() );  
                                     
    // Retrive created connection instance
    CNATFWMultiplexerConnection* connection(
                                    stream->ConnectionL( connectionId ) );
          
    // set up source (ownership tranferred)
    connection->RegisterSource(
        CNATFWMultiplexerProtocolSource::NewL( *connection ) );  
    
    // set up destination (ownership tranferred)
    connection->SetDestination( CNATFWMultiplexerSocketDestination::NewL(
                aStreamId,
                connectionId,
                stream->Port(),
                stream->Qos(),
                session->SocketServer(),
                session->Connection(),         
                stream->StreamProtocol(),
                *connection,
                *this ) );*/
    return 2;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, const TInetAddr& /*aLocalAddr*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::CreateConnectionL" )
    
/*    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
    CNATFWMultiplexerSession* session( SessionByStreamIdL( aStreamId ) );
    
    // Create Connection
    TUint connectionId = stream->
        CreateConnectionL( *this, stream->ConnectionObserverHandler() );  
                                     
    // Retrive created connection instance
    CNATFWMultiplexerConnection* connection(
                                    stream->ConnectionL( connectionId ) );
          
    // set up source (ownership tranferred)
    connection->RegisterSource(
        CNATFWMultiplexerProtocolSource::NewL( *connection ) );  
    
    // set up destination (ownership tranferred)
    connection->SetDestination( CNATFWMultiplexerSocketDestination::NewL(
                aStreamId,
                connectionId,
                stream->Port(),
                stream->Qos(),
                session->SocketServer(),
                session->Connection(),         
                stream->StreamProtocol(),
                *connection,
                *this ) );*/
    return 2;
    }

 
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::OpenTcpConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TNATFWTcpConnectionSetup /*aConfig*/, const TInetAddr& /*aDestAddr*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::OpenTcpConnectionL" )
    }
        
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelTcpConnectionSetupL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CloseTcpConnection(
        TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::CloseTcpConnectionSetup" )
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveSessionL( TUint aSessionId )
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
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::RemoveConnectionL" )
    
/*    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
    
    if ( KMediaConnection == aConnectionId )
        {
        //Default media stream can't be deleted.        
        User::Leave( KErrPermissionDenied );
        }
        
    stream->RemoveConnectionL( aConnectionId );     */
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterIncomingConnectionObserverL( TUint /*aStreamId*/,
        MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterIncomingConnectionObserver" )
/*        
    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterIncomingConnectionObserverL( aObserver );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserver" )
/*
    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterOutgoingConnectionObserverL( aObserver );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterMessageObserverL" )
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterMessageObserverL" )
    }
            
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterIncomingConnectionObserverL(
        TUint /*aStreamId*/, MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserver" )
/*
    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterIncomingConnectionObserverL( aObserver );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserver" )
/*
    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterOutgoingConnectionObserverL( aObserver );*/
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterConnectionObserverL" )
/*
    StreamByIdL( aStreamId )->
        ConnectionObserverHandler().
        RegisterConnectionObserverL( aObserver );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::UnregisterConnectionObserverL" )
/*
    StreamByIdL( aStreamId )->ConnectionObserverHandler().
        UnregisterConnectionObserverL( aObserver );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMediaWrapperL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMediaWrapperL(
    MNATFWMediaWrapper* /*aMediaWrapper*/ )
    {
/*    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::RegisterMediaWrapperL" )
        
    __ASSERT_ALWAYS( aMediaWrapper, User::Leave( KErrNotReady ) );
    
    TUint streamId( aMediaWrapper->StreamId() );
    
    CNATFWMultiplexerStream* stream( StreamByIdL( streamId ) );
    
    stream->RegisterWrapperL( aMediaWrapper );
    
    // set up media source (ownership tranferred)
    CNATFWMultiplexerConnection* connection(
        stream->ConnectionL( KMediaConnection ) );
       
    User::LeaveIfError( connection->RegisterSource(
        CNATFWMultiplexerMediaSource::NewL( *connection, *this,
        stream->WrapperL(), SessionByStreamIdL( streamId )->SocketServer(),
        stream->LocalAddress() ) ) );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )
    
/*    CNATFWMultiplexerConnection* connection = StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId );
    
    __ASSERT_ALWAYS( connection, User::Leave( KErrArgument ) );  
 
    // Get source for plug-in (always the first one)
    RPointerArray<CNATFWMultiplexerSourceBase> sources;
    CleanupClosePushL( sources );
    
    connection->GetSources( sources );
    
    if ( !sources.Count() )
        {
        User::Leave( KErrNotFound );
        }
        
    // Send the message
    User::LeaveIfError( static_cast<CNATFWMultiplexerProtocolSource*>( 
        sources[KDefaultProtocolSource] )->Send( aMessage, KAFUnspec,
        aSenderObserver ) );

    CleanupStack::PopAndDestroy( &sources );*/
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
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )
    
/*    CNATFWMultiplexerConnection* connection( NULL );
    CNATFWMultiplexerStream* stream = StreamByIdL( aStreamId );
    
    if ( aConnectionId )
        {     
        connection = stream->ConnectionL( aConnectionId );      
        }
    else
        {
        connection =
            stream->ConnectionByDestinationAddressL( aDestinationAddress );
        } 
    
    __ASSERT_ALWAYS( connection, User::Leave( KErrArgument ) ); 
  
    // Get source for plug-in (always the first one)
    RPointerArray<CNATFWMultiplexerSourceBase> sources;
    CleanupClosePushL( sources );
    
    connection->GetSources( sources );
    
    __ASSERT_ALWAYS( sources.Count(), User::Leave( KErrNotFound ) );

    // Send the message
    User::LeaveIfError( static_cast<CNATFWMultiplexerProtocolSource*>( 
        sources[KDefaultProtocolSource] )->
            Send( aMessage, aDestinationAddress, aSenderObserver ) );

    CleanupStack::PopAndDestroy( &sources );     */
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/, TBool /*aSendDirectly*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )
/*     
    if ( aSendDirectly )
        {
        CNATFWMultiplexerSocketDestination* dest = 
            static_cast<CNATFWMultiplexerSocketDestination*>( 
            StreamByIdL( aStreamId )->
            ConnectionL( aConnectionId )->Destination() );
          
        dest->PutMessage( aMessage, aSenderObserver );
        }
    else
        {
        this->SendL( aStreamId, aConnectionId, aMessage, aSenderObserver );
        }*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C  void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/, const TDesC8& /*aMessage*/,
    const TInetAddr& /*aNextHopAddress*/, const TInetAddr& /*aDestinationAddress*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SendL" )
    }
        
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SocketHandlesL
// ---------------------------------------------------------------------------
//
//EXPORT_C RConnection& CNcmConnectionMultiplexer::GetSocketHandlesL(
//    TUint /*aStreamId*/, TUint /*aConnectionId*/, RSocketServ& /*aSocketServ*/,
//    RSocket& /*aSocket*/ )
//    {
//    __CONNECTIONMULTIPLEXER( 
//        "CNcmConnectionMultiplexer::GetSocketHandlesL" )
/*    
    CNATFWMultiplexerSocketDestination* dest = 
        static_cast<CNATFWMultiplexerSocketDestination*>( 
        StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->Destination() );
        
    aSocketServ = SessionByStreamIdL( aStreamId )->SocketServer();  
    aSocket = dest->Socket();
    
    return SessionByStreamIdL( aStreamId )->Connection();*/
//    RConnection* connection = NULL;
    
//    return *connection;
//    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::PortStore
// ---------------------------------------------------------------------------
//
EXPORT_C CNcmPortStore& CNcmConnectionMultiplexer::PortStoreL(
    TUint /*aSessionId*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::PortStore" )
    CNcmPortStore* store = NULL;
//    return SessionByIdL( aSessionId )->PortStore();
    return *store;
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateL(
        TUint /*aStreamId*/, TUint /*aConnectionId*/, const TInetAddr& /*aDestAddr*/,
        TNATFWStreamingState /*aState*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SetSendingStateL" )
/*    
    CNATFWMultiplexerSocketDestination* dest = 
        static_cast<CNATFWMultiplexerSocketDestination*>( 
        StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->Destination() );
        
    dest->SetSendingStateL( aDestAddr, aState );  */
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWStreamingState /*aState*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::SetReceivingStateL" )
/*        
    CNATFWMultiplexerSocketDestination* dest = 
        static_cast<CNATFWMultiplexerSocketDestination*>( 
        StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->Destination() );
        
    dest->SetReceivingStateL( aState );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetAcceptedFromAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetAcceptedFromAddressL( 
    TUint /*aStreamId*/, TUint /*aConnectionId*/, const TInetAddr& /*aAddress*/ )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionMultiplexer::SetAcceptedFromAddressL" )
    
/*    CNcmSocketDestination* dest = 
        static_cast<CNcmSocketDestination*>( 
        StreamByIdL( aStreamId )->
        ConnectionL( aConnectionId )->Destination() );
    
    dest->SetAcceptedFromAddress( aAddress );*/
    }


// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionError(
    TUint /*aSessionId*/, TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TMultiplexerConnectionNotifyType /*aNotifyType*/, TInt /*aError*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::MultiplexerError" )
    
/*    CNATFWMultiplexerStream* stream( StreamById( aStreamId ) ); 
    
    if ( stream )
        {
        // Notify connection observers
        stream->ConnectionObserverHandler().
            ConnectionNotify( aStreamId, aConnectionId, aNotifyType, aError );
                                                      
        // Lets notify stream user                  
        iObserver.Notify( aSessionId, aStreamId,
            MNcmConnectionMultiplexerObserver::EStreamError, aError );
        }*/
    }


// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerSocketDestinationObserver
//
// CNcmConnectionMultiplexer::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotify( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::ConnectionNotify" )
/*        
    TRAP_IGNORE(
        ConnectionNotifyL( aStreamId, aConnectionId, aType, aError ) )*/
    }


// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerSocketDestinationObserver
//
// CNcmConnectionMultiplexer::GetNewFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePort( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& /*aPort*/ )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::GetNewFreePort" )
        
    //TRAP_IGNORE( GetNewFreePortL( aStreamId, aConnectionId, aPort ) )
    }
    

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ConnectionNotifyL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotifyL( TUint /*aStreamId*/,
    TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
    TInt /*aError*/ )
    {
    
//    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
       
//    TUint sessionId( SessionByStreamIdL( aStreamId )->SessionId() );
    
    if ( KMediaConnection == aConnectionId )
        {        
        switch ( aType )
            {
            case EMultiplexerReceivingActivated:
                {          
//                MNATFWMediaWrapper& wrapper = stream->WrapperL();
//                TRAPD( error, wrapper.ConnectL() );
      
/*                if ( KErrNone != error )
                    {
                    __CONNECTIONMULTIPLEXER_INT1(
                        "CNcmConnectionMultiplexer::ConnectionNotify - Wrapper connecting error: ", error )
                    
                    CNATFWMultiplexerSocketDestination* dest = 
                        static_cast<CNATFWMultiplexerSocketDestination*>( 
                        stream->ConnectionL( aConnectionId )->Destination() );
                        
                    dest->SetReceivingStateL( EStreamingStatePassive );  
                    
                    iObserver.Notify( sessionId, aStreamId,
                        MNcmConnectionMultiplexerObserver::EStreamError,
                        aError );
                    }
                else
                    {
                    iObserver.Notify( sessionId, aStreamId,
                        MNcmConnectionMultiplexerObserver::
                        EReceivingActivated, aError );
                    }*/
                break;
                }
           
            case EMultiplexerSendingActivated:
                {
/*                MNATFWMediaWrapper& wrapper = stream->WrapperL();           

                TRAPD( error, wrapper.StartListeningL() );
      
                if ( KErrNone != error )
                    {
                    __CONNECTIONMULTIPLEXER_INT1(
                        "CNcmConnectionMultiplexer::ConnectionNotify - Wrapper start listenin error: ", error )
                    
                    CNATFWMultiplexerSocketDestination* dest = 
                        static_cast<CNATFWMultiplexerSocketDestination*>( 
                        stream->ConnectionL( aConnectionId )->Destination() );
                        
                    dest->SetSendingStateL( KAFUnspec, EStreamingStatePassive );  
                    
                    iObserver.Notify( sessionId, aStreamId,
                        MNcmConnectionMultiplexerObserver::EStreamError,
                        aError );               

                    }
                else
                    {
                    iObserver.Notify( sessionId, aStreamId,
                        MNcmConnectionMultiplexerObserver::ESendingActivated
                        ,aError );
                    }  
                              
  */  
                break;
                }
                
            case EMultiplexerReceivingDeactivated:
                {
/*                MNATFWMediaWrapper& wrapper = stream->WrapperL();            
                wrapper.DeactivateSending();

                iObserver.Notify( sessionId, aStreamId,
                    MNcmConnectionMultiplexerObserver::EReceivingDeactivated
                    ,aError );                 
    */
                break;
                }    
                
            case EMultiplexerSendingDeactivated:
                {
/*                MNATFWMediaWrapper& wrapper = stream->WrapperL();             
                wrapper.DeactivateReceiving();

                iObserver.Notify( sessionId, aStreamId,
                    MNcmConnectionMultiplexerObserver::ESendingDeactivated
                    ,aError );                 */
    
                break;
                }  
            case EMultiplexerConnectionError:
                { 
/*                iObserver.Notify( sessionId, aStreamId,
                    MNcmConnectionMultiplexerObserver::EStreamError,
                    aError );              */
                break;
                }
            case EMultiplexerConnectionRemoved:
                {
                break;
                }    
            }       
        }
    else
        {
/*        stream->ConnectionObserverHandler().
            ConnectionNotify( aStreamId, aConnectionId, aType, aError );*/
        }              
    }
  
    
// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerSessionObserver
//
// Called as a result for CreateSessionL() when wrapper is connected
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SessionCreationFailed( TUint /*aSessionId*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::SessionCreationFailed" )
    
//    TRAP_IGNORE( RemoveSessionL( aSessionId ) )
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
//CNATFWMultiplexerStream* CNcmConnectionMultiplexer::StreamById(
//    TUint /*aStreamId*/ )
/*    {
    CNATFWMultiplexerStream* stream( NULL );
    
    TRAP_IGNORE( ( stream = StreamByIdL( aStreamId ) ) )
 
    return stream;
    }*/
 
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
// CNcmConnectionMultiplexer::CreateDefaultConnectionL
// ---------------------------------------------------------------------------
//     
//void CNcmConnectionMultiplexer::CreateDefaultConnectionL( TUint /*aStreamId*/,
//    TUint /*aProtocol*/ )
/*    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::CreateConnectionL" )
    
    CNATFWMultiplexerStream* stream( StreamByIdL( aStreamId ) );
    
    // Create Connection ( Create multiplexer )
    TUint connectionId = stream->
        CreateConnectionL( *this, stream->ConnectionObserverHandler() );  
                                     
    // Retrive created Multiplexer instance
    CNATFWMultiplexerConnection* connection =
        stream->ConnectionL( connectionId );
         
    // set up protocol source (ownership tranferred)
    connection->RegisterSource(
        CNATFWMultiplexerProtocolSource::NewL( *connection ) );  

    // set up destination (ownership tranferred)
    connection->SetDestination( CNATFWMultiplexerSocketDestination::NewL(
                aStreamId,
                connectionId,
                stream->Port(),
                stream->Qos(),
                SessionByStreamIdL( aStreamId )->SocketServer(),
                SessionByStreamIdL( aStreamId )->Connection(),
                aProtocol,
                *connection,
                *this ) );
    }*/

// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerMediaSourceObserver
//
// CNcmConnectionMultiplexer::WrapperSenderError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::WrapperSenderError( TUint /*aStreamId*/,
    TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1( 
        "CNcmConnectionMultiplexer::WrapperSenderObserver - Error: ", aError )
/*        
    this->ConnectionNotify( aStreamId, KMediaConnection,
        EMultiplexerConnectionError, aError );*/
    }

// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::FirstMediaPacketSent
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::FirstMediaPacketSent( TUint /*aStreamId*/ )
    {
/*    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnectionMultiplexer::FirstMediaPacketSent- aStreamId: ", aStreamId )
 
    TInt connectionId( 0 );
    
    TRAP_IGNORE(
        connectionId = StreamByIdL( aStreamId )->MediaConnectionId() )
    
    TRAP_IGNORE( this->ConnectionNotifyL( aStreamId, connectionId,
        EMultiplexerFirstMediaPacketSent, KErrNone ) )*/
    }


// ---------------------------------------------------------------------------
// From class MNATFWMultiplexerMediaSourceObserver
//
// CNcmConnectionMultiplexer::GetFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetFreePort( TUint /*aStreamId*/, TUint& /*aPort*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::GetFreePort" )       
    //TRAP_IGNORE( ( aPort = SessionByStreamIdL( aStreamId )->PortStore().Port() ) );
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSend
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::CancelMessageSend( TUint /*aStreamId*/, TUint /*aConnectionId*/,
        const MNcmSenderObserver* /*aSenderObserver*/ )
    {
    
    }
