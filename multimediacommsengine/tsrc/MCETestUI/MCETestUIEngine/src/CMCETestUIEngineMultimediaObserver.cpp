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
* Description:    Implementation.
*
*/




#include <mcedefs.h>
#include "CMCETestUIEngineMultimediaObserver.h"
#include "TMCETestUIEngineCmdAcceptSession.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIEngine.h"

#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineAudioStream.h"
#include "MCETestUIEngineConstants.h"
#include "TMCETestUIEngineCmdAnswerSession.h"
#include "mcetransactiondatacontainer.h"

#include <MCEInSession.h>
#include <MCEAudioStream.h>
#include <MCESpeakerSink.h>

#include <MCERtpSource.h>
#include <MCEMicSource.h>
#include <MCEFileSource.h>
#include <MCEMediaSource.h>
#include <e32err.h>
#include <e32debug.h>

#include "MCETestUIDebug.h"	


// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIEngineMultimediaObserver* CMCETestUIEngineMultimediaObserver::NewL( 
			CMCETestUIEngine& aEngine )
	{
	CMCETestUIEngineMultimediaObserver* self = 
			new (ELeave) CMCETestUIEngineMultimediaObserver( aEngine );
			
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::CMCETestUIEngineMultimediaObserver
// -----------------------------------------------------------------------------
//
CMCETestUIEngineMultimediaObserver::CMCETestUIEngineMultimediaObserver(
			CMCETestUIEngine& aEngine):
	iEngine( aEngine )
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::ConstructL()
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::~CMCETestUIEngineMultimediaObserver
// -----------------------------------------------------------------------------
//
CMCETestUIEngineMultimediaObserver::~CMCETestUIEngineMultimediaObserver()
	{
	}
		

// -- From MMCEInSessionObserver--------------------------------------------------
	

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::IncomingSession
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::IncomingSession(
			CMceInSession* aSession,
            TMceTransactionDataContainer* /*aContainer*/ )
	{
	if(iEngine.iAutoResponse)
		{
		TInt state = aSession->State();
    
		switch( state ) 
			{
			case CMceSession::EIncoming: 
			    {
			    aSession->UpdateL();
				break;
			    }
			    
			default: 
			    {    
			    break;
			    }
			}  
		}
	else if(iEngine.iVoIPFlag)	
		{
		TInt state = aSession->State();
    
		switch( state ) 
			{
			case CMceSession::EIncoming: 
			    {
			    iEngine.NotifyUser( KMiscIncomingVoIPSession );
			    iEngine.UpdateIncomingSessionVoIPDataL(aSession);
			  	break;
			    }
			    
			default: 
			    {    
			    break;
			    }
			}  
		}
	else
		{
		CMCETestUIEngineInSession& engineSession = 
	    iEngine.CreateInSessionL( aSession ); 
		}
	
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::IncomingUpdate
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* /*aContainer*/ )
	{
//	CMCETestUIEngineInSession& engineSession = 
//	    iEngine.CreateInSessionL( aUpdatedSession); 
	iEngine.IncomingUpdateL( aOrigSession, aUpdatedSession )	;
	}

// -- From MMCESessionObserver -------------------------------------------------

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::SessionStateChanged
// -----------------------------------------------------------------------------
//	
void CMCETestUIEngineMultimediaObserver::SessionStateChanged(
            CMceSession& aSession,
            TMceTransactionDataContainer* aContainer )
	{
	if(iEngine.iAutoResponse)
		{
		TInt state = aSession.State();
    
		
		switch( state ) 
			{
			case CMceSession::EProceeding: 
			    {
			    CMceInSession& inSession = 
                                    static_cast<CMceInSession&>(aSession);
                inSession.AcceptL();
			    break;
			    }
			    
			default: 
			    {    
			    break;
			    }
			}  
		}
	else if(iEngine.iVoIPFlag)
		{
		TInt state = aSession.State();
    	switch( state ) 
			{
			case CMceSession::EProceeding: 
			    {
			    CMceInSession& inSession = 
                                    static_cast<CMceInSession&>(aSession);
                inSession.RingL();
                inSession.AcceptL();
                iEngine.UpdateIncomingSessionVoIPDataAfterAnswerL(inSession);             
                
			    break;
			    }
			case CMceSession::EEstablished: 
				{
				CMceInSession& inSession = 
                                    static_cast<CMceInSession&>(aSession);
//				iEngine.UpdateIncomingSessionVoIPDataAfterAnswerL(inSession);
                CMCETestUIEngineInSession* session = CMCETestUIEngineInSession::NewL(
			                                                           iEngine, &inSession );
			    CleanupStack::PushL( session );
			    iEngine.iInSessions.Append( session );
				iEngine.EngineStateChangedL();	
				CleanupStack::Pop( session );
				}
			default: 
			    {    
			    break;
			    }
			}  
		}
    if( CMceSession::EEstablished == aSession.State() )
        {
        const RPointerArray<CMCETestUIEngineOutSession>& outsessions = 
                                                        iEngine.OutSessions();     
        const RPointerArray<CMCETestUIEngineInSession>& insessions = 
                                                        iEngine.InSessions();

        for ( TInt i = 0; i < outsessions.Count(); ++i )
            {
            if ( &outsessions[i]->Session() == &aSession && outsessions[i]->iHoldInProgress  )
                {
                outsessions[i]->EnableAudioL();
                outsessions[i]->iHoldInProgress = EFalse;
                }
            }
        for ( TInt i = 0; i < insessions.Count(); ++i )
            {
            if ( &insessions[i]->Session() == &aSession && insessions[i]->iHoldInProgress )
                {
                insessions[i]->EnableAudioL();
                insessions[i]->iHoldInProgress = EFalse;
                }
            }
        }
	    
	
	
	
	HBufC8* hbuf = aContainer->GetReasonPhrase();
	if(hbuf)
		{
		HBufC* buf = HBufC::NewL(hbuf->Length());
		TPtr ptr = buf->Des();
		ptr.Copy(*hbuf);
		TBuf16<100> buf16;
		buf16.Append(KUserNotificationSessionStateChanged );
		buf16.Append(KSpace);
		TInt code = aContainer->GetStatusCode();
	    buf16.AppendNum(code);
	    buf16.Append(*buf);
	 	iEngine.NotifyUser(buf16);
	  	delete buf;
		}
	else
		{
		iEngine.NotifyUser( KUserNotificationSessionStateChanged );	
		}	

	delete hbuf;
	iEngine.EngineStateChangedL();	
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::SessionConnectionStateChanged
// -----------------------------------------------------------------------------
//		
void CMCETestUIEngineMultimediaObserver::SessionConnectionStateChanged(
            CMceSession& /*aSession*/,
            TBool /*aActive*/) 
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::Failed
// -----------------------------------------------------------------------------
//                    
void CMCETestUIEngineMultimediaObserver::Failed( CMceSession& /*aSession*/, 
                                                 TInt /*aError*/ )
	{	 
	iEngine.NotifyUser( KUserNotificationSessionFailed );
	iEngine.EngineStateChangedL();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::UpdateFailed
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::UpdateFailed( 
            CMceSession& /*aSession*/, 
            TMceTransactionDataContainer* /*aContainer*/ )
	{
	iEngine.NotifyUser( KUpdateFailed );
	iEngine.EngineStateChangedL();	        
    }



// From MMCETransactionObserver -------------------------------------------------
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::TransactionResponse
// -----------------------------------------------------------------------------
void CMCETestUIEngineMultimediaObserver::TransactionResponse(
						    			CMceSession& /*aSession*/,
						    			TMceTransactionId aTransactionId,
						    			TMceTransactionDataContainer* aContainer )

	{
	MCE_TESTUI_DEBUG1("CMCETestUIEngineMultimediaObserver::TransactionResponse entry")
	
	iEngine.NotifyUser( KResponseReceived );
	iEngine.EngineStateChangedL();	        

	//Clean the Container
	LogContainer( aContainer, aTransactionId );
	
	MCE_TESTUI_DEBUG1("CMCETestUIEngineMultimediaObserver::TransactionResponse exit") 
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::TransactionFailed
// -----------------------------------------------------------------------------	
	    
void CMCETestUIEngineMultimediaObserver::TransactionFailed( 
					                    CMceSession& /*aSession*/,
						    			TMceTransactionId aTransactionId, 
						    			TInt aError )
 
 
 	{
 	MCE_TESTUI_DEBUG1("CMCETestUIEngineMultimediaObserver::TransactionFailed entry")
 	
 	iEngine.RemoveTransactionId( aTransactionId );
 	iEngine.NotifyUser( KTransactionFailed );
 	iEngine.EngineStateChangedL();

	MCE_TESTUI_DEBUG3("CMCETestUIEngineMultimediaObserver::TransactionFailed aTransactionId", aTransactionId )
	MCE_TESTUI_DEBUG3("CMCETestUIEngineMultimediaObserver::TransactionFailed aError", aError )
 	
 	MCE_TESTUI_DEBUG1("CMCETestUIEngineMultimediaObserver::TransactionFailed exit")
	}

// From MMCEInTransactionObserver -------------------------------------------------
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::TransactionResponse
// -----------------------------------------------------------------------------
void CMCETestUIEngineMultimediaObserver::IncomingRequest(
						    			const TDesC8& /*aMethod*/,
										CMceSession& /*aSession*/,
	    								TMceTransactionId aTransactionId,
	    								TMceTransactionDataContainer* aContainer )
	{
 	MCE_TESTUI_DEBUG1("CMCETestUIEngineMultimediaObserver::IncomingRequest entry")
 	
	TUint code = 200;
	_LIT8( KMceSipOK,"OK");
	
	iEngine.AddTransactionIdL( aTransactionId );
	iEngine.NotifyUser( KRequestReceived );
	iEngine.EngineStateChangedL();	  
	
	LogContainer( aContainer, aTransactionId );

	}


// -- from MMCEStreamObserver --------------------------------------------------


// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::StreamStateChanged
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::StreamStateChanged( 
            CMceMediaStream& aStream )
	{	
    iEngine.NotifyStreamStateChangeL( aStream );
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::StreamStateChanged
// -----------------------------------------------------------------------------
//	
void CMCETestUIEngineMultimediaObserver::StreamStateChanged( 
			CMceMediaStream& aStream, 
			CMceMediaSink& /*aSink*/ )
    {
    iEngine.NotifyStreamStateChangeL( aStream );
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::StreamStateChanged
// -----------------------------------------------------------------------------
//	
void CMCETestUIEngineMultimediaObserver::StreamStateChanged( 
			CMceMediaStream& aStream, 
			CMceMediaSource& aSource)
	{
	TInt state = aStream.State();

	TBuf16<100> buf16;
//	buf16.Append(KUserNotificationStreamStateChanged );
//	buf16.Append(KLeftParenthesis);
	TInt transProgress;
	
	switch( state ) 
		{
		case CMceMediaStream::ETranscodingRequired: 
		    buf16.Append(KStreamTranscodingRequired); 
		    iEngine.NotifyUser(buf16);
		    break;
		case CMceMediaStream::ETranscoding: 
			{
			buf16.Append(KStreamTranscoding);
			CMceFileSource& fileSource = static_cast<CMceFileSource&>(aSource);
			transProgress = fileSource.TranscodingProgressL();
			buf16.Append(KLeftParenthesis);
			buf16.AppendNum(transProgress);
			buf16.Append(KRightParenthesis);
			}
		     
		    break;
		default: 
		    buf16.Append(KSessionStateUnknown); 
		}
	if( transProgress == 10 || transProgress == 100 )
		{
		iEngine.NotifyUser(buf16);
		}
	}



// -- from MMCERtpObserver -----------------------------------------------------


// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::SRReceived
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::SRReceived( 
			CMceSession& /*aStream*/, 
			CMceMediaStream& /*aSource*/ )
	{
	RDebug::Print(_L("SR Received "));
	if(iEngine.iRtcpFlag)
		{
		iEngine.NotifyUser( KUserNotificationSRReceived );
		iEngine.EngineStateChangedL();
		}
	else
		{
		iEngine.iSRCount++;
		if(iEngine.iSRCount == 20)
			{
			iEngine.NotifyUser( KUserNotificationSRReceived );
			iEngine.EngineStateChangedL();
			iEngine.iSRCount = 0;
			}
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::RRReceived
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::RRReceived( 
			CMceSession&/* aStream*/,
			CMceMediaStream& /*aSource*/ )
	{
		RDebug::Print(_L("RR Received "));
		if(iEngine.iRtcpFlag)
			{
			iEngine.NotifyUser( KUserNotificationRRReceived );
			iEngine.EngineStateChangedL();
			}
		else
			{
			iEngine.iRRCount++;
			if(iEngine.iRRCount == 20)
				{
				iEngine.NotifyUser( KUserNotificationRRReceived );
				iEngine.EngineStateChangedL();
				iEngine.iRRCount = 0;
				}
			}
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::InactivityTimeout
// -----------------------------------------------------------------------------
//		
void CMCETestUIEngineMultimediaObserver::InactivityTimeout( 
			CMceMediaStream& /*aStream*/,
			CMceRtpSource& /*aSource*/ )
	{
	iEngine.NotifyUser( KUserInactivityTimeoutReceived );
	iEngine.EngineStateChangedL();
	}


// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::Activity
// -----------------------------------------------------------------------------
//		
void CMCETestUIEngineMultimediaObserver::Activity( 
			CMceMediaStream& /*aStream*/,
			CMceRtpSource& /*aSource*/ )
	{
	iEngine.NotifyUser( KUserActivityTimeReceived );
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::IncomingEvent
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::IncomingEvent(
					CMceInEvent* aEvent,
    				TMceTransactionDataContainer*/* aContainer*/ )
	{
	CMCETestUIEngineInEvent& engineEvent = 
	    iEngine.CreateInEventL( aEvent ); 
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::EventStateChanged
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::EventStateChanged(
        			CMceEvent& /*aEvent*/,
    				TMceTransactionDataContainer* aContainer )
	{
	HBufC8* hbuf = aContainer->GetReasonPhrase();
	if(hbuf)
		{
		HBufC* buf = HBufC::NewL(hbuf->Length());
		TPtr ptr = buf->Des();
		ptr.Copy(*hbuf);
		TBuf16<100> buf16;
		buf16.Append(KUserNotificationEventStateChanged );
		buf16.Append(KSpace);
		TInt code = aContainer->GetStatusCode();
	    buf16.AppendNum(code);
	    buf16.Append(*buf);
	 	iEngine.NotifyUser(buf16);
	  	delete buf;
		}
	else
		{
		iEngine.NotifyUser( KUserNotificationEventStateChanged );	
		}	

	delete hbuf;
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::NotifyReceived
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::NotifyReceived(
					CMceEvent&/* aEvent*/,
    				TMceTransactionDataContainer*/* aContainer*/ )
   {
   	iEngine.NotifyUser( KUserNotificationEventNotifyReceived );
	iEngine.EngineStateChangedL();
   }
					
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::EventConnectionStateChanged
// -----------------------------------------------------------------------------
//					
void CMCETestUIEngineMultimediaObserver::EventConnectionStateChanged(
	                CMceEvent& /*aEvent*/,
	                TBool aActive ) 
  {
  if(aActive)
  	{
  	iEngine.NotifyUser( KUserNotificationEventActiveConnectionStateChangedReceived );
  	}
  else
  	{
  	iEngine.NotifyUser( KUserNotificationEventNotActiveConnectionStateChangedReceived );
  	}
  
  iEngine.EngineStateChangedL();
  }
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::Failed
// -----------------------------------------------------------------------------
//  
          
void CMCETestUIEngineMultimediaObserver::Failed
					(CMceEvent& /*aEvent*/, TInt /*aError*/ )
  {
  iEngine.NotifyUser( KUserNotificationEventFailed );
  iEngine.EngineStateChangedL();
  }
					
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::IncomingRefer
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::IncomingRefer(
					CMceInRefer* aRefer,
					const TDesC8& /*aReferTo*/,
    				TMceTransactionDataContainer* /*aContainer*/  )
	{
	CMCETestUIEngineInRefer& engineRefer = 
	    iEngine.CreateInReferL( aRefer ); 
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::ReferStateChanged
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineMultimediaObserver::ReferStateChanged(
        			CMceRefer& /*aRefer*/,
    				TMceTransactionDataContainer* aContainer )
	{
	HBufC8* hbuf = aContainer->GetReasonPhrase();
	if(hbuf)
		{
		HBufC* buf = HBufC::NewL(hbuf->Length());
		TPtr ptr = buf->Des();
		ptr.Copy(*hbuf);
		TBuf16<100> buf16;
		buf16.Append(KUserNotificationReferStateChanged );
		buf16.Append(KSpace);
		TInt code = aContainer->GetStatusCode();
	    buf16.AppendNum(code);
	    buf16.Append(*buf);
	 	iEngine.NotifyUser(buf16);
	  	delete buf;
		}
	else
		{
		iEngine.NotifyUser( KUserNotificationReferStateChanged );	
		}	

	delete hbuf;
	iEngine.EngineStateChangedL();
	}



void CMCETestUIEngineMultimediaObserver::SsrcAdded(
					CMceMediaStream& /*aStream*/,
					CMceRtpSource& /*aSource*/,
					TUint /*aSsrc*/ ) 
	    
	{
	}
void CMCETestUIEngineMultimediaObserver::SsrcRemoved(
					CMceMediaStream& /*aStream*/,
					CMceRtpSource& /*aSource*/,
					TUint /*aSsrc*/ )
	{
	}	
			
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::ReferConnectionStateChanged
// -----------------------------------------------------------------------------
//					
void CMCETestUIEngineMultimediaObserver::ReferConnectionStateChanged(
	                CMceRefer& /*aRefer*/,
	                TBool aActive ) 
  {
  if(aActive)
  	{
  	iEngine.NotifyUser( KUserNotificationReferActiveConnectionStateChangedReceived );
  	}
  else
  	{
  	iEngine.NotifyUser( KUserNotificationReferNotActiveConnectionStateChangedReceived );
  	}
  
  iEngine.EngineStateChangedL();
  }
// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::Failed
// -----------------------------------------------------------------------------
//  
          
void CMCETestUIEngineMultimediaObserver::Failed
					(CMceRefer& /*aRefer*/, TInt /*aError*/ )
  {
  iEngine.NotifyUser( KUserNotificationReferFailed );
  iEngine.EngineStateChangedL();
  }
					

void CMCETestUIEngineMultimediaObserver::FCMsgReceived(CMceSession& /*aSession*/,
					HBufC8* aMessage)
   {
   if(aMessage)
   	{
	HBufC* buf = HBufC::NewL(aMessage->Length());
	TPtr ptr = buf->Des();
	ptr.Copy(*aMessage);
	TBuf16<100> buf16;
	buf16.Append(KFCMessageReceived );
	buf16.Append(KSpace);
	buf16.Append(*buf);
 	iEngine.NotifyUser(buf16);
  	delete buf;
   	}
    delete aMessage;
   	iEngine.EngineStateChangedL(); 
   }
   
void CMCETestUIEngineMultimediaObserver::FCMsgErrorOccurred(CMceSession& /*aSession*/,
					TInt /*aError*/)
   {
   TBuf16<100> buf16;
   buf16.Append(KFCMessageErrorOccured); 
   iEngine.NotifyUser(buf16);   
   iEngine.EngineStateChangedL(); 
   }



// -----------------------------------------------------------------------------
// CMCETestUIEngineMultimediaObserver::Failed
// -----------------------------------------------------------------------------
//  
void CMCETestUIEngineMultimediaObserver::CsStatusChangedL( 
                                        TPoCOmaCsState aStatus )

  {
  iEngine.CsStatusChangedL( aStatus );
  }
  
/**
* Incoming DTMF tone received.
* @param aSession Session that tone was received.
* @param aStream Stream that tone was received.
* @param aTone Received tone.
*/
void  CMCETestUIEngineMultimediaObserver::DtmfToneReceived( CMceSession& /*aSession*/,
                               CMceAudioStream& /*aStream*/,
                               const TChar& aTone )
    {
    TBuf16<100> buf16;
   	buf16.Append(KDtmfToneReceived); 
   	buf16.Append(aTone);
   	iEngine.NotifyUser(buf16);   
   	iEngine.EngineStateChangedL(); 
    }
/**
* DTMF event received.
* @param aSession Session that event was received.
* @param aStream Stream that event was received.
* @param aSource Source that event was received.
* @param aEvent Received event.
*/
void  CMCETestUIEngineMultimediaObserver::DtmfEventReceived( CMceSession& /*aSession*/,
                                CMceAudioStream& /*aStream*/,
                                CMceMediaSource& /*aSource*/,
                                TMceDtmfEvent aEvent )
    {
    TBuf16<100> buf16;
   	buf16.Append(KDtmfEventReceived);
    switch( aEvent ) 
		{
		case EDtmfSendStarted: 
			{
			buf16.Append(KDtmfStarted); 
		    break;
			}
		case EDtmfSendCompleted: 
			{
			buf16.Append(KDtmfCompleted); 
		    break;
			}   
	 	default: 
		    {    
		    break;
		    }  
		   
		}
	iEngine.NotifyUser(buf16);
	iEngine.EngineStateChangedL(); 
    }

/**
* DTMF error occured. If error occurs, DTMFs becomes
* unavailable for that source.
* @param aSession Session that event was received.
* @param aStream Stream that event was received.
* @param aStream Stream that event was received.
* @param aError Error code.
*/
void CMCETestUIEngineMultimediaObserver::DtmfErrorOccured(
	CMceSession& /*aSession*/,
	CMceAudioStream& /*aStream*/,
	CMceMediaSource& /*aSource*/,
	TInt /*aError*/ )
    {
    TBuf16<100> buf16;
   	buf16.Append(KDtmfErrorOccured);
	iEngine.NotifyUser(buf16);
	iEngine.EngineStateChangedL(); 
    }

void CMCETestUIEngineMultimediaObserver::LogContainer( TMceTransactionDataContainer* aContainer,
	TMceTransactionId aTransactionId ) const
	{
	MCE_TESTUI_DEBUG1( "CMCETestUIEngineMultimediaObserver::LogContainer entry" )

	MCE_TESTUI_DEBUG3( "CMCETestUIEngineMultimediaObserver::LogContainer aTransactionId", aTransactionId )
	MCE_TESTUI_DEBUG1( "Headers:" )

	CDesC8Array* arr = NULL;
	HBufC8* p = NULL;
	TInt count = 0;
	
	arr = aContainer->GetHeaders();
	if ( arr )
		{
		count = arr->Count();
		for ( TInt i=0 ; i < count ; ++i )
			{
			MCE_TESTUI_DEBUG6( "", arr->MdcaPoint( i ) )
			}
		delete arr;
		arr = 0;
		}
	
	arr = aContainer->GetContentHeaders();
	if ( arr )
		{
		count = arr->Count();
		for ( TInt i=0 ; i < count ; ++i )
			{
			MCE_TESTUI_DEBUG6( "", arr->MdcaPoint( i ) )
			}
		delete arr;
		arr = 0;
		}

	p = aContainer->GetContentType();
	if ( p )
		{
		MCE_TESTUI_DEBUG6( "ContentType ", *p );
		delete p;
		arr = 0;
		}
	
	p = aContainer->GetContent();
	if ( p )
		{
		MCE_TESTUI_DEBUG6( "Content ", *p );
		delete p;
		arr = 0;
		}
		
	p = aContainer->GetReasonPhrase();
	if ( p )
		{
		MCE_TESTUI_DEBUG6( "ReasonPhrase ", *p );
		delete p;
		arr = 0;
		}
	
	MCE_TESTUI_DEBUG1( "CMCETestUIEngineMultimediaObserver::LogContainer exit" )
	}


// End of File.