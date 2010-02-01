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




#include "mcestateserverestablishing.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mcenatmacros.h"

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::CMceStateServerEstablishing
// -----------------------------------------------------------------------------
//
CMceStateServerEstablishing::CMceStateServerEstablishing ()
    : CMceState( KMceStateServerEstablishing )
	{
	}

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::~CMceStateServerEstablishing
// -----------------------------------------------------------------------------
//
CMceStateServerEstablishing::~CMceStateServerEstablishing()
	{
	}

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateServerEstablishing::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
    //not accepted	
	User::LeaveIfError( aEvent.EventSource() == EMceSip ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );

    User::LeaveIfError(
        aEvent.Code() == EMceItcUpdate ||
        aEvent.Code() == EMceItcRejectSession ||      
        aEvent.Code() == EMceItcEnable ||
        aEvent.Code() == EMceItcDisable ||
        IsExtensionRequestEvent( aEvent )

        ? KErrNone : KErrTotalLossOfPrecision );
    
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateServerEstablishing::EntryL( TMceStateTransitionEvent& aEvent )
	{

	if ( IsExtensionRequestEvent( aEvent ))
		{
		HandleExtensionRequestEventL( aEvent );	
		}

	else
		{
		CMceSipSession& session = aEvent.Session();
		CSIPServerTransaction& request = session.Request();
	    CMceSipSession::TSubState subState = session.SubState();
		
		switch( aEvent.Code() )
	        {
	        case EMceItcUpdate:
	            {
	            EntryUpdateL( aEvent );
	            break;
	            }
	        case EMceItcRejectSession:
	            {
	            CDesC8Array* tempHeaders = session.Body()->iSIPHeaders;
	            CleanupStack::PushL( tempHeaders );
	            session.Body()->iSIPHeaders = 0;
	            session.Actions().SendDeclineL( request, aEvent.ParamClientMessage() );
	            
	            if ( !session.Body()->iSIPHeaders )
	                {
	                session.Body()->iSIPHeaders = tempHeaders;
	                CleanupStack::Pop( tempHeaders );
	                }
	            else
	                {
	                CleanupStack::PopAndDestroy( tempHeaders );
	                }
	            
	            if ( subState == CMceSipSession::EOffering )
	                {
	                session.Actions().StopMedia();
	                session.FCActions().ReleaseFC();
	                }
	            else if ( subState == CMceSipSession::EUpdating )
	                {
	                session.Actions().UpdateFailed();
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
	            //NOP
	            break;
	            }
	        } 
		}
	}

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::EntryUpdateL
// -----------------------------------------------------------------------------
//
void
CMceStateServerEstablishing::EntryUpdateL( TMceStateTransitionEvent& aEvent )
    {
	CMceSipSession& session = aEvent.Session();
	CSIPServerTransaction& request = session.Request();
    CMceSipSession::TSubState subState = session.SubState();
	TMceReturnStatus status = KErrNone;

    //update body
    session.Actions().UpdateBodyL( aEvent.ParamIDs(), 
                                   aEvent.ParamClientMessage() );
    //update MM                                  
    status = session.Actions().Update();
	
    if ( status == KMceReady )
        {
        // Send 183 with SDP if needed
        if ( MCE_NEED_TO_RECEIVE( session ) &&
             session.Extensions().Local( CMceSipExtensions::E100rel ) 
             != CMceSipExtensions::ENotSupported &&
             session.Extensions().Remote( CMceSipExtensions::E100rel ) 
             != CMceSipExtensions::ENotSupported )
            {
            // encode
            NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() )
            
            session.FCActions().UpdateFCAnswerL( *session.Offer() );
            session.Actions().SendResponseL( 
                            request,
                            KMceSipSessionProgress,
                            SipStrConsts::EPhraseSessionProgress );
           //For Long Sesion after this no need to updated key until session
           //established
           if ( session.ActiveBody().SecureSession() )
            	{
            	session.ActiveBody().SecureSession()->iKeyNeedUpdated = EFalse;
            	}                
            // start reserving
           session.Actions().ReserveL();
            // change state
           session.Actions().StateChanged( 
                KMceStateAcknowledgementRequired );
            }
        else
            {
            if ( subState != CMceSipSession::EAnswering )
                {
                // directly send 200 OK
                // encode body
                NAT_WAIT_NO_EXEC( session, session.Actions().EncodeL() )
                
                session.Actions().UpdateSucceed(); //no affect if answering
                session.FCActions().UpdateFCAnswerL( *session.Offer() );
                session.Actions().SendAnswerL( request );
                }
            }
        }
    else if ( MCE_IS_ERROR( status ) )
        {
        MceSip::DiscardRequest( request, KMceSipServerInternalError );
        
        if ( subState == CMceSipSession::EAnswering )
            {
			session.Actions().StopMedia();
            session.FCActions().ReleaseFC();
            //exit same as if client rejects
            aEvent.Code() = EMceItcRejectSession;
            }
        else if ( subState == CMceSipSession::EUpdating )
            {
            session.Actions().UpdateFailed();
            }
        else
            {
            //NOP
            }
        }

    aEvent.ParamStatus() = status;
    }

// -----------------------------------------------------------------------------
// CMceStateServerEstablishing::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateServerEstablishing::ExitL( TMceStateTransitionEvent& aEvent )
	{
	
	if ( IsExtensionRequestEvent( aEvent ))
		{
		//Do Nothing;
		}
	
	else
		{
		CMceSipSession& session = aEvent.Session();
		TInt status = aEvent.ParamStatus();
	    CMceSipSession::TSubState subState = session.SubState();

	    switch ( aEvent.Code() )
	        {
	        case EMceItcUpdate:
	            {
	            if ( status < KErrNone ) //only in updating
	                {
	                //Client must use get session function get the original
	                session.Actions().StateChanged( KMceStateEstablished );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EEstablished,
	                                                      status );
	                }
	            else if ( status == KMceAsync ) //both first and update
	                {
	                session.Actions().StateChanged( KMceStateUpdating );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EReserving );
	                }
	            else //KReady
	                {
	                if ( subState == CMceSipSession::EAnswering )
	                    {
	                    // first INVITE, need to wait client to accept
	                                        									
						session.Actions().StateChanged( KMceStateUpdated );					
										
						session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
																CMceSession::EReserving );
											
	                    session.Actions().ClientStateChangedL( CMceSession::EProceeding );
	                   	                 
	                    }
	                else
	                    {
                        if ( session.Request().Type() == SIPStrings::StringF( SipStrConsts::EUpdate ) )
                            {
                            // UPDATE, not refreshing
                            session.Actions().StateChanged( KMceStateEstablished );
                            session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
                                                                  CMceSession::EEstablished,
                                                                  status );
                            session.Actions().ClientStateChangedL( CMceSession::EEstablished, session.Request() );
                            }
                        else
                            {
	                        // re-INVITE, answer sent
	                        session.Actions().StateChanged( KMceStateAnswering );
	                        session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                          CMceSession::EAnswering );
                            }
	                    }
	                }
	            break;
	            }
	        case EMceItcRejectSession:
	            {
	            if ( subState == CMceSipSession::EAnswering )
	                { // original session rejected
	                session.Actions().StateChanged( KMceStateTerminated );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::ETerminated,
	                                                      status );
	                }
	            else 
	                { // update rejected
	                session.Actions().StateChanged( KMceStateEstablished );
	                session.Actions().ClientStateChanged( aEvent.ParamIDs(), 
	                                                      CMceSession::EEstablished );
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
	}


// End of file

