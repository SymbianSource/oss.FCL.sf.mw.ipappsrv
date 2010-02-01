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




#include "mnatfwsocketmediaconnwrapper.h"
#include "mnatfwconnectivityobserver.h"
#include "natfwpluginapi.h"
#include "natfwstream.h"
#include "natfwsession.h"
#include "mnatfwregistrationcontroller.h"
#include "natfwsocketmediaconnwrapper.h"
#include "cncmconnectionmultiplexer.h"
#include "cncmportstore.h"
#include "cnatfwasynccallback.h"
#include "natfwclientlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWStream::CNATFWStream
// ---------------------------------------------------------------------------
//
CNATFWStream::CNATFWStream(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol, TInt aQoS )
    :
    iQoS( aQoS ),
    iProtocol( aProtocol ),
    iController( aController ),
    iMultiplexer( aMultiplexer ),
    iAsyncCallback( aAsyncCallback ),
    iContext( aContext )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWStream::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWStream::ConstructL()
    {
    iStreamId = iMultiplexer.CreateStreamL( iContext.SessionId(),
        iQoS, iProtocol );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::NewL
// ---------------------------------------------------------------------------
//
CNATFWStream* CNATFWStream::NewL(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol, TInt aQoS )
    {
    CNATFWStream* self = CNATFWStream::NewLC( aController, aMultiplexer,
        aAsyncCallback, aContext, aProtocol, aQoS );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWStream::NewLC
// ---------------------------------------------------------------------------
//
CNATFWStream* CNATFWStream::NewLC(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol, TInt aQoS )
    {
    CNATFWStream* self = new ( ELeave ) CNATFWStream( aController,
        aMultiplexer, aAsyncCallback, aContext, aProtocol, aQoS );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWStream::~CNATFWStream
// ---------------------------------------------------------------------------
//
CNATFWStream::~CNATFWStream()
    {
    __NATFWCLIENT("CNATFWStream::~CNATFWStream")

    iPlugin = NULL;
    TInt err = KErrNone;
    TRAP( err, iMultiplexer.RemoveStreamL( iStreamId ) );

    __NATFWCLIENT_INT1( "CNATFWStream::~CNATFWStream end, ERR:", err )
    }


// Non-derived function

// ---------------------------------------------------------------------------
// Return unique identifier of the stream.
// ---------------------------------------------------------------------------
//
TUint CNATFWStream::StreamId() const
    {
    return iStreamId;
    }

// Derived function

// ---------------------------------------------------------------------------
// CNATFWStream::CreateWrapperL
// ---------------------------------------------------------------------------
//
MNATFWSocketMediaConnWrapper& CNATFWStream::CreateWrapperL( )
    {
    __NATFWCLIENT("CNATFWStream::CreateWrapperL")

    CNATFWSocketMediaConnWrapper* wrapper =
        CNATFWSocketMediaConnWrapper::NewL(
        iMultiplexer.PortStoreL( iContext.SessionId() ).Port(),
        iProtocol, iStreamId );

    // Ownership transfered to Connection Multiplexer
    iMultiplexer.RegisterMediaWrapperL( wrapper );
    return *wrapper;
    }

// ---------------------------------------------------------------------------
// CNATFWStream::SetProtocolPlugin
// ---------------------------------------------------------------------------
//
void CNATFWStream::SetProtocolPlugin( CNATFWPluginApi* aPlugin )
    {
    __NATFWCLIENT("CNATFWStream::SetProtocolPlugin")
    iPlugin = aPlugin;
    }


// ---------------------------------------------------------------------------
// CNATFWStream::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWStream::FetchCandidateL( TUint aAddrFamily )
    {
    __NATFWCLIENT("CNATFWStream::FetchCandidateL")
    __ASSERT_ALWAYS( iPlugin, User::Leave( KErrNotReady ) );

    iPlugin->FetchCandidateL( iStreamId, 0, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNATFWStream::FetchCandidatesL( TUint aCollectionId,
        TUint aComponentId, TUint aAddrFamily )
    {
    __NATFWCLIENT("CNATFWStream::FetchCandidatesL")
    __ASSERT_ALWAYS( iPlugin, User::Leave( KErrNotReady ) );

    iPlugin->FetchCandidatesL( 
        aCollectionId, iStreamId, aComponentId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::SetCredentialsL
// ---------------------------------------------------------------------------
//
void CNATFWStream::SetCredentialsL(
    const CNATFWCandidate& aCandidate,
    const CNATFWCredentials& aCredentials )
    {
    __NATFWCLIENT("CNATFWStream::SetCredentialsL")
    __ASSERT_ALWAYS( iPlugin, User::Leave( KErrNotReady ) );

    iPlugin->SetCredentialsL( aCandidate, aCredentials );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStream::SetReceivingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    TUint connectionForMedia( 0 );
    iPlugin->GetConnectionIdL( aLocalCandidate, connectionForMedia );

    iPlugin->SetReceivingStateL( aLocalCandidate, aState );
    iMultiplexer.SetReceivingStateForMediaL( 
        iStreamId, connectionForMedia, aState );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStream::SetSendingStateL(
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    iDestAddr = aDestAddr;
    TUint connectionForMedia( 0 );
    iPlugin->GetConnectionIdL( aLocalCandidate, connectionForMedia );

    iPlugin->SetSendingStateL( aLocalCandidate, aState, aDestAddr );
    iMultiplexer.SetSendingStateForMediaL(
        iStreamId, connectionForMedia, aState );
    }


// ---------------------------------------------------------------------------
// CNATFWStream::Notify
// ---------------------------------------------------------------------------
//
void CNATFWStream::Notify( TUint aSessionId, TUint aStreamId,
    MNcmConnectionMultiplexerObserver::TNotifyType aType, TInt aError )
    {
    __NATFWCLIENT_INT2(
        "CNATFWStream::Notify, SESSION:", aSessionId, "STREAM:", aStreamId )

    switch ( aType )
        {
        case MNcmConnectionMultiplexerObserver::EReceivingActivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWStream::Notify, EReceivingActivated, ERR:", aError )

            DoNotify( aSessionId, aStreamId,
                MNATFWConnectivityObserver::EReceivingActivated, aError );
            }
            break;

        case MNcmConnectionMultiplexerObserver::EReceivingDeactivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWStream::Notify, EReceivingDeactivated, ERR:", aError )

            DoNotify( aSessionId, aStreamId,
                MNATFWConnectivityObserver::EReceivingDeactivated, aError );
            }
            break;

        case MNcmConnectionMultiplexerObserver::ESendingActivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWStream::Notify, ESendingActivated, ERR:", aError )

            DoNotify( aSessionId, aStreamId,
                MNATFWConnectivityObserver::ESendingActivated, aError );
            }
            break;

        case MNcmConnectionMultiplexerObserver::ESendingDeactivated:
            {
            __NATFWCLIENT_INT1(
                "CNATFWStream::Notify, ESendingDeactivated, ERR:", aError )

            DoNotify( aSessionId, aStreamId,
                MNATFWConnectivityObserver::ESendingDeactivated, aError );
            }
            break;

        case MNcmConnectionMultiplexerObserver::EStreamError:
            {
            __NATFWCLIENT_INT1(
                "CNATFWStream::Notify, EStreamError, ERR:", aError )

            DoNotify( aSessionId, aStreamId,
                MNATFWConnectivityObserver::EGeneralError, aError );
            }
            break;

        default:
            __NATFWCLIENT("CNATFWStream::Notify, DEFAULT")

            ASSERT( EFalse );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWStream::DoNotify
// ---------------------------------------------------------------------------
//
void CNATFWStream::DoNotify( TUint aSessionId, TUint aStreamId,
    MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent, TInt aError )
    {

    CNatFwCallbackInfo::TFunction func =
        static_cast<CNatFwCallbackInfo::TFunction>( aEvent );

    TRAP_IGNORE( iAsyncCallback.MakeCallbackL( iController, func,
        aSessionId, aStreamId, aError, NULL ) )
    }

