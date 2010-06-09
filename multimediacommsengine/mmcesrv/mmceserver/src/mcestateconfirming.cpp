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





#include "mcestateconfirming.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcesipextensions.h"
#include "mcenatmacros.h"
#include <sipdialogassocbase.h>
#include <sipdialog.h>


// -----------------------------------------------------------------------------
// CMceStateConfirming::CMceStateConfirming
// -----------------------------------------------------------------------------
//
CMceStateConfirming::CMceStateConfirming()
    : CMceState( KMceStateConfirming )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateConfirming::~CMceStateConfirming
// -----------------------------------------------------------------------------
//
CMceStateConfirming::~CMceStateConfirming()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateConfirming::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateConfirming::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError( 	                 
       aEvent.Code() == EMceResponse ||
       aEvent.Code() == EMceProvisionalResponse ||
       aEvent.Code() == EMceRedirectionResponse ||
       aEvent.Code() == EMceErrorResponse ||
       aEvent.Code() == EMceItcCancel ||
       aEvent.Code() == EMceMediaUpdated ||      
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateConfirming::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateConfirming::EntryL( TMceStateTransitionEvent& aEvent )
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
	        case EMceResponse:
	            {
	            EntryResponseL( aEvent );
	            break;
	            }
	        case EMceProvisionalResponse:
	            {
	            EntryProvisionalResponseL( aEvent );
	            break;
	            }
	        case EMceRedirectionResponse:
	        case EMceErrorResponse:
	            {
	            EntryErrorResponseL( aEvent );
	            break;
	            }
	        case EMceItcCancel:
	            {
	            EntryCancelL( aEvent );
	            break;
	            }
	        case EMceMediaUpdated:
	            {
	            EntryMediaUpdatedL( aEvent );
	            break;
	            }
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            session.Actions().ControlMediaL( aEvent.ParamIDs(), 
	                                             (TMceItcFunctions)aEvent.Code() );
	            break;
	            }  
	        default:
	            {
	            // NOP
	            break;
	            }
	        }
		}
	}

// -----------------------------------------------------------------------------
// CMceStateConfirming::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateConfirming::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}

	else
		{
	    CMceSipSession& session = aEvent.Session();
	    switch ( aEvent.Code() )
	        {
	        case EMceMediaUpdated:
	            {
	            ExitMediaUpdatedL( aEvent );
	            break;
	            }
	        case EMceItcCancel:
	            {
	            session.Actions().StateChanged( KMceStateCanceled );
	            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::ECancelling );
	            break;
	            }
	        case EMceErrorResponse:
	        case EMceRedirectionResponse:
	            {
	            // session canceled
	            session.Actions().StateChanged( KMceStateCanceled );
	            session.Actions().ClientStateChangedL( CMceSession::ECancelling,
	                                                   session.Response() );
	                                                          
	            break;                                                   
	            }
	        default:
	            {
	            break;
	            }
	        }
		}
	}

// -----------------------------------------------------------------------------
// CMceStateConfirming::EntryResponseL
// -----------------------------------------------------------------------------
//
void CMceStateConfirming::EntryResponseL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    RStringF trx = session.Response().Type();
    
    TMceReturnStatus status = KMceReady;
    CSIPClientTransaction& response = session.Response(); 
    TBool hasContent = MceSip::HasContent( response );
        
    if ( trx == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        // 2xx to INVITE already, send ACK
        session.Actions().SendACKL( response );
        
        if ( hasContent )
            {
            // since the answer is already received, 
            // seize resending UPDATE/PRACK
            TBool interrupted = session.Actions().InterruptConfirming();
            User::LeaveIfError( interrupted ? 
                KErrNone : KErrTotalLossOfPrecision );
            
            // there is SDP, treat it as answer
            User::LeaveIfError( session.Actions().CreateSDP( response ) );
            session.Actions().DecodeL();
            status = session.Actions().UpdateL();
            if ( status == KMceReady )
                {
                if ( MCE_NEED_TO_SEND( session, NULL ) )
                    {
                    // send re-INVITE
                    session.Actions().EncodeL();
                    session.Actions().SendInviteL();
                    }
                aEvent.Code() = EMceMediaUpdated;
                }
            else if ( MCE_IS_ERROR( status ) )
                {
                User::Leave( status );
                }
            else // KMceAsync
                {
                // NOP
                }                
            }
        else
            {
            // TODO: IF ( answerNeeded ) 
            //       THEN wait until the PRACK/UPDATE generates a response.
            //       ELSE consider session established
            }
        }
    else if ( trx == SIPStrings::StringF( SipStrConsts::EUpdate ) ||
              trx == SIPStrings::StringF( SipStrConsts::EPrack ) )
        {
        // 2xx to PRACK/UPDATE
        
        if ( hasContent )
            {
            NAT_WAIT_NO_EXEC( session, (status = session.Actions().CreateSDP( response )));
            if ( !MCE_IS_ERROR( status ) && MCE_NEED_TO_SEND( session, NULL ) )
                {
                if ( session.ActiveBody().SecureSession() )
                	{
                	session.ActiveBody().SecureSession()->iLSReadyToBind = ETrue;
                	}
                session.SetFirstOffer();
                status = session.Actions().DecodeL();
                if ( status == KMceSipPreconditionFailure )
                	{
                	User::Leave( status );
                	}
                else
                	{
                	status = session.Actions().UpdateL();
                	}
                }
            }
        
        if ( status == KMceReady )
            {
            if ( ( MCE_NEED_TO_SEND( session, NULL ) || 
               ( trx == SIPStrings::StringF( SipStrConsts::EPrack ) &&
                 session.Extensions().RemoteIMSRel5() )  )&&
                 session.ActiveBody().Modifier( KMceSecPreconditions )!=
                 	 KMcePreconditionsE2ESupported )
                 
                {
                
                if (!hasContent)
                    {
                        // Do nothing.
                    }
                else
                    {
                    status = session.Actions().UpdateL();
                    __ASSERT_ALWAYS( status == KMceReady, User::Leave( KErrArgument ) );
                    if ( session.ActiveBody().SecureSession() )
                    	{
                    	session.ActiveBody().SecureSession()->iLSReadyToBind = ETrue;
                    	}
                    NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
                    session.Actions().SendUpdateRequestL();
                    }
                }
            aEvent.Code() = EMceMediaUpdated;
            }
        else if ( MCE_IS_ERROR( status ) )
            {
            User::Leave( status );
            }
        else // KMceAsync
            {
            // NOP
            }
        aEvent.ParamStatus() = status;
        }
    else
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    }

// -----------------------------------------------------------------------------
// CMceStateConfirming::EntryProvisionalResponseL
// -----------------------------------------------------------------------------
//
void CMceStateConfirming::EntryProvisionalResponseL( 
                                        TMceStateTransitionEvent& /*aEvent*/ )
    {
    // IF retransmission THEN ignore
    // ELSE IF sent reliably THEN send PRACK
    // 
    // IF 180 ringing THEN give to application
    // TBD: What if 180 Ringing comes here?
    }

// -----------------------------------------------------------------------------
// CMceStateConfirming::EntryErrorResponseL
// -----------------------------------------------------------------------------
//    
void CMceStateConfirming::EntryErrorResponseL( 
                                        TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    RStringF trx = session.Response().Type();
    TUint code = MceSip::ResponseCode( session.Response() );
    
    if ( trx == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        if ( session.SubState() == CMceSipSession::EOffering )
            {
            session.Actions().StateChanged( KMceStateTerminated );
            session.Actions().ClientStateChangedL( CMceSession::ETerminated,
                                                   session.Response() );
            
            }
          
        else if ( session.SubState() == CMceSipSession::EUpdating ||
              session.SubState() == CMceSipSession::ERefreshing )
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
                
        }
    
    else if ( trx == SIPStrings::StringF( SipStrConsts::EUpdate ) ||
              trx == SIPStrings::StringF( SipStrConsts::EPrack ) )
        {
        EntryCancelL( aEvent );
        }
    else
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    }
    
// -----------------------------------------------------------------------------
// CMceStateConfirming::EntryCancelL
// -----------------------------------------------------------------------------
//    
void CMceStateConfirming::EntryCancelL( TMceStateTransitionEvent& aEvent )
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
// CMceStateConfirming::EntryMediaUpdatedL
// -----------------------------------------------------------------------------
//    
void CMceStateConfirming::EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    
    if ( MCE_NEED_TO_SEND( session, NULL ) )
        {
        NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
        
        if ( session.Dialog()->Dialog().State() == CSIPDialog::EEarly )
            {
            session.Actions().SendUpdateRequestL();    
            }
        else
            {
            session.Actions().SendInviteL();
            }
        }    
    }

// -----------------------------------------------------------------------------
// CMceStateConfirming::ExitMediaUpdatedL
// -----------------------------------------------------------------------------
//    
void CMceStateConfirming::ExitMediaUpdatedL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CSIPClientTransaction* update = session.PendingTransaction(
        SIPStrings::StringF( SipStrConsts::EUpdate ) );
    if ( update && !MceSip::TrxCompleted( *update ) )
        {
        // just sent UPDATE
        session.Actions().StateChanged( KMceStateConfirming );
        }
    else
        {
        CSIPClientTransaction* invite = session.PendingTransaction( 
                    SIPStrings::StringF( SipStrConsts::EInvite ) );
        User::LeaveIfError( invite ? KErrNone : 
            KErrTotalLossOfPrecision );

        if ( invite->StateL() == CSIPTransactionBase::ETerminated )
            {
            // Just go to established state
            session.Actions().ClientStateChangedL( 
                                    CMceSession::EEstablished,
                                    *session.Body(),
                                    session.Response() );
            session.Actions().StateChanged( KMceStateEstablished );
            }
        else
            {
            session.Actions().StateChanged( KMceStateOffering );
            }
        }    
    }
    
// End of file
