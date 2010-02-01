/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcestateacknowledgementrequired.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mcenatmacros.h"
#include "mcereliablesender.h"

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::CMceStateAcknowledgementRequired
// -----------------------------------------------------------------------------
//
CMceStateAcknowledgementRequired::CMceStateAcknowledgementRequired()
    : CMceState( KMceStateAcknowledgementRequired )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::~CMceStateAcknowledgementRequired
// -----------------------------------------------------------------------------
//
CMceStateAcknowledgementRequired::~CMceStateAcknowledgementRequired()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateAcknowledgementRequired::DoAcceptL( 
                                        TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError(
       aEvent.Code() == EMcePrack ||
       aEvent.Code() == EMceMediaUpdated ||
       aEvent.Code() == EMceNoPrackReceived ||
       aEvent.Code() == EMceReliableFailed ||
       aEvent.Code() == EMceAck  ||      
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable ||
       aEvent.Code() == EMceItcAcceptSession ||
       aEvent.Code() == EMceItcRejectSession ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::EntryL( 
                                        TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
	
	else
		{
	    switch ( aEvent.Code() )
	        {
	        case EMcePrack:
	            {
	            EntryPrackL( aEvent );
	            break;
	            }
	        case EMceMediaUpdated:
	            {
	            EntryMediaUpdatedL( aEvent );
	            break;
	            }
	        case EMceNoPrackReceived:
	            {
	            EntryNoPrackReceivedL( aEvent );
	            break;             
	            }
	        case EMceReliableFailed:
	            {
	            EntryReliableFailedL ( aEvent ); 
	            break;       
	            }
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            aEvent.Session().Actions().ControlMediaL( aEvent.ParamIDs(), 
	                                             (TMceItcFunctions)aEvent.Code() );
	            break;
	            }  
	        case EMceItcAcceptSession:
	            {
	            EntryEarlyAcceptL( aEvent );
	            break;
	            }
	        
			case EMceItcRejectSession:
	            {
	            CMceSipSession& session = aEvent.Session();
				CSIPServerTransaction& request = session.Request();
				
	            aEvent.Session().Actions().SendDeclineL( request, 
	                                            aEvent.ParamClientMessage() );
				                                            
	            aEvent.Session().Actions().StopMedia();
	            aEvent.Session().FCActions().ReleaseFC();
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
// CMceStateAcknowledgementRequired::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::ExitL( TMceStateTransitionEvent& aEvent )
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
	        case EMcePrack:
	            {
	            if ( MCE_IS_ERROR( status ) )
	                {
	                session.Actions().StateChanged( KMceStateTerminated );
	                session.Actions().ClientStateChangedL( CMceSession::ETerminated );
	                }
	            else
	                {
	                //NOTE this is only for media reservation, not for resource
	                //reservation
	                if ( status == KMceAsync )
	                    {
						//PRACK caused media to be ready asynchronously
	                    }
	                else if ( status == KMceReady && 
	                	    MCE_NEED_TO_RECEIVE( session ) )
	                    {
	                    //media is ready, but preconditions are not met
	                    session.Actions().StateChanged( KMceStateConfirmationRequired );
	                    }
	                                                                       
	                else 
	                    {
	                    // media is ready and preconditions are met
	                    ProceedToUpdatedL( aEvent );
	                    }
	                }
	            break;
	            }
	        case EMceMediaUpdated:
	            {
	            if ( MCE_IS_ERROR( status ) )
	                {
	                session.Actions().StateChanged( KMceStateTerminated );
	                session.Actions().ClientStateChangedL( CMceSession::ETerminated );
	                }
	            else if ( status == KMceReady )
	                {
	                // Other possible state changes handle in entry
	                session.Actions().StateChanged( KMceStateConfirmationRequired );
	                }
	            else // KMceAsync
	                {
	                session.Actions().StateChanged( KMceStateServerReserving );
	                }
	            break;
	            }
	         
	        case EMceNoPrackReceived:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChangedL( CMceSession::ETerminated,
	                                                   KErrTimedOut );
	            break;             
	            }
	            
	        case EMceReliableFailed:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChangedL( CMceSession::ETerminated,
	                                                   aEvent.ParamStatus());
	            break;
	            }
		
			case EMceItcRejectSession:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::ETerminated );
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
// CMceStateAcknowledgementRequired::EntryPrackL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::EntryPrackL( 
                                            TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    // validate PRACK
    TBool match = session.ReliableSender().PrackReceived( session.Request() );
    TMceSipWarningCode warning = KErrNone;
    if ( match )
        {
        if ( MceSip::HasContent( session.Request() ) )
            {
            // PRACK carries SDP
            User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );
          	
            session.Actions().CreateBodyCandidateL( CMceComSession::EInSession );
          	
          	warning = session.Actions().DecodeL();
          	
          	session.ActiveBody().iCodecSelection = KMceCodecSelectionMerge;
          	
            aEvent.ParamStatus() = !warning ? KErrNone : KErrAbort;
            if ( !warning )
                {
                TMceReturnStatus status = session.Actions().UpdateL();
                //Second update is needed.
                status = session.Actions().UpdateL();
                aEvent.ParamStatus() = status;
                if ( status == KMceReady )
                    {
                    aEvent.Code() = EMceMediaUpdated;
                    EntryMediaUpdatedL( aEvent );
                    }
                }
            }
        else
            {
            // Empty PRACK
            // Send 200 OK
            session.Actions().Send200OKL( session.Request() );
            TMceReturnStatus status = KMceReady;
            status = session.Actions().ReserveL();
            
            aEvent.ParamStatus() = status;
            if ( status == KMceAsync )
                {
                // reservation ongoind
                session.Actions().StateChanged( KMceStateServerReserving );
                }
            }
            
        if ( aEvent.Code() != EMceMediaUpdated && 
             MCE_IS_ERROR( aEvent.ParamStatus() ) )
            {
            if ( warning ) //if error, reject offer (4XX out)
                {
                session.Actions().SendRejectOfferWithWarningL( warning, session.Request() );
                }
            else
                {
                MceSip::DiscardRequest( session.Request() );
                }
                
            //reject also invite
            session.DiscardInvite();
            }
        }            
    else // not match
        {
        // Reject the PRACK
        // Suitable Error code can be defined when PRACK is not mached
        // to handle state transition
        aEvent.ParamStatus() = KErrNotFound; 
        MceSip::DiscardRequest( session.Request(), 
                                KMceSipCallOrTransactionDoesNotExist ); 
                                                                          
        }
    }

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::EntryMediaUpdatedL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::EntryMediaUpdatedL( 
                                            TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CSIPServerTransaction& prack = session.Request();
    // verify, that PRACK really is the previous request
    __ASSERT_ALWAYS( prack.Type() == SIPStrings::StringF( SipStrConsts::EPrack ), 
         User::Leave( KErrTotalLossOfPrecision ) );
    
    TMceReturnStatus status = session.Actions().ReserveL();
    aEvent.ParamStatus() = status;
    if ( MCE_IS_ERROR( aEvent.ParamStatus() ) )
        {
        MceSip::DiscardRequest( session.Request() );
            
        //reject also invite
        session.DiscardInvite();
        }
    else 
        {
        // send 2xx to PRACK
        if ( session.ActiveBody().SecureSession() )
        	{
        	session.ActiveBody().SecureSession()->iLSReadyToBind = ETrue;
        	}

		// In KMceNatStateDecodeOffer state, answer is available.
		// Pull mode is not used with long signalling.
		if ( !( session.NatSession() &&
			  ( ( session.NatState() == KMceNatStateCreateOffer )  ||
			    ( session.NatState() == KMceNatStateCreateAnswer ) ||
			    ( session.NatState() == KMceNatStateWait )         ||
			    ( session.NatState() == KMceNatStateConnected ) ) ) )
			{
			session.Actions().EncodeL();
			}
		session.FCActions().UpdateFCAnswerL( *session.Offer() );
        session.Actions().SendAnswerL( prack );

	    session.Actions().UpdateSucceed();
        	
        if ( !MCE_NEED_TO_RECEIVE( session ) &&  !MCE_FORCED_TO_RECEIVE( session ) )
            {
            ProceedToUpdatedL( aEvent );
            } 
        }
    }

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::EntryNoPrackReveivedL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::EntryNoPrackReceivedL( 
    TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CSIPServerTransaction& serverTransaction = session.Request();
    MceSip::DiscardRequest( serverTransaction, 
                           KMceSipServerTimeOut, 
                           KMceNoRetryAfter );
    
    if ( session.SubState() == CMceSipSession::EOffering ||
         session.SubState() == CMceSipSession::EAnswering )
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        }
    else if ( session.SubState() == CMceSipSession::EUpdating )
        {
        session.Actions().UpdateFailed();
        }
    else
        {
        // NOP
        }
    }
    
// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::EntryReliableFailedL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::EntryReliableFailedL( 
    TMceStateTransitionEvent& aEvent )
    {

    CMceSipSession& session = aEvent.Session();
    CSIPServerTransaction& serverTransaction = session.Request();
    MceSip::DiscardRequest( serverTransaction, 
                           KMceSipServerInternalError, 
                           KMceNoRetryAfter );
                   
    if ( session.SubState() == CMceSipSession::EOffering ||
         session.SubState() == CMceSipSession::EAnswering )
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        }
    else if ( session.SubState() == CMceSipSession::EUpdating )
        {
        session.Actions().UpdateFailed();
        }
    else
        {
        // NOP 
        }                           
    }

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::EntryEarlyAcceptL
// -----------------------------------------------------------------------------
//    
void CMceStateAcknowledgementRequired::EntryEarlyAcceptL( 
    TMceStateTransitionEvent& aEvent )
    {
    // Client accepted the session before prack was received, remember that
    // 200 OK for invite is sent when possible.
    CMceSipSession& session = aEvent.Session();
    session.StoreAutoEventL( &aEvent, &session.InitialRequest() );
    }

// -----------------------------------------------------------------------------
// CMceStateAcknowledgementRequired::ProceedToUpdatedL
// -----------------------------------------------------------------------------
//
void CMceStateAcknowledgementRequired::ProceedToUpdatedL(
    TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    
    TMceStateTransitionEvent* acceptEvent = session.StoredAutoEvent();                                          
    if ( acceptEvent && acceptEvent->Code() == EMceItcAcceptSession )
        {
        // Session was accepted before really possible and now accepting is
        // possible, enable automatic event generation to occur at state change
        session.SetAutoEvent( ETrue );
        }
    
    session.Actions().StateChanged( KMceStateUpdated );
    session.Actions().ClientStateChangedL( CMceSession::EProceeding,
                                           *session.Body(),
                                           KErrNone );
    }
    
// End of file
