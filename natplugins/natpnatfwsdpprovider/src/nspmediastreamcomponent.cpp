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
* Description:  Media stream component class implementation
*
*/

#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "mnatfwsocketmediaconnwrapper.h"
#include "nspsessionobserver.h"
#include "nspmediastreamcomponent.h"
#include "nspnatfwasyncstate.h"
#include "nspcontrollerif.h"
#include "nspdefs.h"

const TUint KNSPFetchCandTimerInMicros = 7000000; // 7 seconds
const TUint KNSPRtcpComponentId = 2;

#define TOINETADDR_L( aDesc, aPort ) ConvertTDesC8ToTInetAddrL( aDesc, aPort )
#define NATFWRECEIVING_L( aCand, aState ) \
	iController.SetReceivingStateL( aCand, aState )
#define NATFWSENDING_L( aCand, aState ) \
	iController.SetSendingStateL( aCand, aState, iRemoteAddress, iRemotePort )


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::CNSPMediaStreamComponent
// ---------------------------------------------------------------------------
//
CNSPMediaStreamComponent::CNSPMediaStreamComponent( MNSPControllerIF& aController,
        TUint aSessionId, TUint aCollectionId, TUint aComponentId )
    : iController( aController ),
      iSessionId( aSessionId ),
      iCollectionId( aCollectionId ),
      iComponentId( aComponentId )
    {
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::ConstructL( const TDesC8& aAddress, TUint aPort,
        TUint aProtocol, TUint aMediaTos )
    {
    TInetAddr addr = TOINETADDR_L( aAddress, aPort );
    iAddrFamily = addr.Family();
    
    iStreamId = iController.CreateStreamL( 
        iSessionId, aProtocol, static_cast<TInt>( aMediaTos ) );
    iWrapper = &iController.CreateWrapperL( iSessionId, iStreamId );
    
    iWrapper->SetIncomingAddrL( addr );
    iWrapper->SetReceivingStateL( EStreamingStateActive );
    iWrapper->SetSendingStateL( EStreamingStateActive );
    
    iState = CNSPNATFWAsyncState::NewL();
    iRemoteAddress.Assign( KNullDesC8().AllocL() );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::NewL
// ---------------------------------------------------------------------------
//
CNSPMediaStreamComponent* CNSPMediaStreamComponent::NewL(
        MNSPControllerIF& aController, TUint aSessionId,
        TUint aCollectionId, TUint aComponentId, const TDesC8& aAddress,
        TUint aPort, TUint aProtocol, TUint aMediaTos )
    {
    CNSPMediaStreamComponent* self = CNSPMediaStreamComponent::NewLC(
            aController, aSessionId, aCollectionId, aComponentId,
            aAddress, aPort, aProtocol, aMediaTos );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::NewLC
// ---------------------------------------------------------------------------
//
CNSPMediaStreamComponent* CNSPMediaStreamComponent::NewLC(
        MNSPControllerIF& aController, TUint aSessionId, TUint aCollectionId,
        TUint aComponentId, const TDesC8& aAddress, TUint aPort,
        TUint aProtocol, TUint aMediaTos )
    {
    CNSPMediaStreamComponent* self = new ( ELeave ) CNSPMediaStreamComponent(
            aController, aSessionId, aCollectionId, aComponentId );
    CleanupStack::PushL( self );
    self->ConstructL( aAddress, aPort, aProtocol, aMediaTos );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::~CNSPMediaStreamComponent
// ---------------------------------------------------------------------------
//
CNSPMediaStreamComponent::~CNSPMediaStreamComponent()
    {
    iRemoteAddress.Close();
    iWrapper = NULL;
    delete iState;
    delete iCandidatePair;
    iLocalCandidates.ResetAndDestroy();
    iLocalCandidates.Close();
    iController.Cancel( iTimerTransactionId );
    TRAP_IGNORE( iController.CloseStreamL( iSessionId, iStreamId ) )
    }

// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ControlMediaL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::ControlMediaL(
		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
		TInt aError, TAny* aData )
	{
    NSPLOG_STR( "CNSPMediaStreamComponent::ControlMediaL(), Entry" )
    
    TNatReturnStatus status = KNatAsync;
    
    switch ( aEvent )
    	{
    	case MNATFWConnectivityObserver::ELocalCandidateFound:
    		{
    		iLocalCandidates.AppendL( reinterpret_cast<CNATFWCandidate*>(aData) );
    		break;
    		}
    	
    	case MNATFWConnectivityObserver::ECandidatePairFound:
    		{
    	    delete iCandidatePair;
    	    iCandidatePair = reinterpret_cast<CNATFWCandidatePair*>(aData);
    		break;
    		}
    	
        case MNATFWConnectivityObserver::EReceivingActivated:
            {
            status = HandleReceivingActivated( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::ESendingActivated:
            {
            status = HandleSendingActivated( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::EFetchingCompleted:
            {
            status = HandleFetchingCompleted( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::EConnChecksCompleted:
            {
            status = HandleConnChecksCompleted( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::EReceivingDeactivated:
            {
            status = HandleReceivingDeActivated( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::ESendingDeactivated:
            {
            status = HandleSendingDeActivated( aError );
            break;
            }
        
        case MNATFWConnectivityObserver::EGeneralError:
        	{
        	status = ErrorOccurred( aError );
        	break;
        	}
        
        case MNATFWConnectivityObserver::ESessionCreated:
        default:
            {
            User::Leave( KErrNotReady );
            }
    	}
    
    NSPLOG_INT( "CNSPMediaStreamComponent::ControlMediaL(), Exit, status", status )
    return status;
	}


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ResetAndDestroyCandidates
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::ResetAndDestroyCandidates()
    {
    iLocalCandidates.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::FetchCandidateL()
    {
    iState->CheckStateL( CNSPNATFWAsyncState::ENATFWEventFetchCand );
    
    iController.FetchCandidateL( iSessionId, iStreamId, iAddrFamily );
    iTimerTransactionId = iController.OrderTimeoutL( iSessionId, iStreamId,
            MNATFWConnectivityObserver::EFetchingCompleted,
            KNSPFetchCandTimerInMicros );
    
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventFetchCand );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::FetchCandidatesL()
    {
    iState->CheckStateL( CNSPNATFWAsyncState::ENATFWEventFetchCand );
    
    iController.FetchCandidatesL( iSessionId,
            iStreamId, iCollectionId, iComponentId, iAddrFamily );
    iTimerTransactionId = iController.OrderTimeoutL( iSessionId, iStreamId,
            MNATFWConnectivityObserver::EFetchingCompleted,
            KNSPFetchCandTimerInMicros );
    
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventFetchCand );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::PerformCandidateChecksL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::PerformCandidateChecksL()
    {
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventPerformChecks );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::ActivateL( const TDesC8& aAddress, TUint aPort )
    {
    iState->CheckStateL( CNSPNATFWAsyncState::ENATFWEventRecvAct );
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventRecvDeAct );
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventSendDeAct );
    
    iRemoteAddress.Close();
    iRemoteAddress.Assign( aAddress.AllocL() );
    iRemotePort = aPort;
    
    __ASSERT_ALWAYS( iLocalCandidates.Count(), User::Leave( KErrNotReady ) );
    NATFWRECEIVING_L( *iLocalCandidates[0], EStreamingStateActive );
    NATFWSENDING_L( *iLocalCandidates[0], EStreamingStateActive );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::ActivateL()
    {
    __ASSERT_ALWAYS( iCandidatePair, User::Leave( KErrNotReady ) );
    
    iState->CheckStateL( CNSPNATFWAsyncState::ENATFWEventRecvAct );    
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventRecvDeAct );
    iState->ChangeStateL( CNSPNATFWAsyncState::ENATFWEventSendDeAct );
    
    iRemoteAddress.Close();
    iRemoteAddress.Assign( iCandidatePair->RemoteCandidate().TransportDomainAddr().AllocL() );
    iRemotePort = iCandidatePair->RemoteCandidate().TransportDomainPort();
    
    NATFWRECEIVING_L( iCandidatePair->LocalCandidate(), EStreamingStateActive );
    NATFWSENDING_L( iCandidatePair->LocalCandidate(), EStreamingStateActive );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::DeActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::DeActivateL()
    {
    iState->CheckStateL( CNSPNATFWAsyncState::ENATFWEventRecvDeAct );
    
    __ASSERT_ALWAYS( iLocalCandidates.Count(), User::Leave( KErrNotReady ) );
    NATFWRECEIVING_L( *iLocalCandidates[0], EStreamingStatePassive );
    NATFWSENDING_L( *iLocalCandidates[0], EStreamingStatePassive );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::RemoteAddress
// ---------------------------------------------------------------------------
//
const TDesC8& CNSPMediaStreamComponent::RemoteAddress() const
    {
    return iRemoteAddress;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::RemotePort
// ---------------------------------------------------------------------------
//
TUint CNSPMediaStreamComponent::RemotePort() const
    {
    return iRemotePort;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::StreamId
// ---------------------------------------------------------------------------
//
TUint CNSPMediaStreamComponent::StreamId() const
    {
    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ComponentId
// ---------------------------------------------------------------------------
//
TUint CNSPMediaStreamComponent::ComponentId() const
    {
    return iComponentId;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::LocalCandidates
// ---------------------------------------------------------------------------
//
RPointerArray<CNATFWCandidate>& CNSPMediaStreamComponent::LocalCandidates()
    {
    return iLocalCandidates;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::IceLocalCandidateL
// ---------------------------------------------------------------------------
//
const CNATFWCandidate& CNSPMediaStreamComponent::IceLocalCandidateL()
    {
    __ASSERT_ALWAYS( iCandidatePair, User::Leave( KErrNotReady ) );
    return iCandidatePair->LocalCandidate();
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::IceRemoteCandidateL
// ---------------------------------------------------------------------------
//
const CNATFWCandidate& CNSPMediaStreamComponent::IceRemoteCandidateL()
    {
    __ASSERT_ALWAYS( iCandidatePair, User::Leave( KErrNotReady ) );
    return iCandidatePair->RemoteCandidate();
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::OutgoingAddress
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStreamComponent::OutgoingAddress( TDes8& aAddress, TUint& aPort ) const
    {
    TInetAddr address;
    iWrapper->OutgoingAddr( address );
    TRAPD( error, ConvertTInetAddrToTDes8AndTUintL( address, aAddress, aPort ) );
    return error;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::SetCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamComponent::SetCredentialsL( const CNATFWCredentials& aCredentials )
    {
    iController.SetCredentialsL( iSessionId, iStreamId, aCredentials );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::AttachCandidateL
// ---------------------------------------------------------------------------
//
TBool CNSPMediaStreamComponent::AttachCandidateL( CNATFWCandidate& aCandidate ) const
    {
    if ( aCandidate.ComponentId() == iComponentId )
        {
        aCandidate.SetStreamId( iStreamId );
        return ETrue;
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleReceivingActivated
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleReceivingActivated(
        TInt aError )
    {
    TInt error = iState->CheckState( CNSPNATFWAsyncState::ENATFWEventRecvAct );
    
    if ( KErrNone == error && KErrNone == aError )
        {
        iState->ChangeState( CNSPNATFWAsyncState::ENATFWEventRecvAct );
        return Continue();
        }
    else if ( KErrNone == aError )
        {
        return ErrorOccurred( error );
        }
    else
        {
        return ErrorOccurred( aError );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleSendingActivated
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleSendingActivated(
        TInt aError )
    {
    TInt error = iState->CheckState( CNSPNATFWAsyncState::ENATFWEventSendAct );
    
    if ( KErrNone == error && KErrNone == aError )
        {
        iState->ChangeState( CNSPNATFWAsyncState::ENATFWEventSendAct );
        return Continue();
        }
    else if ( KErrNone == aError )
        {
        return ErrorOccurred( error );
        }
    else
        {
        return ErrorOccurred( aError );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleReceivingDeActivated
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleReceivingDeActivated(
        TInt aError )
    {
    TInt error = iState->CheckState( CNSPNATFWAsyncState::ENATFWEventRecvDeAct );
    
    if ( KErrNone == error && KErrNone == aError )
        {
        iState->ChangeState( CNSPNATFWAsyncState::ENATFWEventRecvDeAct );
        return Continue();
        }
    else if ( KErrNone == aError )
        {
        return ErrorOccurred( error );
        }
    else
        {
        return ErrorOccurred( aError );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleSendingDeActivated
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleSendingDeActivated(
        TInt aError )
    {
    TInt error = iState->CheckState( CNSPNATFWAsyncState::ENATFWEventSendDeAct );
    
    if ( KErrNone == error && KErrNone == aError )
        {
        iState->ChangeState( CNSPNATFWAsyncState::ENATFWEventSendDeAct );
        return Continue();
        }
    else if ( KErrNone == aError )
        {
        return ErrorOccurred( error );
        }
    else
        {
        return ErrorOccurred( aError );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleFetchingCompleted
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleFetchingCompleted( TInt aError )
    {
    iController.Cancel( iTimerTransactionId );
    
    TNatReturnStatus status = 
        ( KErrNone == aError ? Continue() : ErrorOccurred( aError ) );
    
    if ( iLocalCandidates.Count() )
        {
        const CNATFWCandidate& candidate = *iLocalCandidates[0];
        
        // local candidates are never in FQDN format
        if ( candidate.Type() == CNATFWCandidate::EServerReflexive &&
             candidate.Base().CmpAddr( candidate.TransportAddr() ) )
            {
            // we are not behind NAT, no need to proceed with NATFW
            NSPLOG_STR( 
                "CNSPMediaStreamComponent::HandleFetchingCompleted, NO NAT" )
            status = KNatNonFatalErrorLimit;
            }
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::HandleConnChecksCompleted
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::HandleConnChecksCompleted( TInt aError )
    {
    return ( KErrNone == aError ? Continue() : ErrorOccurred( aError ) );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::Continue
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::Continue()
    {
    return ( iState->Continue() ? KNatReady : KNatAsync );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamComponent::ErrorOccurred
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamComponent::ErrorOccurred( TInt aError )
    {
    NSPLOG_INT( "CNSPMediaStreamComponent::ErrorOccurred():", aError )
    
    return ( KErrTimedOut == aError ||
    		 SuppressRtcpIcmp( aError ) ? Continue() : aError );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::SuppressRtcpIcmp
// ---------------------------------------------------------------------------
//
TBool CNSPMediaStreamComponent::SuppressRtcpIcmp( TInt aError ) const
	{
	return ( KNSPRtcpComponentId == iComponentId && NSP_ICMP( aError ) );
	}


// end of file
