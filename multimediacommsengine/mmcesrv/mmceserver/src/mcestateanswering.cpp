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





#include "mcestateanswering.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateAnswering::CMceStateAnswering
// -----------------------------------------------------------------------------
//
CMceStateAnswering::CMceStateAnswering ()
    : CMceState( KMceStateAnswering)
	{
	}


// -----------------------------------------------------------------------------
// CMceStateAnswering::~CMceStateAnswering
// -----------------------------------------------------------------------------
//
CMceStateAnswering::~CMceStateAnswering()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateAnswering::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateAnswering::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    User::LeaveIfError(
      aEvent.Code() == EMceItcEnable  ||
      aEvent.Code() == EMceItcDisable ||
      aEvent.Code() == EMceAck ||
      aEvent.Code() == EMceBye ||
      IsExtensionRequestEvent( aEvent )
      
        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateAnswering::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateAnswering::EntryL( TMceStateTransitionEvent& aEvent )
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
// CMceStateAnswering::StateEntry
// -----------------------------------------------------------------------------
//
void CMceStateAnswering::StateEntry( TMceStateTransitionEvent& aEvent )
    {
    CMceState::StateEntry( aEvent );
    
	CMceSipSession& session = aEvent.Session();
    
    session.Actions().StopPendingTimer();
    
    }

// -----------------------------------------------------------------------------
// CMceStateAnswering::EntryByeL
// -----------------------------------------------------------------------------
//
void CMceStateAnswering::EntryByeL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	
    //send 200OK and stop media and FC
    session.Actions().Send200OKL( session.Request() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
	
    }
        
// -----------------------------------------------------------------------------
// CMceStateAnswering::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateAnswering::ExitL( TMceStateTransitionEvent& aEvent )
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
	        //received BYE causes server & client to terminated state
	        case EMceBye:
	            {
	            session.Actions().StateChanged( KMceStateTerminated );
	            session.Actions().ClientStateChangedL( 
	                CMceSession::ETerminated, session.Request() );
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
			    session.Actions().StateChanged( KMceStateEstablished );
			    session.Actions().ClientStateChangedL( CMceSession::EEstablished, session.Request() );    
	            }
	        }
		}
	}




// End of File

