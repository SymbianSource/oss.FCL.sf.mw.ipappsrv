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




#include <sipdialogassocbase.h> 
#include <sipdialog.h>
#include <sipconnection.h>
#include "mcestate.h"
#include "mceactionset.h"
#include "mcesipsession.h"
#include "mcecomsession.h"
#include "mcesipconnection.h"
#include "mceevents.h"
#include "mcesip.h"
#include "mcesrvlogs.h"

#define MARK_EXIT ETrue

// -----------------------------------------------------------------------------
// CMceState::CMceState
// -----------------------------------------------------------------------------
//
CMceState::CMceState ( TMceStateIndex aId )
    : iId( aId )
	{
	}

// -----------------------------------------------------------------------------
// CMceState::CMceState
// -----------------------------------------------------------------------------
//
TMceStateIndex CMceState::Id ()
	{
	return iId;
	}

// -----------------------------------------------------------------------------
// CMceState::~CMceState
// -----------------------------------------------------------------------------
//
CMceState::~CMceState()
	{
	}

// -----------------------------------------------------------------------------
// CMceState::AcceptL
// -----------------------------------------------------------------------------
//
TBool CMceState::AcceptL( TMceStateTransitionEvent& aEvent )
    {
    TBool ignore = EFalse; 
    
    if ( aEvent.Session().SIPConnection().State() == CSIPConnection::EActive )
        {
    	if ( aEvent.Code() == EMceProvisionalResponse && 
    	     MceSip::ResponseCode( aEvent.Session().Response())==KMceSipTrying )
	        {
	        ignore = ETrue;
	        }
		else
			{
			ignore = !DoAcceptL( aEvent );
			}
        }
    else
        {
        User::Leave( KErrNotReady );
        }

    return !ignore;
                    
    }

// -----------------------------------------------------------------------------
// CMceState::StateEntry
// -----------------------------------------------------------------------------
//
void CMceState::StateEntry( TMceStateTransitionEvent& /*aEvent*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMceState::StateExitL
// -----------------------------------------------------------------------------
//
void CMceState::StateExitL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
    User::LeaveIfNull( &session );
	
    session.CurrentState( MARK_EXIT );
    
    }

// -----------------------------------------------------------------------------
// CMceState::MediaErrorOccured
// -----------------------------------------------------------------------------
//
void CMceState::MediaErrorOccured( TMceStateTransitionEvent& /*aEvent*/ )
	{
	// NOP
	}
	



// -----------------------------------------------------------------------------
// CMceState::IsExtensionRequestEvent
// -----------------------------------------------------------------------------
//

TBool CMceState::IsExtensionRequestEvent( TMceStateTransitionEvent& aEvent ) 
	{
	MCESRV_DEBUG("CMceState::IsExtensionRequestEvent, Entryt");
	TBool extensionRequest = EFalse;
	CMceSipSession& session = aEvent.Session();

	switch ( aEvent.Code() )
        {
        case EMceResponse:
        case EMceProvisionalResponse:
        case EMceRedirectionResponse:
        case EMceErrorResponse:
            {
            if ( &session.Response() )
            	{
            	RStringF method = session.Response().Type();
            	extensionRequest = session.Actions().IsExtensionRequest( method );		
            	}
            break;
            }
            
        case EMceItcRequestSend:
        case EMceItcReplySend:
        case EMceRequest:
        	{
        	extensionRequest = ETrue;	
        	break;	
        	}

   		default:
            {
            //NO OP
            break;
            }
        }

    
	MCESRV_DEBUG("CMceState::IsExtensionRequestEvent, Exit");	

	return extensionRequest;
	}


// -----------------------------------------------------------------------------
// CMceState::HandleExtensionRequestEvent
// -----------------------------------------------------------------------------
//

void CMceState::HandleExtensionRequestEventL( TMceStateTransitionEvent& aEvent )

	{
	//Handle Non-Target Refresh Request Here
	MCESRV_DEBUG("CMceState::HandleExtensionRequestEventL, Entryt");

	switch ( aEvent.Code() )
        {
        case EMceResponse:
        case EMceProvisionalResponse:
        case EMceRedirectionResponse:
        case EMceErrorResponse:
            {
            EntryReceiveExtensionResponseL( aEvent );
            break;
            }
        
        case EMceItcReplySend:
        	{
        	EntrySendExtensionResponseL ( aEvent );
        	break;	
        	}
        	
        case EMceItcRequestSend:	
        	{
        	EntrySendExtensionRequestL ( aEvent );
        	break;	
        	}
       	
        case EMceRequest:
        	{
        	EntryReceiveExtensionRequestL ( aEvent );
        	break;	
        	}

		default:
            {
            break;
            }        	
        }
		
	MCESRV_DEBUG("CMceState::HandleExtensionRequestEvent, Exit");	
	}


// -----------------------------------------------------------------------------
// CMceState::EntrySendExtensionRequestL
// -----------------------------------------------------------------------------
//
void CMceState::EntrySendExtensionRequestL( TMceStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG("CMceState::EntrySendExtensionRequestL, Entryt");                           		
	CMceSipSession& session = aEvent.Session();

	if (( session.Dialog()->Dialog().State() != CSIPDialog::EInit ) &&
		( session.Dialog()->Dialog().State() != CSIPDialog::ETerminated )) 
    
		{
		session.Actions().SendExtensionRequestL( aEvent.ParamClientMessage(), 
											  	aEvent.ParamIDs());
        }
    else
    	{
    	User::Leave( KErrTotalLossOfPrecision );
    	}
  	
	MCESRV_DEBUG("CMceState::EntrySendExtensionRequestL, Exit");                           			
	}



// -----------------------------------------------------------------------------
// CMceState::EntrySendExtensionResponseL
// -----------------------------------------------------------------------------
//
void CMceState::EntrySendExtensionResponseL( TMceStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG("CMceState::EntrySendExtensionResponseL, Entryt");   
	CMceSipSession& session = aEvent.Session();

	session.Actions().SendExtensionResponseL( aEvent.ParamClientMessage(),
											aEvent.ParamIDs() );
						
	MCESRV_DEBUG("CMceState::EntrySendExtensionResponseL, Exit");                           			
	}



// -----------------------------------------------------------------------------
// CMceState::EntryReceiveExtensionRequestL
// -----------------------------------------------------------------------------
//
void CMceState::EntryReceiveExtensionRequestL( TMceStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG("CMceState::EntryReceiveExtensionRequestL, Entryt");   
	CMceSipSession& session = aEvent.Session();

	session.Actions().ReceiveExtensionRequestL( aEvent );		

	MCESRV_DEBUG("CMceState::EntryReceiveExtensionRequestL, Exit");                           			
	
	}



// -----------------------------------------------------------------------------
// CMceState::EntryReceiveExtensionResponseL
// -----------------------------------------------------------------------------
//
void CMceState::EntryReceiveExtensionResponseL( TMceStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG("CMceState::EntryReceiveExtensionResponseL, Entryt");                           		
	CMceSipSession& session = aEvent.Session();
	

	if (( session.Dialog()->Dialog().State() != CSIPDialog::EInit ) &&
		( session.Dialog()->Dialog().State() != CSIPDialog::ETerminated )) 
		
		{
		session.Actions().ReceiveExtensionResponseL( session.Response(), *session.Body() );
		}

    else
    	{
    	User::Leave( KErrTotalLossOfPrecision );
    	}
		  	
	MCESRV_DEBUG("CMceState::EntryReceiveExtensionResponseL, Exit");                           			
	}
	
	

// End of File

