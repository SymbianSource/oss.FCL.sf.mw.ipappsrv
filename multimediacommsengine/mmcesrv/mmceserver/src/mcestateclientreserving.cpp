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





#include "mcestateclientreserving.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcefcactionset.h"

// -----------------------------------------------------------------------------
// CMceStateClientReserving::CMceStateClientReserving
// -----------------------------------------------------------------------------
//
CMceStateClientReserving::CMceStateClientReserving()
    : CMceState( KMceStateClientReserving )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateClientReserving::~CMceStateClientReserving
// -----------------------------------------------------------------------------
//
CMceStateClientReserving::~CMceStateClientReserving()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateClientReserving::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateClientReserving::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError( 	                 
       aEvent.Code() == EMceItcCancel ||
       aEvent.Code() == EMceMediaReserved ||      
       aEvent.Code() == EMceItcEnable ||
       aEvent.Code() == EMceItcDisable ||
       IsExtensionRequestEvent( aEvent )
       
       ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateClientReserving::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateClientReserving::EntryL( TMceStateTransitionEvent& aEvent )
	{
	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}

	else
		{
	    switch( aEvent.Code() )
	        {
	        case EMceMediaReserved:
	            {
	            EntryMediaReservedL( aEvent );
	            break;
	            }
	        case EMceItcCancel:
	            {
	            EntryCancelL( aEvent );
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
// CMceStateClientReserving::EntryMediaReservedL
// -----------------------------------------------------------------------------
//
void CMceStateClientReserving::EntryMediaReservedL( 
                                        TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CSIPClientTransaction* invite = session.PendingTransaction( 
                            SIPStrings::StringF( SipStrConsts::EInvite ) );
    User::LeaveIfError( invite ? KErrNone : KErrTotalLossOfPrecision );
    
    if ( invite->StateL() == CSIPTransactionBase::ETerminated )
        {
        if ( MCE_NEED_TO_SEND( session, NULL ) )
            {
            // create the SDP
            session.Actions().EncodeL();
            // update FC
            session.FCActions().UpdateFCOfferL( *session.Offer() );
            // send re-INVITE
            session.Actions().SendInviteL();
            }
        else
            {
            // Just go to established state
            session.Actions().ClientStateChangedL( CMceSession::EEstablished );
            session.Actions().StateChanged( KMceStateEstablished );
            }
        }
    else
        {
        // create SDP
        session.Actions().EncodeL();
        // update FC
        session.FCActions().UpdateFCOfferL( *session.Offer() );    
        // send update
        session.Actions().SendUpdateRequestL();    
        }
    }

// -----------------------------------------------------------------------------
// CMceStateClientReserving::EntryCancelL
// -----------------------------------------------------------------------------
//    
void CMceStateClientReserving::EntryCancelL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    CMceSipSession::TSubState subState = session.SubState();

    //(try) send CANCEL
    session.Actions().SendCancelL();

    if ( subState == CMceSipSession::EOffering )
        {
        session.Actions().StopMedia();
        session.FCActions().ReleaseFC();
        }
    else if ( subState == CMceSipSession::EUpdating )
        {
        session.Actions().UpdateFailed();
        }    
    }

// -----------------------------------------------------------------------------
// CMceStateClientReserving::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateClientReserving::ExitL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}

	else
		{
		CMceSipSession& session = aEvent.Session();
	    
		switch( aEvent.Code() )
	        {
	        case EMceItcCancel:
	            {
	            session.Actions().StateChanged( KMceStateCanceled );
	            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                  CMceSession::ECancelling );
	            
	            break;
	            }
	        case EMceMediaReserved:
	            {
	            CSIPClientTransaction* update = session.PendingTransaction(
	                            SIPStrings::StringF( SipStrConsts::EUpdate ) );
	            if ( update )
	                {
	                // just sent UPDATE
	                session.Actions().StateChanged( KMceStateConfirming );
	                }
	            else
	                {
	                session.Actions().StateChanged( KMceStateOffering );
	                }
	            break;
	            }
	        default:
	            {
	            break;
	            }
	        }
		}
	}
    
// End of file
