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





#include "mcestateserveroffering.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcenatmacros.h"

// -----------------------------------------------------------------------------
// CMceStateServerOffering::CMceStateServerOffering
// -----------------------------------------------------------------------------
//
CMceStateServerOffering::CMceStateServerOffering ()
    : CMceState( KMceStateServerOffering )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateServerOffering::~CMceStateServerOffering
// -----------------------------------------------------------------------------
//
CMceStateServerOffering::~CMceStateServerOffering()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateServerOffering::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateServerOffering::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    User::LeaveIfError(
      aEvent.Code() == EMceItcEnable  ||
      aEvent.Code() == EMceMediaUpdated ||
      aEvent.Code() == EMceItcDisable ||
      aEvent.Code() == EMceAck ||
      aEvent.Code() == EMceBye ||
       IsExtensionRequestEvent( aEvent )
       
        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateServerOffering::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateServerOffering::EntryL( TMceStateTransitionEvent& aEvent )
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
	        case EMceBye:
	            {
	            EntryByeL( aEvent );
	            break;
	            }
	        case EMceAck:
	            {
	            EntryAckL( aEvent );
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
	            break;
	            }
	        }
		}
    }


// -----------------------------------------------------------------------------
// CMceStateServerOffering::EntryByeL
// -----------------------------------------------------------------------------
//
void CMceStateServerOffering::EntryByeL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	
    //send 200OK and stop media and FC
    session.Actions().Send200OKL( session.Request() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
    }
  
// -----------------------------------------------------------------------------
// CMceStateServerOffering::EntryAckL 
// -----------------------------------------------------------------------------
//  
void CMceStateServerOffering::EntryAckL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    TInt status = KErrNone;
    CMceSipSession::TSubState subState = session.SubState();
    CSIPServerTransaction& request = session.Request();
    CSIPServerTransaction* requestPtr = &request;
    session.ActiveBody().AnswerType() = KMceNegotiationAnswerTypeFinal;

    // if receving ACK without SDP, send error to client, 
    // terminate the session
    if ( (requestPtr!=NULL) && !MceSip::HasContent( request ) )
        {
        EntryAckWithoutSdpL( aEvent ); 
        }
    else
        {
        if ( &session.Response() )
    		{
    		session.Actions().CheckContactIsSecureL( session.Response() );
    		}
    	else
    		{
    		session.Actions().CheckContactIsSecureL( session.Request() );
    		}  
    		  
    	NAT_WAIT_NO_EXEC( session, (status = session.Actions().CreateSDP( session.Request() )));
    	//status = session.Actions().CreateSDP( session.Request() ); 
    	
    	if ( !MCE_IS_ERROR( status ) )
        	{
        	//update FC
            session.FCActions().PeekFCDocumentL( *session.Offer() );
                	
            //decode answer, ignore warning code 
            status = session.Actions().Decode();
               
            if ( !MCE_IS_ERROR( status ) )
                {
                status = session.Actions().UpdateL();
                
            	if ( !MCE_IS_ERROR( status ) )
    		        {
                    //check should wait EMceMediaUpdated
                    aEvent.Code() = status == KMceReady ? 
                                    EMceMediaUpdated : EMceAck;
                    if ( subState == CMceSipSession::EAnswering )
                        {
                        // start floor control only for the initial INVITE
                        session.FCActions().StartFCL();
                        }
    		        }
    		        
				}   
             
        	}
        if ( MCE_IS_ERROR( status ) )
            {
        	//if re-invite
            if ( subState == CMceSipSession::EUpdating )
                {
                //do rollback
                session.Actions().StopUpdatingMedia();
                session.Actions().UpdateFailed(); 
                session.Actions().StateChanged( KMceStateEstablished );
                session.Actions().ClientStateChangedL( CMceSession::EEstablished, *session.Body(), status );                               
                }
            //initial invite
            else 
                {
                //send BYE and stop media
                session.Actions().SendBYEL();
                session.Actions().StopMedia();
                session.FCActions().ReleaseFC();
                session.Actions().StateChanged( KMceStateTerminating );
                session.Actions().ClientStateChangedL( CMceSession::ETerminating );      
                }
            }   
        if ( status == KMceAsync )
            {
            // set session waiting for media callback
            aEvent.Session().SetWaitingMediaCallback( ETrue );    
            }
        }
   	aEvent.ParamStatus() = status;	 
    }
 
// -----------------------------------------------------------------------------
// CMceStateServerOffering::EntryAckWithoutSdpL
// -----------------------------------------------------------------------------
//   
void CMceStateServerOffering::EntryAckWithoutSdpL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CSIPServerTransaction& request = session.Request();
    CMceComSession& body = session.ActiveBody();
    CMceComSession* bodyPtr = &body;
     
    if ( (bodyPtr != NULL) && body.Streams().Count() <= 0 )
        {
        session.Actions().StateChanged( KMceStateEstablished ); 
        session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                               request );	
        }
    else // receive pull mode, answer without SDP
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        session.Actions().StateChanged( KMceStateTerminated ); 
        session.Actions().SendRejectOfferWithWarningL( 
            KMceSipWarnSessionDescriptionParameterNotUnderstood, 
            session.Request() );     
        }
    }
    

// -----------------------------------------------------------------------------
// CMceStateServerOffering::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateServerOffering::ExitL( TMceStateTransitionEvent& aEvent )
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
	        case EMceMediaUpdated:
	            {
	            session.Actions().StateChanged( KMceStateEstablished );  
	            session.Actions().ClientStateChangedL( 
	                    CMceSession::EEstablished,
	                    session.ActiveBody(),
	                    status );           
	            session.Actions().UpdateSucceed();
	            break;
	            }
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            //NOP
	            break;
	            }
	        default:
	            {
	            break;
	            }
	        }
		}
	}


// End of File

