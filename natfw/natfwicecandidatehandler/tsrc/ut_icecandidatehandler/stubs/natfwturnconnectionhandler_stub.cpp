/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Creates connection to Connection Multiplexer and TURN client.
*                Handles connection specific issues and receives notifications
*                through implemented observers. Sends notifications to NAT FW
*                Client through NAT Protocol Plug-in observer.
*
*/




#include "mncmconnectionmultiplexer.h"
#include <cnatfwsettingsapi.h>
#include <mnatfwserversettings.h>
#include "natfwcandidate.h"
#include "mnatfwpluginobserver.h"
#include "natfwstunrelaybinding.h"
#include "natfwstunclient.h"
#include <mnatfwturnsettings.h>

#include "natfwturnconnectionhandler_stub.h"
#include "natfwturnpluginlogs.h"
#include "natfwturnrefreshtimer.h"
#include "natfwturnstreamdata.h"
#include "natfwturnactivedestinationtimer.h"
#include "cturnasynccallback.h"
#include "cnatfwturnserversettings.h"

#include "natfwunsafmessage.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsaftcprelaypacket.h"

const TUint KDefaultStunSrvPort = 3478;
const TUint KDefaultRefreshInterval = 28000000;
const TUint KMicrosecFactor = 1000000;
const TUint32 KWaitTimeAfterReset = 5000000;
// transitioning time changed from 3000000=>0 to get ICE work
const TUint32 KDefaultSrvTransitioningTime = 0;
const TUint32 KRetryTimeAfter439 = 5000;
const TUint KNoBinding = 437;
const TUint KServerTransitioning = 439;
const TUint KTryAlternate = 300;
const TInt KClientSpecificError = -11000;
const TUint32 KWaitTime = 0;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnConnectionHandler::CNATFWTurnConnectionHandler(
    const CNATFWPluginApi& aTurnPlugin,
    MNATFWPluginObserver& aPluginObserver ) 
    :
    CTimer( EPriorityStandard ),
    iTurnPlugin( aTurnPlugin ),
    iPluginObserver( aPluginObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::ConstructL()
    {
    CTimer::ConstructL();
    iAsyncCallback = CTurnAsyncCallback::NewL( iTurnPlugin, iPluginObserver );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnConnectionHandler* CNATFWTurnConnectionHandler::NewL(
    const CNATFWPluginApi& aTurnPlugin,
    MNATFWPluginObserver& aPluginObserver )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::NewL" )
    CNATFWTurnConnectionHandler* self = CNATFWTurnConnectionHandler::NewLC(
        aTurnPlugin, aPluginObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnConnectionHandler* CNATFWTurnConnectionHandler::NewLC(
    const CNATFWPluginApi& aTurnPlugin,
    MNATFWPluginObserver& aPluginObserver )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::NewLC" )
    CNATFWTurnConnectionHandler* self =
        new( ELeave ) CNATFWTurnConnectionHandler( 
            aTurnPlugin, aPluginObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWTurnConnectionHandler::~CNATFWTurnConnectionHandler()
    {
    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::~CNATFWTurnConnectionHandler start" )

    iServerList.ResetAndDestroy();
    CTimer::DoCancel();

    delete iDomain;
    delete iNatSettings;
    delete iTurnRefreshTimer;
    delete iActiveDestinationTimer;

    TInt count( iStreamArray.Count() );
    for ( TInt index = count - 1; index >= 0; index-- )
        {
        DeleteStream( index, ETrue );
        }

    TInt cmdCount( iCallBackCmds.Count() );

    for ( TInt i = 0; i < cmdCount; ++i )
        {
        delete reinterpret_cast<CBase*>( iCallBackCmds[i].iDataPtr );
        }

    iCallBackCmds.Close();
    iStreamArray.Close();
    delete iStunClient;
    delete iTimerServ;
    iConnection.Close();
    iConnMux = NULL;
    delete iAsyncCallback;

    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::~CNATFWTurnConnectionHandler end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::PluginInitializeL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::PluginInitializeL(
    TUint32 aIapId,
    const TDesC8& /*aDomain*/,
    MNcmConnectionMultiplexer& aMultiplexer )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::PluginInitializeL start" )
    iConnMux = &aMultiplexer;
    iIapId = aIapId;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::DoCancel.
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::DoCancel()
    {
    CTimer::DoCancel();
    }


// ---------------------------------------------------------------------------
// Sets active destination request. Before it can be done, a Send Indication
// must be sent. In case of TCP protocol a Connect Request must be sent
// before Send Indication.
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::RunL()
    {
    ExecuteCallBack();
    if ( iCallBackCmds.Count() )
        {
        After( KWaitTime );
        }
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::ConnectServerL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::ConnectServerL(
    const RSocketServ& /*aSocketServ*/,
    const TName& /*aConnectionName*/ )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectServerL start" )
    TCallBackCmd cmd = TCallBackCmd( 0, EConnectServer, 0, 0 );
    iCallBackCmds.AppendL( cmd );
    if ( !IsActive() )
        {
        After( KWaitTime );
        }
    
    /*
    iSocketServ = aSocketServ;
    TName name( aConnectionName );
    User::LeaveIfError( iConnection.Open( iSocketServ, name ) );
    this->TryNextServerL();
    
    // wait MNATFWStunClientObserver::STUNClientInitCompleted
    */
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectServerL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::FetchCandidateL( TUint aStreamId,
    TUint /*aRtoValue*/, TUint /*aAddrFamily*/, 
    const TInetAddr& aBaseAddr )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::FetchCandidateL start" )

    // Store data in array
    TStreamData streamData;
    streamData.iStreamId = aStreamId;

    CNATFWCandidate* newCandidate = CNATFWCandidate::NewLC();
    TInetAddr publicAddr;
    if ( KAfInet6 == aBaseAddr.Family() )
        {
        const TIp6Addr KIp6Addr = {{{1,2,3,4,5,6,7,8,9,1,0,0,0,0,0,2}}};
        publicAddr.SetAddress( KIp6Addr );
        publicAddr.SetPort( 1 );
        }
    else
        {
        publicAddr.SetAddress( INET_ADDR( 168,0,0,1 ) );
        }

    // Set candidate parameters
    newCandidate->SetStreamId( aStreamId );
    newCandidate->SetType( CNATFWCandidate::ERelay );
    newCandidate->SetTransportAddrL( publicAddr );
    newCandidate->SetTransportProtocol( KProtocolInetUdp );
    
    TConnectionData connData;
    connData.iLocalCandidate = newCandidate;
    streamData.iConnArray.AppendL( connData );
    iStreamArray.AppendL( streamData );    

    TCallBackCmd cmd 
        = TCallBackCmd( aStreamId, EFetchCandidate, 0, newCandidate );
    iCallBackCmds.AppendL( cmd );
    CleanupStack::Pop( newCandidate );
    
    if ( !IsActive() )
        {
        After( KWaitTime );
        }
    // wait MNcmConnectionObserver::ConnectionNotify
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::FetchCandidateL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::GetConnectionIdL(
    const CNATFWCandidate& /*aLocalCandidate*/,
    TUint aStreamId,
    TUint& aConnectionId )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::GetConnectionIdL start" )
    
    TInt streamInd( IndexByStreamId( aStreamId ) );
    TInt connInd( KErrNotFound );
    TBool connFound( EFalse );
    
    if ( KErrNotFound != streamInd )
        {
        connInd = iStreamArray[streamInd].iConnArray.Count() - 1;
        }
    
    while ( KErrNotFound != streamInd && !connFound && ( 0 <= connInd ) )
        {
        TConnectionData* connection = ConnectionByIndex( streamInd, connInd );
        
        //if ( aLocalCandidate == *connection->iLocalCandidate )
          //  {
            connFound = ETrue;
            aConnectionId = connection->iConnectionId;
            //}
        
        connInd--;
        }
    
    __ASSERT_ALWAYS( connFound, User::Leave( KErrNotFound ) );
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::GetConnectionIdL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::StartTurnRefresh
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::StartTurnRefresh()
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::StartTurnRefresh start" )

    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::CreateTURNBindingL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::CreateTURNBindingL( TUint /*aStreamId*/,
                                                      TUint /*aConnectionId*/ )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::CreateTURNBindingL" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::SetReceivingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::SetReceivingStateL start" )
    
    TUint streamId( aLocalCandidate.StreamId() );
    TUint connId( 0 );
    TConnectionData* connection = NULL;
    TRAPD( err, GetConnectionIdL( aLocalCandidate, streamId, connId ) );
    
    if ( !err )
        {
        TInt streamInd( IndexByStreamId( streamId ) );
        connection = ConnectionById( streamInd, connId );
        }
    
    /*if ( connection )
        {
        if ( EStreamingStateActive == aState )
            {
            if ( connection->iReceivingActivated  )
                {
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EActiveReceiving, streamId, KErrNone, NULL );
                }
            else
                {
                iConnMux->SetReceivingStateL( streamId, connId,
                    EStreamingStateActive );
                }
            }
        else
            {
            if ( !connection->iReceivingActivated )
                {
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EDeactiveReceiving, streamId, KErrNone, NULL );
                }
            else
                {
                iConnMux->SetReceivingStateL( streamId, connId,
                    EStreamingStatePassive );
                }
            }
        }
    else
        {*/
        if ( EStreamingStateActive == aState )
            {
            iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                EActiveReceiving, streamId, KErrNone, NULL );
            }
        else
            {
            iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                EDeactiveReceiving, streamId, KErrNone, NULL );
            }
        //}
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::SetReceivingStateL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::SetActiveDestinationRequestL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState,
    const TInetAddr& /*aDestAddr*/ )
    {
    __TURNPLUGIN(
    "CNATFWTurnConnectionHandler::SetActiveDestinationRequestL start" )
    
    TUint streamId( aLocalCandidate.StreamId() );
    TUint connId( 0 );
    TConnectionData* connection = NULL;
    TInt streamInd( IndexByStreamId( streamId ) );
    TRAPD( err, GetConnectionIdL( aLocalCandidate, streamId, connId ) );
    
    if ( !err )
        {
        connection = ConnectionById( streamInd, connId );
        }
    
    /*if ( connection )
        {
        if ( EStreamingStateActive == aState && TConnectionData::
            EActDestTransitioning == connection->iActDestState )
            {
            // Only Active Destination reset (sending passivation)
            // can be done when in "Transitioning" state.
            iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                EActiveSending, streamId, KErrNotReady, NULL );
            }
        else
            {
            if ( EStreamingStateActive == aState )
                {
                // Do Set Active Destination Request if the remote
                // address differs from current active destination
                
                if ( !connection->iCurrentActDest.CmpAddr( aDestAddr ) )
                    {
                    connection->iDestAddr.SetAddress( aDestAddr.Address() );
                    connection->iDestAddr.SetPort( aDestAddr.Port() );
                    
                    if ( KProtocolInetTcp == iStreamArray[streamInd].
                        iTransportProtocol )
                        {
                        // We need to do the Connect Request first
                        // and have successfull response to it.
                        connection->iTurnBinding->ConnectRequestL(
                            connection->iDestAddr );
                        }
                    else
                        {
                        connection->iAwaitingDataFromSrv = ETrue;
                        
                        connection->iTurnBinding->SendIndicationL(
                            connection->iDestAddr, KNullDesC8 );
                        }
                    }
                else
                    {
                    // No need to set this destination active again
                    iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                        EActiveSending, streamId, KErrNone, NULL );
                    }
                }
            else
                {
                // Plug-in's client wants to reset Active Destination.
                connection->iDestAddr = KAFUnspec;
                
                connection->iTurnBinding->SetActiveDestinationRequestL(
                    connection->iDestAddr, connection->iTimerValue );
                }
            }
        }
    else
        {*/
        if ( EStreamingStateActive == aState )
            {
            iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                EActiveSending, streamId, KErrNone, NULL );
            }
        else
            {
            iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                EDeactiveSending, streamId, KErrNone, NULL );
            }
        //}

    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::SetActiveDestinationRequestL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::IsRequestOrIndicationL
// ---------------------------------------------------------------------------
//
TBool CNATFWTurnConnectionHandler::IsRequestOrIndicationL(
    const TDesC8& aMessage ) const
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::IsRequestOrIndicationL" )

    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = decoder->DecodeL( aMessage );
    CleanupStack::PopAndDestroy( decoder );

    if ( CNATFWUNSAFMessage::EAllocateRequest == msg->Type() ||
         CNATFWUNSAFMessage::EConnectRequest == msg->Type() ||
         CNATFWUNSAFMessage::ESetActiveDestinationRequest == msg->Type() ||
         CNATFWUNSAFMessage::ESendIndication == msg->Type() )
        {
        delete msg;
        return ETrue;
        }
    else
        {
        delete msg;
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::IsTurnResponseL
// ---------------------------------------------------------------------------
//
TBool CNATFWTurnConnectionHandler::IsTurnResponseL(
    const TDesC8& aMessage ) const
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::IsTurnResponseL" )

    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = decoder->DecodeL( aMessage );
    CleanupStack::PopAndDestroy( decoder );

    if ( CNATFWUNSAFMessage::EAllocateResponse == msg->Type() ||
         CNATFWUNSAFMessage::EAllocateErrorResponse == msg->Type() ||
         CNATFWUNSAFMessage::ESetActiveDestinationResponse == msg->Type() ||
         CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse ==
            msg->Type() ||
         CNATFWUNSAFMessage::EConnectResponse == msg->Type() ||
         CNATFWUNSAFMessage::EConnectErrorResponse == msg->Type() )
        {
        delete msg;
        return ETrue;
        }
    else
        {
        delete msg;
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver.
// CNATFWTurnConnectionHandler::STUNClientInitCompleted
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/,
    TInt aCompletionCode )
    {
    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNClientInitCompleted start" )
    
    if ( KErrNone == aCompletionCode )
        {
        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
            TTurnPluginCallbackInfo::EConnectServer, 0,
            aCompletionCode, NULL ) )
        }
    else
        {
        TInt error( KErrNone );
        TRAP( error, TryNextServerL() );
        
        if ( KErrNone != error )
            {
            delete iStunClient;
            iStunClient = NULL;
            
            TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                TTurnPluginCallbackInfo::EConnectServer, 0,
                ( KClientSpecificError + aCompletionCode), NULL ) )
            }
        }
    
    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNClientInitCompleted end" )
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver
// CNATFWTurnConnectionHandler::STUNBindingEventOccurredL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::STUNBindingEventOccurredL(
    TSTUNBindingEvent aEvent,
    const CBinding& aBinding )
    {
    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNBindingEventOccurredL start" )
    
    const CSTUNRelayBinding& turnBinding =
        static_cast<const CSTUNRelayBinding&>( aBinding );
    TUint streamId( turnBinding.StreamId() );
    TUint connId( turnBinding.ConnectionId() );
    
    TInt streamInd( IndexByStreamId( streamId ) );
    TConnectionData* connection( NULL );
    
    if ( KErrNotFound != streamInd )
        {
        connection = ConnectionById( streamInd, connId );
        }
    
    if ( connection )
        {
        switch ( aEvent )
            {
            case EPublicAddressResolved:
                {
                if ( !connection->iLocalCandidate )
                    {
                    if ( iTurnRefreshTimer && !iTurnRefreshStarted )
                        {
                        StartTurnRefresh();
                        }
                    
                    TInetAddr relayAddr;
                    relayAddr.SetAddress( turnBinding.RelayAddr().Address() );
                    relayAddr.SetPort( turnBinding.RelayAddr().Port() );
                    
                    __TURNPLUGIN_ADDRLOG( "CNATFWTurnConnectionHandler::\
                    STUNBindingEventOccurredL  Adress Resolved ADDRESS: ",
                        relayAddr )
                    
                    CNATFWCandidate* newCandidate = CNATFWCandidate::NewL();
                    CleanupStack::PushL( newCandidate );
                    
                    // Set candidate parameters
                    newCandidate->SetStreamId( streamId );
                    newCandidate->SetType( CNATFWCandidate::ERelay );
                    TInetAddr baseAddr = iConnMux->LocalIPAddressL(
                        streamId, connId );
   
                    newCandidate->SetBase( relayAddr );
                    newCandidate->SetTransportAddrL( relayAddr );
                    newCandidate->SetTransportProtocol(
                        iStreamArray[streamInd].iTransportProtocol );
                    
                    connection->iLocalCandidate = newCandidate;
                    
                    iAsyncCallback->MakeCallbackL(
                        TTurnPluginCallbackInfo::ELocalCandidateFound,
                            streamId, KErrNone, newCandidate );
                    
                    iAsyncCallback->MakeCallbackL(
                        TTurnPluginCallbackInfo::EFetchingEnd, streamId,
                            KErrNone, NULL );
                    
                    CleanupStack::Pop( newCandidate );
                    }
                }
                break;
            
            case ECredentialsRejected:
                {
                __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    STUNBindingEventOccurredL  Credentials Rejected" )
                
                // TURN server rejected the credentials provided that were
                // set with CSTUNClient::SetCredentialsL. Application
                // should obtain valid credentials and then use
                // CSTUNClient::SetCredentialsL.
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EFetchingEnd, streamId, ( KClientSpecificError +
                        KErrPermissionDenied ), NULL );

                // Remove failed entry from array
                DeleteStream( streamInd, ETrue );
                }
                break;
            
            case ETCPConnectOk:
                {
                __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    STUNBindingEventOccurredL  TCP Connect Ok" )
                
                // After TCP connect is OK we can set Active Destination.
                connection->iTurnBinding->SetActiveDestinationRequestL(
                    connection->iPeerAddr, connection->iTimerValue );
                }
                break;
            
            // Success
            case EActiveDestinationSet:
                {
                __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    STUNBindingEventOccurredL  Active Destination Set" )

                if ( TConnectionData::EActDestNoneSet == connection->
                    iActDestState && !connection->iPeerAddr.IsUnspecified() )
                    {
                    if ( connection->iActDestAlreadySetOnServer )
                        {
                        // Active Destination was reset on server. Media
                        // flows in indications while trying to set the
                        // Active Destination again after 5 seconds.
                        iAsyncCallback->MakeCallbackL(
                            TTurnPluginCallbackInfo::EActiveSending,
                                iStreamArray[streamInd].iStreamId, KErrNone,
                                    NULL );

                        iActiveDestinationTimer->StartTimer(
                            KWaitTimeAfterReset, iStreamArray[streamInd].
                                iStreamId, connection->iConnectionId );
                        }
                    else
                        {
                        iAsyncCallback->MakeCallbackL(
                            TTurnPluginCallbackInfo::EActiveSending,
                                iStreamArray[streamInd].iStreamId, KErrNone,
                                    NULL );
                        
                        iActiveDestinationTimer->StartTimer(
                            KDefaultSrvTransitioningTime,
                                iStreamArray[streamInd].iStreamId,
                                    connection->iConnectionId );
                        
                        // If a successful response was received, but
                        // there was a REMOTE-ADDRESS in the request, the
                        // state machine transitions to the "Set" state
                        // after three seconds, and the client sets the
                        // active destination to the value of the REMOTE-
                        // ADDRESS attribute that was in the request.
                        }
                    }
                
                // else If, while in the "None Set" state, the client
                // sent a Set Active Destination request without a
                // REMOTE-ADDRESS, and got a successful response,
                // there is no change in state.
                
                else if ( TConnectionData::EActDestSet == connection->
                    iActDestState )
                    {
                    if ( connection->iPeerAddr.IsUnspecified() ||
                        !connection->iPeerAddr.CmpAddr( connection->
                        iCurrentActDest ) )
                        {
                        connection->iActDestState = TConnectionData::
                            EActDestTransitioning;

                        // the client enters the "Transitioning" state.
                        // While in this state, the client MUST NOT send
                        // a new Set Active Destination request. The value
                        // of the active destination remains unchanged. In
                        // addition, the client sets a timer. This timer
                        // MUST have a value equal to the value of the
                        // TIMER-VAL attribute from the Set Active
                        // Destination response.  This is necessary for
                        // coordinating the state machines between client
                        // and server.

                        iActiveDestinationTimer->StartTimer(
                            connection->iTimerValue, iStreamArray[streamInd].
                                iStreamId, connection->iConnectionId );
                        }
                    
                    // In addition, if, while in the "Set" state, the
                    // client sends a Set Active Destination request whose
                    // REMOTE-ADDRESS attribute equals the current active
                    // destination, and that request generates a success
                    // response, the client remains in the "Set" state.
                    }
                else
                    {
                    // Only Active Destination reset request can be done when
                    // in "Transitioning" state.
                    if ( connection->iPeerAddr.IsUnspecified() )
                        {
                        iActiveDestinationTimer->StartTimer( connection->
                            iTimerValue, iStreamArray[streamInd].iStreamId,
                                connection->iConnectionId );
                        }
                    }
                }
                break;
            }
        }

    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNBindingEventOccurredL end" )
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver
// CNATFWTurnConnectionHandler::STUNBindingErrorOccurred
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::STUNBindingErrorOccurred(
    const CBinding& aBinding,
    TInt aError )
    {
    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNBindingErrorOccurred start" )

    const CSTUNRelayBinding& turnBinding =
        static_cast<const CSTUNRelayBinding&>( aBinding );
    TUint streamId( turnBinding.StreamId() );
    TUint connId( turnBinding.ConnectionId() );
    
    TInt streamInd( IndexByStreamId( streamId ) );
    TConnectionData* connection( NULL );
    
    if ( KErrNotFound != streamInd )
        {
        connection = ConnectionById( streamInd, connId );
        }
    
    if ( connection )
        {
        if ( KNoBinding == aError && connection->iLocalCandidate )
            {
            // A 437 response implies that the allocation has been
            // removed, and thus the state machine destroyed. A client
            // MUST NOT send a new Set Active Destination request prior
            // to the receipt of a response to the previous. The state
            // machine will further limit the transmission of subsequent
            // Set Active Destination requests.
            TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                TTurnPluginCallbackInfo::EActiveSending, streamId,
                    KErrNotFound, NULL ) )

            // Remove entry from array as binding does not exist anymore
            DeleteStream( streamInd, ETrue );
            }
        else if ( KServerTransitioning == aError && connection->
            iLocalCandidate )
            {
            if ( TConnectionData::EActDestSet == connection->iActDestState )
                {
                // When this error is received, the client remains in the
                // "Set" state. The client SHOULD retry its Set Active
                // Destination request, but no sooner than 500ms after
                // receipt of the 439 response.
                iActiveDestinationTimer->StartTimer(
                    KRetryTimeAfter439, iStreamArray[streamInd].iStreamId,
                        connection->iConnectionId );
                }
            else if ( TConnectionData::EActDestNoneSet == connection->
                iActDestState )
                {
                // Set Active Destination request was received by the
                // server over UDP. However, the active destination is
                // already set to another value. The client should reset
                // the active destination, wait for 5 seconds and set the
                // active destination to the new value.
                connection->iActDestAlreadySetOnServer = ETrue;
                
                TRAP_IGNORE( connection->iTurnBinding->
                    SetActiveDestinationRequestL( KAFUnspec, connection->
                        iTimerValue ) )
                }
            else
                {
                // Not possible that this error occurs when in
                // "transitioning" state
                }
            }
        else
            {
            if ( !connection->iLocalCandidate )
                {
                if( KTryAlternate == aError )
                    {
                    __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    STUNBindingErrorOccurred  KTryAlternate == aError" )
                    
                    connection->iServerAddr.SetAddress( turnBinding.
                        AlternateServerAddr().Address() );
                    connection->iServerAddr.SetPort( turnBinding.
                        AlternateServerAddr().Port() );
                    
                    delete connection->iTurnBinding;
                    connection->iTurnBinding = NULL;
                    
                    TRAP_IGNORE( iConnMux->SetSendingStateL( streamId, connId,
                        connection->iServerAddr, EStreamingStateActive ) )
                    // wait MNcmConnectionObserver::ConnectionNotify
                    }
                else
                    {
                    // If error occurs when trying to fetch candidate
                    TInt errcode( KClientSpecificError + ( aError ) );
                    TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                        TTurnPluginCallbackInfo::EFetchingEnd, streamId,
                            errcode, NULL ) )
                    
                    // Remove failed entry from array
                    DeleteStream( streamInd, ETrue );
                    }
                }
            else
                {
                // If error occurs when doing binding refresh
                TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                    TTurnPluginCallbackInfo::EError, streamId,
                        aError, NULL ) )
                
                // Remove failed entry from array
                DeleteStream( streamInd, ETrue );
                }
            }
        }

    __TURNPLUGIN(
        "CNATFWTurnConnectionHandler::STUNBindingErrorOccurred end" )
    }


// ---------------------------------------------------------------------------
// from base class MNcmIncomingConnectionObserver
// CNATFWTurnConnectionHandler::IncomingMessageL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::IncomingMessageL(
    TUint aStreamId,
    const TDesC8& aMessage,
    const TInetAddr& /*aLocalAddr*/,
#ifdef _DEBUG
    const TInetAddr& aFromAddr,
    const TInetAddr& aPeerAddr,
#else
    const TInetAddr& /*aFromAddr*/,
    const TInetAddr& /*aPeerAddr*/,
#endif
    TBool& aConsumed )
    {
    __TURNPLUGIN_ADDRLOG(
        "CNATFWTurnConnectionHandler::IncomingMessageL FROMADDR", aFromAddr )
    __TURNPLUGIN_ADDRLOG(
        "CNATFWTurnConnectionHandler::IncomingMessageL PEERADDR", aPeerAddr )
    
    TInetAddr peerAddrFromIndication( KAFUnspec );
    HBufC8* indicationData( NULL );
    TInt consumingBindingInd( KErrNotFound );
    
    TInt streamInd( IndexByStreamId( aStreamId ) );
    TInt connInd( KErrNotFound );
    
    if ( KErrNotFound != streamInd )
        {
        connInd = iStreamArray[streamInd].iConnArray.Count() - 1;
        }
    
    // Offer message for every binding in the stream until consumed
    while ( KErrNotFound != streamInd && KErrNotFound == consumingBindingInd
        && ( 0 <= connInd ) )
        {
        __TURNPLUGIN( "CNATFWTurnConnectionHandler::IncomingMessageL\
        HandleDataL method to be called" )
        
        TBool consumed( EFalse );
        TConnectionData* connection = ConnectionByIndex( streamInd, connInd );
        
        if ( connection->iTurnBinding )
            {
            if ( indicationData )
                {
                connection->iTurnBinding->HandleDataL( *indicationData,
                    consumed, peerAddrFromIndication );
                }
            else
                {
                indicationData = connection->iTurnBinding->HandleDataL(
                    aMessage, consumed, peerAddrFromIndication );
                }
            }
        
        if ( consumed )
            {
            consumingBindingInd = connInd;
            aConsumed = ETrue;
            }
        
        connInd--;
        }
    
    delete indicationData;
    }


// ---------------------------------------------------------------------------
// from base class MNcmConnectionObserver
// CNATFWTurnConnectionHandler::OutgoingMessageL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::OutgoingMessageL(
    TUint aStreamId,
    TUint aConnectionId,
    const TInetAddr& aPeerAddr,
    const TDesC8& aMessage,
    TBool& aConsumed )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::OutgoingMessageL" )
    
    aConsumed = EFalse;
    
    TBool controlMsgDetected( EFalse );
    TRAPD( err, controlMsgDetected = IsRequestOrIndicationL( aMessage ) )
    
    if ( !controlMsgDetected || err ) // Prevent loopback in msg handling
        {        
        TInt streamInd( IndexByStreamId( aStreamId ) );
        TConnectionData* connection( NULL );
        
        if ( KErrNotFound != streamInd )
            {
            connection = ConnectionById( streamInd, aConnectionId );
            }
        
        if ( connection )
            {
            aConsumed = ETrue;
            
            if ( aPeerAddr.IsUnspecified() )
                {
                if ( TConnectionData::EActDestNoneSet == connection->
                    iActDestState || TConnectionData::EActDestTransitioning ==
                    connection->iActDestState )
                    {
                    __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    OutgoingMessageL  Media will be sent in indication" )
                    
                    connection->iTurnBinding->SendIndicationL( connection->
                        iPeerAddr, aMessage, EFalse );
                    }
                else
                    {
                    __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    OutgoingMessageL  Media will be sent directly" )
                    
                    if ( KProtocolInetTcp == iStreamArray[streamInd].
                        iTransportProtocol )
                        {
                        CNATFWUNSAFTcpRelayPacket* packet =
                            CNATFWUNSAFTcpRelayPacket::NewLC( aMessage,
                                CNATFWUNSAFTcpRelayPacket::EFrameTypeData);
                        
                        CBufBase* message = packet->EncodeL();
                        CleanupStack::PopAndDestroy( packet );
                        
                        CleanupStack::PushL( message );
                        iConnMux->SendL( aStreamId, connection->iConnectionId,
                            message->Ptr(0), ETrue );
                        CleanupStack::PopAndDestroy( message );
                        }
                    else
                        {
                        iConnMux->SendL( aStreamId, connection->iConnectionId,
                            aMessage, ETrue );
                        }
                    }
                }
            else
                {
                // Must be protocol message, send in indication(TURNbis-03, 4)
                __TURNPLUGIN( "CNATFWTurnConnectionHandler::\
                    OutgoingMessageL  PROTOCOL MSG will be sent" )
                __TURNPLUGIN_ADDRLOG( 
                    "CNATFWTurnConnectionHandler::OutgoingMessageL PEERADDR",
                    aPeerAddr )
                connection->iTurnBinding->SendIndicationL( aPeerAddr,
                    aMessage, ETrue );
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// from base class MNcmConnectionObserver
// CNATFWTurnConnectionHandler::IncomingMessageNotify
// ---------------------------------------------------------------------------
//
HBufC8* CNATFWTurnConnectionHandler::IncomingMessageNotify(
    TUint aStreamId,
    const TDesC8& aMessage,
    const TInetAddr& aLocalAddr,
    const TInetAddr& aFromAddr,
    TInetAddr& aPeerAddr )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::IncomingMessageNotify" )
    
    HBufC8* modifiedMessage( NULL );
    TRAPD( error, modifiedMessage = HandleIncomingMessageL( 
        aStreamId, aMessage, aLocalAddr, aFromAddr, aPeerAddr ) )
    if ( KErrNone == error )
        {
        return modifiedMessage;
        }
    else
        {
        __TURNPLUGIN_INT1(
            "CNATFWTurnConnectionHandler::IncomingMessageNotify ERR", error )
        
        return NULL;
        }
    }


// ---------------------------------------------------------------------------
// from base class MNcmConnectionObserver
// CNATFWTurnConnectionHandler::OutgoingMessageNotify
// ---------------------------------------------------------------------------
//
HBufC8* CNATFWTurnConnectionHandler::OutgoingMessageNotify(
    TUint /*aStreamId*/,
    TUint /*aConnectionId*/,
    const TInetAddr& /*aDestinationAddress*/,
    const TDesC8& /*aMessage*/ )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::OutgoingMessageNotify" )
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// from base class MNcmConnectionObserver
// CNATFWTurnConnectionHandler::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::ConnectionNotify(
    TUint aStreamId,
    TUint aConnectionId,
    TConnectionNotifyType aType,
    TInt aError )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectionNotify start" )
    
    TInt streamInd( IndexByStreamId( aStreamId ) );
    TConnectionData* connection( NULL );
    
    if ( KErrNotFound != streamInd )
        {
        connection = ConnectionById( streamInd, aConnectionId );
        }
    
    if ( connection )
        {
        if ( aError || EConnectionError == aType )
            {
            if ( !connection->iLocalCandidate )
                {
                // If error occurs when trying to fetch candidate
                TInt errcode( KClientSpecificError + ( aError ) );
                TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                    TTurnPluginCallbackInfo::EFetchingEnd, aStreamId,
                        errcode, NULL ) )
                }
            else
                {
                TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                    TTurnPluginCallbackInfo::EError, aStreamId,
                        aError, NULL ) )
                }
            
            // Remove failed entry from array
            DeleteStream( streamInd, ETrue );
            }
        else
            {
            switch ( aType )
                {
                case ESendingActivated:
                    {
                    connection->iSendingActivated = ETrue;
                    
                    if ( !connection->iTurnBinding )
                        {
                        TRAP_IGNORE( CreateTURNBindingL( aStreamId,
                            aConnectionId ) )
                        }
                    }
                    break;
                
                case EReceivingActivated:
                    {
                    connection->iReceivingActivated = ETrue;
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TTurnPluginCallbackInfo::EActiveReceiving,
                                aStreamId, KErrNone, NULL ) )
                        }
                    }
                    break;
                
                case ESendingDeactivated:
                    {
                    connection->iSendingActivated = EFalse;
                    }
                    break;
                
                case EReceivingDeactivated:
                    {
                    connection->iReceivingActivated = EFalse;
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TTurnPluginCallbackInfo::EDeactiveReceiving,
                                aStreamId, KErrNone, NULL ) )
                        }
                    }
                    break;
                
                case EConnectionRemoved:
                    {
                    DeleteStream( streamInd, EFalse );
                    }
                    break;
                }
            }
        }
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectionNotify end" )
    }


// ---------------------------------------------------------------------------
// from base class MNATFWRefreshObserver
// CNATFWTurnConnectionHandler::BindingRefreshL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::BindingRefreshL()
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::BindingRefreshL start" )
    
    TInt streamCount( iStreamArray.Count() );
    
    for ( TInt streamInd = 0; streamInd < streamCount; streamInd++ )
        {
        TInt connCount( iStreamArray[streamInd].iConnArray.Count() );
        
        for ( TInt connInd = 0; connInd < connCount; connInd++ )
            {
            TConnectionData* connection = ConnectionByIndex( streamInd,
                connInd );
            connection->iTurnBinding->AllocateRequestL(
                iStreamArray[streamInd].iRtoValue );
            }
        }
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::BindingRefreshL end" )
    }


// ---------------------------------------------------------------------------
// from base class MNATFWTimerObserver
// CNATFWTurnConnectionHandler::TimerTriggeredL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::TimerTriggeredL( TUint aStreamId,
                                                   TUint aConnectionId )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::TimerTriggeredL start" )
    
    TInt streamInd( IndexByStreamId( aStreamId ) );
    TConnectionData* connection( NULL );
    
    if ( KErrNotFound != streamInd )
        {
        connection = ConnectionById( streamInd, aConnectionId );
        }
    
    if ( connection )
        {
        if ( TConnectionData::EActDestTransitioning == connection->
            iActDestState )
            {
            // Once the timer fires, if the REMOTE-ADDRESS was not absent
            // from the Set Active Destination request which caused the
            // client to start the timer, the client moves back to the
            // "Set" state, and then updates the value of the active
            // destination to the value of REMOTE-ADDRESS.
            
            if ( !connection->iPeerAddr.IsUnspecified() )
                {
                // Plug-in's client changed Active Destination on
                // server successfully.
                
                connection->iActDestState = TConnectionData::EActDestSet;
                connection->iCurrentActDest.SetAddress( connection->iPeerAddr.
                    Address() );
                connection->iCurrentActDest.SetPort( connection->iPeerAddr.
                    Port() );
                
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EActiveSending, iStreamArray[streamInd].iStreamId,
                        KErrNone, NULL );
                }
            else
            // If REMOTE-ADDRESS was absent, the client sets the Active
            // Destination to null and enders the "None Set" state.
                {
                // Plug-in's client resetted Active Destination on
                // server successfully.
                
                connection->iActDestState = TConnectionData::EActDestNoneSet;
                connection->iCurrentActDest = KAFUnspec;
                
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EDeactiveSending, iStreamArray[streamInd].iStreamId,
                        KErrNone, NULL );
                }
            }
        else if ( TConnectionData::EActDestNoneSet == connection->
            iActDestState )
            {
            if ( !connection->iActDestAlreadySetOnServer )
                {
                connection->iActDestState = TConnectionData::EActDestSet;
                connection->iCurrentActDest.SetAddress( connection->iPeerAddr.
                    Address() );
                connection->iCurrentActDest.SetPort( connection->iPeerAddr.
                    Port() );
                }
            else
                {
                connection->iActDestAlreadySetOnServer = EFalse;
                
                // Active Destination was reset on server so now
                // try to set Active Destination to remote address.
                connection->iTurnBinding->SetActiveDestinationRequestL(
                    connection->iPeerAddr, connection->iTimerValue );
                }
            }
        else
            {
            if ( !connection->iActDestReqResent )
                {
                // As server was on "transitioning" state for the first
                // Set Active Destination Request, a new request will
                // be sent only once.
                connection->iActDestReqResent = ETrue;
                
                connection->iTurnBinding->SetActiveDestinationRequestL(
                    connection->iPeerAddr, connection->iTimerValue );
                }
            else
                {
                iAsyncCallback->MakeCallbackL( TTurnPluginCallbackInfo::
                    EActiveSending, aStreamId, KErrNotReady, NULL );
                }
            }
        }
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::TimerTriggeredL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::DeleteStream
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::DeleteStream( TUint aStreamInd, 
    TBool aRemoveMuxConn )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::DeleteStream, start" )
    ASSERT( aStreamInd < TUint( iStreamArray.Count() ) );
    
    /*TRAP_IGNORE(
        iConnMux->UnregisterConnectionObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )
    
    TRAP_IGNORE(
        iConnMux->UnregisterIncomingConnectionObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )
    
    TRAP_IGNORE(
        iConnMux->UnregisterOutgoingConnectionObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )
    
    TRAP_IGNORE(
        iConnMux->UnregisterMessageObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )*/
    
    // Remove the stream's connections
    TInt connIndex( iStreamArray[aStreamInd].iConnArray.Count() - 1 );
    
    for ( TInt i( connIndex ); i >= 0; i-- )
        {
        TConnectionData* connection = ConnectionByIndex( aStreamInd, i );
        
        if( connection->iTurnBinding )
            {
            connection->iTurnBinding->CancelRequest();
            
            delete connection->iTurnBinding;
            connection->iTurnBinding = NULL;
            connection->iLocalCandidate = NULL;
            }
        
        if ( aRemoveMuxConn )
            {
            /*TRAP_IGNORE( iConnMux->RemoveConnectionL(
                iStreamArray[aStreamInd].iStreamId, connection->
                    iConnectionId ) )*/
            }
        
        iStreamArray[aStreamInd].iConnArray.Remove( i );
        }
    
    iStreamArray[aStreamInd].iConnArray.Close();
    iStreamArray.Remove( aStreamInd );
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::DeleteStream, end" )
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::FindConnection
// Function expects that system does not have multiple TURN-connections bound
// to the same local address and stream.
// ---------------------------------------------------------------------------
//
TConnectionData* CNATFWTurnConnectionHandler::FindConnection(
    TUint aStreamId,
    const TInetAddr& aLocalAddr )
    {
    // find stream index
    TInt foundStreamInd( KErrNotFound );
    TInt streamInd( iStreamArray.Count() - 1 );
    while ( KErrNotFound == foundStreamInd && 0 <= streamInd )
        {
        if ( iStreamArray[streamInd].iStreamId == aStreamId )
            {
            foundStreamInd = streamInd;
            }
        
        streamInd--;
        }
    
    TInt foundConnectionInd( KErrNotFound );
    if ( KErrNotFound != foundStreamInd )
        {
        // find connection inside stream
        const RArray<TConnectionData>& connArray( 
            iStreamArray[foundStreamInd].iConnArray );
        TInt connectionInd( connArray.Count() - 1 );
        
        while ( KErrNotFound == foundConnectionInd && 0 <= connectionInd )
            {
            const TInetAddr& localAddr( connArray[connectionInd].iLocalAddr );
            if ( localAddr.CmpAddr( aLocalAddr ) )
                {
                foundConnectionInd = connectionInd;
                }
            
            connectionInd--;
            }
        }
    
    if ( ( KErrNotFound != foundStreamInd ) && 
         ( KErrNotFound != foundConnectionInd ) )
        {
        return &(iStreamArray[foundStreamInd].iConnArray[foundConnectionInd]);
        }
    else
        {
        return NULL;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::HandleIncomingMessageL
// ---------------------------------------------------------------------------
//
HBufC8* CNATFWTurnConnectionHandler::HandleIncomingMessageL(
    TUint aStreamId,
    const TDesC8& aMessage,
    const TInetAddr& aLocalAddr,
    const TInetAddr& aFromAddr,
    TInetAddr& aPeerAddr )
    {
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg( NULL );
    TRAPD( error, msg = decoder->DecodeL( aMessage ) )
    CleanupStack::PopAndDestroy( decoder );
    CleanupStack::PushL( msg );
    
    if ( error || NULL == msg )
        {
        // Not STUN message, must be media
        TConnectionData* activeConnection( NULL );
        TInt streamInd( iStreamArray.Count() - 1 );
        
        while ( !activeConnection && 0 <= streamInd )
            {
            TInt connInd = iStreamArray[streamInd].iConnArray.Count() - 1;
            
            while ( !activeConnection && 0 <= connInd )
                {
                activeConnection = ConnectionByIndex( streamInd, connInd );
                
                if ( TConnectionData::EActDestSet != activeConnection->
                    iActDestState )
                    {
                    activeConnection = NULL;
                    }
                
                connInd--;
                }
            
            streamInd--;
            }
        
        if ( activeConnection )
            {
            aPeerAddr = activeConnection->iCurrentActDest;
            }
        else
            {
            aPeerAddr = aFromAddr;
            }
        
        CleanupStack::PopAndDestroy( msg );
        return NULL;
        }
    
    if ( msg->Type() == CNATFWUNSAFMessage::EDataIndication )
        {
        // Must be data from peer, either protocol message or media
        // from nonactivated peer address
        CNATFWUNSAFRemoteAddressAttribute* remoteAddress =
            static_cast<CNATFWUNSAFRemoteAddressAttribute*>
            ( msg->Attribute( CNATFWUNSAFAttribute::ERemoteAddress ) );
        
        if ( remoteAddress )
            {
            aPeerAddr = remoteAddress->Address();
            __TURNPLUGIN_ADDRLOG( 
               "CNATFWTurnConnectionHandler::HandleIncomingMessageL DATAIND",
               aPeerAddr )
            }
        else
            {
            __TURNPLUGIN( 
                "CNATFWTurnConnectionHandler::HandleIncomingMessageL, \
                NO REMOTE ADDR FIELD" )
            }
        
        const TDesC8& data = static_cast<CNATFWUNSAFDataAttribute*>(
            msg->Attribute( CNATFWUNSAFAttribute::EData ) )->Value();
        
        HBufC8* modifiedMsg = data.AllocL();
        CleanupStack::PopAndDestroy( msg );
        return modifiedMsg;
        }
    else
        {
        // This is either response from physical TURN-server or STUN-data from
        // peer address which is set as active destination.
        TConnectionData* activeConnection 
            = FindConnection( aStreamId, aLocalAddr );
        
        if ( activeConnection && 
                !activeConnection->iCurrentActDest.IsUnspecified() )
            {
            aPeerAddr = activeConnection->iCurrentActDest;
            }
        else
            {
            aPeerAddr = aFromAddr;
            }
        
        __TURNPLUGIN_ADDRLOG( 
            "CNATFWTurnConnectionHandler::HandleIncomingMessageL NOT DATAIND",
            aPeerAddr )
        
        CleanupStack::PopAndDestroy( msg );
        return NULL;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::IndexByStreamId
// ---------------------------------------------------------------------------
//
TInt CNATFWTurnConnectionHandler::IndexByStreamId( TUint aStreamId )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::IndexByStreamId" )
    
    TInt index( iStreamArray.Count() - 1 );
    
    while ( 0 <= index )
        {
        if ( aStreamId == iStreamArray[index].iStreamId )
            {
            return index;
            }
        
        index--;
        }

    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::ConnectionById
// ---------------------------------------------------------------------------
//
TConnectionData* CNATFWTurnConnectionHandler::ConnectionById(
    TUint aStreamInd,
    TUint aConnectionId )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectionById" )
    
    TInt index( iStreamArray[aStreamInd].iConnArray.Count() - 1 );
    
    while ( 0 <= index )
        {
        if ( aConnectionId == iStreamArray[aStreamInd].iConnArray[index].
            iConnectionId )
            {
            return &iStreamArray[aStreamInd].iConnArray[index];
            }
        
        index--;
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::ConnectionByIndex
// ---------------------------------------------------------------------------
//
TConnectionData* CNATFWTurnConnectionHandler::ConnectionByIndex(
    TUint aStreamInd,
    TUint aConnectionInd )
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::ConnectionByIndex" )
    
    return &iStreamArray[aStreamInd].iConnArray[aConnectionInd];
    }


// ---------------------------------------------------------------------------
// CNATFWTurnConnectionHandler::GenerateServerListL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::GenerateServerListL()
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::GenerateServerListL start" )
    
    RPointerArray<MNATFWServerSettings> serverArray;
    iTurnSettings->GetTurnServerArrayL( serverArray );
    CleanupClosePushL( serverArray );
    TInt count( serverArray.Count() );
    
    HBufC8* serverAddress =
        iTurnSettings->LatestConnectedServerAddr().AllocLC();
    TUint latestPort = iTurnSettings->LatestConnectedServerPort();
    
    // Add Latest connected server
    if ( serverAddress->Compare( KNullDesC8 ) )
        {
        CTurnServerSettings* settings = CTurnServerSettings::NewLC();
        settings->SetAddressL( *serverAddress );
        settings->SetPort( latestPort );
        
        for ( TInt index( 0 ); index < count; index++ )
            {
            if ( *serverAddress == serverArray[index]->Address() &&
                serverArray[index]->Username().Length() &&
                serverArray[index]->Password().Length() )
                {
                settings->SetUsernameL( serverArray[index]->Username() );
                settings->SetPasswordL( serverArray[index]->Password() );
                break;
                }
            }
            
        if ( settings->Address().Compare( KNullDesC8 ) )
            {
            __TURNPLUGIN( "CNATFWStunConnectionHandler::GenerateServerListL\
            - provisioned server added" )
            iServerList.AppendL( settings );
            }
            
        iServerList.AppendL( settings );
        CleanupStack::Pop( settings );
        }
    
    CleanupStack::PopAndDestroy( serverAddress );
    
    // Add provisioned servers
    for ( TInt index( 0 ); index < count; index++ )
        {
        CTurnServerSettings* settings = CTurnServerSettings::NewLC();
        
        settings->SetAddressL( serverArray[index]->Address() );
        settings->SetPort( serverArray[index]->Port() );
        
        if ( serverArray[index]->Username().Length() &&
            serverArray[index]->Password().Length() )
            {
            settings->SetUsernameL( serverArray[index]->Username() );
            settings->SetPasswordL( serverArray[index]->Password() );
            }
        
        iServerList.AppendL( settings );
        CleanupStack::Pop( settings );
        }
    
    // Domain
    CTurnServerSettings* settings = CTurnServerSettings::NewLC();
    settings->SetAddressL( *iDomain );
    settings->SetPort( ( latestPort ) ? latestPort : KDefaultStunSrvPort );
    
    for ( TInt index( 0 ); index < count; index++ )
        {
        if ( *iDomain == serverArray[index]->Address() &&
            serverArray[index]->Username().Length() &&
            serverArray[index]->Password().Length() )
            {
            settings->SetUsernameL( serverArray[index]->Username() );
            settings->SetPasswordL( serverArray[index]->Password() );
            break;
            }
        }
    
    iServerList.AppendL( settings );
    CleanupStack::Pop( settings );
    CleanupStack::PopAndDestroy( &serverArray );
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::GenerateServerListL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::TryNextServerL
// ---------------------------------------------------------------------------
//
void CNATFWTurnConnectionHandler::TryNextServerL()
    {
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::TryNextServerL start" )
    
    delete iStunClient;
    iStunClient = NULL;
    
    if ( iServerIndex < iServerList.Count() )
        {
        TInt index( iServerIndex );
        iServerIndex++;
        
        TInt refresh( iNatSettings->RefreshIntervalUdp() );
        iTurnRefreshInterval = ( refresh ) ? ( refresh * KMicrosecFactor ) :
            KDefaultRefreshInterval;
        
        TInt retransmitInterval( iTurnSettings->RetransmissionTimeout() );
        
        TUint port = iServerList[index]->Port() ? iServerList[index]->Port() :
            KDefaultStunSrvPort;
        
        iStunClient = CSTUNClient::NewL( retransmitInterval,
                                         iServerList[index]->Address(),
                                         port,
                                         KStunRelay,
                                         iSocketServ,
                                         iConnection,
                                         *iTimerServ,
                                         *this,
                                         ETrue,
                                         EFalse,
                                         EFalse,
                                         iConnMux );
        
        if ( iServerList[index]->Username() && iServerList[index]->
            Password() )
            {
            TRAP_IGNORE( iStunClient->SetCredentialsL( *iServerList[index]->
                Username(), *iServerList[index]->Password() ) )
            }
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    __TURNPLUGIN( "CNATFWTurnConnectionHandler::TryNextServerL end" )
    }


void CNATFWTurnConnectionHandler::ExecuteCallBack()
    {
    if ( iCallBackCmds.Count() )
        {
        if ( EConnectServer == iCallBackCmds[0].iType )
            {
            iPluginObserver.Notify( 
                iTurnPlugin, iCallBackCmds[0].iStreamId, 
                MNATFWPluginObserver::EServerConnected, KErrNone );
            }
        else if ( EFetchCandidate == iCallBackCmds[0].iType )
            {
            iPluginObserver.NewLocalCandidateFound( iTurnPlugin, NULL );
            iPluginObserver.NewLocalCandidateFound( 
                iTurnPlugin, 
                reinterpret_cast<CNATFWCandidate*>( iCallBackCmds[0].iDataPtr ) );
            iPluginObserver.Notify( 
                iTurnPlugin, iCallBackCmds[0].iStreamId, 
                MNATFWPluginObserver::EFetchingCompleted, KErrNone );
           }
        else if ( ESetSendingStatus == iCallBackCmds[0].iType )
            {
            iPluginObserver.Notify( iTurnPlugin, iCallBackCmds[0].iStreamId, 
                static_cast<MNATFWPluginObserver::TNATFWPluginEvent>(
                    iCallBackCmds[0].iData ), KErrNone );
            }
        else
            {
            // NOP
            }
        
        iCallBackCmds.Remove( 0 );
        }
    }

// End of file
