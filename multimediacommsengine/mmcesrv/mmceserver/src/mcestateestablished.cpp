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




#include <sipdialogassocbase.h> 
#include <sipdialog.h>			
#include "mcestateestablished.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mcenatmacros.h"
#include "mcesdpsession.h"
#include "mcesrvlogs.h"
#include "mcenatsipsession.h"

// -----------------------------------------------------------------------------
// CMceStateEstablished::CMceStateEstablished
// -----------------------------------------------------------------------------
//
CMceStateEstablished::CMceStateEstablished ()
    : CMceState( KMceStateEstablished )
	{
	}

// -----------------------------------------------------------------------------
// CMceStateEstablished::~CMceStateEstablished
// -----------------------------------------------------------------------------
//
CMceStateEstablished::~CMceStateEstablished()
	{
	}

// -----------------------------------------------------------------------------
// CMceStateEstablished::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateEstablished::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	if ( !IsExtensionRequestEvent( aEvent ) &&
		 aEvent.Code() == EMceResponse && 
	     MceSip::ResponseCode( aEvent.Session().Response() ) == KMceSipOK )
		{
	    return EFalse;		
		}

    User::LeaveIfError( 	                 
       aEvent.Code() == EMceItcTerminateSession  ||
       aEvent.Code() == EMceItcUpdate  ||
       aEvent.Code() == EMceItcEnable  ||
       aEvent.Code() == EMceItcDisable  ||
       aEvent.Code() == EMceMediaSessionStopped  ||
       aEvent.Code() == EMceInvite  ||
       aEvent.Code() == EMceUpdate ||
       aEvent.Code() == EMceBye ||
       aEvent.Code() == EMceSessionExpired ||
       aEvent.Code() == EMceSessionRefresh ||
       aEvent.Code() == EMceSessionPending ||
       aEvent.Code() == EMceErrorResponse || // 487 for CANCEL
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
           
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateEstablished::StateEntry
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::StateEntry( TMceStateTransitionEvent& aEvent )
    {
    CMceState::StateEntry( aEvent );
    
	CMceSipSession& session = aEvent.Session();
    
    TBool clearSubState = ETrue;
    
    if ( &session.Response() )
        {
        TUint code = MceSip::ResponseCode( session.Response() );
        if ( code != KMceSipRequestPending )
            {
            session.Actions().StopPendingTimer();
            }
        else
            {
            clearSubState = EFalse;
            }
        }
    if ( clearSubState )
        {
        session.SetSubState( CMceSipSession::ENone );
        }
        
    session.CloseBodyBucket();

    session.RemoveCompletedRequests( ETrue );
    
    session.ResetSdpCounts();
    
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
	
	else 
		{

		CMceSipSession& session = aEvent.Session();

	    switch ( aEvent.Code() )
	        {
	        //client has been asked to update session
	        case EMceItcUpdate:
	            {
	            EntryUpdateL( aEvent );
	            break;
	            }
	        //INVITE has been received due session update or refresh
	        case EMceInvite:
	            {
	            EntryInviteL( aEvent );
	            break;
	            }
	        //UPDATE has been received for instance for session expires
	        // or media update
	        case EMceUpdate:
	            {
                EntryInUpdateL( aEvent );
                break;
	            }
	        //client terminates session or session timer expires, 
	        //when session is not refresher BYE is sent and media 
	        //and FC are stopped
	        case EMceSessionExpired:
	        case EMceItcTerminateSession:
	            {
	            EntryTerminateSessionL( aEvent );
	            break;
	            }
	        //BYE has been received
	        case EMceBye:
	            {
	            EntryByeL( aEvent );
	            break;
	            }
	        //media conrol
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            EntryMediaL( aEvent );
	            break;
	            }
	        //refresher's session timer fired
	        //and thus, refresh is launched (INVITE)
	        case EMceSessionRefresh:
	            {
                session.Actions().EncodeL();
                if ( session.IsNatSession() ) 
                    {
                    CMceNatSipSession* natSession = static_cast<CMceNatSipSession*>( &session );
                    natSession->CreateOfferL();
                    }
	            session.FCActions().UpdateFCOfferL( *session.Offer() );
	            session.Actions().SendSessionRefreshL();
	            break;
	            }
	        //pending timer fired, re-send re-INVITE
	        case EMceSessionPending:
	            {
	            session.Actions().SendInviteL();
	            break;
	            }
	        //media has beed stopped or there is media error
	        //session state is not affected, client is only informed
	        case EMceMediaSessionStopped:
	           	{
	           	MediaErrorOccured( aEvent );
	           	break;
	           	}
	              
	        default:
	            {
	            //NOP
	            break;
	            }
	        }
		}
	         
	}

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryUpdateL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryUpdateL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
    
    // create body candidate
    session.Actions().CreateBodyCandidateL( CMceComSession::EOutSession );
    // update body candidate
    session.Actions().UpdateBodyL( aEvent.ParamIDs(), 
                                   aEvent.ParamClientMessage() );
    session.BodyCandidate()->iFcSignallingRequired = 
                                    session.FCActions().NeedToSendOffer();

    if ( &session.Body()->SdpSession() &&
    	 session.Body()->SdpSession().iOOldSchoolCompleted )
        {
        session.BodyCandidate()->iRequireSignalling = ETrue;
        session.Body()->SdpSession().iOOldSchoolCompleted = 0;
        }
        
    //create updated session (body) candidate
    TMceReturnStatus status = session.Actions().UpdateL();
                              
    aEvent.ParamStatus() = status;
    //if creation fails candidate is rolled back to original
    if ( MCE_IS_ERROR( status ) )
        {
        session.Actions().UpdateFailed();
        }
    else if ( status == KMceReady )
        {
        if ( MCE_NEED_TO_SEND( session, NULL ) )
            {
            NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
            session.FCActions().UpdateFCOfferL( *session.Offer() );
            session.Actions().SendInviteL();
            
            session.Actions().StateChanged( KMceStateOffering );
            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
                                                  CMceSession::EOffering );
            }
        else
            {
            session.Actions().UpdateSucceed();
            }
        }
    else
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryInUpdateL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryInUpdateL(TMceStateTransitionEvent& aEvent)
    {
    CMceSipSession& session = aEvent.Session();
    CSIPServerTransaction& request = session.Request();
    CSIPServerTransaction* requestPtr = &request;
    TBool isRefreshing = EFalse;

    session.Actions().CheckContactIsSecureL(request);

    if ( (requestPtr!=NULL) && !MceSip::HasContent(request) )
        {
        isRefreshing = ETrue;
        }
    else
        {
        // update extension information
        session.Extensions().UpdateL(session.Request() );
        // create SDP
        User::LeaveIfError(session.Actions().CreateSDP(session.Request() ) );

        if ( !session.DoesMediaManagerNeedToNegotiate( session.Offer() ) )
            {
            // SDP not changed
            isRefreshing = ETrue;
            }
        }
    
    if (isRefreshing)
        {
        //set substate to refreshing
        session.SetSubState(CMceSipSession::ERefreshing);
        //update timer data in body
        session.Actions().ProcessSessionTimerServerL(session.Request() );

        // Use previous offer
        session.SdpCleanup(session.Offer(), session.PrevOffer() );

        // encode
        session.Actions().EncodeL();

        //update FC in answer
        session.FCActions().UpdateFCAnswerL( *session.Offer() );

        //reset session timer if session is not refresher
        session.Actions().ResetSessionTimerL();
        session.Actions().SendAnswerL(session.Request() );
        
        session.Actions().StateChanged( KMceStateEstablished );
        session.Actions().ClientStateChangedL( CMceSession::EEstablished, session.Request() );  
        }
    else
        {
        // create body candidate
        session.Actions().CreateBodyCandidateL(CMceComSession::EInSession);

        TMceReturnStatus status = KErrNone;

        // decode 
        TMceSipWarningCode warning = session.Actions().DecodeL();

        if ( !warning)
            {
            status = session.Actions().Update();
            if (status == KErrNotSupported)
                {
                warning = KMceSipWarnSessionDescriptionParameterNotUnderstood;
                }
            }

        aEvent.ParamStatus() = status;

        if (warning)
            {
            session.Actions().UpdateFailed();
            session.Actions().SendRejectOfferWithWarningL(warning, session.Request() );
            aEvent.ParamStatus() = warning;
            }
        else
            if ( MCE_IS_ERROR( status ))
                {
                session.Actions().UpdateFailed();
                User::Leave(status);
                }
            else
                {
                session.Actions().ClientUpdateSessionL(session.Request() );
                }
        }
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryInviteL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryInviteL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	CSIPServerTransaction& request = session.Request();
	CSIPServerTransaction* requestPtr = &request;

    session.Actions().CheckContactIsSecureL( request );
     
	if ( (requestPtr!=NULL) && !MceSip::HasContent( request ) )
        {
        // Pull Mode
        session.Extensions().UpdateL( request );
        if ( MCE_NEED_TO_SEND( session, session.Offer() ) ) // is session update
            {
            // create body candidate for Pull mode
            session.Actions().CreateBodyCandidateL( 
                CMceComSession::EInSession );
            }
        session.Actions().ProcessSessionTimerServerL( session.Request() );
        session.Actions().ClientUpdateSessionL( request );	  
        }
	else
	    {
	    // update extension information
	    session.Extensions().UpdateL( session.Request() );
        // create SDP
        User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );

        if ( session.DoesMediaManagerNeedToNegotiate( session.Offer() ) )
            {
            // create body candidate
            session.Actions().CreateBodyCandidateL( CMceComSession::EInSession );
            
            TMceReturnStatus status = KErrNone;
            
            // decode 
            TMceSipWarningCode warning = session.Actions().DecodeL();
            
            if ( !warning )
                {
                status = session.Actions().Update();
                if ( status == KErrNotSupported )
                    {
                    warning = KMceSipWarnSessionDescriptionParameterNotUnderstood;
                    }
                }
            
            aEvent.ParamStatus() = status;

            if ( warning )
                {
                session.Actions().UpdateFailed();
                session.Actions().SendRejectOfferWithWarningL( warning, 
                                                               session.Request() );
                aEvent.ParamStatus() = warning;
                }
            else if ( MCE_IS_ERROR( status ) )
                {
                session.Actions().UpdateFailed();
                User::Leave( status );
                }
            else
                {
                session.Actions().ClientUpdateSessionL( session.Request() );
                }
            }
        else // SDP not changed
            {
            //set substate to refreshing
            session.SetSubState( CMceSipSession::ERefreshing );
            //update timer data in body
            session.Actions().ProcessSessionTimerServerL( session.Request() );
            
            // Use previous offer
            session.SdpCleanup( session.Offer(), session.PrevOffer() );
           
            // encode
            session.Actions().EncodeL();
            
            //update FC in answer
            session.FCActions().UpdateFCAnswerL( *session.Offer() );
            
            session.Actions().SendAnswerL( session.Request() );
            
            session.Actions().StateChanged( KMceStateAnswering );
            session.Actions().ClientStateChangedL( CMceSession::EAnswering,
                                                   session.Request() );
            }
	    }
	//reset session timer
	session.Actions().ResetSessionTimerL();
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryTerminateSessionL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryTerminateSessionL( 
                                            TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();

    session.Actions().SendBYEL( aEvent.ParamClientMessage() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryByeL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryByeL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	
    //send 200OK and stop media and FC
    session.Actions().Send200OKL( session.Request() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
    }

// -----------------------------------------------------------------------------
// CMceStateEstablished::EntryMediaL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::EntryMediaL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();

    session.Actions().ControlMediaL( aEvent.ParamIDs(), 
                                    (TMceItcFunctions)aEvent.Code() );
    }
    
    
    
// -----------------------------------------------------------------------------
// CMceStateEstablished::MediaErrorOccured
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::MediaErrorOccured( TMceStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "CMceStateEstablished::MediaErrorOccured, Entry" );                           	
	CMceSipSession& session = aEvent.Session();
	
	// If ClientMediaStateChangedL leaves while trying to pause the RTP sink,
	// just trap and log it.
	TRAPD( err, session.Actions().ClientMediaStateChangedL( 
                           aEvent.ParamMccEvent(),
                           static_cast<TMceMediaEventCode>( aEvent.Code() ) ) );
	
	if ( err != KErrNone )
		{
		MCESRV_DEBUG_DVALUE( "ClientMediaStateChangedL leaves with error code", err );
		}
	
	MCESRV_DEBUG( "CMceStateEstablished::MediaErrorOccured, Exit" );                           
	}


// -----------------------------------------------------------------------------
// CMceStateEstablished::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateEstablished::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}

	else 
		{
		CMceSipSession& session = aEvent.Session();
		TInt status = aEvent.ParamStatus();

	    switch ( aEvent.Code() )
	        {
	        //received BYE causes server & client to terminated state
	        case EMceBye:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChangedL( 
	                CMceSession::ETerminated, session.Request() );
	            break;
	            }
	        //client's session termination causes 
	        //server & client to terminating state
	        case EMceItcTerminateSession:
	            {
	            session.Actions().StateChanged( KMceStateTerminating );
	            session.Actions().ClientStateChanged( 
	                aEvent.ParamIDs(), CMceSession::ETerminating );
	            break;
	            }
	        //timer expiration causes server & client to terminating state
	        case EMceSessionExpired:
	            {
	            session.Actions().StateChanged( KMceStateTerminating );
	            session.Actions().ClientStateChangedL( CMceSession::ETerminating );
	            break;
	            }
	        //session refresh causes server & client to terminating state
	        case EMceSessionRefresh:
	            {
	            session.Actions().StateChanged( KMceStateOffering );
	            session.Actions().ClientStateChangedL( CMceSession::EOffering );
	            break;
	            }
	        case EMceSessionPending:
	            {
	            session.Actions().StateChanged( KMceStateOffering );
	            session.Actions().ClientStateChangedL( CMceSession::EOffering );
	            break;
	            }
	       
	        case EMceItcUpdate:
	            {
	            //client's update,if decoding has been failed 
	            //or update is not deeded, does not affect 
	            if ( MCE_IS_ERROR( status ) )
	                {
	                //Client will fetch original session with ITC
	                session.Actions().StateChanged( KMceStateEstablished );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EEstablished, 
	                                                      status );
	                }
	            else if ( status == KMceAsync )
	                {
	                session.Actions().StateChanged( KMceStateClientEstablishing );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EOffering );
	                }
	            else if ( status == KMceReady )
	                {
	                session.Actions().StateChanged( KMceStateEstablished );
	                session.Actions().ClientStateChanged( 
	                                                aEvent.ParamIDs(), 
	                                                CMceSession::EEstablished );
	                }
	            else
	                {
	                // NOP
	                }
	            break;
	            }
            case EMceUpdate:
                {
                if ( session.SubState() == CMceSipSession::ERefreshing )
                    {
                    session.Actions().StateChanged(KMceStateEstablished);

                    // If warning occurred, client is not informed
                    if ( MCE_IS_ERROR( status ))
                        {
                        session.Actions().ClientStateChangedL(CMceSession::EEstablished, status);
                        }
                    }
                else
                    {
                    session.Actions().StateChanged( KMceStateServerEstablishing );
	                }
	            break;
	            }
	        case EMceInvite:
	            {
	            if ( status == KErrNone )
	    	        {
	                session.Actions().StateChanged( KMceStateServerEstablishing );
	    	        }
	            else
	                {
	                session.Actions().StateChanged( KMceStateEstablished );
	                
	                // If warning occurred, client is not informed
	                if ( MCE_IS_ERROR( status ) )
	                    {
	                    session.Actions().ClientStateChangedL( 
	                                    CMceSession::EEstablished, status );
	                    }
	                }
	            break;
	            }
	        default:
	            {
	            }
	        }
		}
	}

// End of File
