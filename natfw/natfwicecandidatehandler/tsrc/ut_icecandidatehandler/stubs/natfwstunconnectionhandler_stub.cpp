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
* Description:    Creates connection to Connection Multiplexer and STUN client.
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
#include "natfwstunbinding.h"
#include "natfwstunclient.h"
#include <mnatfwstunsettings.h>

#include "natfwstunconnectionhandler_stub.h"
#include "natfwstunpluginlogs.h"
#include "natfwstunrefreshtimer.h"
#include "natfwstunstreamdata.h"
#include "cstunasynccallback.h"
#include "cnatfwstunserversettings.h"

const TUint KDefaultRefreshInterval = 28000000;
const TUint KDefaultStunSrvPort = 3478;
const TUint KMicrosecFactor = 1000000;
const TUint KTryAlternate = 300;
const TInt KClientSpecificError = -11000;

const TUint32 KWaitTime = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWStunConnectionHandler::CNATFWStunConnectionHandler(
    const CNATFWPluginApi& aStunPlugin,
    MNATFWPluginObserver& aPluginObserver ) 
    : 
    iStunRefreshInterval( KDefaultRefreshInterval ),
    CTimer( EPriorityStandard ),
    iStunPlugin( aStunPlugin ),
    iPluginObserver( aPluginObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::ConstructL(
    MNATFWPluginObserver& aPluginObserver )
    {
    CTimer::ConstructL();
    iAsyncCallback = CStunAsyncCallback::NewL( iStunPlugin, aPluginObserver );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWStunConnectionHandler* CNATFWStunConnectionHandler::NewL( 
    const CNATFWPluginApi& aStunPlugin,
    MNATFWPluginObserver& aPluginObserver )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::NewL" )
    CNATFWStunConnectionHandler* self = CNATFWStunConnectionHandler::NewLC(
        aStunPlugin, aPluginObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWStunConnectionHandler* CNATFWStunConnectionHandler::NewLC( 
    const CNATFWPluginApi& aStunPlugin,
    MNATFWPluginObserver& aPluginObserver )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::NewLC" )
    CNATFWStunConnectionHandler* self = new( ELeave ) 
        CNATFWStunConnectionHandler( aStunPlugin, aPluginObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aPluginObserver );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWStunConnectionHandler::~CNATFWStunConnectionHandler()
    {
    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::~CNATFWStunConnectionHandler start" )

    iServerList.ResetAndDestroy();
    CTimer::DoCancel();

    delete iDomain;
    delete iNatSettings;
    iStunSettings = NULL;
    delete iStunRefreshTimer;

    TInt cmdCount( iCallBackCmds.Count() );
    for ( TInt i = 0; i < cmdCount; ++i )
        {
        delete reinterpret_cast<CBase*>( iCallBackCmds[i].iDataPtr );
        }

    TInt count( iStreamArray.Count() );
    for ( TInt index = count - 1; index >= 0; index-- )
        {
        DeleteStream( index, ETrue );
        }

    iCallBackCmds.Close();
    iStreamArray.Close();
    delete iStunClient;
    delete iTimerServ;
    iConnection.Close();
    iConnMux = NULL;
    delete iAsyncCallback;

    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::~CNATFWStunConnectionHandler end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::PluginInitializeL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::PluginInitializeL(
    TUint32 aIapId,
    const TDesC8& /*aDomain*/,
    MNcmConnectionMultiplexer& aMultiplexer )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::PluginInitializeL start" )

    iConnMux = &aMultiplexer;
    iIapId = aIapId;
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::DoCancel.
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::DoCancel()
    {
    CTimer::DoCancel();
    }


// ---------------------------------------------------------------------------
// Notifies the Plug-in's client of stream's current sending status.
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::RunL()
    {
    ExecuteCallBack();
    if ( iCallBackCmds.Count() )
        {
        After( KWaitTime );
        }
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::ConnectServerL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::ConnectServerL(
    const RSocketServ& /*aSocketServ*/,
    const TName& /*aConnectionName*/ )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectServerL start" )
    
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
    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectServerL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::FetchCandidateL( TUint aStreamId,
    TUint /*aRtoValue*/, TUint /*aAddrFamily*/, 
    const TInetAddr& aBaseAddr )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::FetchCandidateL start" )
      
    // Store data in array
    TStreamData streamData;
    streamData.iStreamId = aStreamId;

    CNATFWCandidate* newCandidate = CNATFWCandidate::NewLC();
    TInetAddr publicAddr;
    if ( KAfInet6 == aBaseAddr.Family() )
        {
        const TIp6Addr KIp6Addr = {{{1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,1}}};
        publicAddr.SetAddress( KIp6Addr );
        }
    else
        {
        publicAddr.SetAddress( INET_ADDR( 1,2,3,4 ) );
        }

    // Set candidate parameters
    newCandidate->SetStreamId( aStreamId );
    newCandidate->SetType( CNATFWCandidate::EServerReflexive );
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
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::FetchCandidateL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::GetConnectionIdL(
    const CNATFWCandidate& /*aLocalCandidate*/,
    TUint aStreamId,
    TUint& aConnectionId )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::GetConnectionIdL start" )
    
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
        
        // TODO: Implementation needs to be changed, too strict comparison. 
        // ICECH sets some values to candidate so it does not match anymore if comparing this way
        // Maybe should use address to find correct candidate??
        //if ( aLocalCandidate == *connection->iLocalCandidate )
          //  {
            connFound = ETrue;
            aConnectionId = connection->iConnectionId;
            //}
        
        connInd--;
        }
    
    __ASSERT_ALWAYS( connFound, User::Leave( KErrNotFound ) );
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::GetConnectionIdL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::StartStunRefresh
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::StartStunRefresh()
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::StartStunRefresh start" )

    }


// ---------------------------------------------------------------------------
// Creates new STUN Binding and stores it to the stream array.
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::CreateSTUNBindingL( TUint /*aStreamId*/, 
                                                      TUint /*aConnectionId*/ )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::CreateSTUNBindingL" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::SetReceivingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::SetReceivingStateL start" )
    
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
                iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
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
                iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
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
            iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                EActiveReceiving, streamId, KErrNone, NULL );
            }
        else
            {
            iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                EDeactiveReceiving, streamId, KErrNone, NULL );
            }
        //}
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::SetReceivingStateL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::SetSendingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState,
    const TInetAddr& /*aDestAddr*/ )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::SetSendingStateL start" )
    
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
        // Client controls media connection, set address filtering
        iConnMux->SetAcceptedFromAddressL( streamId, connId, aDestAddr );
        
        if ( EStreamingStateActive == aState )
            {
            if ( connection->iSendingActivated && connection->iDestAddr.
                CmpAddr( aDestAddr ) )
                {
                iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                    EActiveSending, streamId, KErrNone, NULL );
                }
            else
                {
                connection->iDestAddr.SetAddress( aDestAddr.Address() );
                connection->iDestAddr.SetPort( aDestAddr.Port() );
                
                iConnMux->SetSendingStateL( streamId, connId, aDestAddr,
                    EStreamingStateActive );
                }
            }
        else
            {
            if ( !connection->iSendingActivated )
                {
                iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                    EDeactiveSending, streamId, KErrNone, NULL );
                }
            else
                {
                iConnMux->SetSendingStateL( streamId, connId, aDestAddr,
                    EStreamingStatePassive );
                }
            }
        }
    else
        {*/
        if ( EStreamingStateActive == aState )
            {
            iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                EActiveSending, streamId, KErrNone, NULL );
            }
        else
            {
            iAsyncCallback->MakeCallbackL( TStunPluginCallbackInfo::
                EDeactiveSending, streamId, KErrNone, NULL );
            }
        //}
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::SetSendingStateL end" )
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver.
// CNATFWStunConnectionHandler::STUNClientInitCompleted
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/, TInt aCompletionCode )
    {
    __STUNPLUGIN_INT1( "CNATFWStunConnectionHandler::STUNClientInitCompleted \
         start - Completion code:", aCompletionCode )

    TUint streamId( 0 );
   
    if ( KErrNone == aCompletionCode )
        {
        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
            TStunPluginCallbackInfo::EConnectServer, streamId,
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
                TStunPluginCallbackInfo::EConnectServer, streamId,
                ( KClientSpecificError + aCompletionCode ), NULL ) )
            }
        }
    
    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::STUNClientInitCompleted end" )
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver
// CNATFWStunConnectionHandler::STUNBindingEventOccurredL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::STUNBindingEventOccurredL(
    TSTUNBindingEvent aEvent, 
    const CBinding& aBinding )
    {
    __STUNPLUGIN( 
        "CNATFWStunConnectionHandler::STUNBindingEventOccurredL start" )
        
    const CSTUNBinding& stunBinding =
        static_cast<const CSTUNBinding&>( aBinding );
    TUint streamId( stunBinding.StreamId() );
    TUint connId( stunBinding.ConnectionId() );
    
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
                    if ( iStunRefreshTimer && !iStunRefreshStarted )
                        {
                        StartStunRefresh();
                        }
                    
                    TInetAddr publicAddr;
                    publicAddr.SetAddress( stunBinding.PublicAddr().
                        Address() );
                    publicAddr.SetPort( stunBinding.PublicAddr().Port() );
                    
                    __STUNPLUGIN_ADDRLOG( "CNATFWStunConnectionHandler::\
                    STUNBindingEventOccurredL  Adress Resolved ADDRESS: ",
                        publicAddr )
                    
                    CNATFWCandidate* newCandidate = CNATFWCandidate::NewL();
                    CleanupStack::PushL( newCandidate );
                    
                    // Set candidate parameters
                    newCandidate->SetStreamId( streamId );
                    newCandidate->SetType(
                        CNATFWCandidate::EServerReflexive );
                    TInetAddr baseAddr = iConnMux->LocalIPAddressL(
                        streamId, connId );
   
                    newCandidate->SetBase( baseAddr );
                    newCandidate->SetTransportAddrL( publicAddr );
                    newCandidate->SetTransportProtocol(
                        iStreamArray[streamInd].iTransportProtocol );
                    
                    CNATFWCandidate* copyCandidate = CNATFWCandidate::NewL(
                        *newCandidate );
                    connection->iLocalCandidate = copyCandidate;
                    
                    iAsyncCallback->MakeCallbackL(
                        TStunPluginCallbackInfo::ELocalCandidateFound,
                            streamId, KErrNone, newCandidate );
                    
                    iAsyncCallback->MakeCallbackL(
                        TStunPluginCallbackInfo::EFetchingEnd, streamId,
                        KErrNone, NULL );
                    
                    CleanupStack::Pop( newCandidate );
                    }
                }
                break;

            case ECredentialsRejected:
                {
                // STUN server rejected the credentials provided that were
                // set with CSTUNClient::SetCredentialsL. Application
                // should obtain valid credentials and then use
                // CSTUNClient::SetCredentialsL.
                iAsyncCallback->MakeCallbackL(
                    TStunPluginCallbackInfo::EFetchingEnd, streamId,
                    ( KClientSpecificError + KErrPermissionDenied ),
                    NULL );

                // Remove failed entry from array
                DeleteStream( streamInd, ETrue );
                }
                break;
            }
        }

    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::STUNBindingEventOccurredL end" )
    }


// ---------------------------------------------------------------------------
// From base class MStunClientObserver
// CNATFWStunConnectionHandler::STUNBindingErrorOccurred
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::STUNBindingErrorOccurred(
    const CBinding& aBinding,
    TInt aError )
    {
    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::STUNBindingErrorOccurred start" )

    const CSTUNBinding& stunBinding =
        static_cast<const CSTUNBinding&>( aBinding );
    TUint streamId( stunBinding.StreamId() );
    TUint connId( stunBinding.ConnectionId() );
    
    TInt streamInd( IndexByStreamId( streamId ) );
    TConnectionData* connection( NULL );
    
    if ( KErrNotFound != streamInd )
        {
        connection =  ConnectionById( streamInd, connId );
        }
    
    if ( connection )
        {
        if ( !connection->iLocalCandidate )
            {
            if ( KTryAlternate == aError )
                {
                __STUNPLUGIN( "CNATFWStunConnectionHandler::\
                STUNBindingErrorOccurred  KTryAlternate == aError" )
                
                connection->iStunSrvAddr.SetAddress( stunBinding.
                    AlternateServerAddr().Address() );
                connection->iStunSrvAddr.SetPort( stunBinding.
                    AlternateServerAddr().Port() );
                
                delete connection->iStunBinding;
                connection->iStunBinding = NULL;
                
                TRAP_IGNORE( iConnMux->SetSendingStateL( streamId, connId,
                    connection->iStunSrvAddr, EStreamingStateActive ) )
                // wait MNcmConnectionObserver::ConnectionNotify
                }
            else
                {
                // If error occurs when trying to fetch candidate
                TInt errcode( KClientSpecificError + ( aError ) );
                TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                    TStunPluginCallbackInfo::EFetchingEnd, streamId,
                        errcode, NULL ) )

                // Remove failed entry from array
                DeleteStream( streamInd, ETrue );
                }
            }
        else
            {
            // If error occurs when doing binding refresh
            TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                TStunPluginCallbackInfo::EError, streamId,
                    aError, NULL ) )

            // Remove failed entry from array
            DeleteStream( streamInd, ETrue );
            }
        }

    __STUNPLUGIN(
        "CNATFWStunConnectionHandler::STUNBindingErrorOccurred end" )
    }


// ---------------------------------------------------------------------------
// from base class MNcmIncomingConnectionObserver
// CNATFWStunConnectionHandler::IncomingMessageL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::IncomingMessageL(
    TUint aStreamId,
    const TDesC8& aMessage,
    const TInetAddr& /*aLocalAddr*/,
#ifdef _DEBUG
    const TInetAddr& aFromAddr,
    const TInetAddr& aPeerRemoteAddr,
#else
    const TInetAddr& /*aFromAddr*/,
    const TInetAddr& /*aPeerRemoteAddr*/,
#endif
    TBool& aConsumed )
    {
    __STUNPLUGIN_ADDRLOG(
        "CNATFWStunConnectionHandler::IncomingMessageL FROMADDR", aFromAddr )
    __STUNPLUGIN_ADDRLOG(
        "CNATFWStunConnectionHandler::IncomingMessageL PEERADDR",
            aPeerRemoteAddr )
    
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
        __STUNPLUGIN( "CNATFWStunConnectionHandler::IncomingMessageL\
        HandleDataL method to be called" )
        
        TBool consumed( EFalse );
        TConnectionData* connection = ConnectionByIndex( streamInd, connInd );
        
        if ( connection->iStunBinding )
            {
            if ( indicationData )
                {
                connection->iStunBinding->HandleDataL( *indicationData,
                    consumed, peerAddrFromIndication );
                }
            else
                {
                indicationData = connection->iStunBinding->HandleDataL(
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
// CNATFWStunConnectionHandler::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::ConnectionNotify(
    TUint aStreamId,
    TUint aConnectionId,
    TConnectionNotifyType aType,
    TInt aError )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectionNotify start" )
    
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
                    TStunPluginCallbackInfo::EFetchingEnd, aStreamId, errcode,
                        NULL ) )
                }
            else
                {
                TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                    TStunPluginCallbackInfo::EError, aStreamId, aError,
                        NULL ) )
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
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TStunPluginCallbackInfo::EActiveSending,
                                aStreamId, KErrNone, NULL ) )
                        }
                    else
                        {
                        // Candidate fetching ongoing
                        if ( !connection->iStunBinding )
                            {
                            TRAP_IGNORE( CreateSTUNBindingL( aStreamId,
                                aConnectionId ) )
                            }
                        }
                    }
                    break;
                
                case EReceivingActivated:
                    {
                    connection->iReceivingActivated = ETrue;
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TStunPluginCallbackInfo::EActiveReceiving,
                                aStreamId, KErrNone, NULL ) )
                        }
                    }
                    break;
                
                case ESendingDeactivated:
                    {
                    connection->iSendingActivated = EFalse;
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TStunPluginCallbackInfo::EDeactiveSending,
                                aStreamId, KErrNone, NULL ) )
                        }
                    }
                    break;
                
                case EReceivingDeactivated:
                    {
                    connection->iReceivingActivated = EFalse;
                    
                    if ( connection->iLocalCandidate )
                        {
                        TRAP_IGNORE( iAsyncCallback->MakeCallbackL(
                            TStunPluginCallbackInfo::EDeactiveReceiving,
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

    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectionNotify end" )
    }


// ---------------------------------------------------------------------------
// from base class MNATFWRefreshObserver
// CNATFWStunConnectionHandler::BindingRefreshL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::BindingRefreshL()
    {
    TInt streamCount( iStreamArray.Count() );
    
    for ( TInt streamInd = 0; streamInd < streamCount; streamInd++ )
        {
        TInt connCount( iStreamArray[streamInd].iConnArray.Count() );
        
        for ( TInt connInd = 0; connInd < connCount; connInd++ )
            {
            TConnectionData* connection = ConnectionByIndex( streamInd,
                connInd );
            
            connection->iStunBinding->SendRequestL( connection->iStunSrvAddr,
                EFalse, iStreamArray[streamInd].iRtoValue );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::DeleteStream
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::DeleteStream( TUint aStreamInd,
    TBool aRemoveMuxConn )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::DeleteStream, start" )
    ASSERT( aStreamInd < TUint( iStreamArray.Count() ) );
    
    TRAP_IGNORE( 
        iConnMux->UnregisterIncomingConnectionObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )
    TRAP_IGNORE( 
        iConnMux->UnregisterConnectionObserverL(
            iStreamArray[aStreamInd].iStreamId, *this ) )
    
    // Remove the stream's connections
    TInt connIndex( iStreamArray[aStreamInd].iConnArray.Count() - 1 );
    
    for ( TInt i( connIndex ); i >= 0; i-- )
        {
        TConnectionData* connection = ConnectionByIndex( aStreamInd, i );
        
        if( connection->iStunBinding )
            {
            connection->iStunBinding->CancelRequest();
            
            delete connection->iStunBinding;
            connection->iStunBinding = NULL;
            delete connection->iLocalCandidate;
            connection->iLocalCandidate = NULL;
            }
        
        if ( aRemoveMuxConn )
            {
            TRAP_IGNORE( iConnMux->RemoveConnectionL(
                iStreamArray[aStreamInd].iStreamId, connection->
                    iConnectionId ) )
            }
        
        iStreamArray[aStreamInd].iConnArray.Remove( i );
        }
    
    iStreamArray[aStreamInd].iConnArray.Close();
    iStreamArray.Remove( aStreamInd );
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::DeleteStream, end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::IndexByStreamId
// ---------------------------------------------------------------------------
//
TInt CNATFWStunConnectionHandler::IndexByStreamId( TUint aStreamId )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::IndexByStreamId" )
    
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
// CNATFWStunConnectionHandler::GenerateServerListL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::GenerateServerListL()
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::GenerateServerListL start" )
    
    RPointerArray<MNATFWServerSettings> serverArray;
    iStunSettings->GetStunServerArrayL( serverArray );
    
    CleanupClosePushL( serverArray );
    TInt count( serverArray.Count() );
    
    HBufC8* serverAddress =
        iStunSettings->LatestConnectedServerAddr().AllocLC();
    TUint latestPort = iStunSettings->LatestConnectedServerPort();
    
    // Add Latest connected STUN server
    if ( serverAddress->Compare( KNullDesC8 ) )
        {
        __STUNPLUGIN( "CNATFWStunConnectionHandler::GenerateServerListL\
        - Latest stun server added" )
        
        CStunServerSettings* settings = CStunServerSettings::NewLC();
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
        
        iServerList.AppendL( settings );
        CleanupStack::Pop( settings );
        }
    
    CleanupStack::PopAndDestroy( serverAddress );
    
    // Add provisioned servers
    for ( TInt index( 0 ); index < count; index++ )
        {

        CStunServerSettings* settings = CStunServerSettings::NewLC();
        
        settings->SetAddressL( serverArray[index]->Address() );
        settings->SetPort( serverArray[index]->Port() );
        
        if ( serverArray[index]->Username().Length() &&
            serverArray[index]->Password().Length() )
            {
            settings->SetUsernameL( serverArray[index]->Username() );
            settings->SetPasswordL( serverArray[index]->Password() );
            }
        
        if ( settings->Address().Compare( KNullDesC8 ) )
            {
            __STUNPLUGIN( "CNATFWStunConnectionHandler::GenerateServerListL\
            - provisioned server added" )
            iServerList.AppendL( settings );
            }
        
        CleanupStack::Pop( settings );
        }
    
    // Domain
    CStunServerSettings* settings = CStunServerSettings::NewLC();
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
    __STUNPLUGIN( "CNATFWStunConnectionHandler::GenerateServerListL end" )
    }


// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::TryNextServerL
// ---------------------------------------------------------------------------
//
void CNATFWStunConnectionHandler::TryNextServerL()
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::TryNextServerL start" )
    
    delete iStunClient;
    iStunClient = NULL;
    
    if ( iServerIndex < iServerList.Count() )
        {
        TInt index( iServerIndex );
        iServerIndex++;
        
        TInt refresh( iNatSettings->RefreshIntervalUdp() );
        iStunRefreshInterval =
            ( refresh ) ? ( refresh * KMicrosecFactor ) :
                KDefaultRefreshInterval;
        
        TInt retransmitInterval( iStunSettings->RetransmissionTimeout() );
        TBool useSharedSecret( iNatSettings->UseSharedSecret() );
        
        TBool useLongTerm( EFalse );
        
        if ( useSharedSecret &&
            iServerList[index]->Username() &&
            iServerList[index]->Password() )
            {
            useLongTerm = ETrue;
            }
        
        TUint port = iServerList[index]->Port() ?
            iServerList[index]->Port() : KDefaultStunSrvPort;
        
        iStunClient = CSTUNClient::NewL( retransmitInterval,
                                         iServerList[index]->Address(),
                                         port,
                                         KStun,
                                         iSocketServ,
                                         iConnection,
                                         *iTimerServ,
                                         *this,
                                         useSharedSecret,
                                         EFalse,
                                         EFalse,
                                         iConnMux );
        
        if ( useLongTerm )
            {
            TRAP_IGNORE( iStunClient->SetCredentialsL( *iServerList[index]->
                Username(), *iServerList[index]->Password() ) )
            }
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    __STUNPLUGIN( "CNATFWStunConnectionHandler::TryNextServerL end" )
    }

void CNATFWStunConnectionHandler::ExecuteCallBack()
    {
    if ( iCallBackCmds.Count() )
        {
        if ( EConnectServer == iCallBackCmds[0].iType )
            {
            iPluginObserver.Notify( 
                iStunPlugin, iCallBackCmds[0].iStreamId, 
                MNATFWPluginObserver::EServerConnected, KErrNone );
            }
        else if ( EFetchCandidate == iCallBackCmds[0].iType )
            {
            iPluginObserver.NewLocalCandidateFound( iStunPlugin, NULL );
            iPluginObserver.NewLocalCandidateFound( 
                iStunPlugin, 
                reinterpret_cast<CNATFWCandidate*>( iCallBackCmds[0].iDataPtr ) );
            iPluginObserver.Notify( 
                iStunPlugin, iCallBackCmds[0].iStreamId, 
                MNATFWPluginObserver::EFetchingCompleted, KErrNone );
           }
        else if ( ESetSendingStatus == iCallBackCmds[0].iType )
            {
            iPluginObserver.Notify( iStunPlugin, iCallBackCmds[0].iStreamId, 
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

// ---------------------------------------------------------------------------
// CNATFWStunConnectionHandler::ConnectionById
// ---------------------------------------------------------------------------
//
TConnectionData* CNATFWStunConnectionHandler::ConnectionById(
    TUint aStreamInd,
    TUint aConnectionId )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectionById" )
    
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
// CNATFWStunConnectionHandler::ConnectionByIndex
// ---------------------------------------------------------------------------
//
TConnectionData* CNATFWStunConnectionHandler::ConnectionByIndex(
    TUint aStreamInd,
    TUint aConnectionInd )
    {
    __STUNPLUGIN( "CNATFWStunConnectionHandler::ConnectionByIndex" )
    
    return &iStreamArray[aStreamInd].iConnArray[aConnectionInd];
    }

// End of file
