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





#include "mcestatecanceled.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateCanceled::CMceStateCanceled
// -----------------------------------------------------------------------------
//
CMceStateCanceled::CMceStateCanceled ()
    : CMceState( KMceStateCanceled )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateCanceled::~CMceStateCanceled
// -----------------------------------------------------------------------------
//
CMceStateCanceled::~CMceStateCanceled()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateCanceled::AcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateCanceled::AcceptL( TMceStateTransitionEvent& aEvent )
    {
    TBool accept = CMceState::AcceptL( aEvent );
    
    if ( !accept && aEvent.Code() == EMceProvisionalResponse )//100 Trying
        {
        accept = ETrue;
        }
        
    return accept;
                    
    }


// -----------------------------------------------------------------------------
// CMceStateCanceled::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateCanceled::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    //not accepted	
	User::LeaveIfError( aEvent.EventSource() == EMceClient ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );


    if ( aEvent.EventSource() == EMceMedia || 
         aEvent.Code() == EMceRedirectionResponse )
        {
        //ignore
        return EFalse;
        }
    else
        {
    	return ETrue;
        }
	
	}

// -----------------------------------------------------------------------------
// CMceStateCanceled::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateCanceled::EntryL( TMceStateTransitionEvent& aEvent )
	{

	CMceSipSession& session = aEvent.Session();
	RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );

    switch ( aEvent.Code() )
        {
        case EMceResponse:
            {
            if ( MceSip::IsResponseTo( session.Response(), invite ) )
                {
                session.Actions().SendACKL( session.Response() );
                if ( session.SubState() == CMceSipSession::EOffering )
                    {
                    session.Actions().SendBYEL();
                    }
                }
            else//200OK to CANCEL
                {
                //NOP
                }
            break;
            }
        case EMceProvisionalResponse:
            {
            //if CANCEL is not send yet
            if ( !session.PendingTransaction( SIPStrings::StringF( SipStrConsts::ECancel ) ) )
                {
                session.Actions().SendCancelL();
                }
            break;
            }
        case EMceErrorResponse://487
        default:
            {
            }
        }

    
	}

// -----------------------------------------------------------------------------
// CMceStateCanceled::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateCanceled::ExitL( TMceStateTransitionEvent& aEvent)
	{
	
	CMceSipSession& session = aEvent.Session();
	RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );

    if ( aEvent.Code() != EMceProvisionalResponse )
        {
        if ( session.SubState() == CMceSipSession::EOffering )
            {
            if ( aEvent.Code() == EMceResponse && 
                 MceSip::IsResponseTo( session.Response(), invite ) )
                {
                session.Actions().StateChanged( KMceStateTerminating );
                session.Actions().ClientStateChangedL( CMceSession::ETerminating, 
                                                       session.Response() );    
                }
            else 
                {
                session.Actions().StateChanged( KMceStateTerminated );
                session.Actions().ClientStateChangedL( CMceSession::ETerminated, 
                                                       session.Response() );    
                }
            }
        else
            {
            session.Actions().StateChanged( KMceStateEstablished );
            session.Actions().ClientStateChangedL( CMceSession::EEstablished,
                                                   *session.Body(),
                                                   session.Response() );	
            }
        }
	
	}

