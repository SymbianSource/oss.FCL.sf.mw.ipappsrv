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




#include "mcestateidle.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"

// -----------------------------------------------------------------------------
// CMceStateIdle::CMceStateIdle
// -----------------------------------------------------------------------------
//
CMceStateIdle::CMceStateIdle ()
    : CMceState( KMceStateIdle )
	{
	}

// -----------------------------------------------------------------------------
// CMceStateIdle::~CMceStateIdle
// -----------------------------------------------------------------------------
//
CMceStateIdle::~CMceStateIdle()
	{
	}

// -----------------------------------------------------------------------------
// CMceStateIdle::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateIdle::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
    //not accepted	
	User::LeaveIfError( aEvent.EventSource() == EMceMedia ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );
                        
    //accepted
    User::LeaveIfError(
       aEvent.EventSource() == EMceSip && aEvent.Code() == EMceInvite ? KErrNone :
       aEvent.EventSource() == EMceClient && aEvent.Code() == EMceItcEstablishSession ? KErrNone :
       KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateIdle::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateIdle::EntryL( TMceStateTransitionEvent& aEvent )
	{
	switch( aEvent.Code() )
	    {
	    case EMceItcEstablishSession://going to send INVITE out
	        {
	        EntryEstablishL( aEvent );
            break;
	        }
	    case EMceInvite: //INVITE in
	        {
	        EntryInviteL( aEvent );
	        break;
	        }
        default:
            {
            //NOP
            break;
            }
	    }
	}

// -----------------------------------------------------------------------------
// CMceStateIdle::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateIdle::ExitL( TMceStateTransitionEvent& aEvent )
	{
	CMceSipSession& session = aEvent.Session();
	TInt status = aEvent.ParamStatus();
	
	switch( aEvent.Code() )
	    {
	    case EMceItcEstablishSession:
	        {
            //server state -> client establishing, client state -> offering
            session.Actions().ClientStateChanged( aEvent.ParamIDs(),
            									  CMceSession::EOffering );
	        if ( status == KMceAsync )
	            {
                session.Actions().StateChanged( KMceStateClientEstablishing );
	            }
            else
                {
                session.Actions().StateChanged( KMceStateOffering );
	            }
	        break;
	        }
	    case EMceInvite:
	        {
			if ( status == KErrNone || status == KMceAsync )
				{
				if ( ( session.NatSession() && !session.IsNatDisabled() ) &&
						( &aEvent.Session().Request()) && 		
						MceSip::HasContent( aEvent.Session().Request() ))
					{
					// Whether synchronous or asynchronous, wait NAT plugin to
					// get initialized
					session.Actions().StateChanged( KMceStateServerInitializing );
					}
				else
					{
					session.Actions().StateChanged( KMceStateServerEstablishing );	
					}
                }
            else // if error, server state -> terminated
                {
                session.Actions().StateChanged( KMceStateTerminated );
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

// -----------------------------------------------------------------------------
// CMceStateIdle::EntryEstablishL
// -----------------------------------------------------------------------------
//
void CMceStateIdle::EntryEstablishL( TMceStateTransitionEvent& aEvent )
    {
    //update session body based on session received from client
    //and start waiting EMceMceMediaUpdated
    CMceSipSession& session = aEvent.Session();
    TInt status = KErrNone;

    session.Actions().UpdateBodyL( aEvent.ParamIDs(),
                                   aEvent.ParamClientMessage() );
    status = session.Actions().UpdateL();                                           

    if ( status == KMceReady )
        {
        session.Actions().EncodeL();
        session.FCActions().UpdateFCOfferL( *session.Offer() );
        session.Actions().SendInviteL();
        }  
    aEvent.ParamStatus() = status;  
    }

// -----------------------------------------------------------------------------
// CMceStateIdle::EntryInviteL
// -----------------------------------------------------------------------------
//    
void CMceStateIdle::EntryInviteL( TMceStateTransitionEvent& aEvent )
    {
    // Handle incoming INVITE
    CMceSipSession& session = aEvent.Session();
    TInt status = KErrNone;
    session.Extensions().UpdateL( session.Request() );
    session.Actions().CheckContactIsSecureL( session.Request() );
    
    // Pull mode situation
    if ( &session.Request() && !MceSip::HasContent( session.Request() ) )
        {
        session.Actions().ProcessSessionTimerServerL( session.Request() );
        session.Actions().ClientUpdateIncomingSessionL( session.Request() );
	    }
	else
	    {
	    //create SDP
        status = session.Actions().CreateSDP( session.Request() );
        if ( MCE_IS_ERROR( status ) )
            {
            if ( MceSip::IsSIPStackError( status ) )
                {
                MceSip::DiscardRequest( session.Request(), status );
                }
            else
                {
                User::Leave( status );
                }
            }
        else
            {
            //session timer
            session.Actions().ProcessSessionTimerServerL( session.Request() );
        
            //decode received SDP offer
            //and start waiting EMceItcUpdate, if everything OK
            TMceSipWarningCode warning = session.Actions().DecodeL();

            if ( !warning )
                {
                status = session.Actions().Update();
                if ( status == KErrNotSupported )
                    {
                    warning = KMceSipWarnSessionDescriptionParameterNotUnderstood;
                    }
                }
                
            if ( warning ) //if error, reject offer (4XX out)
                {
                session.Actions().SendRejectOfferWithWarningL( warning, session.Request() );
                status = KErrAbort;
                }
            else
                {
                //if OK show SDP to FC and send session, created based on SDP
                //to client for updating
                User::LeaveIfError( status );

				if ( !session.NatSession() || session.IsNatDisabled() )
					{
					session.ContinueHandlingIncomingSessionL();
					}
                }
            }
	    }
	aEvent.ParamStatus() = status;
    }

// End Of File
