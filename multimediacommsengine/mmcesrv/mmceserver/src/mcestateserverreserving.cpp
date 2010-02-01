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





#include "mcestateserverreserving.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateServerReserving::CMceStateServerReserving
// -----------------------------------------------------------------------------
//
CMceStateServerReserving::CMceStateServerReserving ()
    : CMceState( KMceStateServerReserving )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateServerReserving::~CMceStateServerReserving
// -----------------------------------------------------------------------------
//
CMceStateServerReserving::~CMceStateServerReserving()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateServerReserving::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateServerReserving::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError(
       aEvent.Code() == EMceUpdate ||
       aEvent.Code() == EMceMediaReserved ||      
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateServerReserving::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateServerReserving::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
		
	else
		{
	    switch ( aEvent.Code() )
	        {
	        case EMceUpdate:
	            {
	            EntryUpdateL( aEvent );
	            break;
	            }
	        case EMceMediaReserved:
	            {
	            EntryMediaReservedL( aEvent );
	            break;
	            }
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            aEvent.Session().Actions().ControlMediaL( aEvent.ParamIDs(), 
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
// CMceStateServerReserving::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateServerReserving::ExitL( TMceStateTransitionEvent& aEvent )
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
	        case EMceUpdate:
	            {
	            if ( MCE_IS_ERROR( status ) )
	                {
	                session.Actions().StateChanged( KMceStateTerminated );
	                session.Actions().ClientStateChangedL( CMceSession::ETerminated );
	                }
	            else if ( status == KMceAsync )
	                {
	                // go to confirmation required state to wait
	                // update to complete. New reservation needed after that.
	                session.Actions().StateChanged( 
	                    KMceStateConfirmationRequired );
	                }
	            else // KMceReady
	                {
	                // NOP
	                }
	            break;
	            }
	        case EMceMediaReserved:
	            {
	            // other state changes are handled in entry
	            session.Actions().StateChanged( KMceStateConfirmationRequired );
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
// CMceStateServerReserving::EntryUpdateL
// -----------------------------------------------------------------------------
//
void CMceStateServerReserving::EntryUpdateL( 
                                        TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    TMceSipWarningCode warning = KErrNone;
    
    warning = session.Actions().DecodeL();
    aEvent.ParamStatus() = !warning ? KErrNone : KErrAbort;
    if ( !warning )
        {
        TMceReturnStatus status = session.Actions().UpdateL();
        aEvent.ParamStatus() = status;
        }
            
    if ( MCE_IS_ERROR( aEvent.ParamStatus() ) )
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
    
// -----------------------------------------------------------------------------
// CMceStateServerReserving::EntryMediaReservedL
// -----------------------------------------------------------------------------
//    
void CMceStateServerReserving::EntryMediaReservedL( 
                                        TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    if ( MCE_NEED_TO_RECEIVE( session ) )
        {
        // find out, is the UPDATE received yet
        CSIPServerTransaction* update = NULL;
        RStringF trx = session.Request().Type();
        if ( trx == SIPStrings::StringF( SipStrConsts::EUpdate ) )
            {
            update = &session.Request();
            }
        
        if ( update )
            {
            session.Actions().EncodeL();
            session.FCActions().UpdateFCAnswerL( *session.Offer() );
            session.Actions().SendAnswerL( *update );
            session.Actions().StateChanged( KMceStateUpdated );
            session.Actions().ClientStateChangedL( CMceSession::EProceeding,
                                                   *session.Body(),
                                                   KErrNone );        
            }
        }
    else
        {
        session.Actions().StateChanged( KMceStateUpdated );
        session.Actions().ClientStateChangedL( CMceSession::EProceeding,
                                               *session.Body(),
                                               KErrNone );        
        }
    }
    
// End of file
