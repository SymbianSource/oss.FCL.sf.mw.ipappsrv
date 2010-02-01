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
* Description:    API class to the NAT Connectivity FW
*
*/




#include <e32base.h>
#include "natfwconnectivityapi.h"
#include "natfwcandidate.h"

#include "natfwclient.h"
#include "natfwcredentials.h"
#include "natfwclientlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CNATFWConnectivityApi
// ---------------------------------------------------------------------------
//
CNATFWConnectivityApi::CNATFWConnectivityApi( )
    {
    __NATFWCLIENT("CNATFWConnectivityApi::CNATFWConnectivityApi")
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::ConstructL()
    {
    __NATFWCLIENT("CNATFWConnectivityApi::ConstructL")

    iClient = CNATFWClient::NewL();
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWConnectivityApi* CNATFWConnectivityApi::NewL()
    {
    CNATFWConnectivityApi* self = CNATFWConnectivityApi::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWConnectivityApi* CNATFWConnectivityApi::NewLC()
    {
    CNATFWConnectivityApi* self = new ( ELeave ) CNATFWConnectivityApi();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Default C++ destructor
// ---------------------------------------------------------------------------
//
CNATFWConnectivityApi::~CNATFWConnectivityApi()
    {
    __NATFWCLIENT("CNATFWConnectivityApi::~CNATFWConnectivityApi")

    delete iClient;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::RegisterObserverForEventsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::RegisterObserverForEventsL(
        MNATFWConnectivityObserver& aObserver, TUint16 aEvents )
    {
    __NATFWCLIENT_INT1(
        "CNATFWConnectivityApi::RegisterObserverForEventsL, EVENTS:", aEvents )

    iClient->RegisterObserverForEventsL( aObserver, aEvents );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UnregisterObserverForEventsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::UnregisterObserverForEvents(
        MNATFWConnectivityObserver& aObserver,
        TUint16 aEvents )
    {
    __NATFWCLIENT_INT1(
        "CNATFWConnectivityApi::UnregisterObserverForEvents, EVENTS:", aEvents )

    iClient->UnregisterObserverForEvents( aObserver, aEvents );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWConnectivityApi::CreateSessionL(
        TUint32 aIapId, const TDesC8& aDomain )
    {
    __NATFWCLIENT_INT1("CNATFWConnectivityApi::CreateSessionL, IAPID:", aIapId )
    __NATFWCLIENT_STR8(
        "CNATFWConnectivityApi::CreateSessionL, DOMAIN:", aDomain )

    return iClient->CreateSessionL( aIapId, aDomain );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::LoadPluginL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::LoadPluginL( TUint aSessionId,
        const CDesC8Array& aPlugins, TInt& aLoadedPluginInd )
    {
    __NATFWCLIENT("CNATFWConnectivityApi::LoadPluginL")

    iClient->LoadPluginL( aSessionId, aPlugins, aLoadedPluginInd );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWConnectivityApi::CreateStreamL( TUint aSessionId,
        TUint aProtocol, TInt aQoS )
    {
    __NATFWCLIENT_INT3("CNATFWConnectivityApi::CreateStreamL, SESSION:",
        aSessionId, "PROTOCOL:", aProtocol, "QOS:", aQoS )

    return iClient->CreateStreamL( aSessionId, aProtocol, aQoS );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateWrapperL
// ---------------------------------------------------------------------------
//
EXPORT_C MNATFWSocketMediaConnWrapper& CNATFWConnectivityApi::CreateWrapperL(
        TUint aSessionId, TUint aStreamId )
    {
    __NATFWCLIENT_INT2("CNATFWConnectivityApi::CreateWrapperL, SESSION:",
        aSessionId, "STREAM", aStreamId )

    return iClient->CreateWrapperL( aSessionId, aStreamId );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::FetchCandidateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::FetchCandidateL(
        TUint aSessionId, TUint aStreamId, TUint aAddrFamily )
    {
    __NATFWCLIENT_INT2("CNATFWConnectivityApi::FetchCandidateL, SESSION:",
        aSessionId, "STREAM:", aStreamId )

    iClient->FetchCandidateL( aSessionId, aStreamId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::FetchCandidatesL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::FetchCandidatesL(
        TUint aSessionId, TUint aStreamId,
        TUint aCollectionId, TUint aComponentId,
        TUint aAddrFamily )
    {
    __NATFWCLIENT_INT4("CNATFWConnectivityApi::FetchCandidatesL, SESSION:",
        aSessionId, "STREAM:", aStreamId, "COLLECTION:",
        aCollectionId, "COMPONENT", aComponentId )

    iClient->FetchCandidatesL( aSessionId, aStreamId,
        aCollectionId, aComponentId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetRoleL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::SetRoleL(
        TUint aSessionId, TNATFWIceRole aRole )
    {
    __NATFWCLIENT_INT2("CNATFWConnectivityApi::SetRoleL, SESSION:",
        aSessionId, "MODE:", aRole )

    iClient->SetRoleL( aSessionId, aRole );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetCredentialsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::SetCredentialsL(
        CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials )
    {
    __NATFWCLIENT_INT2("CNATFWConnectivityApi::SetCredentialsL, SESSION:",
        aCandidate.SessionId(), "STREAM:", aCandidate.StreamId() )

    iClient->SetCredentialsL( aCandidate, aCredentials );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::PerformConnectivityChecksL(
        TUint aSessionId, RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __NATFWCLIENT_INT1(
        "CNATFWConnectivityApi::PerformConnectivityChecksL, SESSION:",
        aSessionId )

    iClient->PerformConnectivityChecksL( aSessionId, aRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    __NATFWCLIENT_INT1(
        "CNATFWConnectivityApi::UpdateIceProcessingL with PAIRS, SESSION:",
        aSessionId )

    iClient->UpdateIceProcessingL( aSessionId, aPeerSelectedPairs );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __NATFWCLIENT_INT1(
    "CNATFWConnectivityApi::UpdateIceProcessingL with REMOTECANDS, SESSION:",
        aSessionId )

    iClient->UpdateIceProcessingL( aSessionId, aRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetReceivingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::SetReceivingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState )
    {
    __NATFWCLIENT_INT3("CNATFWConnectivityApi::SetReceivingStateL, SESSION:",
        aLocalCandidate.SessionId(), 
        "STREAM:", aLocalCandidate.StreamId(), 
        "STATE:", aState )

    iClient->SetReceivingStateL( aLocalCandidate, aState );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::SetSendingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    __NATFWCLIENT_INT3("CNATFWConnectivityApi::SetSendingStateL, SESSION:",
        aLocalCandidate.SessionId(), 
        "STREAM:", aLocalCandidate.StreamId(), 
        "STATE:", aState )
    __NATFWCLIENT_ADDR( "CNATFWConnectivityApi::SetSendingStateL, ADDRESS",
        aDestAddr )

    iClient->SetSendingStateL( aLocalCandidate, aState, aDestAddr );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::SetSendingStateL(
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort )
    {
    __NATFWCLIENT_INT3("CNATFWConnectivityApi::SetSendingStateL, SESSION:",
        aLocalCandidate.SessionId(), 
        "STREAM:", aLocalCandidate.StreamId(), 
        "STATE:", aState )

    iClient->SetSendingStateL( aLocalCandidate, aState, aDestAddr, aPort );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CloseStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::CloseStreamL(
        TUint aSessionId, TUint aStreamId )
    {
    __NATFWCLIENT_INT2("CNATFWConnectivityApi::CloseStreamL, SESSION:",
        aSessionId, "STREAM:", aStreamId )

    iClient->CloseStreamL( aSessionId, aStreamId );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CloseSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWConnectivityApi::CloseSessionL( TUint aSessionId )
    {
    __NATFWCLIENT_INT1(
        "CNATFWConnectivityApi::CloseSessionL, SESSION:", aSessionId )

    iClient->CloseSessionL( aSessionId );
    }
