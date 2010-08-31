/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include <sipinvitedialogassoc.h>
#include <sipprofile.h>
#include <sdpdocument.h>
#include <siptoheader.h>
#include <sipdialog.h>
#include <sipcontactheader.h>
#include "mcesipsession.h"
#include "mcecssession.h"
#include "mceservercore.h"
#include "fcpluginengine.h"
#include "mcesipconnection.h"
#include "mcesipstatemachine.h"
#include "mcestate.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclient.pan"
#include "mcesrvlogs.h"
#include "mcecomsession.h"
#include "mcecomaudiocodec.h"
#include "mcesipextensions.h"
#include "mcereliablesender.h" 
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "mcetimermanager.h"
#include "mcemediastate.h"

#define MCE_IGNORE_MEDIA_ERROR ( ( defined __WINSCW__ ) || ( defined __WINS__ ) )\
                            && ( !defined MCESRV_UNIT_TESTING ) 
    
// -----------------------------------------------------------------------------
// CMceSipSession::NewL
// -----------------------------------------------------------------------------
//
CMceSipSession* CMceSipSession::NewL( CMceCsSession& aClientSession, 
                                      CMceSipConnection& aSIPConnection,
								      CMceSipStateMachine& aStateMachine,
								      CSIPProfile& aProfile )
	{
    CMceSipSession* self = CMceSipSession::NewLC( aClientSession, 
                                                  aSIPConnection, 
                                                  aStateMachine,
                                                  aProfile );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipSession::NewLC
// -----------------------------------------------------------------------------
//
CMceSipSession* CMceSipSession::NewLC ( CMceCsSession& aClientSession, 
                                        CMceSipConnection& aSIPConnection,
 								        CMceSipStateMachine& aStateMachine,
 								        CSIPProfile& aProfile )
	{
    CMceSipSession* self = new (ELeave) CMceSipSession( aClientSession, 
                                                        aSIPConnection, 
                                                        aStateMachine,
                                                        aProfile );
    CleanupStack::PushL(self);
    self->ConstructL( aClientSession );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipSession::NewLC
// -----------------------------------------------------------------------------
//
CMceSipSession* CMceSipSession::NewLC ( CMceCsSession& aClientSession, 
                                        CMceSipConnection& aSIPConnection,
 								        CMceSipStateMachine& aStateMachine )
	{
    CMceSipSession* self = new (ELeave) CMceSipSession( aClientSession, 
                                                        aSIPConnection, 
                                                        aStateMachine );
    CleanupStack::PushL(self);
    self->ConstructL( aClientSession );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipSession::CMceSipSession
// -----------------------------------------------------------------------------
//
CMceSipSession::CMceSipSession ( CMceCsSession& aClientSession, 
								 CMceSipConnection& aSIPConnection,
								 CMceSipStateMachine& aStateMachine,
								 CSIPProfile& aProfile )
: CMceCsSubSession( KMceCSSIPSession, aClientSession, aSIPConnection, \
					aProfile ),
  iStateMachine( &aStateMachine ),
  iSubState( ENone ),
  iUseFC( EFalse )
	{
	}

// -----------------------------------------------------------------------------
// CMceSipSession::CMceSipSession
// -----------------------------------------------------------------------------
//
CMceSipSession::CMceSipSession ( CMceCsSession& aClientSession, 
								 CMceSipConnection& aSIPConnection,
								 CMceSipStateMachine& aStateMachine )
: CMceCsSubSession( KMceCSSIPSession, aClientSession, aSIPConnection ),
  iStateMachine( &aStateMachine ),
  iSubState( ENone ),
  iUseFC( EFalse )
	{
	}
	
	
// -----------------------------------------------------------------------------
// CMceSipSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipSession::ConstructL ( CMceCsSession& aClientSession )
	{
    MCESRV_DEBUG("CMceSipSession::ConstructL, Entry");
	
	CMceCsSubSession::ConstructL( aClientSession );
	
	iTimer = CMceTimerManager::KNoSuchTimer;
	
	iPendingTimer = CMceTimerManager::KNoSuchTimer;
	
	iCurrentState = &iStateMachine->State( KMceStateIdle );

    iExtensions= CMceSipExtensions::NewL();
    
    iReliableSender = CMceReliableSender::NewL( *this );
    
    iRSeq = KErrNone;
    
    iPreviousState = KMceStateIdle;
    
    iForceSDPSending = EFalse;
    
    iNatSession = EFalse;
                		
    MCESRV_DEBUG("CMceSipSession::ConstructL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceSipSession::~CMceSipSession
// -----------------------------------------------------------------------------
//
CMceSipSession::~CMceSipSession()
	{
    MCESRV_DEBUG("CMceSipSession::~CMceSipSession, Entry");
    
    CloseMedia( ETrue );
    CMceComSession::Delete( iNewBodyCandidate, Manager().MediaManager() );
    
    PrevOfferSafeDelete();	

	delete iOffer;
    Manager().TimerManager().Stop( iTimer );
	
	if ( iFCSession )
		{
		ServerCore().DeregisterFCSession( *this, *iFCSession );	 
		ServerCore().FCPluginEngine().ReleaseSession( *iFCSession); 
		}
	iForkedDialogs.ResetAndDestroy(); 
	delete iMediaAuthorizationToken;
	delete iExtensions;
	delete iReliableSender;
	iRemoteUri.ResetAndDestroy();

    MCESRV_DEBUG("CMceSipSession::~CMceSipSession, Exit");
  	}

// -----------------------------------------------------------------------------
// CMceSipSession::CloseMedia         
// -----------------------------------------------------------------------------
//
void CMceSipSession::CloseMedia( TBool aDelete )
	{
    MCESRV_DEBUG("CMceSipSession::CloseMedia, Entry");

    if ( aDelete )
        {
        CMceComSession::Delete( iBody, Manager().MediaManager() );
        }
    else
        {
        Manager().MediaManager().CloseSession( *iBody );
        }

    CloseBodyBucket();
    
    MCESRV_DEBUG("CMceSipSession::CloseMedia, Exit");
	}

// -----------------------------------------------------------------------------
// CMceSipSession::CloseBodyBucket         
// -----------------------------------------------------------------------------
//
void CMceSipSession::CloseBodyBucket()
	{
    MCESRV_DEBUG("CMceSipSession::CloseBodyBucket, Entry");
	
    while( iBodyBucket.Count() > 0 )
        {
        CMceComSession* body = iBodyBucket[ 0 ];
        if ( &body->SdpSession() )
            {
            if( body->SdpSession().Context() == Dialog() )
                {
                //fork situation
                CMceComSession* tmpBody = body;
                body = iBody;
                iBody = tmpBody;
                }
            body->SdpSession().AttachContext( NULL );
            }
        CMceComSession::Delete( body, Manager().MediaManager() );
        iBodyBucket.Remove( 0 );
        }
    
    iBodyBucket.ResetAndDestroy();
    
    MCESRV_DEBUG("CMceSipSession::CloseBodyBucket, Exit");
	}
	
// -----------------------------------------------------------------------------
// CMceSipSession::ReliableSender         
// -----------------------------------------------------------------------------
//
CMceReliableSender& CMceSipSession::ReliableSender()
    {
    return *iReliableSender;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoInitializeIncomingDialogL
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoInitializeIncomingDialogL( 
    CSIPServerTransaction& /*aTransaction*/ )
    {
    MCESRV_DEBUG("CMceSipSession::DoInitializeIncomingDialogL, Entry");
    
    FCActions().InitializeFCL();
    
    SetBodyL();
    
    MCESRV_DEBUG("DoInitializeIncomingDialogL: new session body:");
    MCESRV_DEBUG_COMSESSION( iBody );
    
    MCESRV_DEBUG("CMceSipSession::DoInitializeIncomingDialogL, Exit");    
    }

// -----------------------------------------------------------------------------
// CMceSipSession::CanDispose
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::CanDispose()
    {
    return ( CurrentState().Id() == KMceStateTerminated &&
             HasInitialInviteTransactionBeenCompleted() ); 
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Actions
// -----------------------------------------------------------------------------
//
TMceActionSet CMceSipSession::Actions()
    {
    return TMceActionSet( *this );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::FCActions
// -----------------------------------------------------------------------------
//
TMceFCActionSet CMceSipSession::FCActions()
    {
    return TMceFCActionSet( *this );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::CurrentState
// -----------------------------------------------------------------------------
//
CMceState& CMceSipSession::CurrentState( TBool aMarkExit )
    {
    if ( aMarkExit )
        {
        iExitState = iCurrentState;
        }
    return *iCurrentState;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Body
// -----------------------------------------------------------------------------
//
CMceComSession* CMceSipSession::Body()
    {
    CMceComSession* body = iBody;

    if ( iBodyBucket.Count() > 0 )
        {
        body = &iBody->SdpSession() && iBody->SdpSession().Context() == Dialog() ? 
               iBody : NULL;
        TInt index = 0;
        while( !body && index < iBodyBucket.Count() )
            {
            body = iBodyBucket[ index++ ];
            body = &body->SdpSession() && body->SdpSession().Context() == Dialog() ? body : NULL;
            }

        body = !body ? iBody : body;
        
        }
    
    return body;    
    }

// -----------------------------------------------------------------------------
// CMceSipSession::BodyCandidate
// -----------------------------------------------------------------------------
//
CMceComSession* CMceSipSession::BodyCandidate()
    {
    return iNewBodyCandidate;
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::SetOffer
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetOffer( CSdpDocument* aSdpDocument )
    {
    if ( aSdpDocument != iOffer )
        {
        delete iOffer;
        iOffer = aSdpDocument;
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Offer
// -----------------------------------------------------------------------------
//
CSdpDocument* CMceSipSession::Offer()
    {
    return iOffer;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SetPrevOffer
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetPrevOffer( CSdpDocument* aSdpDocument )
    {
    if ( aSdpDocument != iPrevOffer )
        {
        PrevOfferSafeDelete();
        iPrevOffer = aSdpDocument;
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::PrevOffer
// -----------------------------------------------------------------------------
//
CSdpDocument* CMceSipSession::PrevOffer()
    {
    return iPrevOffer;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::StorePrevOffer
// -----------------------------------------------------------------------------
//
void CMceSipSession::StorePrevOffer()
    {
    MCESRV_DEBUG( "*** CMceSipSession::StorePrevOfferL, Entry" );
    
    // If refreshing, existing prev offer is going to be taken in use and
    // we don't want to override it with offer.
    if ( iOffer && SubState() != ERefreshing )
        {
        MCESRV_DEBUG( "*** CMceSipSession::StorePrevOfferL, Copy" );
        CSdpDocument* clonedOffer = NULL;
        TRAP_IGNORE( ( clonedOffer = iOffer->CloneL() ) );
        SetPrevOffer( clonedOffer );
        }
    
    MCESRV_DEBUG( "*** CMceSipSession::StorePrevOfferL, Exit" ); 
    }

// -----------------------------------------------------------------------------
// CMceSipSession::IncrementSentSdpCount
// -----------------------------------------------------------------------------
//
void CMceSipSession::IncrementSentSdpCount()
    {
    iSentSdpCount++;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::IncrementReceivedSdpCount
// -----------------------------------------------------------------------------
//   
void CMceSipSession::IncrementReceivedSdpCount()
    {
    iReceivedSdpCount++;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ResetSdpCounts
// -----------------------------------------------------------------------------
//
void CMceSipSession::ResetSdpCounts()
    {
    iSentSdpCount = 0;
    iReceivedSdpCount = 0; 
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoesMediaManagerNeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::DoesMediaManagerNeedToNegotiate( 
    CSdpDocument* aSdpDocument )
    {
    CMceMediaManager& mediaManager = Manager().MediaManager();
    return mediaManager.NeedToNegotiate( ActiveBody(), aSdpDocument );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::NatSession
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::NatSession() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::NatState
// -----------------------------------------------------------------------------
//
TInt CMceSipSession::NatState() const
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::IsNatDisabled
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::IsNatDisabled() const
	{
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMceSipSession::StartSessionTimerL
// -----------------------------------------------------------------------------
//
void CMceSipSession::StartSessionTimerL()
    {
    MCESRV_DEBUG("CMceSipSession::StartSessionTimerL, Entry");
    
    const TUint KTwo = 2;
    
    CMceTimerManager& timerManager = Manager().TimerManager();
    
    if( iBody->iTimeout > 0 )
    	{
    	TUint32 interval;
    	
	    if( iBody->iRefresh )
	    	{
	    	// if this end should perform refreshes then the reccommended
	    	// time is half the session interval
	    	interval = ( iBody->iTimeout * KMceSecToMillisec ) / KTwo;
	    	}
	    else
	    	{
	    	// the other end is performing refreshes, so set the timeout
	    	// to full session interval and send bye in case of no refresh
	    	interval = iBody->iTimeout * KMceSecToMillisec;
	    	}

        if ( timerManager.IsRunning( iTimer ) )
            {
    		timerManager.Stop( iTimer );
            }
            
        iTimer = timerManager.StartL( this, interval );
        }
    MCESRV_DEBUG("CMceSipSession::StartSessionTimerL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::StartPendingTimerL
// -----------------------------------------------------------------------------
//
void CMceSipSession::StartPendingTimerL( TUint aInterval )
    {
    MCESRV_DEBUG("CMceSipSession::StartPendingTimerL, Entry");
    
    CMceTimerManager& timerManager = Manager().TimerManager();
    
    iPendingTimer = timerManager.StartL( this, aInterval );
    
    MCESRV_DEBUG("CMceSipSession::StartPendingTimerL, Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::NextState
// -----------------------------------------------------------------------------
//
void CMceSipSession::NextState( TUint aNextStateId )
    {
    MCESRV_DEBUG("CMceSipSession::NextState, Entry");
    
    iPreviousState = iCurrentState->Id();
    
    MCESRV_DEBUG_STATE("old state", iPreviousState );
    MCESRV_DEBUG_STATE("new state", aNextStateId );
    
    ClearRemoteUri( aNextStateId );
    iCurrentState = &iStateMachine->State( aNextStateId );
    
    // Automatic event generation
    //
    TMceStateTransitionEvent* autoEvent = StoredAutoEvent();
    if ( AutoEvent() && autoEvent && iCurrentlyProcessedAutoEvent != autoEvent )
        {
        // Remember what event is processed at the moment in order to avoid
        // multiple generations of same event if nested state changes occur.
        iCurrentlyProcessedAutoEvent = autoEvent;
        
        TRAPD( err, DoAutoEventL( *autoEvent ) );
        if ( err )
            {
            MCESRV_DEBUG("CMceSipSession::NextState, automatic event failed");
            err++;
            }
            
        // Do it just once
        SetAutoEvent( EFalse );
        iCurrentlyProcessedAutoEvent = NULL;
        }

    MCESRV_DEBUG("CMceSipSession::NextState, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::MethodCode
// -----------------------------------------------------------------------------
//
TMceSipEventCode CMceSipSession::MethodCode( RStringF method )
    {
    TMceSipEventCode eventCode = (TMceSipEventCode)KErrNotFound;
    
    if ( method == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        eventCode = EMceInvite;
        }
    else if ( method == SIPStrings::StringF( SipStrConsts::ECancel ) )
        {
        eventCode = EMceCancel;
        }
    else if ( method == SIPStrings::StringF( SipStrConsts::EAck ) )
        {
        eventCode = EMceAck;
        }
    else if ( method == SIPStrings::StringF( SipStrConsts::EBye ) )
        {
        eventCode = EMceBye;
        }
    else if ( method == SIPStrings::StringF( SipStrConsts::EPrack ) )
        {
        eventCode = EMcePrack;
        }
    else if ( method == SIPStrings::StringF( SipStrConsts::EUpdate ) )
        {
        eventCode = EMceUpdate;
        }
    else if ( Actions().IsExtensionRequest( method ))
    	{
    	eventCode = EMceRequest;	
    	}
    return eventCode;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ActiveBody
// -----------------------------------------------------------------------------
//
CMceComSession& CMceSipSession::ActiveBody()
    {
    TSubState subState = SubState();
    
    CMceComSession* body = NULL;
    
    if ( subState != EUpdating )
        {
        body = Body();
        }
    else
        {
        // If session is already accepted/rejected, body candidate does not
        // exist anymore but the session is still in updating substate.
        body = BodyCandidate() ? BodyCandidate() : Body();
        }
        
    return *body;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::UpdateMediaL
// -----------------------------------------------------------------------------
//
TMceReturnStatus CMceSipSession::UpdateMediaL()
    {
    MCESRV_DEBUG("CMceSipSession::UpdateMediaL, Entry");

    TMceReturnStatus status = Manager().MediaManager().UpdateL( ActiveBody() );
    
    TUint signallingTos = ActiveBody().Modifier( KMceSignallingTypeOfService );
    if ( signallingTos != KMaxTUint16 )
        {
        SIPConnection().UpdateConnectionL( signallingTos );
        }
    
    MCESRV_DEBUG("CMceSipSession::UpdateMediaL, Exit");
    
    return status;
    }
        
// -----------------------------------------------------------------------------
// CMceSipSession::ReserveL
// -----------------------------------------------------------------------------
//
TMceReturnStatus CMceSipSession::ReserveL()
    {
    MCESRV_DEBUG("CMceSipSession::ReserveL, Entry");
    
    TMceReturnStatus status = KMceReady;
    HBufC8* token = MediaAuthorizationToken();
    if ( token )
        {
        // session has received a media authorization token    
        CMceMediaManager& mediaManager = Manager().MediaManager();   
        status = mediaManager.ReserveL( ActiveBody(), *token );
        }
    
    MCESRV_DEBUG_DVALUE( "ReserveL: status", status);
        
    MCESRV_DEBUG("CMceSipSession::ReserveL, Exit");
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::NeedToSend
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::NeedToSend( CSdpDocument* aSdpDocument )
    {
    MCESRV_DEBUG( "CMceSipSession::NeedToSend, Entry" )
	
	CMceMediaManager& mediaManager = Manager().MediaManager();

    TBool needToSend = 
        ( DoesMediaManagerNeedToNegotiate( aSdpDocument ) || 
          (( iSentSdpCount < iReceivedSdpCount ) && 
          	( mediaManager.GetRole( ActiveBody()) == EMceRoleAnswerer )));
     
    MCESRV_DEBUG_DVALUE( "CMceSipSession::NeedToSend, Exit", needToSend )
    
    return needToSend;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::NeedToReceive
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::NeedToReceive()
    {
    MCESRV_DEBUG( "CMceSipSession::NeedToReceive, Entry" );
    
    CMceMediaManager& mediaManager = Manager().MediaManager();
    TBool confirmationNeeded = mediaManager.ConfirmationNeeded( ActiveBody() );
    
    MCESRV_DEBUG( "CMceSipSession::NeedToReceive, Exit" );
    
    return confirmationNeeded;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::AcceptMethod
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::AcceptMethod( RStringF aMethod )
    {
    return MethodCode( aMethod ) != (TMceSipEventCode)KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMceSipSession::RequestReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::RequestReceived( TBool aInsideDialog, CSIPDialog& aDialog  )
    {
    MCESRV_DEBUG("CMceSipSession::RequestReceived, Entry");
    
    if ( Request().Type() == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        MCESRV_DEBUG("CMceSipSession::RequestReceived, invite");
        if ( aInsideDialog && iCurrentState->Id() == KMceStateAnswering )
            {
            MCESRV_DEBUG("CMceSipSession::RequestReceived, ack missing, fake it");
            
            HandleSIPEvent( EMceAck, aDialog ); 
            }
        else if ( iCurrentState->Id() != KMceStateEstablished && 
                  iCurrentState->Id() != KMceStateIdle )
            {
            
            MCESRV_DEBUG("there is pending request");
            // Send 491
            MceSip::DiscardRequest( Request(), KMceSipRequestPending );
            delete PopRequest();
            return;
            }
        else
            {
            // NOP
            }
        }
        
    MCESRV_DEBUG("Handle new request");
    TMceSipEventCode eventCode = MethodCode( MceSip::Method( Request() ) );
    
    HandleSIPEvent( eventCode, aDialog );
    
    MCESRV_DEBUG("CMceSipSession::RequestReceived, Exit");
    }


// -----------------------------------------------------------------------------
// CMceSipSession::HandleSIPEvent
// -----------------------------------------------------------------------------
//
void CMceSipSession::HandleSIPEvent( TMceSipEventCode aEventCode, CSIPDialog& aDialog )
    {
    MCESRV_DEBUG("CMceSipSession::HandleSIPEvent, Entry");
    
    if ( Body() && &Body()->SdpSession() )
        {
        Body()->SdpSession().AttachContext( Dialog() );
        }

    TMceStateTransitionEvent event( *this, aEventCode, aDialog );
    
    if ( iStateMachine->Consumes( event ) )
        {
        TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
                "HandleSIPEvent: state machine causes ERROR", error );
             
            TRAPD( handleError, HandleSIPEventErrorL( aEventCode, error ) );
            if ( handleError != KErrNone )
                {
                LocalErrorOccured( KMceStateError, error );
                }
            }
        }
    else
        {
        MceSip::DiscardRequest( Request(), KMceSipNotImplemented );
        }
        
    MCESRV_DEBUG("CMceSipSession::HandleSIPEvent, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::HandleSIPEventErrorL
// -----------------------------------------------------------------------------
//
void CMceSipSession::HandleSIPEventErrorL( TMceSipEventCode aEventCode,
                                           TInt aError )
    {
    switch( aEventCode )
        {
        case EMceProvisionalResponse:
        case EMceRedirectionResponse:
        case EMceResponse:
        case EMceErrorResponse:
            { 
            if ( SubState() == EUpdating )
                {
                Actions().StopUpdatingMedia();
                Actions().UpdateFailed(); 
                Actions().StateChanged( KMceStateEstablished );
                Actions().ClientStateChangedL( CMceSession::EEstablished, *iBody, aError );                               
                }
            else
                {
                Actions().StopMedia();
                FCActions().ReleaseFC();
                CSIPDialog::TState dialogState = Dialog()->Dialog().State();
                if ( dialogState == CSIPDialog::EConfirmed )
                    {
                    // ACK is already sent, terminate session with BYE
                    Actions().SendBYEL();
                    Actions().StateChanged( KMceStateTerminating );
                    Actions().ClientStateChangedL( CMceSession::ETerminating );                
                    }
                else if ( dialogState == CSIPDialog::EEarly )
                    {
                    Actions().SendCancelL();
                    Actions().StateChanged( KMceStateCanceled );
                    Actions().ClientStateChangedL( CMceSession::ECancelling );                
                    }
                // for 3xx response error situation
                else if ( dialogState == CSIPDialog::ETerminated )
                    {
                    Actions().StateChanged( KMceStateTerminated );
                    Actions().ClientStateChangedL( 
                        CMceSession::ETerminated, *Body(), Response() );                         
                    }
                else
                    {
                    // NOP
                    }   
                }
            
            break;
            }
        default:
            {
            if ( aError == KErrTotalLossOfPrecision )
                {
                // unexpected request is rejected, but does not
                // affect on session.
                if ( Request().Type() == SIPStrings::StringF( 
                    SipStrConsts::EPrack ) )
                    {
                    // unexpected PRACK
                    MceSip::DiscardRequest( Request(), 
                        KMceSipCallOrTransactionDoesNotExist );
                    }
                else
                    {
                    // Default handling for other methods
                    MceSip::DiscardRequest( Request() );
                    }
                }
            else
                {
                Actions().StopMedia();
                FCActions().ReleaseFC();
            
                // other errors cause session to be terminated
                MceSip::DiscardRequest( Request(), aError );
                if ( aEventCode == EMcePrack || 
                     aEventCode == EMceUpdate )
                    {
                    //reject also invite
                    DiscardInvite();
                    }
                CSIPDialog::TState dialogState = Dialog()->Dialog().State();
                if ( dialogState == CSIPDialog::EConfirmed )
                    {
                    // terminate established session with BYE
                    Actions().SendBYEL();
                    Actions().StateChanged( KMceStateTerminating );
                    Actions().ClientStateChangedL( CMceSession::ETerminating );                
                    }
                else
                    {
                    Actions().StateChanged( KMceStateTerminated );
                    Actions().ClientStateChangedL( CMceSession::ETerminated );
                    }
                }
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SdpCleanup
// -----------------------------------------------------------------------------
//
void CMceSipSession::SdpCleanup( CSdpDocument* aPrevious, CSdpDocument* aReplacement )
    {   
    if ( iBody )
        {
        iBody->SdpCleanup( aPrevious, aReplacement );
        }
        
    if ( iNewBodyCandidate )
        {
        iNewBodyCandidate->SdpCleanup( aPrevious, aReplacement );
        }
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::CreateIncomingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipSession::CreateIncomingDialogL( 
    CSIPServerTransaction& aInitialRequest )
    {
    MCESRV_DEBUG("CMceSipSession::CreateIncomingDialogL, Entry");
    __ASSERT_ALWAYS( ProfileConfigured(), User::Leave( KErrArgument ) );
        
    CSIPInviteDialogAssoc* dialog = 
            CSIPInviteDialogAssoc::NewL( aInitialRequest, Profile() );
        
    MCESRV_DEBUG("CMceSipSession::CreateIncomingDialogL, Exit");
    
    return dialog;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::CreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipSession::CreateOutgoingDialogL( 
                                                    TMceDialogType aDialogType,
                                                    CDesC8Array& /*aParams*/,
                                                    CSIPDialog& aExistingDialog )
    {
    MCESRV_DEBUG("CMceSipSession::CreateOutgoingDialogL( existing dlg ), Entry");
    
    __ASSERT_ALWAYS( aDialogType == KMceDlgTypeInvite, User::Leave( KErrArgument ) );
    
    
    CSIPInviteDialogAssoc* dialog = 
        CSIPInviteDialogAssoc::NewL( aExistingDialog );
        
    MCESRV_DEBUG("CMceSipSession::CreateOutgoingDialogL( existing dlg ), Exit");
    
    return dialog;   
    }

// -----------------------------------------------------------------------------
// CMceSipSession::CreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipSession::CreateOutgoingDialogL( 
                                                    TMceDialogType aDialogType,
                                                    CDesC8Array& aParams )
    {
    MCESRV_DEBUG("CMceSipSession::CreateOutgoingDialogL( new dlg ), Entry");
    
    __ASSERT_ALWAYS( aDialogType == KMceDlgTypeInvite, 
        User::Leave( KErrArgument ) );

	CSIPInviteDialogAssoc* inviteDialog = NULL;   
    CSIPConnection& connection = SIPConnection().Connection();
    if ( ProfileConfigured() )
	    {
	    const CSIPProfile& profile = Profile();
		TPtrC8 originator = aParams.MdcaPoint( KMceArrayIndexOriginator );
	    TPtrC8 recipient  = aParams.MdcaPoint( KMceArrayIndexRecipient );
	    CSIPToHeader* toHeader = CSIPToHeader::DecodeL( recipient );
	    CleanupStack::PushL( toHeader );
	    CUri8* remoteUri = CUri8::NewLC( toHeader->SIPAddress().Uri8().Uri() );
	    
	    // create targetset with remoteUri of the request
	    AddRemoteUriL( *remoteUri );
	    CSIPFromHeader* fromHeader = NULL;
	    if ( originator.Length() > 0 )
	        {
	        fromHeader = MceSip::ToFromHeaderL( originator );
	        CleanupStack::PushL( fromHeader );
	        }

	    //create dialog
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: originator",originator);
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: recipient",recipient);
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: remote uri",
	        remoteUri->Uri().UriDes() );
	    
	    inviteDialog = 
	      CSIPInviteDialogAssoc::NewL( connection, 
	                                   remoteUri, 
	                                   profile, 
	                                   fromHeader, 
	                                   toHeader );
	    if ( fromHeader )
	        {
	        CleanupStack::Pop( fromHeader );
	        }
	    CleanupStack::Pop( remoteUri );
	    CleanupStack::Pop( toHeader );
	    }
	else
		{
		TPtrC8 originator = aParams.MdcaPoint( KMceArrayIndexOriginator );
	    TPtrC8 recipient  = aParams.MdcaPoint( KMceArrayIndexRecipient );
	    CSIPToHeader* toHeader = CSIPToHeader::DecodeL( recipient );
	    CleanupStack::PushL( toHeader );
	    CUri8* remoteUri = CUri8::NewLC( toHeader->SIPAddress().Uri8().Uri() );
	    
	    // Add remote uri of the request to an array
	    AddRemoteUriL( *remoteUri );
	    
	    CSIPFromHeader* fromHeader = NULL;
	    if ( originator.Length() > 0 )
	        {
	        fromHeader = MceSip::ToFromHeaderL( originator );
	        CleanupStack::PushL( fromHeader );
	        }

	    //create dialog
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: originator",originator);
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: recipient",recipient);
	    MCESRV_DEBUG_SVALUE("CreateOutgoingDialogL: remote uri",
	        remoteUri->Uri().UriDes() );
	    
	    inviteDialog = 
	      CSIPInviteDialogAssoc::NewL( connection,
	      							   fromHeader, 
	                                   remoteUri, 
	                                   toHeader );
	    if ( fromHeader )
	        {
	        CleanupStack::Pop( fromHeader );
	        }
	    CleanupStack::Pop( remoteUri );
	    CleanupStack::Pop( toHeader );
		
		}    
	MCESRV_DEBUG("CMceSipSession::CreateOutgoingDialogL( new dlg ), Exit");
    return inviteDialog;    
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ReCreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
void CMceSipSession::ReCreateOutgoingDialogL()
    {
    RStringF tagT = SIPStrings::StringF( SipStrConsts::ETag );
    MCESRV_DEBUG("CMceSipSession::ReCreateOutgoingDialogL, Entry");
    CSIPClientTransaction& response = Response();
        
    const CSIPResponseElements* respElements = response.ResponseElements(); 
               
    const CSIPToHeader*  toHdr  = respElements->ToHeader();
      
    CSIPToHeader* newToHdr = static_cast<CSIPToHeader *>( toHdr->CloneL() );
   
    newToHdr->DeleteParam( tagT );
    
    CleanupStack::PushL( newToHdr ); 
         
    CUri8* remoteUri = CUri8::NewLC( toHdr->SIPAddress().Uri8().Uri() ); 
    
    CSIPInviteDialogAssoc* newSipBaseDialog = 
        DoCreateOutgoingDialogL( remoteUri, newToHdr );
        
    CleanupStack::Pop( remoteUri );
    CleanupStack::Pop( newToHdr );
                              
    TUint32 oldDialogId = DialogId();
  
    MCE_DELETE( iDialog ); 
    SetDialog( CMceCsSubSession::EOutSession, newSipBaseDialog, oldDialogId );
    
    ResetInitialInvite();
    MCESRV_DEBUG("CMceSipSession::ReCreateOutgoingDialogL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::CreateDialogFrom3XXResponseL
// -----------------------------------------------------------------------------
//
void CMceSipSession::CreateDialogFrom3XXResponseL()
    {
    MCESRV_DEBUG("CMceSipSession::CreateDialogFrom3XXResponseL, Entry");
    TInt count = iRemoteUri.Count(); 
    RDebug::Print( _L("CMceSipSession::CreateDialogFrom3XXResponseL, iRemoteUri.Count()= %d"), count );
    __ASSERT_ALWAYS( count > 0, User::Leave( KErrArgument ) );

    const CUri8* nextUri = iRemoteUri[count-1]; 
   
    // create CSIPToHeader 
    CSIPToHeader* newToHdr = CSIPToHeader::DecodeL( nextUri->Uri().UriDes() );
    // remove tag    
    RStringF tagT = SIPStrings::StringF( SipStrConsts::ETag );
    newToHdr->DeleteParam( tagT );
    CleanupStack::PushL( newToHdr ); 
    
    // create new remote uri
    TUriParser8 uri8Parser;
    User::LeaveIfError( uri8Parser.Parse( nextUri->Uri().UriDes() ) );
    
    CUri8* remoteUri = CUri8::NewLC( uri8Parser );
    
    // create new dialog association
    CSIPInviteDialogAssoc* newSipBaseDialog = 
        DoCreateOutgoingDialogL( remoteUri, newToHdr );
        
    CleanupStack::Pop( remoteUri );
    CleanupStack::Pop( newToHdr );
    
    TUint32 oldDialogId = DialogId();
  
    MCE_DELETE( iDialog ); 
    SetDialog( CMceCsSubSession::EOutSession, newSipBaseDialog, oldDialogId );
    
    ResetInitialInvite();  
    
    if ( Body() && &Body()->SdpSession() )
        {
        Body()->SdpSession().AttachContext( NULL ); // Clear first
        Body()->SdpSession().AttachContext( Dialog() );
        }
    
    MCESRV_DEBUG("CMceSipSession::CreateDialogFrom3XXResponseL, Exit");
    }
  
// -----------------------------------------------------------------------------
// CMceSipSession::ReUseDialog
// -----------------------------------------------------------------------------
//
TInt CMceSipSession::ReUseDialog()
	{
	return iDialog->Dialog().ReuseInitialRequestData();
	}

// -----------------------------------------------------------------------------
// CMceSipSession::DoCreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
CSIPInviteDialogAssoc* CMceSipSession::DoCreateOutgoingDialogL( 
                                                    CUri8* remoteURI, 
                                                    CSIPToHeader* to )
    {
    __ASSERT_ALWAYS( ProfileConfigured(), User::Leave( KErrArgument ) );

    CSIPConnection& connection = SIPConnection().Connection();
    const CSIPProfile& profile = Profile();
          
    return CSIPInviteDialogAssoc::NewL( connection, remoteURI, profile,
    									NULL, to );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Dialog
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipSession::Dialog() const
    {
    CSIPDialogAssocBase* dialog = NULL;
    if ( iForkedDialogs.Count() == 0 )
        {
        dialog = CMceCsSubSession::Dialog();
        }
    else
        {
        dialog = iCurrentDialog;
        }

    return dialog;    
    }
    
    

// -----------------------------------------------------------------------------
// CMceSipSession::Consumes
// -----------------------------------------------------------------------------
//

TBool CMceSipSession::Consumes( CSIPDialog& aDialog, 
   			    				CSIPServerTransaction& aTransaction, 
               	 				TBool& aAssociatesWithDialog )
               	 
	{
    
    TBool consumes = CMceCsSubSession::Consumes( aDialog, aTransaction,
                                                 aAssociatesWithDialog );
    
    if ( !aAssociatesWithDialog && 
         CMceCsSubSession::Dialog()->Dialog() == aDialog ) 
        //dialog might be forked and current dialog is the forked one
        {
        aAssociatesWithDialog = ETrue;
        consumes = AcceptMethod( MceSip::Method( aTransaction ) );
        }
    else if ( !aAssociatesWithDialog )
        {
        //dialog might be forked and current dialog is the original one
        CSIPInviteDialogAssoc* forkedDlg = NULL;
        TInt index = 0;
        while( !aAssociatesWithDialog && index < iForkedDialogs.Count() )
            {
            forkedDlg = iForkedDialogs[ index++ ];
            if ( forkedDlg->Dialog() == aDialog )
                {
                aAssociatesWithDialog = ETrue;
                consumes = AcceptMethod( MceSip::Method( aTransaction ) );
                }
            }
        }
    else
        {
        //NOP
        }

    return consumes;
    
	}               	 


// -----------------------------------------------------------------------------
// CMceSipSession::InviteCompleted
// -----------------------------------------------------------------------------
//
void CMceSipSession::InviteCompleted( CSIPClientTransaction& aTransaction )
    {
    CMceCsSubSession::InviteCompleted( aTransaction );
    iForkedDialogs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::ResponseReceived()
    {
    MCESRV_DEBUG("CMceSipSession::ResponseReceived, Entry");
       
    if ( iCurrentState->Id() == KMceStateEstablished &&
         Response().Type() == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        //Send ACK for signaling integrality
        TRAP_IGNORE( Actions().SendACKL( Response() ) );
        // If the 2xx response in the same dialog that established current session
        // Just reply ACK for it, otherwise, send bye to terminate the dialog
        if ( iDialog != iCurrentDialog )
            {   
            // Send BYE to end forked dialog
            TRAP_IGNORE( Actions().SendBYEL() );
            // Set the original dialog back as the current dialog
            iCurrentDialog = iDialog;
            }        
        }
    else
        {
        if ( Response().Type() == SIPStrings::StringF( SipStrConsts::EInvite ) ||
        		Response().Type() == SIPStrings::StringF( SipStrConsts::EPrack ) )
            {
            // If one of the forked dialogs generated 200 OK response 
            // for a session that is not in established state,
            // replace the actual dialog with that
            HandleForkedDialogs();
            }
        else
            {
            iCurrentDialog = iDialog;
            }
        HandleSIPEvent( EMceResponse, Dialog()->Dialog() );  
        }
    
    MCESRV_DEBUG("CMceSipSession::ResponseReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ProvisionalResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::ProvisionalResponseReceived()
    {
    MCESRV_DEBUG("CMceSipSession::ProvisionalResponseReceived, Entry");
    
    HandleSIPEvent( EMceProvisionalResponse, Dialog()->Dialog() );
    
    MCESRV_DEBUG("CMceSipSession::ProvisionalResponseReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::RedirectionResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::RedirectionResponseReceived()
    {
    MCESRV_DEBUG("CMceSipSession::RedirectionResponseReceived, Entry");
    
    HandleForkedDialogs();
    
    HandleSIPEvent( EMceRedirectionResponse, Dialog()->Dialog());
    
    iForkedDialogs.ResetAndDestroy();
    
    MCESRV_DEBUG("CMceSipSession::RedirectionResponseReceived, Exit");
    }
 
// -----------------------------------------------------------------------------
// CMceSipSession::ErrorResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::ErrorResponseReceived()
    {
    MCESRV_DEBUG("CMceSipSession::ErrorResponseReceived, Entry");
    
    HandleForkedDialogs();

    HandleSIPEvent( EMceErrorResponse, Dialog()->Dialog()  );
    
    iForkedDialogs.ResetAndDestroy();
    
    MCESRV_DEBUG("CMceSipSession::ErrorResponseReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::StandAloneRequestReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::StandAloneRequestReceived()
    {
    MCESRV_DEBUG("CMceSipSession::StandAloneRequestReceived, Entry");
    // Method is acceptable for session, but receiving them outside session
    // is not acceptable. 
    MceSip::DiscardRequest( Request() );
    
    MCESRV_DEBUG("CMceSipSession::StandAloneRequestReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SubState
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetSubState( CMceSipSession::TSubState aSubState )
    {
    MCESRV_DEBUG("CMceSipSession::SetSubState, Entry");
    MCESRV_DEBUG_SUBSTATE("SetSubState: old sub state", iSubState );
    
    iSubState = aSubState;
    
    MCESRV_DEBUG_SUBSTATE("SetSubState: new sub state", iSubState );
    MCESRV_DEBUG("CMceSipSession::SetSubState, Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::SubState
// -----------------------------------------------------------------------------
//
CMceSipSession::TSubState CMceSipSession::SubState()
    {
    return iSubState;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::UpdateAccepted
// -----------------------------------------------------------------------------
//
void CMceSipSession::UpdateAccepted()
    {
    CMceComSession::Delete( iBody, Manager().MediaManager() );
    iBody = BodyCandidate();
    iBody->iCloneOrigin = NULL;
    iNewBodyCandidate = NULL;
    iBody->iFcSignallingRequired = EFalse;
    iBody->iRequireSignalling = EFalse;
    iBody->iPullModeUpdate = EFalse;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::UpdateRejected
// -----------------------------------------------------------------------------
//
void CMceSipSession::UpdateRejected()
    {
    if ( iBody && &iBody->SdpSession() && iBody->SdpSession().iOOldSchool )
        {
        if ( iBody->SdpSession().iOOldSchoolProceeding )  
            {
            iBody->SdpSession().iOOldSchool = 0;
            iBody->SdpSession().iOOldSchoolProceeding = 0;
            iBody->SdpSession().iOOldSchoolCompleted = 1;  
            }
        else
            {
            iBody->SdpSession().iOOldSchoolProceeding = 1; 
            iBody->SdpSession().iOOldSchoolCompleted = 0;    
            }
        }     
    // reset the remoteUri Array if updated failed
    iRemoteUri.ResetAndDestroy();
 
    CMceComSession::Delete( iNewBodyCandidate, Manager().MediaManager() ); 
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SetBodyL
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetBodyL( CMceComSession* aSession )
    {
    __ASSERT_ALWAYS( !iBody, User::Leave( KErrArgument ) );
    TInetAddr nextHop;
    
    TInetAddr localIpAddress = SIPConnection().LocalIPAddressL( NextHopL( nextHop ) );
    
    if ( !aSession )
        {
        iBody = CMceComSession::NewL( CMceComSession::EInSession );
        SetSubState( EAnswering );
        }
    else
        {
        __ASSERT_ALWAYS( aSession->iType == CMceComSession::EOutSession, 
                         User::Leave( KErrArgument ) );
        iBody = aSession;
        SetSubState( EOffering );
        
        }
    
	iBody->SetObserver( *this );
	MCE_SET_LOCAL_IP_ADDR( iBody, localIpAddress );
	iBody->iIapId = SIPConnection().AccessPointId();
	iBody->iDialogId = DialogId();
	
	iExtensions->Update( *iBody );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::PrepareIdsL
// -----------------------------------------------------------------------------
//
void CMceSipSession::PrepareIdsL( TMceIds& aIds ) const
    {    
    aIds.iMsgType = EMceItcMsgTypeSession;
    aIds.iManagerType = KMceCSSIPSession;
    aIds.iDialogType = KMceDlgTypeInvite;
    aIds.iDialogID = DialogId();
    
    if ( ProfileConfigured() )
    	{
		TUint32 profileId;
	    User::LeaveIfError( Profile().GetParameter( KSIPProfileId, profileId ) );
	    
	    aIds.iProfileID = profileId;    	
    	}    
    
	aIds.iSecureFlag =  iBody->iIsSecureSession;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::InviteTransaction
// -----------------------------------------------------------------------------
//
CSIPClientTransaction* CMceSipSession::InviteTransaction()
    {
    CSIPClientTransaction* trx = NULL;
    TInt index = 0;
    
    while( !trx && index < PendingTransactions().Count() )
        {
        trx = PendingTransactions()[ index++ ];
        TBool isInvite = 
            trx->Type() == SIPStrings::StringF( SipStrConsts::EInvite );
        TBool isCompleted = MceSip::TrxCompleted( *trx );
        
        trx = isInvite && !isCompleted ? trx : NULL;
        }

    return trx;
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::StandAloneResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::StandAloneResponseReceived( 
                                        TMceSipResponseType /*aResponseType*/ )
    {
    MCESRV_DEBUG("CMceSipSession::StandAloneResponseReceived, Entry");
    
    //if response to CANCEL
    if ( MceSip::IsResponseTo( Response(), 
         SIPStrings::StringF( SipStrConsts::ECancel ) ) )
        {
        ResponseReceived();
        }
    else
        {
        MCESRV_DEBUG("StandAloneResponseReceived: NOT CANCEL. IGNORED");
        //NOP
        }
        
    MCESRV_DEBUG("CMceSipSession::StandAloneResponseReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipSession::ErrorOccured( TInt aError )
    {
    MCESRV_DEBUG("CMceSipSession::ErrorOccured, Entry")
    
    TUint newState( KMceStateError );
    CMceSession::TState newClientState( CMceSession::ETerminated );
        
    if ( CurrentState().Id() == KMceStateTerminating )
        {
        // Failed to send BYE. ignore and set the state to terminated.
        newState = KMceStateTerminated;
        }
    else if ( ( aError == KErrSIPNoAckReceived && ( SubState() == EAnswering || SubState() == EUpdating ) ) ||
              ( CurrentState().Id() == KMceStateOffering ) ||
              ( CurrentState().Id() == KMceStateConfirming ) )
        {
        if ( Dialog() )
            {
            CSIPDialog::TState dialogState = Dialog()->Dialog().State();
            switch ( dialogState )
                {
                case CSIPDialog::EEarly:
                    {
                    TRAP_IGNORE( Actions().SendCancelL() )
                    newState = KMceStateCanceled;
                    newClientState = CMceSession::ECancelling;
                    break; 
                    }
                case CSIPDialog::EConfirmed:
                    {
                    TRAP_IGNORE( Actions().SendBYEL() )
                    newState = KMceStateTerminating;
                    newClientState = CMceSession::ETerminating;
                    break;
                    }
                default:
                    {
                    newState = KMceStateTerminated;
                    break;
                    }
                }
            }
        }
    
    if ( CurrentState().Id() != KMceStateTerminating )
        {
        Actions().StopMedia();
        FCActions().ReleaseFC();
        }
    
    Actions().StateChanged( newState );
    TRAP_IGNORE( Actions().ClientStateChangedL( newClientState, aError ) )
    MCESRV_DEBUG("CMceSipSession::ErrorOccured, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoErrorOccured( TInt aError, 
                                     CSIPTransactionBase& aTransaction )
    {
	// If Error is for Extension Request    
    if ( Actions().IsExtensionRequest( aTransaction.Type()) )
    	{
    	Actions().ReceiveExtensionError( aTransaction, aError );
    	}
    else
    	{
    	ErrorOccured( aError );    		
    	}
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipSession::LocalErrorOccured( TMceStateIndex aNewState, TInt aWithError )
    {
    MCESRV_DEBUG("CMceSipSession::LocalErrorOccured( state, error), Entry");
    MCESRV_DEBUG_DVALUE("ERROR", aWithError );
    
    if ( aNewState == KMceStateError )
        {
        Actions().StopMedia();
        FCActions().ReleaseFC();
        }
        
    Actions().StateChanged( aNewState );
    Actions().ClientErrorOccured( aWithError );
    
    MCESRV_DEBUG("CMceSipSession::LocalErrorOccured( state, error), Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Terminate
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::Terminate()
    {
    MCESRV_DEBUG( "CMceSipSession::Terminate, Entry" );
    TRAPD( error, DoTerminateL() );
    if ( error )
        {
        MCESRV_DEBUG_DVALUE( "CMceSipSession::Terminate, ERROR:", error );
        Actions().StateChanged( KMceStateTerminated );
        }
    MCESRV_DEBUG( "CMceSipSession::Terminate, Exit" );
    return CanDispose();
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DiscardInvite
// -----------------------------------------------------------------------------
//
void CMceSipSession::DiscardInvite()
    {
    CSIPServerTransaction* invite = FIRST_REQUEST();
    if ( MceSip::TrxType( *invite ) == SipStrConsts::EInvite )
        {
        MceSip::DiscardRequest( *invite );
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoTerminateL
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoTerminateL()
    {
    TMceStateIndex currentState = CurrentState().Id();
    
    MCESRV_DEBUG_STATE("DoTerminateL: current state", currentState );
    
    Actions().StopMedia();
    FCActions().ReleaseFC();
    
    switch( currentState )
        {
        case KMceStateAnswering:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateAnswering -> BYE");
            Actions().SendBYEL();
            //going to still wait ACK or error that ACK not received
            break;
            }
        case KMceStateEstablished:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateEstablished -> BYE");
            Actions().SendBYEL();
            Actions().StateChanged( KMceStateTerminating );
            break;
            }
        case KMceStateError:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateError:");
            
            TMceStateIndex nextState = KMceStateTerminated;
            if ( iExitState )
                {
                TMceStateIndex exitStateId = iExitState->Id();
                MCESRV_DEBUG_STATE("from state", exitStateId );
                
                if ( iExitState->Id() == KMceStateEstablished || 
                     iExitState->Id() == KMceStateAnswering )
                    {
                    MCESRV_DEBUG("DoTerminateL: KMceStateError -> BYE");
                    Actions().SendBYEL();
                    nextState = KMceStateTerminating;
                    }
                }
            Actions().StateChanged( nextState );
            break;
            }
        case KMceStateUpdated:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateUpdated -> DECLINE");
            if ( &Request() )
                {
                CMceMsgSIPData msg;
                Actions().SendDeclineL(Request(), msg );
                }
            Actions().StateChanged( KMceStateTerminated );
            break;
            }
        case KMceStateIdle:
        case KMceStateServerInitializing:
        case KMceStateTerminating:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateUpdated, KMceStateIdle");
            Actions().StateChanged( KMceStateTerminated );
            break;
            }
        case KMceStateOffering:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateOffering");
            if ( SubState() == EOffering )
                {
                CSIPClientTransaction* lastResponse = &Response();
    
                if ( lastResponse && MceSip::TrxCompleted( *lastResponse ) 
                && ( PendingTransaction(SIPStrings::StringF( 
                		SipStrConsts::EInvite )) == NULL ))
                    {
                    // 200 OK already received and waiting for media, bye can be sent
                    MCESRV_DEBUG("DoTerminateL: KMceStateOffering -> BYE");
                    Actions().SendBYEL();
                    Actions().StateChanged( KMceStateTerminating );
                    }
                else
                    {
                    MCESRV_DEBUG("DoTerminateL: KMceStateOffering -> CANCEL");
                    Actions().SendCancelL();
                    Actions().StateChanged( KMceStateCanceled );
                    }
                }
            break;
            }
        case KMceStateServerEstablishing:
            {
            MCESRV_DEBUG("DoTerminateL: KMceStateServerEstablishing -> DECLINE");
            if ( &Request() && Dialog() )
                {
                MceSip::DiscardRequest( Request(), KMceSipDecline );
                if ( Dialog()->Dialog().State() == CSIPDialog::EConfirmed )
                    {
                    Actions().SendBYEL();
                    Actions().StateChanged( KMceStateTerminating );
                    }
                else
                    {
                    Actions().StateChanged( KMceStateTerminated );
                    }
                }
            else
                {
                Actions().StateChanged( KMceStateTerminated );
                }
            break;
            }
        default:
            {
            //NOP
            }
            
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoConnectionStateChanged( TBool aIsActive )
    {
    MCESRV_DEBUG("CMceSipSession::ConnectionStateChanged, Entry");
    
    if ( iBody )
        {
        iBody->iIsConnectionActive = aIsActive;
        }
        
    MCESRV_DEBUG("CMceSipSession::ConnectionStateChanged, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Canceled
// -----------------------------------------------------------------------------
//
void CMceSipSession::Canceled()
    {
    MCESRV_DEBUG("CMceSipSession::Canceled, Entry");
    
    if ( IsSipDialogConfirmed() )
        {
        // cancel the reinvite shouldn't terminate original session.
        Actions().StopUpdatingMedia();
        Actions().UpdateFailed(); 
        Actions().StateChanged( KMceStateEstablished );
        TRAP_IGNORE( Actions().ClientStateChangedL( 
            CMceSession::EEstablished, *Body(), KErrCancel ) );                               
        }
    else
        {
        Actions().StopMedia();
        FCActions().ReleaseFC();
        Actions().StateChanged( KMceStateTerminated );
        TRAP_IGNORE( Actions().ClientStateChangedL( CMceSession::ETerminated ) );
        }
    
    MCESRV_DEBUG("CMceSipSession::Canceled, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoServiceL
// -----------------------------------------------------------------------------
//
HBufC8* CMceSipSession::DoServiceL( TMceIds& aIds, 
                                    TMceItcFunctions aFunction )
    {
    MCESRV_DEBUG("CMceSipSession::DoServiceL( no message ), Entry");
    MCESRV_DEBUG_EVENT("ITC function", aFunction );
    
    HBufC8* returnMessage = NULL;
    
    TMceStateTransitionEvent event( *this, aFunction, aIds );
    if ( iStateMachine->Consumes( event ) )
        {
        MCESRV_DEBUG("handler: state machine");
        iStateMachine->ProcessL( event );
        }
    else
        {
        switch ( aFunction )
            {
            case EMceItcFCGetMediaLines:
            	{
                MCESRV_DEBUG("handler: floor control");
                returnMessage = FCActions().FCGetMediaLinesL();
                break;		    
            	} 
            case EMceItcGetSession:
            	{
                MCESRV_DEBUG("handler: session");
                returnMessage = Actions().BodyToTextL();
                break;		    
            	} 
            //rtp sink
            case EMceItcSendSR:
            case EMceItcSendRR:
            case EMceItcSetFastForward:
            case EMceItcSetFastRewind:
            case EMceItcDisableInactivityTimer:
            //dtmf
            case EMceItcIsDtmfActive:
            case EMceItcStartDtmfTone:
            case EMceItcStopDtmfTone:
            case EMceItcSendDtmfTone:
            case EMceItcCancelSendDtmfToneSequence:
                {
                MCESRV_DEBUG("handler: media");
                returnMessage = Actions().ControlMediaL( aIds, aFunction );
                break;
                }
            default:
                {
                MCESRV_DEBUG("NOT SUPPORTED");
                User::Leave( KErrNotSupported );
                break;
                }
            }
        }
        
    MCESRV_DEBUG("CMceSipSession::DoServiceL( no message ), Exit");
    
    return returnMessage;    
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoServiceL( TMceIds& aIds, 
                                 TMceItcFunctions aFunction, 
                                 const TDesC8& aMessage )
    {
    MCESRV_DEBUG("CMceSipSession::DoServiceL( write data ), Entry");
    MCESRV_DEBUG_EVENT("ITC function", aFunction );
    
    switch ( aFunction )
        {
        case EMceItcInitFC:
        	{
            MCESRV_DEBUG("handler: floor control");
            iUseFC = ETrue;
            FCActions().InitializeFCL ( aIds, aMessage );
            break;		    
        	} 
        case EMceItcUpdateFC:
        	{
            MCESRV_DEBUG("handler: floor control");
            iUseFC = ETrue;
            FCActions().UpdateFCL( aIds, aMessage );
            break;		    
        	} 
        case EMceItcSendFCMsg:
        	{
            MCESRV_DEBUG("handler: floor control");
            FCActions().SendFCMsgL ( aMessage );
            break;		    
        	} 
        //speaker sink
        case EMceItcSetVolume:
        case EMceItcSetAudioRouting:
        //mic source
        case EMceItcSetGain:
        //rtp source
        case EMceItcEnableInactivityTimer:
        //camera source
        case EMceItcSetZoomFactor:
        case EMceItcSetDigitalZoomFactor:
        case EMceItcSetContrast:
        case EMceItcSetBrightness:
        case EMceItcSetExposure:
        case EMceItcSetWhiteBalance:
        //display sink
        case EMceItcSetDisplayIndex:
        case EMceItcSetDisplayPriority:
        case EMceItcSetDisplayRect:
        case EMceItcSetDisplayRotation:
        //file source
        case EMceItcSetFilePosition:
        case EMceItcSetFileInfo:
        //dtmf
        case EMceItcSendDtmfToneSequence:
            {
            MCESRV_DEBUG("handler: media");
            Actions().ControlMediaL( aIds, aFunction, aMessage );
            break;
            }
        default:
            {
            MCESRV_DEBUG("NOT SUPPORTED");
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
    MCESRV_DEBUG("CMceSipSession::DoServiceL( write data ), Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoServiceL( TMceIds& aIds, 
                                 TMceItcFunctions aFunction, 
                                 TPtr8& aMessage )
    {
    MCESRV_DEBUG("CMceSipSession::DoServiceL( read data ), Entry");
    MCESRV_DEBUG_EVENT("ITC function", aFunction );
    
    switch ( aFunction )
        {
        case EMceItcVolume:
        case EMceItcAudioRouting:
        case EMceItcGain:
        case EMceItcZoomFactor:
        case EMceItcDigitalZoomFactor:
        case EMceItcContrast:
        case EMceItcBrightness:
        case EMceItcExposure:
        case EMceItcWhiteBalance:
        case EMceItcDisplayRect:
        case EMceItcDisplayPriority:
        case EMceItcDisplayRotation:
        case EMceItcFileInfo:
        case EMceItcFilePosition:
        case EMceItcSetCameraIndex:
            {
            MCESRV_DEBUG("handler: media");
            Actions().ControlMediaL( aIds, aFunction, aMessage );
            break;
            }
        default:
            {
            MCESRV_DEBUG("NOT SUPPORTED");
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
    MCESRV_DEBUG("CMceSipSession::DoServiceL( read data ), Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipSession::DoServiceL( TMceIds& aIds, 
                                 TMceItcFunctions aFunction, 
                                 CMceMsgBase& aMessage )
    {
    MCESRV_DEBUG("CMceSipSession::DoServiceL( message ), Entry");
    MCESRV_DEBUG_EVENT("ITC function", aFunction );
    

    TMceStateTransitionEvent event( *this, aFunction, aIds, aMessage );
    if ( iStateMachine->Consumes( event ) )
        {
        MCESRV_DEBUG("handler: state machine");
        iStateMachine->ProcessL( event );
        }
    else
        {
        switch ( aFunction )
            {
            case EMceItcFCSetMediaLines:
            	{
                MCESRV_DEBUG("handler: floor control");
                FCActions().FCSetMediaLinesL ( aMessage );
                break;		    
            	} 
            default:
                {
                MCESRV_DEBUG("NOT SUPPORTED");
                User::Leave( KErrNotSupported );
                break;
                }
            }
        }
            
    MCESRV_DEBUG("CMceSipSession::DoServiceL( message ), Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Updated
// -----------------------------------------------------------------------------
// 	       
void CMceSipSession::Updated( CMceComSession& aSession )
    {
    MCESRV_DEBUG("CMceSipSession::Updated, Entry");
    
    __ASSERT_ALWAYS( &aSession == iBody || &aSession == iNewBodyCandidate, 
        User::Panic( KMceServerPanic, KErrArgument ) );
    
    TMceStateTransitionEvent event( *this, EMceMediaUpdated );
    TRAPD( error, iStateMachine->ProcessL( event ) );
    if ( error != KErrNone )
        {
        MCESRV_DEBUG_DVALUE("Updated: state machine causes ERROR", error );
        LocalErrorOccured( KMceStateError, error );
        }
    
    MCESRV_DEBUG("CMceSipSession::Updated, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Reserved
// -----------------------------------------------------------------------------
// 	       
void CMceSipSession::Reserved( CMceComSession& aSession )
    {
    MCESRV_DEBUG("CMceSipSession::Reserved, Entry");
    
    __ASSERT_ALWAYS( &aSession == iBody || &aSession == iNewBodyCandidate, 
        User::Panic( KMceServerPanic, KErrArgument ) );
    
    TMceStateTransitionEvent event( *this, EMceMediaReserved );
    TRAPD( error, iStateMachine->ProcessL( event ) );
    if ( error != KErrNone )
        {
        MCESRV_DEBUG_DVALUE("Reserved: state machine causes ERROR", error );
        LocalErrorOccured( KMceStateError, error );
        }
    
    MCESRV_DEBUG("CMceSipSession::Reserved, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SessionClosed
// -----------------------------------------------------------------------------
//
void CMceSipSession::SessionClosed( CMceComSession& aSession )
    {
    MCESRV_DEBUG("CMceSipSession::SessionClosed, Entry");
    
    __ASSERT_ALWAYS( &aSession == iBody || &aSession == iNewBodyCandidate, 
        User::Panic( KMceServerPanic, KErrArgument ) );
    
    TMceStateTransitionEvent event( *this, EMceMediaSessionStopped );
    TRAPD( error, iStateMachine->ProcessL( event ) );
    if ( error != KErrNone )
        {
        MCESRV_DEBUG_DVALUE(
            "SessionClosed: state machine causes ERROR", error );
        LocalErrorOccured( KMceStateTerminated, error );
        }
        
    MCESRV_DEBUG("CMceSipSession::SessionClosed, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::EventReceived
// -----------------------------------------------------------------------------
//
void CMceSipSession::EventReceived( TMceMccComEvent& aEvent )
    {
    MCESRV_DEBUG("CMceSipSession::EventReceived, Entry");

    if ( aEvent.iItcEvent == EMceItcFCMsgReceived &&
         FCSession() && 
         aEvent.iItcDataDesC )
        {
        HBufC8* data = NULL;
        TRAPD( err, data = aEvent.iItcDataDesC->AllocL() );
        if( err == KErrNone )
            {
            FCReceivedData( data, FCSession() );    
            }
        else
            {
            MCESRV_DEBUG("RtcpSignalReceived, Alloc failed");
            }
        }
    else
        {
        TRAPD( err, Actions().ClientMediaStateChangedL( &aEvent ) );
        if( err != KErrNone )
        	{
            MCESRV_DEBUG("RtcpSignalReceived, ClientMediaStateChanged failed");
            }
        }
        
    MCESRV_DEBUG("CMceSipSession::EventReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::MediaError
// -----------------------------------------------------------------------------
//
void CMceSipSession::MediaError( TMceMccComEvent& aEvent )
    {
#if MCE_IGNORE_MEDIA_ERROR

    if ( aEvent.iError < 0 )
        {
        MCESRV_DEBUG_DVALUE("error", aEvent.iError );
        MCESRV_DEBUG("CMceSipSession::MediaError, Exit");
        return;
        }

#else

    TMceStateTransitionEvent event( *this, aEvent, EMceMediaError );
	CMceState& currentState = CurrentState();
	currentState.MediaErrorOccured( event );
	   
#endif
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::SessionCreatedL
// -----------------------------------------------------------------------------
//
void CMceSipSession::SessionCreatedL( CMceComSession* aSession )
    {
    MCESRV_DEBUG("CMceSipSession::SessionCreated, Entry");
    
    __ASSERT_ALWAYS( iBody, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSession, User::Leave( KErrArgument ) );
    aSession->SdpSession().AttachContext( Dialog() );
    
    iBodyBucket.AppendL( iBody );
    iBody = aSession;

    MCESRV_DEBUG("CMceSipSession::SessionCreated, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CMceSipSession::TimerExpiredL( TMceTimerId aTimerId, TAny* /*aTimerParam*/ )
    {
    MCESRV_DEBUG("CMceSipSession::TimerExpiredL, Entry");
    TMceStateIndex currentState = CurrentState().Id();
    
    if( aTimerId == iTimer && iBody->iRefresh )
    	{
        TMceStateTransitionEvent event( *this, EMceSessionRefresh );
        TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
                "TimerExpiredL: state machine causes ERROR", error );
            LocalErrorOccured( currentState, error );
            }
    	}
    else if( aTimerId == iPendingTimer )
        {
        TMceStateTransitionEvent event( *this, EMceSessionPending );
        TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
                "TimerExpiredL: state machine causes ERROR", error );
            LocalErrorOccured( currentState, error );
            }
        }
    else
    	{
    	// session has expired, terminate
    	TMceStateTransitionEvent event( *this, EMceSessionExpired );
    	TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
                "TimerExpiredL: state machine causes ERROR", error );
	        LocalErrorOccured( KMceStateTerminated, error );
            }
    	}
    	
    if ( aTimerId == iTimer )
        {
        iTimer = CMceTimerManager::KNoSuchTimer;
        }
        
    MCESRV_DEBUG("CMceSipSession::TimerExpiredL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::UseFC
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::UseFC()
    {
    return iUseFC;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::FCReceivedData
// -----------------------------------------------------------------------------
//
void CMceSipSession::FCReceivedData( HBufC8* aData, 
                                     const CFCSession* /*aFCSession*/ )
    {

    MCESRV_DEBUG("CMceSipSession::FCReceivedData, Entry");
    
    TMceIds ids;
    ids.iCallbackID = EMceItcFCMsgReceived;
	            
    TRAPD( error, SendToClientL( ids, aData, NULL ) );
    if ( error != KErrNone )
        {
        delete aData;
        }

    MCESRV_DEBUG("CMceSipSession::FCReceivedData, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipSession::FCErrorNotify
// -----------------------------------------------------------------------------
//
void CMceSipSession::FCErrorNotify( TInt aErrCode, 
                                    const CFCSession* /*aFCSession*/ )
    {
    TMceIds ids;
    ids.iCallbackID = EMceItcFCErrorReceived;
    ids.iStatus = aErrCode;
    SendToClient( ids );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::InviteForkedL
// -----------------------------------------------------------------------------
//
void CMceSipSession::InviteForkedL( CSIPClientTransaction& aTransaction,
          		                    CSIPInviteDialogAssoc* aDialogAssoc )
    {
    __ASSERT_ALWAYS( aDialogAssoc, User::Leave( KErrArgument ) );
    iForkedDialogs.AppendL( aDialogAssoc );
    ResponseReceivedWithinDialog( aTransaction, *aDialogAssoc );
    }

// -----------------------------------------------------------------------------
// CMceSipSession::HandleForkedDialogs
// -----------------------------------------------------------------------------
//
void CMceSipSession::HandleForkedDialogs()
    {
    
    // If one of the forked dialogs generated final response,
    // replace the actual dialog with that
    CSIPInviteDialogAssoc* forkedDlg = NULL;
    CSIPDialog::TState forkedDlgState = ( CSIPDialog::TState )( KErrNotFound );
    for ( TInt ind = 0; ind < iForkedDialogs.Count(); ind++ )
        {
        forkedDlg = iForkedDialogs[ ind ];
        forkedDlgState = forkedDlg->Dialog().State();
        if ( forkedDlgState == CSIPDialog::EConfirmed || 
             forkedDlgState == CSIPDialog::ETerminated )
            {
            CSIPInviteDialogAssoc* origDlg = static_cast<CSIPInviteDialogAssoc*>( iDialog );
            iDialog = forkedDlg;
            iForkedDialogs.Remove( ind );
            iForkedDialogs.Insert( origDlg,ind );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::WaitingMediaCallback
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::WaitingMediaCallback() const
    {
    return iWaitingMediaCallback;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SetWaitingMediaCallback
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetWaitingMediaCallback( TBool aWaiting )
    {
    iWaitingMediaCallback = aWaiting;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::SetMediaAuthorizationToken
// -----------------------------------------------------------------------------
//
void CMceSipSession::SetMediaAuthorizationToken( HBufC8* aToken )
    {
    delete iMediaAuthorizationToken;
    iMediaAuthorizationToken = aToken;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::Extensions
// -----------------------------------------------------------------------------
//
CMceSipExtensions& CMceSipSession::Extensions()
    {
    return *iExtensions;
    }

// -----------------------------------------------------------------------------
// CMceSipSession::MediaAuthorizationToken
// -----------------------------------------------------------------------------
//
HBufC8* CMceSipSession::MediaAuthorizationToken() const
    {
    return iMediaAuthorizationToken;    
    }

// -------------------------------------------------------------------------------
// CMceSipSession::NoPrackReceived
// -------------------------------------------------------------------------------
//
void CMceSipSession::NoPrackReceived( CSIPServerTransaction& aTransaction )
    {
    if ( Request() == aTransaction )
        {
        MCESRV_DEBUG("CMceSipSession::NoPrackReceived");
        TMceStateTransitionEvent event( *this, EMceNoPrackReceived );
        
        TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
            "SessionClosed: state machine causes ERROR", error );
            LocalErrorOccured( KMceStateError, error );
            }
        MCESRV_DEBUG("CMceSipSession::NoPrackReceived, Exit");              
        }
    }

// -------------------------------------------------------------------------------
// CMceSipSession::ReliableFailed
// -------------------------------------------------------------------------------
//
void CMceSipSession::ReliableFailed( CSIPServerTransaction& aTransaction,
									 TInt aError )
	{
    MCESRV_DEBUG("CMceSipSession::ReliableFailed, Entry");
    MCESRV_DEBUG_DVALUE("error", aError );
    
    if ( Request() == aTransaction )
        {
        TMceStateTransitionEvent event( *this, EMceReliableFailed, aError );
     
        TRAPD( error, iStateMachine->ProcessL( event ) );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE(
            "SessionClosed: state machine causes ERROR", error );
            LocalErrorOccured( KMceStateError, error );
            }
        MCESRV_DEBUG("CMceSipSession::ReliableFailed, Exit");       
        }              
    }
    
// -------------------------------------------------------------------------------
// CMceSipSession::SetRSeq 
// -------------------------------------------------------------------------------
//
void CMceSipSession::SetRSeq( TUint aRSeq )
    {
    if ( aRSeq )
        {
        iRSeq = aRSeq;        
        }
    }

// -------------------------------------------------------------------------------
// CMceSipSession::RSeq  
// -------------------------------------------------------------------------------
//
TInt CMceSipSession::RSeq()
    {
    return iRSeq;
    }

// -------------------------------------------------------------------------------
// CMceSipSession::DoAutoEventL  
// -------------------------------------------------------------------------------
//    
void CMceSipSession::DoAutoEventL( TMceStateTransitionEvent& aEvent )
    {         
    CMceMsgBase* msg = &aEvent.ParamClientMessage();
    TMceIds emptyIds;
    TMceIds* ids = &aEvent.ParamIDs();
    ids = ids != NULL ? ids : &emptyIds;
    if ( msg )
        {
        DoServiceL( *ids, static_cast<TMceItcFunctions>( aEvent.Code() ), *msg );
        }
    else
        {
        DoServiceL( *ids, static_cast<TMceItcFunctions>( aEvent.Code() ) );
        }
    }
 
// -------------------------------------------------------------------------------
// CMceSipSession::AddRemoteUriL  
// -------------------------------------------------------------------------------
//    
void CMceSipSession::AddRemoteUriL( const CUri8& aRemoteUri )  
    {
    MCESRV_DEBUG("CMceSipSession::AddRemoveUriL, Entry");       
   
    TInt uriFound = FindRemoteUri( aRemoteUri );
    
    User::LeaveIfError( uriFound == KErrNotFound ? KErrNone : KErrAlreadyExists );
   
    CUri8* remoteUri = CUri8::NewLC( aRemoteUri.Uri() );
    iRemoteUri.AppendL( remoteUri ); 	
    CleanupStack::Pop();   
    RDebug::Print(_L("CMceSipSession::AddRemoveUriL, iRemoteUri.Count()=%d"),iRemoteUri.Count() );
	MCESRV_DEBUG("CMceSipSession::AddRemoveUriL, Exit");       
    }

// -------------------------------------------------------------------------------
// CMceSipSession::FindRemoteUri  
// -------------------------------------------------------------------------------
// 
TInt CMceSipSession::FindRemoteUri( const CUri8& aRemoteUri )
    {
    MCESRV_DEBUG("CMceSipSession::FindRemoteUri, Entry"); 
          
    TInt ret = KErrNotFound;
    
    for( TInt i=0; ( i<iRemoteUri.Count() ) && ( ret==KErrNotFound ); i++ )
        {
        if( MceSip::CompareUri( aRemoteUri, *iRemoteUri[i] ) == KErrNone )
            {
            ret = i; 
            RDebug::Print(_L("CMceSipSession::FindRemoteUri,i=%d"), i);    
            }
        }
        
    MCESRV_DEBUG("CMceSipSession::FindRemoteUri, Exit"); 
          
    return ret;
    }

// -------------------------------------------------------------------------------
// CMceSipSession::AddRemoteUriFromResponseL
// -------------------------------------------------------------------------------
//  
void CMceSipSession::AddRemoteUriFromResponseL( 
    CSIPResponseElements& aResponseElements )
    {
    MCESRV_DEBUG("CMceSipSession::AddRemoteUriFromResponseL, Entry");       

   	CSIPHeaderBase* contact = MceSip::FindHeader( 
   	    aResponseElements.MessageElements(), 
   	    SIPStrings::StringF( SipStrConsts::EContactHeader ) );
	
    User::LeaveIfError( contact ? KErrNone : KErrNotFound );

    const CUri8& uri = static_cast< CSIPContactHeader* >( contact )->
							SIPAddress()->Uri8();
							
    AddRemoteUriL( uri );                            
    
    MCESRV_DEBUG("CMceSipSession::AddRemoteUriFromResponseL, Exit");       
    }

// -------------------------------------------------------------------------------
// CMceSipSession::ClearRemoteUri
// -------------------------------------------------------------------------------
// 
void CMceSipSession::ClearRemoteUri( const TMceStateIndex aState )
    {
    MCESRV_DEBUG("CMceSipSession::ClearRemoteUri, Entry");       

    if ( aState == KMceStateEstablished || 
         aState == KMceStateTerminated ||
         aState == KMceStateUpdated )
        {
        RDebug::Print(_L("CMceSipSession::ClearRemoteUri, aState=%d"), aState);       
        RDebug::Print(_L("CMceSipSession::ClearRemoteUri, iRemoteUri.Count=%d"),
        			  iRemoteUri.Count());       

        iRemoteUri.ResetAndDestroy(); 
        }
    MCESRV_DEBUG("CMceSipSession::ClearRemoteUri, Exit");       
    }

// -----------------------------------------------------------------------------
// CMceSipSession::ForceSDPSendingStatus
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::ForceSDPSendingStatus()
	{
	return 	iForceSDPSending;
	}

// -----------------------------------------------------------------------------
// CMceSipSession::PrevOfferSafeDelete
// -----------------------------------------------------------------------------
//
void CMceSipSession::PrevOfferSafeDelete()
    {
    if ( iPrevOffer && iPrevOffer != iOffer )
	    {
	    MCE_DELETE( iPrevOffer );
	    }
    }

// -----------------------------------------------------------------------------
// CMceSipSession::PreviousState
// -----------------------------------------------------------------------------
//
TUint CMceSipSession::PreviousState()    
	{
	return iPreviousState;	
	}   

// -----------------------------------------------------------------------------
// CMceSipSession::ServerInitializedL
// -----------------------------------------------------------------------------
//
void CMceSipSession::ServerInitializedL()
	{
	TMceStateTransitionEvent event( *this, EMceServerInitialized );
    TRAPD( error, iStateMachine->ProcessL( event ) );
    if ( error != KErrNone )
        {
        MCESRV_DEBUG_DVALUE( "ServerInitializedL: state machine causes ERROR",
        					 error );
        LocalErrorOccured( KMceStateError, error );
        }
	}

// -----------------------------------------------------------------------------
// CMceSipSession::ContinueHandlingIncomingSessionL
// -----------------------------------------------------------------------------
//
void CMceSipSession::ContinueHandlingIncomingSessionL()
	{
	FCActions().PeekFCDocumentL( *Offer() );
    Actions().ClientUpdateIncomingSessionL( Request() );
	}
	
// -----------------------------------------------------------------------------
// CMceSipSession::IsSipDialogConfirmed
// -----------------------------------------------------------------------------
//	
TBool CMceSipSession::IsSipDialogConfirmed() const
    {
    TBool confirmed( EFalse );
    if ( Dialog() )
        {
        confirmed = ( Dialog()->Dialog().State() == CSIPDialog::EConfirmed );
        }
    return confirmed;
    }
    
// -----------------------------------------------------------------------------
// CMceSipSession::IsNatSession
// -----------------------------------------------------------------------------
//
TBool CMceSipSession::IsNatSession()
    {
    return iNatSession;
    }
	
TInt CMceSipSession::ForkedDialogsCount()
	{
	return iForkedDialogs.Count();
	}
// -----------------------------------------------------------------------------
// CMceSipSession::ForceUpdateStreamL
// -----------------------------------------------------------------------------
//
void CMceSipSession::ForceUpdateStreamL()
	{
	MCESRV_DEBUG("CMceSipSession::ForceUpdateStream, Entry");    
	if( iBody->SdpSession().Context() == Dialog() )
		{
		Manager().MediaManager().ForceEnableSinkState( ActiveBody() );
		}
	else
		{
		ActiveBody().SdpSession().UpdateSecureStreamL( ActiveBody() );
		Manager().MediaManager().UpDateStreamStateL( *iBody, ActiveBody() );
		__ASSERT_ALWAYS( iFirstOffer, User::Leave( KErrArgument ) );
		ActiveBody().SdpSession().iSdpDocument = iFirstOffer;
		}
	MCESRV_DEBUG("CMceSipSession::ForceUpdateStream, Exit");    
	}
// -----------------------------------------------------------------------------
// CMceSipSession::ResetCurrentDialog
// -----------------------------------------------------------------------------
//
void CMceSipSession::ResetCurrentDialog()
	{
	if( iBodyBucket.Count() == 0 )
		{
		iCurrentDialog = iDialog;
		}
	}
void CMceSipSession::SetFirstOffer()
	{	
	if( iBodyBucket.Count() == 0 )
		{
		CSdpDocument* clonedOffer = NULL;
		TRAP_IGNORE( ( clonedOffer = iOffer->CloneL() ) );
		iFirstOffer = clonedOffer;
		}
	}
//  End of File
