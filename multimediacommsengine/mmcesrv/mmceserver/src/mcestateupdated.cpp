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





#include "mcestateupdated.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcenatmacros.h"

// -----------------------------------------------------------------------------
// CMceStateUpdated::CMceStateUpdated
// -----------------------------------------------------------------------------
//
CMceStateUpdated::CMceStateUpdated ()
    : CMceState( KMceStateUpdated )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateUpdated::~CMceStateUpdated
// -----------------------------------------------------------------------------
//
CMceStateUpdated::~CMceStateUpdated()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateUpdated::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateUpdated::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    User::LeaveIfError(
      aEvent.Code() == EMceItcEnable  ||
      aEvent.Code() == EMceItcDisable  ||
      aEvent.Code() == EMceItcAcceptSession ||
      aEvent.Code() == EMceItcRejectSession ||
      aEvent.Code() == EMceItcCallInQueue ||   
      aEvent.Code() == EMceItcRing ||
      IsExtensionRequestEvent( aEvent )
      
        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateUpdated::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateUpdated::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
	
	else
		{
		CMceSipSession& session = aEvent.Session();
		CSIPServerTransaction& request = session.Request();
		CSIPServerTransaction* requestPtr = &request;
			
	    switch ( aEvent.Code() )
	        {
	        case EMceItcAcceptSession:
	            {            
	            if ( (requestPtr!=NULL) && !MceSip::HasContent( request ) )
	            	{
	                NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );

			        session.FCActions().UpdateFCAnswerL( *session.Offer() );
			       	session.Actions().SendAnswerL( request, aEvent.ParamClientMessage() );
			       	}
	            else if ( MCE_NEED_TO_SEND( session, NULL ) )
	                {
	                //encode body
	                if( session.IsNatSession() )
	                	{
	                	 //nothing to do
	                	}
	                else
	                	{
	                	NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
	                	}
	                
	                session.FCActions().UpdateFCAnswerL( *session.Offer() );
	                session.Actions().SendAnswerL( request, 
	                                               aEvent.ParamClientMessage() );
	                session.FCActions().StartFCL();                       
	                }
	            else 
	                {
	                session.Actions().Send200OKL( session.Request() );
	                session.FCActions().StartFCL();
	                }                
	 
	            break;
	            }
	        case EMceItcRejectSession:
	            {
	            session.Actions().SendDeclineL( request, 
	                                            aEvent.ParamClientMessage() );
	            session.Actions().StopMedia();
	            session.FCActions().ReleaseFC();
	            break;
	            }
	        case EMceItcRing:
	            {
	            TBool reliable = session.Actions().SendRingL( request, 
	                                        aEvent.ParamClientMessage() );
	            if ( reliable )
	                {
	                // need to wait for PRACK
	                session.Actions().StateChanged( KMceStateAcknowledgementRequired );
	                }
	            break;
	            }
	        case EMceItcCallInQueue:
	            {
	            TBool reliable = session.Actions().SendSipQueuedL( request, 
	                                        aEvent.ParamClientMessage() );
	            if ( reliable )
	                {
	                // need to wait for PRACK
	                session.Actions().StateChanged( KMceStateAcknowledgementRequired );
	                }
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
	            }
	        }
		}

	}

// -----------------------------------------------------------------------------
// CMceStateUpdated::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateUpdated::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}
	
	else
		{
		CMceSipSession& session = aEvent.Session();
		CSIPServerTransaction& request = session.Request();
		CSIPServerTransaction* requestPtr = &request;

	    switch ( aEvent.Code() )
	        {
	        case EMceItcAcceptSession:
	            {
	            if ( (requestPtr!=NULL) && !MceSip::HasContent( request ) )
	            	{
	                // Pull Mode
			        session.Actions().StateChanged( KMceStateServerOffering );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::EAnswering ); 
			       	}
			    else
	    		    {
	    		    session.Actions().StateChanged( KMceStateAnswering );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::EAnswering );  
	    		    }   	
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
	            }
	        }

		}
	
	}



// End of File

