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





#include "mcestateterminating.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateTerminating::CMceStateTerminating
// -----------------------------------------------------------------------------
//
CMceStateTerminating::CMceStateTerminating ()
    : CMceState( KMceStateTerminating )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateTerminating::~CMceStateTerminating
// -----------------------------------------------------------------------------
//
CMceStateTerminating::~CMceStateTerminating()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateTerminating::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateTerminating::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
    // EMceBye event can be received in terminating state 
    // when both MO and MT side terminate the session at the same time
    User::LeaveIfError( 	                 
       aEvent.Code() == EMceAck  ||
       aEvent.Code() == EMceResponse  ||
       aEvent.Code() == EMceErrorResponse ||
       aEvent.Code() == EMceProvisionalResponse ||
       aEvent.Code() == EMceMediaSessionStopped ||
       aEvent.Code() == EMceBye ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
  
    //ignore EMceMediaSessionStopped
    //and final responses other than response to BYE
    TBool ignore = aEvent.Code() == EMceMediaSessionStopped ||
                   aEvent.Code() == EMceAck ||
                   ( aEvent.Code() != EMceBye &&
                     !MceSip::IsResponseTo( aEvent.Session().Response(), 
                        SIPStrings::StringF( SipStrConsts::EBye )) );
                  
	return !ignore;	
	}

// -----------------------------------------------------------------------------
// CMceStateTerminating::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateTerminating::EntryL( TMceStateTransitionEvent& aEvent )
	{
	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}
		
	
	// when bye event is received, send the 200 ok for bye.
	else if ( aEvent.Code() == EMceBye )
    	{
        EntryByeL( aEvent );	    
    	}
	}

// -----------------------------------------------------------------------------
// CMceStateTerminating::EntryByeL
// -----------------------------------------------------------------------------
//
void CMceStateTerminating::EntryByeL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	
    //send 200OK and stop media and FC
    session.Actions().Send200OKL( session.Request() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
    }
    
// -----------------------------------------------------------------------------
// CMceStateTerminating::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateTerminating::ExitL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}
	
	else
		{
		CMceSipSession& session = aEvent.Session();
		
	    session.Actions().StateChanged( KMceStateTerminated );
	    
	    if ( aEvent.Code() == EMceBye )
	        {
	        session.Actions().ClientStateChangedL( CMceSession::ETerminated, session.Request() );
	        }
	    else
	        {
	        session.Actions().ClientStateChangedL( CMceSession::ETerminated, session.Response() );
	        }
		}
	}



// End of File

