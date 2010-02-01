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





#include "mcestateconfirmationrequired.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateConfirmationRequired::CMceStateConfirmationRequired
// -----------------------------------------------------------------------------
//
CMceStateConfirmationRequired::CMceStateConfirmationRequired ()
    : CMceState( KMceStateConfirmationRequired )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateConfirmationRequired::~CMceStateConfirmationRequired
// -----------------------------------------------------------------------------
//
CMceStateConfirmationRequired::~CMceStateConfirmationRequired()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateConfirmationRequired::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateConfirmationRequired::DoAcceptL( 
                                            TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError(
       aEvent.Code() == EMceUpdate ||
       aEvent.Code() == EMceMediaUpdated  ||      
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable ||
       aEvent.Code() == EMceItcRejectSession ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateConfirmationRequired::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateConfirmationRequired::EntryL( TMceStateTransitionEvent& aEvent )
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
	        case EMceMediaUpdated:
	            {
	            EntryMediaUpdatedL( aEvent );
	            break;
	            }
	        case EMceItcEnable:
	        case EMceItcDisable:
	            {
	            aEvent.Session().Actions().ControlMediaL( aEvent.ParamIDs(), 
	                                             (TMceItcFunctions)aEvent.Code() );
	            break;
	            } 
	        case EMceItcRejectSession:
	            {
	            aEvent.Session().Actions().SendDeclineL( 
            								   aEvent.Session().Request(), 
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
// CMceStateConfirmationRequired::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateConfirmationRequired::ExitL( TMceStateTransitionEvent& aEvent )
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
	                // TBD: update failed, change state
	                }
	            // if update was async, wait in this state for media
	            // updated event. If update is ready, entered with media updated.
	            break;
	            }
	        case EMceMediaUpdated:
	            {
	            if ( MCE_IS_ERROR( status ) )
	                {
	                // TBD: reservation failed, need to change state
	                }
	            else if ( status == KMceAsync )
	                {
	                // reserving, go to server reserving
	                session.Actions().StateChanged( KMceStateServerReserving );
	                }
	            else // ready
	                {
	                session.Actions().StateChanged( KMceStateUpdated );
	                session.Actions().ClientStateChangedL( 
	                                               CMceSession::EProceeding,
	                                               *session.Body(),
	                                               KErrNone );        
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
	            // NOP
	            break;
	            }
	        }
		}
	}

// -----------------------------------------------------------------------------
// CMceStateConfirmationRequired::EntryUpdateL
// -----------------------------------------------------------------------------
//
void CMceStateConfirmationRequired::EntryUpdateL( 
                                        TMceStateTransitionEvent& aEvent )
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
// CMceStateConfirmationRequired::EntryMediaUpdatedL
// -----------------------------------------------------------------------------
//    
void CMceStateConfirmationRequired::EntryMediaUpdatedL( 
                                        TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    
    TMceReturnStatus status = session.Actions().ReserveL();
    aEvent.ParamStatus() = status;
    if ( MCE_IS_ERROR( status ) )
        {
        // TBD: reservation failed
        }
    else if ( status == KMceAsync )
        {
        // NOP
        }
    else // ready, send 2xx to update
        {
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
            }
        else
            {
            User::Leave( KErrTotalLossOfPrecision );
            }
        }
    }
// End of file
