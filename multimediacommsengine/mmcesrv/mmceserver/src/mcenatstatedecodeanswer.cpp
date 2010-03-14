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



#include "mcenatstatedecodeanswer.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcenatevents.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::CMceNatStateDecodeAnswer
// -----------------------------------------------------------------------------
//
CMceNatStateDecodeAnswer::CMceNatStateDecodeAnswer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::~CMceNatStateDecodeAnswer
// -----------------------------------------------------------------------------
//
CMceNatStateDecodeAnswer::~CMceNatStateDecodeAnswer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::Accept 
// -----------------------------------------------------------------------------
//
TBool CMceNatStateDecodeAnswer::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    if ( EMceNatAnswer == aEvent.NatCode() ||
         EMceNatError == aEvent.NatCode() ||
         ( ( EMceAck == aEvent.Code() ) &&
             MceSip::HasContent( aEvent.Session().Request() )) ||
        EMceNatICMPError == aEvent.NatCode() )
            
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeAnswer::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateDecodeAnswer::EntryL, Entry" );
	
	TMceReturnStatus status = KMceAsync;
	status = aEvent.NatActions().DecodeAnswerL();
	
	if ( KMceReady == status )
	    {
	    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateConnected );
	    }
    else
        {
        aEvent.ParamStatus() = status;
        }
    
	MCESRV_DEBUG( "*** CMceNatStateDecodeAnswer::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeAnswer::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateDecodeAnswer::ExitL, Entry" );
	
	switch ( aEvent.NatCode() )
	    {
	    case EMceNatError:
	    case EMceNatICMPError:
	        {
	        ExitErrorL( aEvent );
	        break; 
	        }
	    default:
	        {
	        ExitDefaultL( aEvent );
	        break;
	        }
	    }
	
    MCESRV_DEBUG( "*** CMceNatStateDecodeAnswer::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeAnswer::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
    {
    TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
    
    if (( aEvent.NatCode() == EMceNatICMPError && 
    		KMceStateEstablished == sipState ) || 
    		aEvent.NatCode() == EMceNatError )
	    
    	{
    	aEvent.Session().Actions().ClientErrorOccured( aEvent.ParamStatus() );
    	aEvent.NatActions().Continue();
    	User::Leave( aEvent.ParamStatus() );		
    	}
    }

// -----------------------------------------------------------------------------
// CMceNatStateDecodeAnswer::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeAnswer::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
   	User::LeaveIfNull( aEvent.Document() );
    CMceSipSession& session = aEvent.Session();

	CSdpDocument* previous = session.Offer();
	aEvent.Session().SetOffer( aEvent.Document() );
	aEvent.NatActions().SetOffered( KMceNatOfferSdp );
	aEvent.NatActions().SdpCleanup( previous, session.Offer() );
	if( EMceProvisionalResponse == aEvent.Code() )
		{
		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateWait );		
		}
	else
		{
		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateConnected );		
		}
    }

// End of File

