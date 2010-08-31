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





#include "mcestateupdating.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mceclientserver.h"
#include "mcenatmacros.h"


// -----------------------------------------------------------------------------
// CMceStateUpdating::CMceStateUpdating
// -----------------------------------------------------------------------------
//
CMceStateUpdating::CMceStateUpdating ()
    : CMceState( KMceStateUpdating )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateUpdating::~CMceStateUpdating
// -----------------------------------------------------------------------------
//
CMceStateUpdating::~CMceStateUpdating()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateUpdating::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateUpdating::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{

    User::LeaveIfError(
      aEvent.Code() == EMceMediaUpdated ||
      aEvent.Code() == EMceItcEnable ||
      aEvent.Code() == EMceItcDisable ||
      IsExtensionRequestEvent( aEvent )
      
        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateUpdating::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateUpdating::EntryL( TMceStateTransitionEvent& aEvent )
	{
	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}

	else
		{
		CMceSipSession& session = aEvent.Session();
		CSIPServerTransaction& request = session.Request();
		CSIPServerTransaction* requestPtr = &request;
		
		switch( aEvent.Code() )
	    	{
	    	case EMceMediaUpdated:
	    	    {
	    	    if ( session.SubState() == CMceSipSession::EUpdating )
	    	        {
	    	        if ( aEvent.ParamStatus() == KErrNone )
	                    {
	                    NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
	                    session.FCActions().UpdateFCAnswerL( *session.Offer() );
	                    if ( !((requestPtr!=NULL) && !MceSip::HasContent( request )) )
	        	            { // if not pull mode, do UpdateSucceed()
	                        session.Actions().UpdateSucceed();
	        	            }
	                    session.Actions().SendAnswerL( request );
	                    }
	                else
	                    {
	        	        MceSip::DiscardRequest( request, aEvent.ParamStatus() );
	                    session.Actions().UpdateFailed();
	                    }
	                }
	            else
	                {
	                if ( aEvent.ParamStatus() == KErrNone )
	    	            {
	    	            // Send 183 with SDP if needed
	    	            if ( ( MCE_NEED_TO_RECEIVE( session ) &&
	    	                 session.Extensions().Local( CMceSipExtensions::E100rel ) 
	    	                 != CMceSipExtensions::ENotSupported &&
	    	                 session.Extensions().Remote( CMceSipExtensions::E100rel ) 
	    	                 != CMceSipExtensions::ENotSupported ) || MCE_FORCED_TO_RECEIVE( session ))
	    	                {
	    	                // encode
	    	                if ( session.ActiveBody().SecureSession() )
	                        	{
	    	                	session.ActiveBody().SecureSession()->iLSReadyToBind = EFalse;
	                        	}
	    	                NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() );
	                        session.FCActions().UpdateFCAnswerL( *session.Offer() );
	                        session.Actions().SendResponseL( 
	                                        request,
	                                        KMceSipSessionProgress,
	                                        SipStrConsts::EPhraseSessionProgress );
	                        // start reserving
	                        session.Actions().ReserveL();
	                        if ( session.ActiveBody().SecureSession() )
	                        	{
	                        	session.ActiveBody().SecureSession()->iKeyNeedUpdated = EFalse;
	                        	}
	                        // change state
	                        session.Actions().StateChanged( 
	                            KMceStateAcknowledgementRequired );
	                        }
	    	            }
	                else
	                    {
	                    EntryMediaErrorL( aEvent );
	                    }
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
	    	    break;
	    	    }
	    	    
	    	}
		}
	}

// -----------------------------------------------------------------------------
// CMceStateUpdating::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateUpdating::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}
	
	else 
		{
		CMceSipSession& session = aEvent.Session();
		TInt status = aEvent.ParamStatus();
	    CSIPServerTransaction& request = session.Request();
	    CSIPServerTransaction* requestPtr = &request;

		
		switch( aEvent.Code() )
	    	{
		    case EMceMediaUpdated:
	    	    {
	    	    if ( session.SubState() == CMceSipSession::EUpdating )
	    	        {
	    	        if ( status == KErrNone )
	    	            {
	    	            if ( (requestPtr!=NULL) && !MceSip::HasContent( request ) )
	        	            { // pull mode
	        	            session.Actions().StateChanged( KMceStateServerOffering );
	                        session.Actions().ClientStateChangedL( 
	                            CMceSession::EProceeding,
	                            session.ActiveBody(),
	                            KErrNone );  
	        	            }
	                    else
	                        {
                            if ( request.Type() == SIPStrings::StringF( SipStrConsts::EUpdate ) )
                                {
                                session.Actions().StopPendingTimer();
                                session.Actions().ClientStateChangedL( CMceSession::EEstablished, session.Request() );
                                session.Actions().StateChanged( KMceStateEstablished );  
                                }
                            else
                                {
	                            session.Actions().ClientStateChangedL( CMceSession::EProceeding );
	                            session.Actions().StateChanged( KMceStateAnswering );   
                                } 
	                        }
	    	            }
	                else
	                    {
	                    session.Actions().ClientStateChangedL( CMceSession::EEstablished, 
	                                                           *session.Body(), 
	                                                           status );
	                    session.Actions().StateChanged( KMceStateEstablished );
	                    }
	    	        }
	    	    else
	    	        {
	    	        if ( aEvent.ParamStatus() < 0 )
	    	            {
	                    session.Actions().StateChanged( KMceStateTerminated );
	                    session.Actions().ClientStateChangedL( CMceSession::ETerminated, 
	                                                           status );
	    	            }
	                else
	                    {
	                    session.Actions().StateChanged( KMceStateUpdated );
	                    session.Actions().ClientStateChangedL( CMceSession::EProceeding,
	                                                           *session.Body(),
	                                                           KErrNone );
	                    }
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

// -----------------------------------------------------------------------------
// CMceStateUpdating::EntryMediaErrorL
// -----------------------------------------------------------------------------
//
void CMceStateUpdating::EntryMediaErrorL( TMceStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
	CSIPServerTransaction& request = session.Request();
	
    MceSip::DiscardRequest( request, aEvent.ParamStatus() );
    session.Actions().StopMedia();
    session.FCActions().ReleaseFC();
    }


// End of File

