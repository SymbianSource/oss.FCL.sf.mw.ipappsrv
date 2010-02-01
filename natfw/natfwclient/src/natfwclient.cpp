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
* Description:    Implementation class of the NAT Connectivity FW
*
*/




#include "natfwconnectivityapi.h"
#include "natfwpluginapi.h"
#include "mnatfwconnectivityobserver.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"

#include "natfwclient.h"
#include "cncmconnectionmultiplexer.h"
#include "natfwclientlogs.h"
#include <cnatfwsettingsapi.h>
#include "cnatfwasynccallback.h"

#include "natfwsession.h"
#include "natfwstream.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWClient::CNATFWClient
// ---------------------------------------------------------------------------
//
CNATFWClient::CNATFWClient()
    {
    __NATFWCLIENT("CNATFWClient::CNATFWClient")
    }


// ---------------------------------------------------------------------------
// CNATFWClient::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWClient::ConstructL()
    {
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    iAsyncCallback = CNatFwAsyncCallback::NewL();
    }


// ---------------------------------------------------------------------------
// CNATFWClient::NewL
// ---------------------------------------------------------------------------
//
CNATFWClient* CNATFWClient::NewL()
    {
    __NATFWCLIENT("CNATFWClient::NewL")
    CNATFWClient* self = CNATFWClient::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWClient::NewLC
// ---------------------------------------------------------------------------
//
CNATFWClient* CNATFWClient::NewLC()
    {
    __NATFWCLIENT("CNATFWClient::NewLC")
    CNATFWClient* self = new( ELeave ) CNATFWClient();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWClient::~CNATFWClient()
// ---------------------------------------------------------------------------
//
CNATFWClient::~CNATFWClient()
    {
    __NATFWCLIENT("CNATFWClient::~CNATFWClient")

    iSessions.ResetAndDestroy();
    iEventRegistry.Close();
    delete iConnMux;
    delete iAsyncCallback;
    }


// ---------------------------------------------------------------------------
// CNATFWClient::RegisterObserverForEventL
// ---------------------------------------------------------------------------
//
void CNATFWClient::RegisterObserverForEventsL(
        MNATFWConnectivityObserver& aObserver, TUint16 aEvents )
    {
    __NATFWCLIENT_INT1(
        "CNATFWClient::RegisterObserverForEventsL, EVENTS:", aEvents )

    for ( TInt i(0); i < iEventRegistry.Count(); ++i )
        {
        if ( &iEventRegistry[i].Observer() == &aObserver )
            {
            // Observer already in array
            iEventRegistry[i].ProcessRegistrationL( aEvents );
            return;
            }
        }

    // A new observer instance detected
    TNATFWEventRegistration reg = TNATFWEventRegistration( aObserver );
    reg.ProcessRegistrationL( aEvents );
    iEventRegistry.AppendL( reg );
    }

// ---------------------------------------------------------------------------
// CNATFWClient::UnregisterObserverForEvents
// ---------------------------------------------------------------------------
//
void CNATFWClient::UnregisterObserverForEvents(
        MNATFWConnectivityObserver& aObserver, TUint16 aEvents )
    {
    __NATFWCLIENT_INT1(
        "CNATFWClient::UnregisterObserverForEvents, EVENTS:", aEvents )

    for ( TInt i(0); i < iEventRegistry.Count(); ++i )
        {
        if ( &iEventRegistry[i].Observer() == &aObserver )
            {
            // Observer instance found
            iEventRegistry[i].ProcessUnregistration( aEvents );
            if ( !iEventRegistry[i].Events() )
                {
                // No registered events left, remove the whole entry
                iEventRegistry.Remove( i );
                }
            return;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNATFWClient::CreateSessionL
// ---------------------------------------------------------------------------
//
TUint CNATFWClient::CreateSessionL( TUint32 aIapId, const TDesC8& aDomain )
    {
    __NATFWCLIENT("CNATFWClient::CreateSessionL")

    CNATFWSession* session = CNATFWSession::NewL( *iConnMux, *this,
        *iAsyncCallback, aDomain, aIapId );

    CleanupStack::PushL( session );
    iSessions.AppendL( session );
    CleanupStack::Pop( session );

    return session->SessionId();
    }


// ---------------------------------------------------------------------------
// CNATFWClient::LoadPluginL
// ---------------------------------------------------------------------------
//
void CNATFWClient::LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
        TInt& aLoadedPluginInd )
    {
    __NATFWCLIENT("CNATFWClient::LoadPluginL")

    SessionByIdL( aSessionId )->LoadPluginL( aPlugins, aLoadedPluginInd );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNATFWClient::CreateStreamL( TUint aSessionId, TUint aProtocol,
        TInt aQoS )
    {
    __NATFWCLIENT("CNATFWClient::CreateStreamL")

    return SessionByIdL( aSessionId )->CreateStreamL( aProtocol, aQoS );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::CreateWrapperL
// ---------------------------------------------------------------------------
//
MNATFWSocketMediaConnWrapper& CNATFWClient::CreateWrapperL(
        TUint aSessionId, TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWClient::CreateWrapperL")

    return SessionByIdL( aSessionId )->StreamByIdL( aStreamId)
        ->CreateWrapperL();
    }


// ---------------------------------------------------------------------------
// CNATFWClient::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWClient::FetchCandidateL( TUint aSessionId, TUint aStreamId,
        TUint aAddrFamily )
    {
    __NATFWCLIENT("CNATFWClient::FetchCandidateL")

    SessionByIdL( aSessionId )->FetchCandidateL( aStreamId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNATFWClient::FetchCandidatesL( TUint aSessionId, TUint aStreamId,
        TUint aStreamCollectionId, TUint aComponentId, TUint aAddrFamily )
    {
    __NATFWCLIENT("CNATFWClient::FetchCandidatesL")

    SessionByIdL( aSessionId )->FetchCandidatesL( aStreamId,
        aStreamCollectionId, aComponentId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SetRoleL
// ---------------------------------------------------------------------------
//
void CNATFWClient::SetRoleL( TUint aSessionId, TNATFWIceRole aRole )
    {
    __NATFWCLIENT("CNATFWClient::SetControllingModeL")

    SessionByIdL( aSessionId )->SetRoleL( aRole );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SetCredentialsL
// ---------------------------------------------------------------------------
//
void CNATFWClient::SetCredentialsL(
        CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials )
    {
    __NATFWCLIENT("CNATFWClient::SetCredentialsL")

    if ( KAFUnspec == aCandidate.TransportAddr().Family() &&
         aCandidate.TransportDomainAddr().Length() )
        {
        // Try resolving from FQDN
        ResolveFQDNAddrL( aCandidate );
        }

    SessionByIdL( aCandidate.SessionId() )
        ->StreamByIdL( aCandidate.StreamId() )
            ->SetCredentialsL( aCandidate, aCredentials );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
void CNATFWClient::PerformConnectivityChecksL(
        TUint aSessionId, RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __NATFWCLIENT("CNATFWClient::PerformConnectivityChecksL")

    for ( TInt i(0); i < aRemoteCandidates.Count(); i++ )
        {
        if ( KAFUnspec == aRemoteCandidates[i]->TransportAddr().Family() &&
             aRemoteCandidates[i]->TransportDomainAddr().Length() )
            {
             // Try resolving from FQDN
            ResolveFQDNAddrL( *aRemoteCandidates[i] );
            }
        }

    SessionByIdL( aSessionId )
        ->PerformConnectivityChecksL( aRemoteCandidates );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWClient::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    SessionByIdL( aSessionId )->UpdateIceProcessingL( aPeerSelectedPairs );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWClient::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    for ( TInt i(0); i < aRemoteCands.Count(); i++ )
        {
        if ( KAFUnspec == aRemoteCands[i]->TransportAddr().Family() &&
             aRemoteCands[i]->TransportDomainAddr().Length() )
            {
            // Try resolving from FQDN
            ResolveFQDNAddrL( *aRemoteCands[i] );
            }
        }

    SessionByIdL( aSessionId )->UpdateIceProcessingL( aRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWClient::SetReceivingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    SessionByIdL( aLocalCandidate.SessionId() )
        ->StreamByIdL( aLocalCandidate.StreamId() )
            ->SetReceivingStateL( aLocalCandidate, aState );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWClient::SetSendingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    SessionByIdL( aLocalCandidate.SessionId() )
        ->StreamByIdL( aLocalCandidate.StreamId() )
            ->SetSendingStateL( aLocalCandidate, aState, aDestAddr );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWClient::SetSendingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState,
        const TDesC8& aDestAddr, TUint aPort )
    {
    __NATFWCLIENT_STR8("CNATFWClient::SetSendingStateL - FQDN", aDestAddr )
    __NATFWCLIENT_INT1("CNATFWClient::SetSendingStateL - PORT", aPort )

    TInetAddr result;
    HBufC* addrBuf = HBufC::NewLC( aDestAddr.Length() );
    TPtr addrPtr( addrBuf->Des() );
    User::LeaveIfError(
        CnvUtfConverter::ConvertToUnicodeFromUtf8( addrPtr, aDestAddr ) );
        
    KErrNone != result.Input( addrPtr ) ?
        // This is an FQDN not convertable to an IP address
        iConnMux->ResolveDestinationAddressL(
            aLocalCandidate.StreamId(), aDestAddr, aPort, result ) :
        // Valid IP address
        result.SetPort( aPort );
    CleanupStack::PopAndDestroy( addrBuf );

    SetSendingStateL( aLocalCandidate, aState, result );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::CloseStreamL
// ---------------------------------------------------------------------------
//
void CNATFWClient::CloseStreamL( TUint aSessionId, TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWClient::CloseStreamL")

    SessionByIdL( aSessionId )->CloseStreamL( aStreamId );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::CloseSessionL
// ---------------------------------------------------------------------------
//
void CNATFWClient::CloseSessionL( TUint aSessionId )
    {
    __NATFWCLIENT("CNATFWClient::CloseSessionL")

    CNATFWSession* session = SessionByIdL( aSessionId );

    TInt index = iSessions.FindL( session );
    iSessions.Remove( index );
    delete session;
    }


// ---------------------------------------------------------------------------
// From class MNcmConnectionMultiplexerObserver.
// CNATFWClient::Notify
// ---------------------------------------------------------------------------
//
void CNATFWClient::Notify( TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError )
    {
    __NATFWCLIENT("CNATFWClient::Notify")

    CNATFWSession* session = NULL;

    if ( MNcmConnectionMultiplexerObserver::ESessionCreated == aType )
        {
        DoNotify( MNATFWConnectivityObserver::ESessionCreated, aSessionId,
            aStreamId, aError, NULL );
        }
    else if ( MNcmConnectionMultiplexerObserver::ESendingActivated ==
        aType )
        {
        TRAP_IGNORE( ( session = SessionByIdL( aSessionId ) ) )
        if ( session )
            {
            const CNATFWPluginApi& dummyParam
                = reinterpret_cast<const CNATFWPluginApi&>( *this );
            session->Notify( dummyParam, aStreamId,
                MNATFWPluginObserver::ESendingActivated, aError );
            }
        }
    else if ( MNcmConnectionMultiplexerObserver::ESendingDeactivated ==
        aType )
        {
        TRAP_IGNORE( ( session = SessionByIdL( aSessionId ) ) )
        if ( session )
            {
            const CNATFWPluginApi& dummyParam
                = reinterpret_cast<const CNATFWPluginApi&>( *this );
            session->Notify( dummyParam, aStreamId,
                MNATFWPluginObserver::ESendingDeactivated, aError );
            }
        }
    else if ( MNcmConnectionMultiplexerObserver::EReceivingActivated ==
        aType )
        {
        TRAP_IGNORE( ( session = SessionByIdL( aSessionId ) ) )
        if ( session )
            {
            const CNATFWPluginApi& dummyParam
                = reinterpret_cast<const CNATFWPluginApi&>( *this );
            session->Notify( dummyParam, aStreamId,
                MNATFWPluginObserver::EReceivingActivated, aError );
            }
        }
    else if ( MNcmConnectionMultiplexerObserver::EReceivingDeactivated ==
        aType )
        {
        TRAP_IGNORE( ( session = SessionByIdL( aSessionId ) ) )
        if ( session )
            {
            const CNATFWPluginApi& dummyParam
                = reinterpret_cast<const CNATFWPluginApi&>( *this );
            session->Notify( dummyParam, aStreamId,
                MNATFWPluginObserver::EReceivingDeactivated, aError );
            }
        }
    else
        {
        // Show event to the stream so that state info relating to the
        // stream can be updated.
        CNATFWStream* stream = FindStreamById( aStreamId );
        if ( stream )
            {
            stream->Notify( aSessionId, aStreamId, aType, aError );
            }
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWRegistrationController.
// CNATFWClient::Registry
// ---------------------------------------------------------------------------
//
const RArray<TNATFWEventRegistration>& CNATFWClient::Registry()
    {
    return iEventRegistry;
    }


// ---------------------------------------------------------------------------
// CNATFWClient::SessionByIdL
// ---------------------------------------------------------------------------
//
CNATFWSession* CNATFWClient::SessionByIdL( TUint aSessionId )
    {
    __NATFWCLIENT("CNATFWClient::SessionByIdL")

    TInt ind( iSessions.Count() );

    while ( ind-- )
        {
        if ( iSessions[ind]->SessionId() == aSessionId )
            {
            return iSessions[ind];
            }
        }

    User::Leave( KErrNotFound );
    return NULL;    // lint #527
    }


// ---------------------------------------------------------------------------
// CNATFWClient::FindStreamById
// ---------------------------------------------------------------------------
//
CNATFWStream* CNATFWClient::FindStreamById( TUint aStreamId )
    {
    __NATFWCLIENT("CNATFWClient::FindStreamById")

    TInt sessInd( iSessions.Count() );

    while ( sessInd-- )
        {
        CNATFWStream* stream = iSessions[sessInd]->StreamById( aStreamId );
        if ( stream )
            {
            return stream;
            }
        }

    return NULL;
    }


// ---------------------------------------------------------------------------
// CNATFWClient::ResolveFQDNAddrL
// ---------------------------------------------------------------------------
//
void CNATFWClient::ResolveFQDNAddrL( CNATFWCandidate& aCandidate )
    {
    TInetAddr result;
    iConnMux->ResolveDestinationAddressL(
        aCandidate.StreamId(),
        aCandidate.TransportDomainAddr(),
        aCandidate.TransportDomainPort(),
        result );
    aCandidate.SetTransportAddrL( result );
    }


// ---------------------------------------------------------------------------
// CNATFWClient::DoNotify
// ---------------------------------------------------------------------------
//
void CNATFWClient::DoNotify(
    MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
    TUint aSessionId, TUint aStreamId, TInt aErrCode,
    TAny* aEventData )
    {
    CNatFwCallbackInfo::TFunction func =
        static_cast<CNatFwCallbackInfo::TFunction>( aEvent );
    TRAP_IGNORE( iAsyncCallback->MakeCallbackL( *this,
        func, aSessionId, aStreamId, aErrCode, aEventData ) )
    }

