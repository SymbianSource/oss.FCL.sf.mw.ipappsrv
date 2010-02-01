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
* Description:   
*
*/




#include <badesca.h>
#include "natfwpluginapi.h"
#include "mnatfwconnectivityobserver.h"
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "natfwsession.h"
#include "natfwstream.h"
#include "mnatfwregistrationcontroller.h"
#include <cnatfwsettingsapi.h>
#include "natfwclientlogs.h"
#include "cncmconnectionmultiplexer.h"
#include "cnatfwasynccallback.h"

const TUint KSendingActivationsNeeded = 2;

const TUint KReceivingActivationsNeeded = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWSession::CNATFWSession
// ---------------------------------------------------------------------------
//
CNATFWSession::CNATFWSession( CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        TUint32 aIapId )
    :
    iIapId( aIapId ),
    iController( aController ),
    iMultiplexer( aMultiplexer ),
    iAsyncCallback( aAsyncCallback ),
    iServerConnectionState( EConnectionUnspecified )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWSession::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWSession::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    TUint startPort( 0 );
    TUint endPort( 0 );

    iNatSettings = CNATFWNatSettingsApi::NewL( aDomain );

    iNatSettings->GetPortArea( startPort, endPort );

    iSessionId = iMultiplexer.CreateSessionL( iIapId, startPort, endPort );
    }


// ---------------------------------------------------------------------------
// CNATFWSession::NewL
// ---------------------------------------------------------------------------
//
CNATFWSession* CNATFWSession::NewL( CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        const TDesC8& aDomain,
        TUint32 aIapId )
    {
    CNATFWSession* self =
        CNATFWSession::NewLC( aMultiplexer, aController, aAsyncCallback,
            aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSession::NewLC
// ---------------------------------------------------------------------------
//
CNATFWSession* CNATFWSession::NewLC( CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        const TDesC8& aDomain,
        TUint32 aIapId )
    {
    CNATFWSession* self = new ( ELeave ) CNATFWSession( aMultiplexer,
        aController, aAsyncCallback, aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWSession::~CNATFWSession()
    {
    iSendingStatusCounts.Close();
    iReceivingStatusCounts.Close();
    iFetchCandidateQueue.Close();
    iStreams.ResetAndDestroy();
    delete iDomain;
    delete iPlugin;
    delete iNatSettings;

    REComSession::FinalClose();

    TInt err = KErrNone;
    TRAP( err, iMultiplexer.RemoveSessionL( iSessionId ) );
    __NATFWCLIENT_INT1("CNATFWSession::~CNATFWSession, ERR:", err )
    }


// Non-derived function

// ---------------------------------------------------------------------------
// CNATFWSession::LoadPluginL()
// ---------------------------------------------------------------------------
//
void CNATFWSession::LoadPluginL( const CDesC8Array& aPlugins,
        TInt& aLoadedPluginInd )
    {
    __NATFWCLIENT("CNATFWSession::LoadPluginL")
    __ASSERT_ALWAYS( aPlugins.Count(), User::Leave( KErrArgument ) );

    CNATFWPluginApi::TNATFWPluginInitParams
        initParams( *this, iMultiplexer, *iDomain, iIapId );

    TInt error( KErrNotFound );
    TInt count( aPlugins.Count() );
    TInt index( 0 );

    delete iPlugin;
    iPlugin = NULL;

    while ( !iPlugin && index < count )
        {
        TRAP( error, iPlugin =
            CNATFWPluginApi::NewL( aPlugins[index], initParams ) );
        index++;
        }

    TInt ind( iStreams.Count() );
    while ( ind-- )
        {
        iStreams[ind]->SetProtocolPlugin( iPlugin );
        }

    User::LeaveIfError( error );
    aLoadedPluginInd = --index;

    RSocketServ* socketServ = NULL;
    TName connectionName;

    socketServ = iMultiplexer.GetSessionInfoL( iSessionId, connectionName );
    iPlugin->ConnectServerL( *socketServ, connectionName );
    iServerConnectionState = EConnectionInProgress;

    __NATFWCLIENT("CNATFWSession::LoadPluginL Exit")
    }


// ---------------------------------------------------------------------------
// CNATFWSession::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWSession::FetchCandidateL( TUint aStreamId, TUint aAddrFamily )
    {
    __ASSERT_ALWAYS( iServerConnectionState != EConnectionUnspecified,
        User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iServerConnectionState != EConnectionFailed,
        User::Leave( KErrNotReady ) );

    if ( iServerConnectionState == EConnectionConnected )
        {
        StreamByIdL( aStreamId )->FetchCandidateL( aAddrFamily );
        }
    else
        {
        TFetchingData data( aStreamId, 0, 0, aAddrFamily, EFalse );
        iFetchCandidateQueue.InsertL( data, 0 );
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSession::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNATFWSession::FetchCandidatesL( TUint aStreamId, TUint aCollectionId,
        TUint aComponentId, TUint aAddrFamily )
    {
    __ASSERT_ALWAYS( iServerConnectionState != EConnectionUnspecified,
        User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iServerConnectionState != EConnectionFailed,
        User::Leave( KErrNotReady ) );

    if ( iServerConnectionState == EConnectionConnected )
        {
        StreamByIdL( aStreamId )->FetchCandidatesL(
            aCollectionId, aComponentId, aAddrFamily );
        }
    else
        {
        TFetchingData data( aStreamId, aCollectionId,
            aComponentId, aAddrFamily, ETrue );

        iFetchCandidateQueue.InsertL( data, 0 );
        }
    }

// ---------------------------------------------------------------------------
// CNATFWSession::SessionId
// ---------------------------------------------------------------------------
//
TUint CNATFWSession::SessionId() const
    {
    __NATFWCLIENT("CNATFWSession::SessionId")

    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNATFWSession::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNATFWSession::CreateStreamL(
        TUint aProtocol, TInt aQoS )
    {
    __NATFWCLIENT("CNATFWSession::CreateStreamL")

    CNATFWStream* stream = CNATFWStream::NewL( iController, iMultiplexer,
        iAsyncCallback, *this, aProtocol, aQoS );
    CleanupStack::PushL( stream );

    iStreams.AppendL( stream );
    stream->SetProtocolPlugin( iPlugin );

    CleanupStack::Pop( stream );
    return stream->StreamId();
    }


// ---------------------------------------------------------------------------
// CNATFWSession::SetRoleL
// ---------------------------------------------------------------------------
//
void CNATFWSession::SetRoleL( TNATFWIceRole aRole )
    {
    __NATFWCLIENT("CNATFWSession::SetRoleL")

    iPlugin->SetRoleL( aRole );
    }


// ---------------------------------------------------------------------------
// CNATFWSession::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
void CNATFWSession::PerformConnectivityChecksL(
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __NATFWCLIENT("CNATFWSession::PerformConnectivityChecksL")

    iPlugin->PerformConnectivityChecksL( aRemoteCandidates );
    }


// ---------------------------------------------------------------------------
// CNATFWSession::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWSession::UpdateIceProcessingL(
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    iPlugin->UpdateIceProcessingL( aPeerSelectedPairs );
    }


// ---------------------------------------------------------------------------
// CNATFWSession::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWSession::UpdateIceProcessingL(
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    iPlugin->UpdateIceProcessingL( aRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNATFWSession::CloseStream
// ---------------------------------------------------------------------------
//
void CNATFWSession::CloseStreamL( TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWSession::CloseStreamL")

    CNATFWStream* stream = StreamByIdL( aStreamId );
    iStreams.Remove( iStreams.FindL( stream ) );
    delete stream;
    }


// ---------------------------------------------------------------------------
// CNATFWSession::StreamById
// ---------------------------------------------------------------------------
//
CNATFWStream* CNATFWSession::StreamById( TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWSession::StreamById")

    TInt ind( iStreams.Count() );

    while ( ind-- )
        {
        if ( iStreams[ind]->StreamId() == aStreamId )
            {
            return iStreams[ind];
            }
        }

    return NULL;
    }


// ---------------------------------------------------------------------------
// CNATFWSession::StreamByIdL
// ---------------------------------------------------------------------------
//
CNATFWStream* CNATFWSession::StreamByIdL( TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWSession::StreamByIdL")

    CNATFWStream* stream = StreamById( aStreamId );
    if ( !stream )
        {
        User::Leave( KErrNotFound );
        }

    return stream;
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CNATFWSession::Error
// ---------------------------------------------------------------------------
//
void CNATFWSession::Error( const CNATFWPluginApi& /*aPlugin*/,
        TUint aStreamId, TInt aErrorCode )
    {
    __NATFWCLIENT("CNATFWSession::Error")

    DoNotify( aStreamId, MNATFWConnectivityObserver::EGeneralError,
        aErrorCode, NULL );
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CNATFWSession::Notify
// ---------------------------------------------------------------------------
//
void CNATFWSession::Notify( const CNATFWPluginApi& /*aPlugin*/,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode )
    {
    __NATFWCLIENT("CNATFWSession::Notify")

    // Activation notify must be waited from both plugin
    // and mediastream before notifying NATFW client

    switch ( aEvent )
        {
        case MNATFWPluginObserver::EServerConnected:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify EServerConnected, ERR:", aErrCode )

            if ( KErrNone != aErrCode )
                {
                iServerConnectionState = EConnectionFailed;
                }
            else
                {
                iServerConnectionState = EConnectionConnected;
                }
            HandleQueuedItems();
            }
            break;

        case MNATFWPluginObserver::ESendingActivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify ESendingActivated, ERR:", aErrCode )

            TInt index = iSendingStatusCounts.Count() - 1;
            TInt foundStreamInd( KErrNotFound );

            while ( 0 <= index && KErrNotFound == foundStreamInd )
                {
                if ( aStreamId == iSendingStatusCounts[index].iStreamId )
                    {
                    foundStreamInd = index;

                    if ( KErrNone != aErrCode )
                        {
                        iSendingStatusCounts[foundStreamInd].iErrorOccurred
                            = ETrue;
                        }

                    iSendingStatusCounts[foundStreamInd].iActivatedCount++;
                    }

                index--;
                }

            if ( KErrNotFound != foundStreamInd )
                {
                if ( KSendingActivationsNeeded ==
                    iSendingStatusCounts[foundStreamInd].iActivatedCount )
                    {
                    // The second notify for current stream's sending
                    // activation has occurred so NATFW Stream can be
                    // notified
                    CNATFWStream* stream = StreamById( aStreamId );
                    if ( stream )
                        {
                        if ( iSendingStatusCounts[foundStreamInd].
                            iErrorOccurred )
                            {
                            // Reset the error occurred flag
                            iSendingStatusCounts[foundStreamInd].iErrorOccurred
                                = EFalse;
                            // Do notification with error code
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    ESendingActivated, KErrGeneral );
                            }
                        else
                            {
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    ESendingActivated, KErrNone );
                            }
                        }
                    }
                }
            else
                {
                // The first notify for stream's sending activation has
                // occurred so store stream's status data in array
                TStatusCounter sendingActivated;
                sendingActivated.iActivatedCount++;
                sendingActivated.iStreamId = aStreamId;

                if ( KErrNone != aErrCode )
                    {
                    sendingActivated.iErrorOccurred = ETrue;
                    }

                iSendingStatusCounts.Append( sendingActivated );
                }
            }
            break;

        case MNATFWPluginObserver::ESendingDeactivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify ESendingDeactivated, ERR:", aErrCode )

            TInt index = iSendingStatusCounts.Count() - 1;
            TInt foundStreamInd( KErrNotFound );

            while ( 0 <= index && KErrNotFound == foundStreamInd )
                {
                if ( aStreamId == iSendingStatusCounts[index].iStreamId &&
                    iSendingStatusCounts[index].iActivatedCount != 0 )
                    {
                    foundStreamInd = index;

                    if ( aErrCode != KErrNone )
                        {
                        iSendingStatusCounts[foundStreamInd].iErrorOccurred
                            = ETrue;
                        }

                    iSendingStatusCounts[foundStreamInd].iActivatedCount--;
                    }

                index--;
                }

            if ( KErrNotFound != foundStreamInd )
                {
                if ( 0 == iSendingStatusCounts[foundStreamInd].
                    iActivatedCount )
                    {
                    CNATFWStream* stream = StreamById( aStreamId );
                    if ( stream )
                        {
                        if ( iSendingStatusCounts[foundStreamInd].
                            iErrorOccurred )
                            {
                            // Reset the error occurred flag
                            iSendingStatusCounts[foundStreamInd].iErrorOccurred
                                = EFalse;
                            // Do notification with error code
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    ESendingDeactivated, KErrGeneral );
                            }
                        else
                            {
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    ESendingDeactivated, KErrNone );
                            }
                        }
                    }
                }
            }
            break;

        case MNATFWPluginObserver::EReceivingActivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify EReceivingActivated, ERR:", aErrCode )

            TInt index = iReceivingStatusCounts.Count() - 1;
            TInt foundStreamInd( KErrNotFound );

            while ( 0 <= index && KErrNotFound == foundStreamInd )
                {
                if ( aStreamId == iReceivingStatusCounts[index].iStreamId )
                    {
                    foundStreamInd = index;

                    if ( KErrNone != aErrCode )
                        {
                        iReceivingStatusCounts[foundStreamInd].iErrorOccurred
                            = ETrue;
                        }

                    iReceivingStatusCounts[foundStreamInd].iActivatedCount++;
                    }

                index--;
                }

            if ( KErrNotFound != foundStreamInd )
                {
                if ( KReceivingActivationsNeeded ==
                    iReceivingStatusCounts[foundStreamInd].iActivatedCount )
                    {
                    // The second notify for current stream's receiving
                    // activation has occurred so NATFW Stream can be
                    // notified
                    CNATFWStream* stream = StreamById( aStreamId );
                    if ( stream )
                        {
                        if ( iReceivingStatusCounts[foundStreamInd].
                            iErrorOccurred )
                            {
                            // Reset the error occurred flag
                            iReceivingStatusCounts[foundStreamInd].iErrorOccurred
                                = EFalse;
                            // Do notification with error code
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    EReceivingActivated, KErrGeneral );
                            }
                        else
                            {
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    EReceivingActivated, KErrNone );
                            }
                        }
                    }
                }
            else
                {
                // The first notify for stream's receiving activation has
                // occurred so store stream's status data in array
                TStatusCounter receivingActivated;
                receivingActivated.iActivatedCount++;
                receivingActivated.iStreamId = aStreamId;

                if ( KErrNone != aErrCode )
                    {
                    receivingActivated.iErrorOccurred = ETrue;
                    }

                iReceivingStatusCounts.Append( receivingActivated );
                }
            }
            break;

        case MNATFWPluginObserver::EReceivingDeactivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify EReceivingDeactivated, ERR:", aErrCode )

            TInt index = iReceivingStatusCounts.Count() - 1;
            TInt foundStreamInd( KErrNotFound );

            while ( 0 <= index && KErrNotFound == foundStreamInd )
                {
                if ( aStreamId == iReceivingStatusCounts[index].iStreamId &&
                    iReceivingStatusCounts[index].iActivatedCount != 0 )
                    {
                    foundStreamInd = index;

                    if ( aErrCode != KErrNone )
                        {
                        iReceivingStatusCounts[foundStreamInd].iErrorOccurred
                            = ETrue;
                        }

                    iReceivingStatusCounts[foundStreamInd].iActivatedCount--;
                    }

                index--;
                }

            if ( KErrNotFound != foundStreamInd )
                {
                if ( 0 == iReceivingStatusCounts[foundStreamInd].
                    iActivatedCount )
                    {
                    CNATFWStream* stream = StreamById( aStreamId );
                    if ( stream )
                        {
                        if ( iReceivingStatusCounts[foundStreamInd].
                            iErrorOccurred )
                            {
                            // Reset the error occurred flag
                            iReceivingStatusCounts[foundStreamInd].iErrorOccurred
                                = EFalse;
                            // Do notification with error code
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    EReceivingDeactivated, KErrGeneral );
                            }
                        else
                            {
                            stream->Notify( iSessionId, aStreamId,
                                MNcmConnectionMultiplexerObserver::
                                    EReceivingDeactivated, KErrNone );
                            }
                        }
                    }
                }
            }
            break;

        case MNATFWPluginObserver::EFetchingCompleted:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify EFetchingCompleted, ERR:", aErrCode )

            DoNotify( aStreamId, MNATFWConnectivityObserver::EFetchingCompleted,
                aErrCode, NULL );
            }
            break;

        case MNATFWPluginObserver::EConnChecksCompleted:
            {
            __NATFWCLIENT_INT1(
                "CNATFWSession::Notify EConnChecksCompleted, ERR:", aErrCode )

            DoNotify( aStreamId, MNATFWConnectivityObserver::EConnChecksCompleted,
                aErrCode, NULL );
            }
            break;

        default:
            {
            __NATFWCLIENT("CNATFWSession::Notify DEFAULT")
            ASSERT( EFalse );
            }
            break;
        }

    __NATFWCLIENT("CNATFWSession::Notify Exit")
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CNATFWSession::NewCandidatePairFound
// ---------------------------------------------------------------------------
//
void CNATFWSession::NewCandidatePairFound(
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidatePair* aCandidatePair )
    {
    __NATFWCLIENT("CNATFWSession::NewCandidatePairFound")

    if ( aCandidatePair )
        {
        const_cast<CNATFWCandidate&>(
            aCandidatePair->RemoteCandidate() ).SetSessionId( iSessionId );
        const_cast<CNATFWCandidate&>(
            aCandidatePair->LocalCandidate() ).SetSessionId( iSessionId );
        DoNotify( aCandidatePair->LocalCandidate().StreamId(), 
            MNATFWConnectivityObserver::ECandidatePairFound,
            KErrNone, aCandidatePair );
        }
    else
        {
        __NATFWCLIENT("CNATFWSession::NewCandidatePairFound - NULL POINTER")
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CNATFWSession::NewLocalCandidateFound
// ---------------------------------------------------------------------------
//
void CNATFWSession::NewLocalCandidateFound(
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidate* aCandidate )
    {
    __NATFWCLIENT("CNATFWSession::NewLocalCandidateFound")

    if ( aCandidate )
        {
        aCandidate->SetSessionId( iSessionId );
        DoNotify( aCandidate->StreamId(), 
            MNATFWConnectivityObserver::ELocalCandidateFound,
            KErrNone, aCandidate );
        }
    else
        {
        __NATFWCLIENT("CNATFWSession::NewLocalCandidateFound - NULL POINTER")
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSession::HandleQueuedItems
// ---------------------------------------------------------------------------
//
void CNATFWSession::HandleQueuedItems()
    {
    __NATFWCLIENT("CNATFWSession::HandleQueuedItems - ENTRY")

    TInt index( iFetchCandidateQueue.Count() - 1 );
    TUint streamId( 0 );
    TUint collectionId( 0 );
    TUint componentId( 0 );
    TUint addrFamily( KAFUnspec );
    TBool iceSpecific( EFalse );

    while ( 0 <= index )
        {
        streamId = iFetchCandidateQueue[index].iStreamId;
        collectionId = iFetchCandidateQueue[index].iCollectionId;
        componentId = iFetchCandidateQueue[index].iComponentId;
        addrFamily = iFetchCandidateQueue[index].iAddrFamily;
        iceSpecific = iFetchCandidateQueue[index].iICESpecific;
        iFetchCandidateQueue.Remove( index );

        CNATFWStream* str( NULL );
        str = StreamById( streamId );

        TInt error( KErrNone );
        if ( str )
            {
            if( iceSpecific )
                {
                TRAP( error,
                    str->FetchCandidatesL( 
                        collectionId, componentId, addrFamily ) );
                }
            else
                {
                TRAP( error, str->FetchCandidateL( addrFamily ) );
                }
            }

        if ( KErrNone != error )
            {
            DoNotify( streamId, MNATFWConnectivityObserver::EFetchingCompleted,
                error, NULL );
            }

        --index;
        }

    __NATFWCLIENT("CNATFWSession::HandleQueuedItems - EXIT")
    }


// ---------------------------------------------------------------------------
// CNATFWSession::DoNotify
// ---------------------------------------------------------------------------
//
void CNATFWSession::DoNotify( TUint aStreamId,
    MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
    TInt aErrCode,
    TAny* aEventData )
    {
    CNatFwCallbackInfo::TFunction func =
        static_cast<CNatFwCallbackInfo::TFunction>( aEvent );

    TRAP_IGNORE( iAsyncCallback.MakeCallbackL( iController, func,
        iSessionId, aStreamId, aErrCode, aEventData ) )

    }
