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

#include <e32base.h>
#include <badesca.h>
#include "natfwconnectivityapi.h"
#include "natfwcandidate.h"
#include "mnatfwconnectivityobserver.h"
#include "nspdefs.h"
#include "s_natfwsocketmediaconnwrapper.h"
#include "nsptls.h"

const TUint KSessionIdBase = 10;
const TUint KStreamIdBase = 100;

inline void LeaveWithPrintL( TInt aError )
	{
	NSPLOG_INT( "(STUB)LEAVE SERVICE, WITH CODE:", aError )
	User::Leave( aError );
	}

#define NSP_STORE_EVENT( aEvent ) NSPTls::Storage()->iLastAction = aEvent
#define NSP_LEAVE_SERVICE \
	__ASSERT_ALWAYS( !NSPTls::Storage()->iLeaveService,\
	LeaveWithPrintL( NSPTls::Storage()->iLeaveService ) )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CNATFWConnectivityApi
// ---------------------------------------------------------------------------
// 
CNATFWConnectivityApi::CNATFWConnectivityApi()
	: iSessionIdBase( KSessionIdBase ),
	  iStreamIdBase( KStreamIdBase )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::ConstructL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::~CNATFWConnectivityApi
// ---------------------------------------------------------------------------
// 
CNATFWConnectivityApi::~CNATFWConnectivityApi()
    {
    iWrappers.ResetAndDestroy();
    iWrappers.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::NewL
// ---------------------------------------------------------------------------
// 
CNATFWConnectivityApi* CNATFWConnectivityApi::NewL()
    {
    CNATFWConnectivityApi* self = new (ELeave) CNATFWConnectivityApi;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::NewLC
// ---------------------------------------------------------------------------
// 
CNATFWConnectivityApi* CNATFWConnectivityApi::NewLC()
    {
    CNATFWConnectivityApi* self = new (ELeave) CNATFWConnectivityApi;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::RegisterObserverForEventsL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::RegisterObserverForEventsL(
        MNATFWConnectivityObserver& /*aObserver*/, TUint16 aEvents )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::RegisterObserverForEventsL, aEvents:", aEvents )
    NSP_STORE_EVENT( TNSPStorage::ERegisterObserver );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UnregisterObserverForEvents
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::UnregisterObserverForEvents(
        MNATFWConnectivityObserver& /*aObserver*/, TUint16 aEvents )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::UnregisterObserverForEvents, aEvents:", aEvents )
    NSP_STORE_EVENT( TNSPStorage::EUnRegisterObserver );
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateSessionL
// ---------------------------------------------------------------------------
// 
TUint CNATFWConnectivityApi::CreateSessionL( TUint32 aIap, const TDesC8& aDomain )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CreateSessionL, aIapId:", (TUint) aIap )
    NSPLOG8_STR2( "(STUB)CNATFWConnectivityApi::CreateSessionL, aDomain:", aDomain )
    NSP_STORE_EVENT( TNSPStorage::ECreateSession );
    NSP_LEAVE_SERVICE;
    
    return (iSessionIdBase++);
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::LoadPluginL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::LoadPluginL( TUint aSessionId,
        const CDesC8Array& aPlugins, TInt& aPluginIndex )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::LoadPluginL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::LoadPluginL, aPlugins.Count():", aPlugins.MdcaCount() )
    NSP_STORE_EVENT( TNSPStorage::ELoadPlugin );
    NSP_LEAVE_SERVICE;
    
    if ( aPlugins.MdcaCount() )
        {
        NSPLOG8_STR2( "(STUB)CNATFWConnectivityApi::LoadPluginL, aPlugin[0]:", aPlugins.MdcaPoint(0) )
        }
    
    aPluginIndex = 0;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNATFWConnectivityApi::CreateStreamL( TUint aSessionId,
        TUint aProtocol, TInt aQoS )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CreateStreamL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CreateStreamL, aProtocol:", aProtocol )
    NSPLOG_INT( "(STUB)CNATFWConnectivityApi::CreateStreamL, aQoS:", aQoS )
    NSP_STORE_EVENT( TNSPStorage::ECreateStream );
    NSP_LEAVE_SERVICE;
    
    return (iStreamIdBase++);
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CreateWrapperL
// ---------------------------------------------------------------------------
//
MNATFWSocketMediaConnWrapper& CNATFWConnectivityApi::CreateWrapperL(
        TUint aSessionId, TUint aStreamId )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CreateWrapperL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CreateWrapperL, aStreamId:", aStreamId )
    NSP_STORE_EVENT( TNSPStorage::ECreateWrapper );
    NSP_LEAVE_SERVICE;
    
    CNSPWrapper* wrapper = CNSPWrapper::NewL( aStreamId );
    CleanupStack::PushL( wrapper );
    iWrappers.AppendL( wrapper );
    CleanupStack::Pop( wrapper );
    return (*wrapper);
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::FetchCandidateL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::FetchCandidateL( TUint aSessionId, TUint aStreamId,
		TUint aAddrFamily )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidateL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidateL, aStreamId:", aStreamId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidateL, aAddrFamily:", aAddrFamily )
    NSP_STORE_EVENT( TNSPStorage::EFetchCandidate );
    NSP_LEAVE_SERVICE;    
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::FetchCandidatesL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::FetchCandidatesL( TUint aSessionId, TUint aStreamId,
		TUint aCollectionId, TUint aComponentId, TUint aAddrFamily )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidatesL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidatesL, aStreamId:", aStreamId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidatesL, aCollectionId:", aCollectionId )
	NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidatesL, aComponentId:", aComponentId )
	NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::FetchCandidatesL, aAddrFamily:", aAddrFamily )
    NSP_STORE_EVENT( TNSPStorage::EFetchCandidates );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetRoleL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::SetRoleL( TUint aSessionId, TNATFWIceRole aRole )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetRoleL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetRoleL, aRole:", (TUint)aRole )
    NSP_STORE_EVENT( TNSPStorage::ESetRole );
    NSP_LEAVE_SERVICE;    
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetCredentialsL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::SetCredentialsL(
        CNATFWCandidate& aCandidate, 
        const CNATFWCredentials& /*aIdentification*/ )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetCredentialsL, aSessionId:", aCandidate.SessionId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetCredentialsL, aStreamId:", aCandidate.StreamId() )
    NSP_STORE_EVENT( TNSPStorage::ESetCredentials );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::PerformConnectivityChecksL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::PerformConnectivityChecksL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::PerformConnectivityChecksL, aRemoteCands.Count():", aRemoteCands.Count() )
    NSP_STORE_EVENT( TNSPStorage::EPerformConnectivityChecks );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::UpdateIceProcessingL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::UpdateIceProcessingL, aPeerSelectedPairs.Count():", aPeerSelectedPairs.Count() )
    NSP_STORE_EVENT( TNSPStorage::EUpdateIcePeers );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::UpdateIceProcessingL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::UpdateIceProcessingL, aRemoteCands.Count():", aRemoteCands.Count() )
    NSP_STORE_EVENT( TNSPStorage::EUpdateIceRemoteCands );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetReceivingStateL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aSessionId:", aLocalCandidate.SessionId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aStreamId:", aLocalCandidate.StreamId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aStatus:", (TUint)aState )
    NSP_STORE_EVENT( TNSPStorage::ESetReceivingState );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetSendingStateL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
		TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aSessionId:", aLocalCandidate.SessionId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aStreamId:", aLocalCandidate.StreamId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aState:", (TUint)aState )    
    NSP_STORE_EVENT( TNSPStorage::ESetSendingState );
    NSP_LEAVE_SERVICE;
    
    TBuf<40> output;
    aDestAddr.Output( output );
    NSPLOG_STR2( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aDestAddr:", output )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aDestAddr.Port():", aDestAddr.Port() )
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWConnectivityApi::SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
		TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aSessionId:", aLocalCandidate.SessionId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetReceivingStateL, aStreamId:", aLocalCandidate.StreamId() )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aState:", (TUint)aState )    
    NSPLOG8_STR2( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aDestAddr:", aDestAddr )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::SetSendingStateL, aPort    :", aPort )
    NSP_STORE_EVENT( TNSPStorage::ESetSendingState );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CloseStreamL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::CloseStreamL( TUint aSessionId, TUint aStreamId )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CloseStreamL, aSessionId:", aSessionId )
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CloseStreamL, aStreamId:", aStreamId )
    NSP_STORE_EVENT( TNSPStorage::ECloseStream );
    NSP_LEAVE_SERVICE;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectivityApi::CloseSessionL
// ---------------------------------------------------------------------------
// 
void CNATFWConnectivityApi::CloseSessionL( TUint aSessionId )
    {
    NSPLOG_UINT( "(STUB)CNATFWConnectivityApi::CloseSessionL, aSessionId:", aSessionId )
    NSP_STORE_EVENT( TNSPStorage::ECloseSession );
    NSP_LEAVE_SERVICE;
    }

