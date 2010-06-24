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
#include <sipclienttransaction.h>
#include <sipresponseelements.h>
#include <sipcseqheader.h>
#include "mcestateoffering.h"
#include "mcesipsession.h"
#include "mcesipextensions.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcesip.h"
#include "mcesipsession.h"
#include "mcenatmacros.h"
#include "mcesdpsession.h"
#include "mcesrvlogs.h"
#include "mcemediastate.h"
#include "mcemediadefs.h"
#include "mcemediamanager.h"

// -----------------------------------------------------------------------------
// CMceStateOffering::CMceStateOffering
// -----------------------------------------------------------------------------
//
CMceStateOffering::CMceStateOffering ()
    : CMceState( KMceStateOffering ),
      iLastResponse ( 0 ),
	  iReadyToSendACK ( ETrue )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateOffering::~CMceStateOffering
// -----------------------------------------------------------------------------
//
CMceStateOffering::~CMceStateOffering()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateOffering::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateOffering::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    User::LeaveIfError( 	                 
       aEvent.Code() == EMceUpdate ||
       aEvent.Code() == EMceResponse  ||
       aEvent.Code() == EMceProvisionalResponse ||
       aEvent.Code() == EMceRedirectionResponse ||
       aEvent.Code() == EMceErrorResponse ||
       aEvent.Code() == EMceMediaUpdated ||
       aEvent.Code() == EMceItcCancel ||    
       aEvent.Code() == EMceBye ||  
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable || 
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
           
	return ETrue;
	
	}

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
	
    else
    	{
	    switch( aEvent.Code() )
	        {
            case EMceUpdate:
                {
                EntryUpdateL( aEvent );
                break;
                }
	        case EMceProvisionalResponse:
	            {
	            EntryProvisionalResponseL( aEvent );
	            break;
	            }
	        //200 OK
	        case EMceResponse:
	            {
	            EntryResponseL( aEvent );
	            break;                
	            }
	        //error
	        case EMceRedirectionResponse:
	            {
	            EntryRedirectionResponseL( aEvent );
	            break;   
	            }
	        case EMceErrorResponse:
	            {
	            EntryErrorResponseL( aEvent );
	            break;                
	            }
	        //if client cancels, send CANCEL            
	        case EMceItcCancel:
	            {
	            EntryCancelL( aEvent );
	            break;
	            }
	        case EMceMediaUpdated:
	            {
                CMceSipSession& session = aEvent.Session();
                CSIPServerTransaction* update = &session.Request();

                if ( update && update->Type() == 
                    SIPStrings::StringF( SipStrConsts::EUpdate ) )
                    {
                    EntryMediaUpdatedL( aEvent );
                    }
                else
                    {
                    if ( aEvent.Session().SubState() == CMceSipSession::EUpdating )
                        {
                        EntryAnswerToUpdateDecodedL( aEvent );
                        }
                    else
                        {
                        EntryAnswerToOfferDecodedL( aEvent );
                        }
                    }
	            break;
	            }
	      
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            aEvent.Session().Actions().ControlMediaL( aEvent.ParamIDs(), 
	                                             (TMceItcFunctions)aEvent.Code() );
	            break;
	            } 
	        case EMceBye:
	            {
	            EntryByeL( aEvent );
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
// CMceStateOffering::EntryUpdateL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryUpdateL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    if ( MceSip::HasContent( session.Request() ) )
        {
        // UPDATE carries SDP
        User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );
    
        session.Actions().DecodeL();
        TMceReturnStatus status = session.Actions().UpdateL();
        aEvent.ParamStatus() = status;
        if ( MCE_IS_ERROR( status ) )
            {
            // TBD: update failed, reject
            }
        else if ( status == KMceAsync )
            {
            // NOP
            }
        else // ready
            {
            aEvent.Code() = EMceMediaUpdated;
            EntryMediaUpdatedL( aEvent );
            }
        }
    else 
        {
        // TBD: handle UPDATE without content
        }
        
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryProvisionalResponseL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();

    TMceReturnStatus status = session.Actions().ReserveL();
    aEvent.ParamStatus() = status;
    if ( MCE_IS_ERROR( status ))
        {
        // TBD: reservation failed
        }
    else
        if (status == KMceAsync)
            {
            // NOP
            }
        else // ready, send 2xx to update
            {
            CSIPServerTransaction* update= NULL;
            RStringF trx = session.Request().Type();
            if (trx == SIPStrings::StringF(SipStrConsts::EUpdate) )
                {
                update = &session.Request();
                }

            if (update)
                {
                session.Actions().EncodeL();
                session.FCActions().UpdateFCAnswerL( *session.Offer() );
                session.Actions().SendAnswerL( *update);
                }
            else
                {
                User::Leave(KErrTotalLossOfPrecision);
                }
            }
	}

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryProvisionalResponseL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryProvisionalResponseL( 
    TMceStateTransitionEvent& aEvent )
    {
    TInt status = KErrNone;
    CMceSipSession& session = aEvent.Session();
    CSIPClientTransaction& response = session.Response();
    iLastResponse = MceSip::ResponseCode( response );
     
    session.Extensions().UpdateL( response );
    session.Actions().CheckContactIsSecureL( response );

    session.ActiveBody().AnswerType() = KMceNegotiationAnswerTypeIntermediate;
    
    TBool IsFork = EFalse;
    if ( MceSip::ResponseCode( session.Response() ) == KMceSipSessionProgress && 
    	   session.ForkedDialogsCount() ) 
        {
        if( session.ActiveBody().NegotiationState().Id() == KMceMediaNegotiated ||
        	 session.ActiveBody().NegotiationState().Id() == KMceOfferingMedia )
        	{
        IsFork = ETrue;
        	}
        else
        	{
        	session.ResetCurrentDialog();
        	}
  	    }
    if ( session.Actions().NeedToProcessL( aEvent ) || IsFork )
        {
        if ( MCE_NEED_TO_SEND( session, NULL ) || IsFork )
            {
            if ( MceSip::HasContent( response ) )
                {
                // create SDP answer
                //status = session.Actions().CreateSDP( response );
                NAT_WAIT_NO_EXEC( session, (status = session.Actions().CreateSDP( response )));
        	    
                // handle SDP content
                if ( !MCE_IS_ERROR( status ) )
                    {
                    // process session timer
                    session.Actions().ProcessSessionTimerClientL( response );
                    // update FC
                    session.FCActions().PeekFCDocumentL( *session.Offer() );
                
                    //decode answer, ignore warning code 
                    if ( session.ActiveBody().SecureSession())
                    	{
                    	session.ActiveBody().SecureSession()->iLSReadyToBind = EFalse;
                    	}
                    session.Actions().DecodeL();

                    
                    // update media
                    status = session.Actions().UpdateL();
                    }
                }
        
            //if there was an error
            if ( MCE_IS_ERROR( status ) )
                {
                EntryCancelL( aEvent );
                //transition exit is performed here
                session.Actions().StateChanged( KMceStateCanceled );
                session.Actions().ClientStateChangedL( CMceSession::ECancelling, 
                                                       aEvent.ParamStatus() );
                }
            else if ( status == KMceAsync )
                {
                aEvent.Session().SetWaitingMediaCallback( ETrue );
                }
            else // KMceReady 
                {
                EntryAnswerToOfferDecodedL( aEvent );
                }
            }
        else
            {
            EntryAnswerToOfferDecodedL( aEvent );
            }
        }
    aEvent.ParamStatus() = status;
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryRedirectionResponseL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryRedirectionResponseL( 
    TMceStateTransitionEvent& aEvent )
    {
    MCESRV_DEBUG("CMceStateOffering::EntryRedirectionResponseL, Entry");

	CMceSipSession& session = aEvent.Session();
    
    session.ResetSdpCounts();
    
    switch ( MceSip::ResponseCode( session.Response() ) )
        {   
        case KMceSipMovedTemporarily:  
            {
            // any 3xx response for updating will be handled as
            // a error situation
            if ( session.SubState() != CMceSipSession::EUpdating )
                {
                // get remoteUri information from response 
                session.AddRemoteUriFromResponseL( 
                    *const_cast<CSIPResponseElements*>(session.Response().ResponseElements()) );
                // create a new inviteAssociationDialog
                session.CreateDialogFrom3XXResponseL();
                // send the invite again
                session.Actions().SendInviteL();    
                }            
            break;
            }
        // include KMceSipUseProxy, KMceSipAlternativeService,
        // KMceSipMultipleChoices, KMceSipMovedPermanently
        default: 
            {
            // error situation
            break;
            }
        }
    
    MCESRV_DEBUG("CMceStateOffering::EntryRedirectionResponseL, Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceStateOffering::ExitRedirectionEventL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::ExitRedirectionEventL( 
    TMceStateTransitionEvent& aEvent )
    {
    MCESRV_DEBUG("CMceStateOffering::RedirectionErrorResponseL, Entry");

    CMceSipSession& session = aEvent.Session();
    CMceSipSession::TSubState subState = session.SubState();
    TUint responseCode = MceSip::ResponseCode( session.Response() );
    
    if ( subState == CMceSipSession::EOffering )
        {
        if ( responseCode == KMceSipMovedTemporarily )  
            {
            session.Actions().ClientStateChangedL( 
                CMceSession::EOffering, *session.Body(), session.Response() );           
            }
        else
            {
            session.Actions().StopMedia();
            session.FCActions().ReleaseFC();
            session.Actions().StateChanged( KMceStateTerminated );
            session.Actions().ClientStateChangedL( 
                CMceSession::ETerminated, *session.Body(), session.Response() );       
            }
        }
    else if ( session.SubState() == CMceSipSession::EUpdating ) 
        {
        session.Actions().StopUpdatingMedia();
        session.Actions().UpdateFailed();
        session.Actions().StateChanged( CMceSession::EEstablished );
        session.Actions().ClientStateChangedL( 
            CMceSession::EEstablished, *session.Body(),
            session.Response(), KErrGeneral );                                    
        }
    else
        {
        // do nothing  
        }
    
    MCESRV_DEBUG("CMceStateOffering::RedirectionErrorResponseL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryResponseL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryResponseL( TMceStateTransitionEvent& aEvent )
    {
    TInt status = KErrNone;
	CMceSipSession& session = aEvent.Session();
    CMceSipSession::TSubState subState = session.SubState();
    TInt currentResponse = MceSip::ResponseCode( session.Response() );
	
	// If media handling is ongoing while 200OK arrives with certain SDP content
    // don't send ACK thus the UAS will retransmit 200OK untile pervious meida handling
    // finish.
 
    if ( KMceSipRinging == iLastResponse &&
    	 KMceSipOK == currentResponse && 
    	 MceSip::HasContent( session.Response() ) )
    	{
		if ( session.WaitingMediaCallback() )
    		{
			iReadyToSendACK = EFalse;
			MCESRV_DEBUG("Waiting media callback, do not send ACK for this 200OK")
    		return;
    		}
    	else
    		{
			iReadyToSendACK = ETrue;
			MCESRV_DEBUG("Ready to send ACK")
    		}
    	}
    //send ACK
    session.Actions().SendACKL( session.Response() );
	session.Actions().CheckContactIsSecureL( session.Response() );
    session.ActiveBody().AnswerType() = KMceNegotiationAnswerTypeFinal;
    
    iLastResponse = currentResponse;

	if ( !session.WaitingMediaCallback() )
	    {
	    if ( MceSip::HasContent( session.Response() ) )
	        {
	        //process session timer
            session.Actions().ProcessSessionTimerClientL( session.Response() );
            
            if ( subState != CMceSipSession::ERefreshing )
                {
                //create SDP answer
        	    NAT_WAIT_NO_EXEC( session, (status = session.Actions().CreateSDP( session.Response() ) ));
        	    
            	if ( !MCE_IS_ERROR( status ) )
            	    {
                    //update FC
                    session.FCActions().PeekFCDocumentL( *session.Offer() );
                	
            	    //decode answer, ignore warning code 
                    if ( session.ActiveBody().SecureSession())
                    	{
                    	session.ActiveBody().SecureSession()->iLSReadyToBind = ETrue;
                    	}
                    status = session.Actions().Decode();

                    // update media and start waiting EMceMediaUpdated (if needed) 
				    if ( !MCE_IS_ERROR( status ) )
						{
		                status = session.Actions().UpdateL();
		                //if everything OK, starts floor control
		        		if ( !MCE_IS_ERROR( status ) )
		    		        {
		                    //check should wait EMceMediaUpdated
		                    aEvent.Code() = status == KMceReady ? 
		                                    EMceMediaUpdated : EMceResponse;
		                    if ( subState == CMceSipSession::EOffering )
		                        {
		                        // start floor control only for the initial INVITE
		                        session.FCActions().StartFCL();
		                        }
		    		        }
						}
            	    }
                }
            }
	    else // no content
    	    {
    	    if  ( !MCE_NEED_TO_RECEIVE( session ) )
    	        {
    	        if ( session.ActiveBody().SecureSession())
    	            {
    	            if( session.ForkedDialogsCount() )
    	            	{
    	            	session.ForceUpdateStreamL();
    	            	}
    	            }
                status = session.Actions().UpdateL();
    	        // SDP answer was received before
    	        // go to established state
    	        session.Actions().StateChanged( KMceStateEstablished );
                //session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                //                               session.Response() );
    	        session.Actions().ClientStateChangedL( CMceSession::EEstablished, 
                                                   *session.Body(),
                                                   session.Response(),
                                                   status );
    	        }
    	    else
    	        {
    	        // SDP answer is not received
    	        User::Leave( KErrNotReady );
    	        }
    	    }
        
	    aEvent.ParamStatus() = status;
        
        //if there was an error
        if ( MCE_IS_ERROR( status ) )
            {
        	//if first invite
            if ( subState == CMceSipSession::EOffering )
                {
                //send BYE and stop media
                session.Actions().SendBYEL();
                session.Actions().StopMedia();
                }
            //if updating
            else if ( subState == CMceSipSession::EUpdating )
                {
                //do rollback
                session.Actions().UpdateFailed();
                if ( session.Body() && &session.Body()->SdpSession() &&
                     session.Body()->SdpSession().iOOldSchoolCompleted )
                    {
                    session.Actions().ClientStateChangedL( 
                        CMceSession::EEstablished, *( session.Body() ), status );                         
                    }       
                }
            else
                {
                //NOP
                }
            }
        else if ( status == KMceAsync )
            {
            // set session waiting for media callback
            aEvent.Session().SetWaitingMediaCallback( ETrue );
            }
        else if ( status == KMceReady && subState == CMceSipSession::EUpdating )
            {
            session.Actions().UpdateSucceed();
            }
        else
            {
            // NOP
            }
	    }
    }
    

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryErrorResponseL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryErrorResponseL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
    
    session.ResetSdpCounts();
    
    CMceSipSession::TSubState subState = session.SubState();
    
    TUint code = MceSip::ResponseCode( session.Response() );
    TInt errorCode = KErrNone;
       
    if ( code == KMceSipSessionIntervalTooSmall )
    	{
    	if ( session.Dialog()->Dialog().State() == CSIPDialog::EConfirmed )	
    		{
    		//sending re-INVITE after 422 for re-INVITE
    		session.Actions().ProcessSessionTimerClientL( session.Response() );
			//send again
			session.Actions().SendSessionRefreshL();		
	   		}
	   		
		else if ( session.Dialog()->Dialog().State() == CSIPDialog::ETerminated )	   		
			{
			//sending re-INVITE after 422 for INVITE
			errorCode = session.ReUseDialog();	
            if ( errorCode == KErrNone )
				{
			   	session.Actions().ProcessSessionTimerClientL( session.Response() );
			 	session.Actions().SendInviteL();
				}
	  		}
    	}
    else if ( code == KMceSipExtensionRequired )    
        {
        session.Extensions().UpdateL( session.Response() );
        session.ReCreateOutgoingDialogL();
        session.Actions().SendInviteL();
        }     
	//if first invite, stop media and FC
    else if ( subState == CMceSipSession::EOffering )
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        }
	//if update, reject update
    else if ( subState == CMceSipSession::EUpdating ||
              subState == CMceSipSession::ERefreshing )
        {
	 	if ( code == KMceSipCallOrTransactionDoesNotExist ||
             code == KMceSipRequestTimeout ) 
            { 
            session.Actions().UpdateFailed();
            session.Actions().SendBYEL();
            session.Actions().StopMedia();
            session.FCActions().ReleaseFC();
            session.Actions().StateChanged( KMceStateTerminated );
            session.Actions().ClientStateChangedL( CMceSession::ETerminated,
                                                   session.Response() );
            }
         else
            {
            session.Actions().UpdateFailed(); 
            session.Actions().StateChanged( KMceStateEstablished );
            session.Actions().ClientStateChangedL( CMceSession::EEstablished, 
                                                   *session.Body(),
                                                   session.Response(),
                                                   KErrCancel );
            }
        }
    else
        {
        //NOP
        }
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryCancelL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryCancelL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
    CMceSipSession::TSubState subState = session.SubState();

    //(try) send CANCEL
    session.Actions().SendCancelL();

    if ( subState == CMceSipSession::EOffering )
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        }
    else if ( subState == CMceSipSession::EUpdating )
        {
        session.Actions().UpdateFailed();
        }
        
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryAnswerToUpdateDecodedL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryAnswerToUpdateDecodedL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();

    //if media error, media unusable -> send BYE, stop media and FC
    if ( aEvent.ParamStatus() == KErrNone )
        {
        session.Actions().UpdateSucceed();
        }
    else
        {
        session.Actions().UpdateFailed();
        }
    
    }

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryAnswerToOfferDecodedL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryAnswerToOfferDecodedL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
    CSIPClientTransaction* response = &session.Response();
    
    TInt status = KErrNone;
        
    if ( MceSip::ResponseType( *response ) == E1XX &&
         session.Extensions().LevelL( *response, CMceSipExtensions::E100rel )
         == CMceSipExtensions::ERequired )
        
        { 
        TBool hasContent = MceSip::HasContent( *response );
		
        // TBD: Quick Fix: Need to have Support in MediaManager to ask do we need 
        // to send SDP or not, decision must be based on conf line, and 2ndly 
        // based on weither negotiation has completed or not.
        TBool sendSdp = hasContent &&
                        ( MCE_NEED_TO_SEND( session, NULL ) ||
                          session.Extensions().RemoteIMSRel5() ||
                          session.ActiveBody().Modifier( KMceSecPreconditions ) ==
                           KMcePreconditionsE2ESupported );
        //if there is required header for precodition but no precondition
        //Prack should indicate precondition failure
        if ( MceSip::ResponseType( *response ) == E1XX &&
         	session.Extensions().LevelL( *response, CMceSipExtensions::ESecPreconds )
         	== CMceSipExtensions::ERequired )
        	{
        	session.ActiveBody().iRemoteSecPreconditionsRequired = ETrue;
        	}
        if ( sendSdp )
            {
            status = session.Actions().UpdateL();
            __ASSERT_ALWAYS( status == KMceReady, User::Leave( KErrArgument ) ); 
            //need not to change secure keys
            if ( session.ActiveBody().SecureSession() &&
            	 MceSip:: ResponseCode( *response ) == KMceSipSessionProgress)
            	{
            	session.ActiveBody().SecureSession()->iKeyNeedUpdated = EFalse;
            	}
            NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() )	
            //session, session.Actions().EncodeL(); 
            }

		
        session.Actions().SendPrackL( sendSdp );
        
        aEvent.Code() = EMceProvisionalResponse;
        }
        
    }
    

// -----------------------------------------------------------------------------
// CMceStateOffering::EntryByeL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::EntryByeL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	
    //send 200OK and stop media and FC
    session.Actions().Send200OKL( session.Request() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
	
    }
    
// -----------------------------------------------------------------------------
// CMceStateOffering::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::ExitL( TMceStateTransitionEvent& aEvent )
	{
	if ( !iReadyToSendACK )
		{
		MCESRV_DEBUG("Not ready, don't change any state");
		return;
		}
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}


	else
		{
		CMceSipSession& session = aEvent.Session();
		CSIPClientTransaction* response = &session.Response(); 
		TInt status = aEvent.ParamStatus();
		
	    switch( aEvent.Code() )
	        {
	        //received BYE causes server & client to terminated state
	        case EMceBye:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChangedL( 
	                CMceSession::ETerminated, session.Request() );
	            break;
	            }      
	        case EMceMediaUpdated:
	            {
	            if ( MceSip::ResponseType( *response ) == E2XX &&
                        response->ResponseElements()->CSeqHeader()->Method() == 
                        SIPStrings::StringF( SipStrConsts::EInvite ) )
		    		{
			   		//server and client state established,
		            //if update was successful Body() returns previous BodyCandidate(),
		            //if update was failed Body() returns original body
		            session.Actions().ClientStateChangedL( CMceSession::EEstablished,
		                                                   *session.Body(),
		                                                   session.Response(),
		                                                   status );
		            session.Actions().StateChanged( KMceStateEstablished );
               	    }	            
	            break;
	            }
	        case EMceResponse:
	        case EMceErrorResponse:
	        case EMceProvisionalResponse:
	        case EMceRedirectionResponse:
	            {
	            ExitSIPEventL( aEvent );
	            break;                
	            }
	       
	        case EMceItcCancel:
	            {
	            session.Actions().StateChanged( KMceStateCanceled );
	            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::ECancelling );
	            break;
	            }
	            
	        default:
	            {
	            //NOP
	            }
	            
	        }
		}
	
	}


// -----------------------------------------------------------------------------
// CMceStateOffering::ExitSIPEventL
// -----------------------------------------------------------------------------
//
void CMceStateOffering::ExitSIPEventL( TMceStateTransitionEvent& aEvent )
    {
    
	CMceSipSession& session = aEvent.Session();
	TInt status = aEvent.ParamStatus();
    CMceSipSession::TSubState subState = session.SubState();
	
	if ( aEvent.Code() == EMceResponse )
	    {
        //error in response handling
        if ( status < KErrNone && subState == CMceSipSession::EOffering )//failed
            {
            //no need to update body to client
            session.Actions().StateChanged( KMceStateTerminating );
            session.Actions().ClientStateChangedL( CMceSession::ETerminating,
                                                   session.Response(),
                                                   status );
            }
        else if ( status < KErrNone && subState == CMceSipSession::EUpdating )//failed
            {
            //original body is updated to client
            session.Actions().ClientStateChangedL( CMceSession::EEstablished, 
                                                   *session.Body(),
                                                   session.Response(),
                                                   status );
            session.Actions().StateChanged( KMceStateEstablished );
            }
        else if ( subState == CMceSipSession::ERefreshing )
            {
            session.Actions().StateChanged( KMceStateEstablished );
            session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                                   session.Response() );
            }
        else
            {
            // SDP Answer was received previously, ignore the contents if any
            // Get into established state.
            if ( !session.WaitingMediaCallback() &&
                 !MCE_NEED_TO_RECEIVE( session ) )
                {
                session.Actions().StateChanged( KMceStateEstablished );
                session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                                       session.Response() );
                }
            else
                {
                //wait EMceAnswerDecoded or EMceAnswerToUpdateDecoded
                }
            }
	    }
    else if ( aEvent.Code() == EMceErrorResponse )
        {
        if ( subState == CMceSipSession::EOffering )
            {
            TUint code = MceSip::ResponseCode( session.Response() );
            if ( code != KMceSipExtensionRequired && code != KMceSipSessionIntervalTooSmall  )  
                {
                session.Actions().StateChanged( KMceStateTerminated );
                session.Actions().ClientStateChangedL( CMceSession::ETerminated,
                                                      session.Response() );
                }
            }
        else if ( subState == CMceSipSession::EUpdating )
            {
            if ( MceSip::ResponseCode( 
                 session.Response() ) == KMceSipCallOrTransactionDoesNotExist ||
                 MceSip::ResponseCode( 
                 session.Response() ) == KMceSipRequestTimeout ) 
                {
                session.Actions().StateChanged( KMceStateTerminating );
                session.Actions().ClientStateChangedL( CMceSession::ETerminating,
                                                       session.Response() );
                }
            else
                {
                session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                                       *session.Body(),
                                                       session.Response(),
                                                       KErrCancel );
                session.Actions().StateChanged( KMceStateEstablished );
                }
            }
        else
            {//MediaUpdated
            if ( session.PendingTransaction( SIPStrings::StringF( SipStrConsts::EPrack ) ) )
                {
                session.Actions().StateChanged( KMceStateConfirming );
                }
            else
                {
                session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                                   session.Response() );
                session.Actions().StateChanged( KMceStateEstablished );
                }
            }
        }
    else if ( aEvent.Code() == EMceRedirectionResponse )
        {
        ExitRedirectionEventL( aEvent );
        }
        
        
  	else//EMceProvisionalResponse
        {
        if ( MceSip::ResponseCode( session.Response() ) > KMceSipTrying && 
             session.Actions().NeedToProcessL( aEvent ) )
            {
            // update RSeq of a handled response to the session if it contains
            TInt rseqValueOfLatestResponse = KErrNotFound;
            
            rseqValueOfLatestResponse = MceSip::RSeq( *session.Response().ResponseElements() );
            
            if ( rseqValueOfLatestResponse != KErrNotFound )
            	{
            	session.SetRSeq( rseqValueOfLatestResponse );		
            	}
                        
            session.Actions().ClientStateChangedL( CMceSession::EOffering,
                *session.Body(), session.Response() );
            }
        
        if ( session.PendingTransaction( SIPStrings::StringF( SipStrConsts::EPrack ) ) )
            {
            session.Actions().StateChanged( KMceStateConfirming );
            }
        }
        
    }



// End of File

