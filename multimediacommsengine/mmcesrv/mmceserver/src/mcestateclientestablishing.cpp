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





#include "mcestateclientestablishing.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcenatmacros.h"

// -----------------------------------------------------------------------------
// CMceStateClientEstablishing::CMceStateClientEstablishing
// -----------------------------------------------------------------------------
//
CMceStateClientEstablishing::CMceStateClientEstablishing ()
    : CMceState( KMceStateClientEstablishing )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateClientEstablishing::~CMceStateClientEstablishing
// -----------------------------------------------------------------------------
//
CMceStateClientEstablishing::~CMceStateClientEstablishing()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateClientEstablishing::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateClientEstablishing::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
    //not accepted	
	User::LeaveIfError( aEvent.EventSource() == EMceSip ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );
	
    User::LeaveIfError(
      aEvent.Code() == EMceMediaUpdated ||
      aEvent.Code() == EMceItcCancel ||
      aEvent.Code() == EMceItcEnable ||
      aEvent.Code() == EMceItcDisable ||
      IsExtensionRequestEvent( aEvent )
      
        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateClientEstablishing::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateClientEstablishing::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}

	else
		{
		CMceSipSession& session = aEvent.Session();
	    CMceSipSession::TSubState subState = session.SubState();
		
	    switch ( aEvent.Code() )
	        {
	        // media update ready
	        case EMceMediaUpdated:
	            {
	            //send INVITE and  start waiting SIP response
	            NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
	            
		        session.FCActions().UpdateFCOfferL( *session.Offer() );
	            session.Actions().SendInviteL();
	            break;
	            }
	        //client has been called cancel, stop media and FC
	        //no sending of CANCEL, because INVITE has not been sent yet
	        case EMceItcCancel:
	            {
	            if ( subState == CMceSipSession::EOffering )
	                {
	                session.Actions().StopMedia();
	                session.FCActions().ReleaseFC();
	                }
	            else if ( subState == CMceSipSession::EUpdating )
	                {
	                session.Actions().UpdateFailed();
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
	            //NOP
	            break;
	            }
	        }
		}
	}

// -----------------------------------------------------------------------------
// CMceStateClientEstablishing::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateClientEstablishing::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}
	
	else 
		{
		CMceSipSession& session = aEvent.Session();
	    CMceSipSession::TSubState subState = session.SubState();
		
	    switch ( aEvent.Code() )
	        {
	        case EMceMediaUpdated:
	            {
	            //if OK, server state -> offering, no client state transition
	    	    session.Actions().StateChanged( KMceStateOffering );
	    	    break;
	            }
	        case EMceItcCancel:
	            {
	            //CANCEL has not been sent -> directly to terminated 
	            //state both client and server
	            if ( subState == CMceSipSession::EOffering )
	                {
	                session.Actions().StateChanged( KMceStateTerminated );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::ETerminated );
	                }
	            else
	                {
	                session.Actions().StateChanged( KMceStateEstablished );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EEstablished );
	                }
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



// End of File.

